#if !defined  _RUNTIME_SIGNAL_HPP
    #define _RUNTIME_SIGNAL_HPP

    #include "runtime/Task.hpp"
    #include <csignal>
    #include <map>

    namespace runtime {

        enum class SIG : int {
            ABRT = SIGABRT,
            FPE = SIGFPE,
            ILL = SIGILL,
            INT = SIGINT,
            SEGV = SIGSEGV,
            TERM = SIGTERM
        };

        class Signal {
            private:
                Signal();

            protected:

                static std::map<runtime::SIG, runtime::Task> callbacks;
                static void entryPoint(int signum);

            public:

                static void attach(runtime::SIG sig, runtime::Task task);
                static void detach(runtime::SIG sig);
                static void trigger(runtime::SIG sig);
                static bool hasAttached(runtime::SIG sig);

        };
    }

#endif
