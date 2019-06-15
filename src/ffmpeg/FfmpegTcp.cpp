#include "ffmpeg/FfmpegTcp.hpp"
#include "exceptions/Exception.hpp"

#include <iostream>

using namespace ffmpeg;
using namespace net;
using runtime::LoopInside;
using exceptions::Exception;

FfmpegTcp::FfmpegTcp(const unsigned int port, const size_t bufferSize, FfmpegListener &listener):
LoopInside(), ioCtx(), socket(ioCtx), port(port), bufferSize(bufferSize), listener(listener){}

FfmpegTcp::~FfmpegTcp(){}

void FfmpegTcp::loop(){

    unsigned char buffer[this->bufferSize];
    size_t length = 0;
    try{
        length = boost::asio::read(
            this->socket,
            boost::asio::buffer(buffer, this->bufferSize)
        );
    }
    catch (std::exception& e){
        std::cerr << "Exception: " << e.what() << "\n";
        return;
    }

    this->listener.onFfmpegReceive(buffer, length);
}

void FfmpegTcp::run(){

    if(this->running) return;

    try{
        TcpResolver resolver(this->ioCtx);
        boost::asio::connect(this->socket, resolver.resolve(
            "127.0.0.1",
            std::to_string(this->port)
        ));
    }
    catch (std::exception& e){
        throw Exception(
            "Erro ao conectar ao FFMPEG",
            Exception(e)
        );
    }

    LoopInside::run();
}

void FfmpegTcp::stop(){
    LoopInside::stop(true);
}
