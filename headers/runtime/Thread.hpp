#if !defined  _RUNTIME_THREAD_HPP
    #define _RUNTIME_THREAD_HPP

    #include <thread>
    #include <chrono>

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

                template<typename C>
                static void sleep(const unsigned long millis);
        };

        template<typename C>
        void Thread::sleep(const unsigned long time){
            std::this_thread::sleep_for(C(time));
        }

    }



#endif
