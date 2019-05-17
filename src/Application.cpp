#include "Application.hpp"

#include <iostream>
#include <fstream>

#include "av/V4L2DeviceInput.hpp"
#include "av/VideoMemoryOutput.hpp"
#include "av/Transcoder.hpp"

#include "ws/Session.hpp"
#include "ws/Server.hpp"

#include "runtime/Signal.hpp"
#include "runtime/Task.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/InvalidJsonConfigException.hpp"

using av::V4L2DeviceInput;
using av::VideoMemoryOutput;
using av::Transcoder;

using ws::Server;
using ws::Session;
using ws::IOBuffer;
using ws::HTTPRequest;
using ws::HTTPWSResponse;

using runtime::Signal;
using runtime::Task;
using runtime::UnixMessageQueueListener;
using json::JsonObject;
using exceptions::Exception;
using exceptions::InvalidJsonConfigException;
using exceptions::WsErrorException;

Application* Application::instance = NULL;
Application::Application(Application const&){}
Application& Application::operator=(Application const&){ return *(Application::instance); }

Application::Application(){
    this->loop = true;
    this->server = NULL;
    this->videoIn = NULL;
    this->videoOut = NULL;
    this->transcoder = NULL;
    this->mql = NULL;
    this->hasUpdate = true;
}

Application::~Application(){
    this->stop();
}

Application& Application::app(){
    if(Application::instance == NULL) Application::instance = new Application();
    return *(Application::instance);
}

int Application::exit(int code){

    if(Application::instance != NULL){

        if(code != 0){
            Application::instance->state = Application::STREAM_STATE::ERROR;
        }

        Application::instance->triggerStatusUpdate();
        Application::instance->updateStatusFile();

        delete Application::instance;
        Application::instance = NULL;
    }

    return code;
}

int Application::main(int argc, char const *argv[]){

    if(argc < 2){
        std::cout << "Missing status file output." << '\n';
        return 1;
    }

    this->statusFile = argv[1];
    this->updateStatusFile();

    Signal::attach(runtime::SIG::INT, Task([](runtime::TaskContext * const ctx){
        std::cout << "* Received SIG::INT" << '\n';
        Application::app().stop();
    }));

    this->mql = new UnixMessageQueueListener<QUEUE_MSG_BS>(this->statusFile, [](String &msg){
        Application::app().onQueueMessage(msg);
    }, true);

    av::initAll();

    std::cout << "Status will be updated in \"" << this->statusFile << "\"..." << '\n';
    std::cout << "Waiting for command..." << '\n';
    this->mql->run();

    Application::STREAM_STATE s = this->state;
    this->state = Application::STREAM_STATE::WAITING;

    while(this->loop){

        if(s != this->state){

            s = this->state;
            switch (s) {
                case Application::STREAM_STATE::STOPPED:
                    std::cout << "STREAM_STATE::STOPPED" << '\n';
                break;
                case Application::STREAM_STATE::WAITING:
                    std::cout << "STREAM_STATE::WAITING" << '\n';
                break;
                case Application::STREAM_STATE::RUNNING:
                    std::cout << "STREAM_STATE::RUNNING" << '\n';
                break;
                case Application::STREAM_STATE::ERROR:
                    std::cout << "STREAM_STATE::ERROR" << '\n';
                break;
            }

            this->triggerStatusUpdate();
        }

        this->updateStatusFile();
    }

    return 0;
}

void Application::updateStatusFile(){

    if(!this->hasUpdate) return;
    this->hasUpdate = false;

    json::DynamicJsonDocument status(256);

    switch (this->state) {
        case Application::STREAM_STATE::STOPPED:
            status["status"] = "STOPPED";
        break;
        case Application::STREAM_STATE::WAITING:
            status["status"] = "WAITING";
        break;
        case Application::STREAM_STATE::RUNNING:
            status["status"] = "RUNNING";
        break;
        case Application::STREAM_STATE::ERROR:
            status["status"] = "ERROR";
        break;
    }

    std::fstream fout;
    fout.open(this->statusFile, std::fstream::out | std::fstream::trunc);

    if(fout.good()){
        fout << status << "\n";
        fout.flush();
    }
    else{
        std::cout << "Missing status file output." << '\n';
    }

    fout.close();
}

void Application::triggerStatusUpdate(){
    this->hasUpdate = true;
}

