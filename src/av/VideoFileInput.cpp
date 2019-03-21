#include "av/VideoFileInput.hpp"

using namespace av;
using namespace json;

VideoFileInput::VideoFileInput(String fileName) : VideoInput(){
    this->fileName = fileName;
}

VideoFileInput::~VideoFileInput(){
    this->close();
}

void VideoFileInput::init(){
    DynamicJsonDocument doc(1024);
    this->init(doc.as<JsonObject>());
}

void VideoFileInput::init(JsonObject cfg){
    this->openFile(this->fileName);
    this->initVideoStream();
    this->initCodec();
}

void VideoFileInput::close(){
    this->closeCodec();
    this->closeFile();
}

void VideoFileInput::dump() const {
    Format::dump(this->fileName, false);
}