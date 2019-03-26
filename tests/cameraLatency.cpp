#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite cameralatency
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <chrono>
#include <iomanip> 

#include "types.hpp"
#include "av/V4L2DeviceInput.hpp"
#include "exceptions/Exception.hpp"

using namespace av;
using namespace exceptions;

unsigned long micros(){
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}

BOOST_AUTO_TEST_CASE(v4l2_camera_latency_test){

    BOOST_REQUIRE_MESSAGE(boost::unit_test::framework::master_test_suite().argc > 5, "agrs: <device> <max_packets> <width> <height> <framerate>");

    String device(boost::unit_test::framework::master_test_suite().argv[1]);
    unsigned maxPackets = std::stoi(boost::unit_test::framework::master_test_suite().argv[2]);

    json::DynamicJsonDocument j(1024);
    j["width"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[3]);
    j["height"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[4]);
    j["framerate"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[5]);

    BOOST_TEST_MESSAGE(" * JSON used: \"" << j << "\"");

    av::initAll();

    V4L2DeviceInput camera(device);
    BOOST_CHECK_NO_THROW(camera.init(j.as<json::JsonObject>()));

    camera.dump();

    unsigned readed = 0;
    double average = 0;
    double latency = 0;
    double max = -1;
    double min = -1;

    BOOST_TEST_MESSAGE(" * Reading " << maxPackets << " packets from \"" << device << "\"...");
    for(unsigned i = 0; i < maxPackets; ++i){

        AVPacket *packet = av_packet_alloc();

        latency = micros();
        camera.read(packet, false, false);
        latency = micros() - latency;

        average = average + (latency - average)/(++readed);

        if(max < 0) max = latency;
        if(min < 0) min = latency;

        if(latency > max) max = latency;
        if(latency < min) min = latency;

        av_packet_free(&packet);
    }

    camera.close();

    BOOST_TEST_MESSAGE(" * Latency average: " << std::setprecision(4) << (average/1000) << " ms");
    BOOST_TEST_MESSAGE(" * Latency max: " << std::setprecision(4) << (max/1000) << " ms");
    BOOST_TEST_MESSAGE(" * Latency min: " << std::setprecision(4) << (min/1000) << " ms");
}
