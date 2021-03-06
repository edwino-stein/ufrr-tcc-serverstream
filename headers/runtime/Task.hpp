#if !defined  _RUNTIME_TASK_HPP
    #define _RUNTIME_TASK_HPP

    #include "types.hpp"
    #include "runtime/TaskContext.hpp"

    namespace runtime {

        using TaskHandle = AFunction<void(TaskContext * const)>;

        class Task {

            protected:

                runtime::TaskContext * taskCtx;
                runtime::TaskHandle task;

            public:

                Task();
                Task(runtime::TaskContext * const taskCtx, const runtime::TaskHandle task);
                Task(const runtime::TaskHandle task);

                Task &operator=(const Task& other);

                virtual ~Task();
                virtual void run();
        };
    }

#endif
