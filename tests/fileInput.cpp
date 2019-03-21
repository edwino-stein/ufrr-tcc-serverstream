#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite fileInput
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "types.hpp"
#include "av/VideoFileInput.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"

using namespace av;
using namespace exceptions;

#define JSON(d, o, ...) json::DynamicJsonDocument d(1024); \
                        json::deserializeJson(d, #__VA_ARGS__); \
                        json::JsonObject o = d.as<json::JsonObject>()

#define EXAMPLE_FILE_MP4 "SampleVideo_1280x720.mp4"
#define NO_FILE "no_file.mp4"
#define INVALID_FILE "bad_file.mp4"

bool checkExceptionOutput(const Exception &e){
    BOOST_TEST_MESSAGE("*********************** OUTPUT e.what() ***********************");
    BOOST_TEST_MESSAGE(e.what());
    BOOST_TEST_MESSAGE("***************************************************************");
    return true;
}

BOOST_AUTO_TEST_CASE(init_object_av_not_initted_error){

    JSON(doc, cfg, {});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileInput fi(EXAMPLE_FILE_MP4);

    BOOST_CHECK_THROW(fi.init(cfg), AvErrorException);
}

BOOST_AUTO_TEST_CASE(init_object_no_error){
    av::initAll();

    JSON(doc, cfg, {});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileInput fi(EXAMPLE_FILE_MP4);

    BOOST_CHECK_NO_THROW(fi.init(cfg));

    fi.close();
}

BOOST_AUTO_TEST_CASE(init_object_no_error_no_json){
    av::initAll();

    VideoFileInput fi(EXAMPLE_FILE_MP4);

    BOOST_CHECK_NO_THROW(fi.init());
    fi.dump();
    fi.close();
}


BOOST_AUTO_TEST_CASE(init_object_no_file_error){
    av::initAll();

    JSON(doc, cfg, {});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileInput fi(NO_FILE);

    BOOST_CHECK_EXCEPTION(fi.init(cfg), AvErrorException, checkExceptionOutput);

    fi.close();
}

BOOST_AUTO_TEST_CASE(init_object_bad_file_error){
    av::initAll();

    JSON(doc, cfg, {});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    VideoFileInput fi(INVALID_FILE);

    BOOST_CHECK_EXCEPTION(fi.init(cfg), AvErrorException, checkExceptionOutput);
    fi.close();
}
