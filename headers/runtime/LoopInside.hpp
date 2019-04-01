#if !defined  _RUNTIME_LOOP_INSIDE_HPP
    #define _RUNTIME_LOOP_INSIDE_HPP

    #include "runtime/Thread.hpp"

    namespace runtime {

        class LoopInside : public runtime::Thread {

            protected:
                virtual void loop()=0;

            private:
                static void entryPoint(LoopInside * const t);

            public:

                LoopInside();
                virtual ~LoopInside();

                virtual void run() override;
        };
    }

#endif
