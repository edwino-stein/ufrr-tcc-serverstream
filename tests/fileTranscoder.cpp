#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite filetranscoder
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <chrono>
#include <iomanip> 

#include "types.hpp"
#include "av/VideoFileInput.hpp"
#include "av/VideoFileOutput.hpp"
#include "av/FilterGraph.hpp"
#include "av/DecodeListener.hpp"
#include "exceptions/Exception.hpp"

using namespace av;
using namespace exceptions;

unsigned long micros(){
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}

class Transcoder : public av::DecodeListener {
    public:
        VideoFileOutput &out;
        FilterGraph &fg;

        Transcoder(VideoFileOutput &out, FilterGraph &fg) : out(out), fg(fg) {}

        void onDecoded(AVFrame *frame){

            frame->pts = frame->best_effort_timestamp;
            this->fg.push(frame);

            AVFrame *newFrame = av_frame_alloc();
            while(this->fg.pop(newFrame)) this->out.encode(newFrame, true);
            av_frame_free(&newFrame);
        }
};

BOOST_AUTO_TEST_CASE(file_transcoder){

    BOOST_REQUIRE_MESSAGE(boost::unit_test::framework::master_test_suite().argc > 8, "agrs: <input> <output> <demuxer> <codec> <width> <height> <framerate> <bitrate>");

    String inFile(boost::unit_test::framework::master_test_suite().argv[1]);
    String outFile(boost::unit_test::framework::master_test_suite().argv[2]);

    json::DynamicJsonDocument j(1024);
    j["demuxer"] = boost::unit_test::framework::master_test_suite().argv[3];
    j["codec"] = boost::unit_test::framework::master_test_suite().argv[4];
    j["width"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[5]);
    j["height"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[6]);
    j["framerate"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[7]);
    j["bitrate"] = std::stoi(boost::unit_test::framework::master_test_suite().argv[8]);

    BOOST_TEST_MESSAGE(" * JSON used: \"" << j << "\"");

    av::initAll();

    VideoFileInput input(inFile);
    BOOST_REQUIRE_NO_THROW(input.init());

    VideoFileOutput output(outFile);
    BOOST_REQUIRE_NO_THROW(output.init(j.as<json::JsonObject>()));

    FilterGraph fg;
    BOOST_REQUIRE_NO_THROW(fg.init(&input, &output));

    Transcoder tras(output, fg);

    input.dump();
    output.dump();

    unsigned readed = 0;
    double average = 0;
    double latency = 0;
    double max = -1;
    double min = -1;

    BOOST_TEST_MESSAGE(" * Transcoding \"" << inFile << "\" into \"" << outFile << "\"...");

    while(true){

        latency = micros();

        if(!input.read(false)) break;
        input.decode(&tras);

        latency = micros() - latency;

        average = average + (latency - average)/(++readed);

        if(max < 0) max = latency;
        if(min < 0) min = latency;

        if(latency > max) max = latency;
        if(latency < min) min = latency;
    }

    BOOST_TEST_MESSAGE(" * Latency average: " << std::setprecision(4) << (average/1000) << " ms");
    BOOST_TEST_MESSAGE(" * Latency max: " << std::setprecision(4) << (max/1000) << " ms");
    BOOST_TEST_MESSAGE(" * Latency min: " << std::setprecision(4) << (min/1000) << " ms");

    fg.flush(&output);
    output.close();
    input.close();
}
