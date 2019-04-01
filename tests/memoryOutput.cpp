#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite memoryOutput
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "types.hpp"
#include "av/VideoMemoryOutput.hpp"
#include "exceptions/Exception.hpp"

using namespace av;
using namespace exceptions;

#define JSON(d, o, ...) json::DynamicJsonDocument d(1024); \
                        json::deserializeJson(d, #__VA_ARGS__); \
                        json::JsonObject o = d.as<json::JsonObject>()

av::memoryWriteHandle callback = [](uint8_t * const buffer, const int bfSize, av::VideoMemoryOutput * const me){
    return 0;
};

BOOST_AUTO_TEST_CASE(init_object_av_not_initted_error){

    // av::initAll();

    JSON(doc, cfg, {'codec': 'mpeg1video', 'demuxer': 'mpegts', 'width': 960, 'height': 720, 'bitrate': 1000000, 'framerate': 25});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoMemoryOutput mo(callback);
    BOOST_CHECK_THROW(mo.init(cfg), Exception);
    mo.close();
}

BOOST_AUTO_TEST_CASE(init_object_no_error){
    av::initAll();

    JSON(doc, cfg, {'codec': 'mpeg1video', 'demuxer': 'mpegts', 'width': 960, 'height': 720, 'bitrate': 1000000, 'framerate': 25, "bframes": 2});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoMemoryOutput mo(callback);
    BOOST_CHECK_NO_THROW(mo.init(cfg));

    mo.dump();
    mo.close();
}

BOOST_AUTO_TEST_CASE(init_object_no_error_no_json){
    av::initAll();

    VideoMemoryOutput mo(callback);
    BOOST_CHECK_NO_THROW(mo.init());

    mo.dump();
    mo.close();
}
