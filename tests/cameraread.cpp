#include <iostream>
#include <chrono>
#include <iomanip> 

#include "types.hpp"

#include "av/V4L2DeviceInput.hpp"
#include "exceptions/Exception.hpp"

using exceptions::Exception;

unsigned long millis(){
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

unsigned long micros(){
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}


int main(int argc, char const *argv[]){

    av::initAll();

    av::V4L2DeviceInput input("/dev/video0");
    unsigned long ts = 0;
    unsigned long pi = 0;

    try{
        input.init();
    }
    catch(Exception &e){
        std::cout << e.what();
    }
    input.dump();

    while(true){

        AVPacket *packet = av_packet_alloc();

        try{

            ts = micros();
            input.read(packet);
            double delay = micros() - ts;

            std::cout << "#" << pi++ <<"delay: " << std::setprecision(4) << (delay/1000) << "ms; " << std::endl;
        }
        catch(Exception &e){
            std::cout << e.what();
            av_packet_free(&packet);
            break;
        }

        av_packet_free(&packet);
    }

    input.close();
    return 0;
}

