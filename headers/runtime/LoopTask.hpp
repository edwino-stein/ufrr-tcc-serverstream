#if !defined  _RUNTIME_LOOP_TASK_HPP
    #define _RUNTIME_LOOP_TASK_HPP

    #include "runtime/AsyncTask.hpp"

    namespace runtime {

        class LoopTask : public runtime::AsyncTask {

            protected:
                bool keepLoop;

            private:
                static void entryPoint(LoopTask * const t);

            public:

                LoopTask(runtime::TaskContext * const taskCtx, const runtime::TaskHandle task);
                LoopTask(const runtime::TaskHandle task);
                virtual ~LoopTask();

                virtual void run() override;
                virtual void stop(bool join = false);
        };
    }

#endif
