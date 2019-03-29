#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite cameraToScreen
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <chrono>
#include <iomanip>

#include "types.hpp"
#include "av/V4L2DeviceInput.hpp"
#include "av/DecodeListener.hpp"
#include "exceptions/Exception.hpp"

#include <SDL.h>
#include <SDL_thread.h>

extern "C" {
    #include "libswscale/swscale.h"
}

using namespace av;
using namespace exceptions;

unsigned long micros(){
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}

class Screen : public av::DecodeListener {
    public:

        SDL_Overlay *bmp;
        SDL_Surface *screen;

        struct SwsContext *swsCtx;

        size_t frameHeight;
        size_t frameWidth;

        Screen(V4L2DeviceInput &input){

            this->frameWidth = input.codecCtx->width;
            this->frameHeight = input.codecCtx->height;

            this->screen = SDL_SetVideoMode(
                this->frameWidth,
                this->frameHeight,
                24,
                0
            );

            this->bmp = SDL_CreateYUVOverlay(
                this->frameWidth,
                this->frameHeight,
                SDL_YV12_OVERLAY,
                screen
            );

            this->swsCtx = sws_getContext(
                this->frameWidth,
                this->frameHeight,
                input.codecCtx->pix_fmt,
                this->frameWidth,
                this->frameHeight,
                AV_PIX_FMT_YUV420P,
                SWS_BILINEAR,
                NULL,
                NULL,
                NULL
          	);
        }

        void onDecoded(AVFrame *frame){

            SDL_LockYUVOverlay(this->bmp);

            AVPicture pict;
        	pict.data[0] = this->bmp->pixels[0];
        	pict.data[1] = this->bmp->pixels[2];
        	pict.data[2] = this->bmp->pixels[1];

            pict.linesize[0] = this->bmp->pitches[0];
        	pict.linesize[1] = this->bmp->pitches[2];
        	pict.linesize[2] = this->bmp->pitches[1];

        	sws_scale(
                this->swsCtx,
                (uint8_t const * const *) frame->data,
        		frame->linesize,
                0,
                this->frameHeight,
        		pict.data,
                pict.linesize
            );

            SDL_UnlockYUVOverlay(this->bmp);

            SDL_Rect rect;
            rect.x = 0;
            rect.y = 0;
            rect.w = this->frameWidth;
            rect.h = this->frameHeight;
            SDL_DisplayYUVOverlay(this->bmp, &rect);
        }
};

BOOST_AUTO_TEST_CASE(displaying_video){

    BOOST_REQUIRE_MESSAGE(boost::unit_test::framework::master_test_suite().argc > 4, "agrs: <input> <width> <height> <framerate>");

    String inDevice(boost::unit_test::framework::master_test_suite().argv[1]);
    json::DynamicJsonDocument j(1024);
    j["width"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[2]);
    j["height"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[3]);
    j["framerate"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[4]);

    BOOST_TEST_MESSAGE(" * JSON used: \"" << j << "\"");

    BOOST_REQUIRE_EQUAL(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER), 0);

    av::initAll();

    V4L2DeviceInput input(inDevice);
    BOOST_REQUIRE_NO_THROW(input.init(j.as<json::JsonObject>()));
    input.dump();

    Screen screen(input);

    double latency = 0;
    unsigned readed = 0;
    double average = 0;
    double max = -1;
    double min = -1;

    BOOST_TEST_MESSAGE(" * Displaying \"" << inDevice << "\"...");

    bool loop = true;
    while(loop){

        latency = micros();
        if(!input.read(false)) break;
        input.decode(&screen);
        latency = micros() - latency;

        average = average + (latency - average)/(++readed);

        if(max < 0) max = latency;
        if(min < 0) min = latency;

        if(latency > max) max = latency;
        if(latency < min) min = latency;

        SDL_Event event;
        SDL_PollEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                SDL_Quit();
                loop = false;
            break;
            default:
                break;
        }
    }

    BOOST_TEST_MESSAGE(" * Latency average: " << std::setprecision(4) << (average/1000) << " ms");
    BOOST_TEST_MESSAGE(" * Latency max: " << std::setprecision(4) << (max/1000) << " ms");
    BOOST_TEST_MESSAGE(" * Latency min: " << std::setprecision(4) << (min/1000) << " ms");

    input.close();
}
