#include "Application.hpp"

#include <iostream>

#include "av/V4L2DeviceInput.hpp"
#include "av/VideoMemoryOutput.hpp"
#include "av/Transcoder.hpp"

#include "ws/Session.hpp"
#include "ws/Server.hpp"

#include "runtime/Signal.hpp"
#include "runtime/Task.hpp"
#include "exceptions/Exception.hpp"

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
using json::JsonObject;
using exceptions::Exception;

Application* Application::instance = NULL;
Application::Application(Application const&){}
Application& Application::operator=(Application const&){ return *(Application::instance); }

Application::Application(){
    this->server = NULL;
    this->videoIn = NULL;
    this->videoOut = NULL;
    this->transcoder = NULL;
}

Application::~Application(){
    this->stop();
}

Application& Application::app(){
    if(Application::instance == NULL) Application::instance = new Application();
    return *(Application::instance);
}

int Application::main(int argc, char const *argv[]){

    Signal::attach(runtime::SIG::INT, Task([](runtime::TaskContext * const ctx){
        std::cout << "* Received runtime::SIG::INT" << '\n';
        Application::app().stop();
        std::exit(0);
    }));

    av::initAll();

    //TODO: This string comes from other place
    String strJson = "{\
        'decoder':{\
            'device':'/dev/video0',\
            'width':640,\
            'height':480,\
            'framerate':30,\
            'rescaleTs':false\
        },\
        'encoder':{\
            'demuxer':'mpegts',\
            'codec':'mpeg1video',\
            'width':640,\
            'height':480,\
            'framerate':30,\
            'bitrate':1000000,\
            'bframes':0,\
            'rescaleTs':true\
        }\
    }";

    json::DynamicJsonDocument jsonDoc(1024);
    json::deserializeJson(jsonDoc, strJson);
    std::cout << "JSON: " << jsonDoc << '\n';

    try{
        this->initVideo(jsonDoc.as<JsonObject>());
        this->server = new Server(8888, this, true);
    }
    catch(Exception &e){
        std::cout << e.what() << '\n';
        return 1;
    }

    this->start();

    while(true);
    return 0;
}

void Application::start(){
    if(this->server != NULL) this->server->run();
    if(this->transcoder != NULL) this->transcoder->run();
}

void Application::stop(){

    if(this->server != NULL){
        this->server->stop();
        delete this->server;
        this->server = NULL;
    }

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

void Application::initVideo(JsonObject cfg){

    this->videoIn = new V4L2DeviceInput(cfg["decoder"]["device"].as<const char *>());
    this->videoOut = new VideoMemoryOutput([](uint8_t * const data, const int size, VideoMemoryOutput * const vmo){

        IOBuffer buffer;
        boost::asio::const_buffer cbf = boost::asio::buffer(data, size);
        size_t n = boost::asio::buffer_copy(buffer.prepare(size), cbf);
        buffer.commit(n);

        Application::app().server->broadcast(buffer);

        return 0;
    });

    this->transcoder = new Transcoder(this->videoIn, this->videoOut);
    this->transcoder->init(cfg);
}

void Application::onMessage(Session * const session, IOBuffer &data){}

void Application::onClose(Session * const session, const int code){
    std::cout << "Connection closed" << '\n';
}

void Application::onError(Session * const session, Exception e){}

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
