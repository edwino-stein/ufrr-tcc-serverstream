#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite transcoder
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <chrono>
#include <iomanip>

#include "types.hpp"
#include "av/Transcoder.hpp"
#include "av/VideoFileInput.hpp"
#include "av/VideoFileOutput.hpp"

#include "exceptions/Exception.hpp"

using namespace av;
using namespace exceptions;

BOOST_AUTO_TEST_CASE(transcoder){

    BOOST_REQUIRE_MESSAGE(boost::unit_test::framework::master_test_suite().argc > 8, "agrs: <input> <output> <demuxer> <codec> <width> <height> <framerate> <bitrate>");

    String inFile(boost::unit_test::framework::master_test_suite().argv[1]);
    String outFile(boost::unit_test::framework::master_test_suite().argv[2]);

    json::DynamicJsonDocument j(1024);
    j["encoder"]["demuxer"] = boost::unit_test::framework::master_test_suite().argv[3];
    j["encoder"]["codec"] = boost::unit_test::framework::master_test_suite().argv[4];
    j["encoder"]["width"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[5]);
    j["encoder"]["height"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[6]);
    j["encoder"]["framerate"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[7]);
    j["encoder"]["bitrate"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[8]);
    j["encoder"]["bframes"] = 0;
    j["encoder"]["rescaleTs"] = true;

    j["decoder"]["rescaleTs"] = false;

    BOOST_TEST_MESSAGE(" * JSON used: \"" << j << "\"");

    av::initAll();

    VideoFileInput input(inFile);
    VideoFileOutput output(outFile);

    Transcoder transcoder(&input, &output);
    BOOST_REQUIRE_NO_THROW(transcoder.init(j.as<json::JsonObject>()));

    input.dump();
    output.dump();

    BOOST_TEST_MESSAGE(" * Transcoding \"" << inFile << "\" into \"" << outFile << "\"...");
    transcoder.run();
    while(!transcoder.readError);
    transcoder.stop();

    output.close();
    input.close();
}
