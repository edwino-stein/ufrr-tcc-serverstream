#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite tasks
#include <boost/test/unit_test.hpp>

#include "runtime/Task.hpp"
#include "runtime/AsyncTask.hpp"
#include "runtime/LoopTask.hpp"
#include "runtime/LoopInside.hpp"

using namespace runtime;

class TaskClass : public TaskContext {
    public:
        bool done = false;
        void doTask(){
            BOOST_TEST_MESSAGE("TaskClass::doTask");
            this->done = true;
        }
};

class LoopInsideTask : public LoopInside {
    protected:
        void loop() override {
            this->count++;
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
        }

    public:

        int count;

        LoopInsideTask(){
            this->count = 0;
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

BOOST_AUTO_TEST_CASE(async_task_test){

    bool runnedby = false;

    AsyncTask t([&runnedby](TaskContext * const ctx){
        runnedby = true;
    });

    BOOST_REQUIRE(!runnedby);

    t.run();
    t.stop();

    BOOST_REQUIRE(runnedby);
}

BOOST_AUTO_TEST_CASE(async_task_count_test){

    int count = 0;

    AsyncTask t([&count](TaskContext * const ctx){
        while(count < 10){
            count++;
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
        }
    });

    t.run();

    BOOST_TEST_MESSAGE("waiting for async task...");
    while(count < 10);

    t.stop();
    BOOST_REQUIRE(count >= 10);
}

BOOST_AUTO_TEST_CASE(loop_task_count_test){

    int count = 0;

    LoopTask t([&count](TaskContext * const ctx){
        count++;
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    });

    t.run();

    BOOST_TEST_MESSAGE("waiting for loop task...");
    while(count < 10);

    t.stop();
    BOOST_REQUIRE(count >= 10);
}

BOOST_AUTO_TEST_CASE(inside_loop_task_count_test){

    LoopInsideTask t;
    t.run();

    BOOST_TEST_MESSAGE("waiting for inside loop task...");
    while(t.count < 10);

    t.stop();
    BOOST_REQUIRE(t.count >= 10);
}
