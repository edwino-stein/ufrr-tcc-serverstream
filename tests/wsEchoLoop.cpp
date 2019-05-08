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

        void onError(ws::Session * const session, exceptions::WsErrorException e){std::cout << "onError" << '\n';}
        bool onIsAcceptable(ws::HTTPRequest &request){ return true; }
        void onAccept(ws::HTTPWSResponse &response){}
};

BOOST_AUTO_TEST_CASE(echo_server){

    bool loop = true;
    runtime::Signal::attach(
        runtime::SIG::ABRT,
        runtime::Task([&loop](runtime::TaskContext * const ctx){
            loop = false;
        })
    );

    Listener l;
    Server *server = NULL;

    runtime::Signal::attach(
        runtime::SIG::INT,
        runtime::Task([&server, &l](runtime::TaskContext * const ctx){

            if(server != NULL){
                BOOST_TEST_MESSAGE(" * Stopping server...");
                server->stop();
                delete server;
            }

            BOOST_TEST_MESSAGE(" * Waiting connections on \"ws://0.0.0.0:8888/\" (Ctrl+C to restart)...");
            server = new Server(8888, &l);
            server->run();

        })
    );

    BOOST_TEST_MESSAGE(" * Running, to close send a abort signal (kill -6 pid)...");
    runtime::Signal::trigger(runtime::SIG::INT);

    while(loop);
    BOOST_TEST_MESSAGE(" * Stopping server...");
    server->stop();
    delete server;
}
