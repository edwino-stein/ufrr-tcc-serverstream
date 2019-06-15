#include "Application.hpp"

#include <iostream>

#include "ws/ws.hpp"
#include "ws/Session.hpp"
#include "ws/Server.hpp"
#include "ffmpeg/FfmpegTcp.hpp"
#include "runtime/LoopTask.hpp"
#include "runtime/Signal.hpp"
#include "runtime/Task.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/WsErrorException.hpp"

using ws::Server;
using ws::Session;
using ws::IOBuffer;
using ws::HTTPRequest;
using ws::HTTPWSResponse;
using ffmpeg::FfmpegTcp;

using runtime::Signal;
using runtime::Task;
using runtime::Thread;
using exceptions::Exception;
using exceptions::WsErrorException;

Application::Application() : exitCode(0){}
Application::~Application(){}

int Application::main(int argc, char const *argv[]){

    Application &me = *this;

    if(argc < 2){
        std::cerr << "Missing port for Websocket server" << '\n';
        return 1;
    }

    unsigned int wsPort = std::atoi(argv[1]);
    if(!this->isValidPort(wsPort)){
        std::cerr << " * Invalid port for Websocket server" << '\n';
        return 1;
    }

    if(argc < 3){
        std::cerr << " * Missing port for FFMPEG TCP client" << '\n';
        return 1;
    }

    unsigned int ffmpegPort = std::atoi(argv[2]);
    if(!this->isValidPort(ffmpegPort)){
        std::cerr << " * Invalid port for FFMPEG TCP client" << '\n';
        return 1;
    }

    size_t ffmpegBuffer = 1024;
    if(argc >= 4){
        ffmpegBuffer = std::atoi(argv[3]);
    }

    Signal::attach(runtime::SIG::INT, Task([&me](runtime::TaskContext * const ctx){
        std::cout << " * Received SIG::INT (" << (int) runtime::SIG::INT  << ")\n";
        me.exitCode = 2;
    }));

    Server wsServer(wsPort, *this);
    FfmpegTcp ffmpeg(ffmpegPort, ffmpegBuffer, *this);

    runtime::LoopTask broadcastTask([&me, &wsServer](runtime::TaskContext * const ctx){

        me.broadcastQueueMtx.lock();
        if(me.broadcastQueue.empty()){
            me.broadcastQueueMtx.unlock();
            return;
        }

        IOBuffer buffer(me.broadcastQueue.front());
        me.broadcastQueue.pop();
        me.broadcastQueueMtx.unlock();

        wsServer.broadcast(buffer);
    });

    wsServer.run();
    broadcastTask.run();
    bool ffmpegConnected = false;

    while(this->exitCode == 0){

        if(!ffmpegConnected){

            std::cout << " * Trying to connect to FFMPEG..." << '\n';

            try{
                ffmpeg.run();
                ffmpegConnected = true;
            }
            catch(Exception &e){
                std::cout << e.what() << '\n';
                Thread::sleep<std::chrono::seconds>(1);
            }

            if(ffmpegConnected) std::cout << " * Connected successfully with FFMPEG!" << '\n';
        }

        std::cout << std::flush;
        Thread::sleep<std::chrono::milliseconds>(100);
    }

    ffmpeg.stop();
    wsServer.stop();
    broadcastTask.stop(true);

    return this->exitCode;
}

void Application::onFfmpegReceive(unsigned char data[], const size_t length){
    IOBuffer buffer(ws::bytesToIoBuffer(data, length));
    this->broadcastQueueMtx.lock();
    this->broadcastQueue.push(buffer);
    this->broadcastQueueMtx.unlock();
}

void Application::onFfmpegError(Exception& e) {
    if(this->exitCode == 0){
        std::cout << " * Error when receiving data from FFMPEG:" << e.what() << '\n';
        std::cout << std::flush;
        this->exitCode = 1;
    }
}

bool Application::isValidPort(const unsigned int port) const {
    return (port > 0) && (port < 65535);
}

void Application::onClose(Session &session, const int code){
    std::cout << " * Connection closed: " << session.metaData.toString() <<'\n';
}

void Application::onConnection(Session &session){
    session.setType(ws::SessionMessageType::BINARY);
    std::cout << " * New connection: " << session.metaData.toString() <<'\n';
}

bool Application::onIsAcceptable(HTTPRequest &request){
    return true;
}

void Application::onAccept(HTTPWSResponse &response){
    response.insert(ws::http::field::server, "MPEG1 Video WS Streamer");
    response.insert(ws::http::field::sec_websocket_protocol, "ws-mpegts");
}

void Application::onMessage(Session &session, IOBuffer &data){}
void Application::onError(Session &session, WsErrorException e){}
