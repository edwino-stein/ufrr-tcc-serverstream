#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite tasks
#include <boost/test/unit_test.hpp>

#include "runtime/UnixMessageQueueListener.hpp"

using namespace runtime;

BOOST_AUTO_TEST_CASE(unixqm_test){

    bool stop = false;
    UnixMessageQueueListener umq("README.md", [&stop](String &msg){
        BOOST_TEST_MESSAGE(" * Message: \"" << msg << "\"");
        stop = true;
    }, 255, true);


    BOOST_TEST_MESSAGE("Waiting for message...");
    umq.run();
    while(!stop);

    umq.destroyQueue();
    umq.stop(true);
}
