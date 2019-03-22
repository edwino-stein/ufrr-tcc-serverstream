#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite v4l2input
#include <boost/test/unit_test.hpp>

#include "types.hpp"
#include "av/V4L2DeviceInput.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"
#include "exceptions/InvalidJsonConfigException.hpp"

using namespace av;
using namespace exceptions;

#define JSON(d, o, ...) json::DynamicJsonDocument d(1024); \
                        json::deserializeJson(d, #__VA_ARGS__); \
                        json::JsonObject o = d.as<json::JsonObject>()

#define DEVICE "/dev/video0"
#define NO_DEVICE "no_file"

bool checkExceptionOutput(const Exception &e){
    BOOST_TEST_MESSAGE("*********************** OUTPUT e.what() ***********************");
    BOOST_TEST_MESSAGE(e.what());
    BOOST_TEST_MESSAGE("***************************************************************");
    return true;
}

BOOST_AUTO_TEST_CASE(init_object_av_not_initted_error){

    V4L2DeviceInput fi(DEVICE);
    BOOST_CHECK_THROW(fi.init(), AvErrorException);
}

BOOST_AUTO_TEST_CASE(init_object_no_error){
    av::initAll();

    JSON(doc, cfg, {'width': 640, 'height': 480, 'framerate': 30});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    V4L2DeviceInput fi(DEVICE);

    BOOST_CHECK_NO_THROW(fi.init(cfg));

    fi.dump();
    fi.close();
}

BOOST_AUTO_TEST_CASE(init_object_no_error_no_json){
    av::initAll();

    V4L2DeviceInput fi(DEVICE);

    BOOST_CHECK_NO_THROW(fi.init());

    fi.dump();
    fi.close();
}

BOOST_AUTO_TEST_CASE(init_object_no_device_error){
    av::initAll();

    V4L2DeviceInput fi(NO_DEVICE);

    BOOST_CHECK_EXCEPTION(fi.init(), Exception, checkExceptionOutput);

    fi.close();
}

BOOST_AUTO_TEST_CASE(init_object_invalid_width){
    av::initAll();

    JSON(doc, cfg, {'height': 480, 'framerate': 30});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    V4L2DeviceInput fi(DEVICE);

    BOOST_CHECK_THROW(fi.init(cfg), InvalidJsonConfigException);

    fi.dump();
    fi.close();
}

BOOST_AUTO_TEST_CASE(init_object_invalid_height){
    av::initAll();

    JSON(doc, cfg, {'width': 640, 'framerate': 30});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    V4L2DeviceInput fi(DEVICE);

    BOOST_CHECK_THROW(fi.init(cfg), InvalidJsonConfigException);

    fi.dump();
    fi.close();
}

BOOST_AUTO_TEST_CASE(init_object_invalid_framerate){
    av::initAll();

    JSON(doc, cfg, {'width': 640, 'height': 480});
    BOOST_TEST_MESSAGE(" * JSON used: \"" << cfg << "\"");

    V4L2DeviceInput fi(DEVICE);

    BOOST_CHECK_THROW(fi.init(cfg), InvalidJsonConfigException);

    fi.dump();
    fi.close();
}