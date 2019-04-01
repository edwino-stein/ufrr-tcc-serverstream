#if !defined  _RUNTIME_THREAD_HPP
    #define _RUNTIME_THREAD_HPP

    #include <thread>

    namespace runtime {

        class Thread {

            protected:
                std::thread pThread;
                bool running;
                virtual bool isJoinable() const;

            public:

                Thread();
                virtual ~Thread();

                virtual void run()=0;
                virtual void stop(const bool join = true);
        };
    }

#endif
