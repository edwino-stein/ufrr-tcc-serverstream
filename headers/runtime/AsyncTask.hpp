#if !defined  _RUNTIME_ASYNC_TASK_HPP
    #define _RUNTIME_ASYNC_TASK_HPP

    #include <thread>
    #include "runtime/Task.hpp"
    #include "runtime/Thread.hpp"

    namespace runtime {

        class AsyncTask : public runtime::Task, public runtime::Thread {

            private:
                static void entryPoint(AsyncTask * const t);

            public:

                AsyncTask(runtime::TaskContext * const taskCtx, const runtime::TaskHandle task);
                AsyncTask(const runtime::TaskHandle task);
                virtual ~AsyncTask();

                virtual void run() override;
        };
    }

#endif
