#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite tasks
#include <boost/test/unit_test.hpp>

#include "runtime/Task.hpp"
using namespace runtime;

class TaskClass : public TaskContext {
    public:
        bool done = false;
        void doTask(){
            BOOST_TEST_MESSAGE("TaskClass::doTask");
            this->done = true;
        }
};

BOOST_AUTO_TEST_CASE(task_test){

    bool runnedby = false;

    Task t([&runnedby](TaskContext * const ctx){
        runnedby = true;
    });

    BOOST_REQUIRE(!runnedby);

    t.run();

    BOOST_REQUIRE(runnedby);
}

BOOST_AUTO_TEST_CASE(task_test_with_context){

    TaskClass tc;

    Task t(&tc, [](TaskContext * const ctx){
        ((TaskClass *) ctx)->doTask();
    });

    BOOST_REQUIRE(!tc.done);

    t.run();

    BOOST_REQUIRE(tc.done);
}