void Application::onQueueMessage(String &msg){

    json::DynamicJsonDocument jsonMessage(JSON_BS);
    json::deserializeJson(jsonMessage, msg);

    if(!jsonMessage["cmd"].is<const char *>()) return;
    String cmd(jsonMessage["cmd"].as<const char *>());

    if(cmd == "start")
        return this->onStart(jsonMessage["extra"].as<json::JsonObject>());

    if(cmd == "stop")
        return this->onStop(jsonMessage["extra"].as<json::JsonObject>());
}

void Application::onStart(json::JsonObject extra){

    if(this->state == Application::STREAM_STATE::RUNNING) return;
    if(extra.isNull()) return;

    std::cout << "Received command start: " << extra << std::endl;

    try{
        this->initWs(extra);
        this->initVideo(extra);
    }
    catch(Exception &e){
        this->stopVideo();
        this->stopWs();
        std::cout << e.what() << '\n';
        return;
    }

    this->start();
    this->state = Application::STREAM_STATE::RUNNING;
}

void Application::onStop(json::JsonObject extra){

    if(this->state == Application::STREAM_STATE::WAITING) return;
    std::cout << "Received command stop: " << extra << std::endl;

    this->stopVideo();
    this->stopWs();
    this->state = Application::STREAM_STATE::WAITING;
}

void Application::start(){
    if(this->server != NULL) this->server->run();
    if(this->transcoder != NULL) this->transcoder->run();
}

void Application::stop(){

    if(this->state == Application::STREAM_STATE::STOPPED) return;

    this->mql->destroyQueue();
    this->mql->stop(true);
    delete this->mql;

    this->stopVideo();
    this->stopWs();
    this->state = Application::STREAM_STATE::STOPPED;
    this->loop = false;
}

void Application::initVideo(JsonObject cfg){

    if(!cfg["inputDevice"].is<const char *>()) throw InvalidJsonConfigException("inputDevice", "String");

    this->videoIn = new V4L2DeviceInput(cfg["inputDevice"].as<const char *>());
    this->videoOut = new VideoMemoryOutput([](uint8_t * const data, const int size, VideoMemoryOutput * const vmo){

        IOBuffer buffer;
        boost::asio::const_buffer cbf = boost::asio::buffer(data, size);
        size_t n = boost::asio::buffer_copy(buffer.prepare(size), cbf);
        buffer.commit(n);

        //TODO: Será adicionada numa fila e enviado por outra thread
        if(Application::app().server != NULL)
            Application::app().server->broadcast(buffer);

        return 0;
    });

    this->transcoder = new Transcoder(this->videoIn, this->videoOut);
    this->transcoder->init(cfg);
}

void Application::stopVideo(){

    if(this->transcoder != NULL){
        this->transcoder->stop();
        delete this->transcoder;
        this->transcoder = NULL;
    }

    if(this->videoOut != NULL){
        this->videoOut->close();
        delete this->videoOut;
        this->videoOut = NULL;
    }

    if(this->videoIn != NULL){
        this->videoIn->close();
        delete this->videoIn;
        this->videoIn = NULL;
    }
}

void Application::stopWs(){
    if(this->server != NULL){
        this->server->stop();
        delete this->server;
        this->server = NULL;
    }
}

void Application::initWs(json::JsonObject cfg){

    if(!cfg["serverPort"].is<int>()) throw InvalidJsonConfigException("serverPort", "Int ([0,65535])");

    int port = cfg["serverPort"];
    if(port < 0 || port > 65535) throw InvalidJsonConfigException("serverPort", "Int ([0,65535])");

    this->server = new Server(port, this);
}

void Application::onMessage(Session * const session, IOBuffer &data){}

void Application::onClose(Session * const session, const int code){
    std::cout << "Connection closed" << '\n';
}

void Application::onError(Session * const session, WsErrorException e){}

void Application::onConnection(Session * const session){
    session->setType(ws::SessionMessageType::BINARY);
    std::cout << "New connection" << '\n';
}

bool Application::onIsAcceptable(HTTPRequest &request){
    return true;
}

void Application::onAccept(HTTPWSResponse &response){
    response.insert(ws::http::field::server, "MPEG1 Video WS Streamer");
    response.insert(ws::http::field::sec_websocket_protocol, "ws-mpegts");
}
