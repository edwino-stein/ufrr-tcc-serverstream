#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite wsEcho
#include <boost/test/unit_test.hpp>

#include "types.hpp"
#include "ws/Server.hpp"
#include "ws/Session.hpp"
#include "exceptions/Exception.hpp"

using namespace ws;
using namespace exceptions;


BOOST_AUTO_TEST_CASE(init_server){

    Server server(8888, NULL);

    BOOST_TEST_MESSAGE(" * Running server per 2 seconds...");
    server.run();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    BOOST_TEST_MESSAGE(" * Stopping server...");
    server.stop();
}
