#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite signals
#include <boost/test/unit_test.hpp>

#include "types.hpp"
#include "runtime/Task.hpp"
#include "runtime/Signal.hpp"

using namespace runtime;

BOOST_AUTO_TEST_CASE(trigger_sig_int){

    bool catched = false;
    Task t([&catched](TaskContext * const ctx){
        BOOST_TEST_MESSAGE(" * SIG INT test 1");
        catched = true;
    });

    Signal::attach(SIG::INT, t);
    Signal::trigger(SIG::INT);

    BOOST_CHECK(catched);
}

BOOST_AUTO_TEST_CASE(not_trigger_sig_int){

    bool catched = false;
    Task t([&catched](TaskContext * const ctx){
        BOOST_TEST_MESSAGE(" * SIG INT test 2");
        catched = true;
    });

    Signal::attach(SIG::INT, t);
    // Signal::trigger(SIG::INT);

    BOOST_CHECK(!catched);
}

BOOST_AUTO_TEST_CASE(user_trigger_sig_int){

    bool catched = false;
    Task t([&catched](TaskContext * const ctx){
        BOOST_TEST_MESSAGE(" * SIG INT test 3");
        catched = true;
    });

    Signal::attach(SIG::INT, t);

    BOOST_TEST_MESSAGE(" * Esperando SIGINT (Ctrl+C)...");
    while(!catched);
    BOOST_CHECK(catched);
}
