#include "av/V4L2DeviceInput.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/InvalidJsonConfigException.hpp"

using namespace av;
using exceptions::Exception;
using exceptions::InvalidJsonConfigException;

V4L2DeviceInput::V4L2DeviceInput(String device) : VideoInput(){
    this->device = device;
}

V4L2DeviceInput::~V4L2DeviceInput(){
    this->close();
}

void V4L2DeviceInput::init(){
    DynamicJsonDocument doc(1024);

    doc["width"] = 640;
    doc["height"] = 480;
    doc["framerate"] = 30;

    this->init(doc.as<JsonObject>());
}

void V4L2DeviceInput::init(JsonObject cfg){

    if(!cfg["width"].is<int>())
        throw InvalidJsonConfigException("width", "int");

    if(!cfg["height"].is<int>())
        throw InvalidJsonConfigException("height", "int");

    if(!cfg["framerate"].is<int>())
        throw InvalidJsonConfigException("framerate", "int");


    String res = std::to_string(std::abs(cfg["width"].as<int>())) + "x" + std::to_string(std::abs(cfg["height"].as<int>()));

    AVDictionary *options = NULL;

    av_dict_set(
        &options,
        "video_size",
        res.c_str(),
        0
    );

    av_dict_set(
        &options,
        "framerate",
        std::to_string(std::abs(cfg["framerate"].as<int>())).c_str(),
        0
    );

    this->_format = av_find_input_format("v4l2");
    this->openFile(this->device, &options);
    this->initVideoStream();
    this->initCodec();

    av_dict_free(&options);
}

void V4L2DeviceInput::close(){
    this->closeCodec();
    this->closeFile();
}

void V4L2DeviceInput::dump() const {
    Format::dump(this->device, false);
}