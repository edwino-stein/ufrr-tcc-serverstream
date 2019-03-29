#if !defined  _RUNTIME_ASYNC_TASK_HPP
    #define _RUNTIME_ASYNC_TASK_HPP

    #include <thread>
    #include "runtime/Task.hpp"

    namespace runtime {

        class AsyncTask : public runtime::Task {

            protected:

                std::thread pThread;

            private:
                static void entryPoint(AsyncTask * const t);

            public:

                AsyncTask(runtime::TaskContext * const taskCtx, const runtime::TaskHandle task);
                AsyncTask(const runtime::TaskHandle task);
                virtual ~AsyncTask();

                virtual void run() override;
                virtual void join();
                virtual bool isJoinable() const;
        };
    }

#endif
