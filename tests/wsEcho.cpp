#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite wsEcho
#include <boost/test/unit_test.hpp>

#include "types.hpp"
#include "ws/Server.hpp"
#include "ws/Session.hpp"
#include "ws/ServerListener.hpp"
#include "exceptions/Exception.hpp"
#include "runtime/Signal.hpp"
#include "runtime/Task.hpp"

using namespace ws;
using namespace exceptions;

class Listener : public ServerListener{
    public:

        void onConnection(ws::Session * const session){
            BOOST_TEST_MESSAGE(" * New connection");
        }

        void onMessage(ws::Session * const session, IOBuffer &data){
            BOOST_TEST_MESSAGE(" * Received \"" << ws::ioBufferToString(data) << "\"");
            session->send(data);
        }

        void onClose(ws::Session * const session, const int code){
            BOOST_TEST_MESSAGE(" * Connection closed");
        }

        void onError(ws::Session * const session, exceptions::Exception e){}
        bool onIsAcceptable(ws::HTTPRequest &request){ return true; }
        void onAccept(ws::HTTPWSResponse &response){}
};

BOOST_AUTO_TEST_CASE(init_server){

    Server server(8888, NULL);

    BOOST_TEST_MESSAGE(" * Running server per 2 seconds...");
    server.run();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    BOOST_TEST_MESSAGE(" * Stopping server...");
    server.stop();
}

BOOST_AUTO_TEST_CASE(echo_server){

    bool loop = true;
    runtime::Signal::attach(
        runtime::SIG::INT,
        runtime::Task([&loop](runtime::TaskContext * const ctx){
            loop = false;
        })
    );

    Listener l;
    Server server(8888, &l, true);

    server.run();
    BOOST_TEST_MESSAGE(" * Waiting connections on \"ws://0.0.0.0:8888/\" (Ctrl+C to stop)...");
    while(loop);

    BOOST_TEST_MESSAGE(" * Stopping server...");
    server.stop();
}
