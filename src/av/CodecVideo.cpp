#include "av/CodecVideo.hpp"

using namespace av;

CodecVideo::CodecVideo() : Codec(), videoStreamIdx(_videoStreamIdx), videoStream(_videoStream) {
    this->_videoStreamIdx = -1;
    this->_videoStream = NULL;
}

CodecVideo::~CodecVideo(){}
