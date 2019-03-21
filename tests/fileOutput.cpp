#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite fileInput
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "types.hpp"
#include "av/VideoFileOutput.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"
#include "exceptions/InvalidJsonConfigException.hpp"

using namespace av;
using namespace exceptions;

#define JSON(d, o, ...) json::DynamicJsonDocument d(1024); \
                        json::deserializeJson(d, #__VA_ARGS__); \
                        json::JsonObject o = d.as<json::JsonObject>()

#define EXAMPLE_FILE_MP4 "output.mp4"

bool checkExceptionOutput(const Exception &e){
    BOOST_TEST_MESSAGE("*********************** OUTPUT e.what() ***********************");
    BOOST_TEST_MESSAGE(e.what());
    BOOST_TEST_MESSAGE("***************************************************************");
    return true;
}

BOOST_AUTO_TEST_CASE(init_object_av_not_initted_error){

    // av::initAll();

    JSON(doc, cfg, {'codec': 'mpeg1video', 'demuxer': 'mpegts', 'width': 960, 'height': 720, 'bitrate': 1000000, 'framerate': 25});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_THROW(fo.init(cfg), Exception);
    fo.close();
}

BOOST_AUTO_TEST_CASE(init_object_no_error){
    av::initAll();

    JSON(doc, cfg, {'codec': 'mpeg1video', 'demuxer': 'mpegts', 'width': 960, 'height': 720, 'bitrate': 1000000, 'framerate': 25, "bframes": 2});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_NO_THROW(fo.init(cfg));

    fo.dump();
    fo.close();
}

BOOST_AUTO_TEST_CASE(init_object_no_error_no_json){
    av::initAll();

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_NO_THROW(fo.init());

    fo.dump();
    fo.close();
}

BOOST_AUTO_TEST_CASE(init_object_invalid_demuxer){
    av::initAll();

    JSON(doc, cfg, {'codec': 'mpeg1video', 'width': 960, 'height': 720, 'bitrate': 1000000, 'framerate': 25});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_THROW(fo.init(cfg), InvalidJsonConfigException);

    fo.dump();
    fo.close();
}

BOOST_AUTO_TEST_CASE(init_object_invalid_codec){
    av::initAll();

    JSON(doc, cfg, {'demuxer': 'mpegts', 'width': 960, 'height': 720, 'bitrate': 1000000, 'framerate': 25});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_THROW(fo.init(cfg), InvalidJsonConfigException);

    fo.dump();
    fo.close();
}

BOOST_AUTO_TEST_CASE(init_object_invalid_width){
    av::initAll();

    JSON(doc, cfg, {'codec': "mpeg1video", 'demuxer': 'mpegts', 'height': 720, 'bitrate': 1000000, 'framerate': 25});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_THROW(fo.init(cfg), InvalidJsonConfigException);

    fo.dump();
    fo.close();
}

BOOST_AUTO_TEST_CASE(init_object_invalid_hight){
    av::initAll();

    JSON(doc, cfg, {'codec': "mpeg1video", 'demuxer': 'mpegts', 'width': 960, 'bitrate': 1000000, 'framerate': 25});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_THROW(fo.init(cfg), InvalidJsonConfigException);

    fo.dump();
    fo.close();
}

BOOST_AUTO_TEST_CASE(init_object_invalid_bitrate){
    av::initAll();

    JSON(doc, cfg, {'codec': "mpeg1video", 'demuxer': 'mpegts', 'width': 960, 'height': 720, 'framerate': 25});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_THROW(fo.init(cfg), InvalidJsonConfigException);

    fo.dump();
    fo.close();
}

BOOST_AUTO_TEST_CASE(init_object_invalid_framerate){
    av::initAll();

    JSON(doc, cfg, {'codec': "mpeg1video", 'demuxer': 'mpegts', 'width': 960, 'height': 720, 'bitrate': 1000000});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_THROW(fo.init(cfg), InvalidJsonConfigException);

    fo.dump();
    fo.close();
}

BOOST_AUTO_TEST_CASE(init_object_invalid_pixfmt){
    av::initAll();

    JSON(doc, cfg, {'codec': "mpeg1video", 'demuxer': 'mpegts', 'width': 960, 'height': 720, 'bitrate': 1000000, 'framerate': 25, "pixfmt": "invalidPixFmt"});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_THROW(fo.init(cfg), Exception);

    fo.dump();
    fo.close();
}

BOOST_AUTO_TEST_CASE(init_object_incompatible_codec_pixfmt){
    av::initAll();

    JSON(doc, cfg, {'codec': "mpeg1video", 'demuxer': 'mpegts', 'width': 960, 'height': 720, 'bitrate': 1000000, 'framerate': 25, "pixfmt": "yuyv422"});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileOutput fo(EXAMPLE_FILE_MP4);
    BOOST_CHECK_THROW(fo.init(cfg), Exception);

    fo.dump();
    fo.close();
}