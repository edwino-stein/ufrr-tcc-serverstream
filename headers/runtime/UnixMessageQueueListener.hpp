#if !defined  _UNIX_MESSAGE_QUEUE_HPP
    #define _UNIX_MESSAGE_QUEUE_HPP

    #include "types.hpp"
    #include "runtime/LoopInside.hpp"

    #include <sys/types.h>
     #include <sys/ipc.h>

    namespace runtime {

        class UnixMessageQueueListener : public runtime::LoopInside {

            protected:

                struct MsgBuffer {
                    long mtype;
                    char mtext[1];
                };

                key_t key;
                AFunction<void(String &msg)> callback;
                const size_t buffSize;
                int msqId;

                void init(key_t key, const bool recreate);
                void loop() override;

                static int getQueue(key_t key, int flags);
                static int createQueue(key_t key, int flags);
                static bool queueExists(key_t key);
                static void removeQueue(int msqId);

            public:

                UnixMessageQueueListener(key_t key, AFunction<void(String &msg)> callback, const size_t buffSize = 255, const bool recreate = true);
                UnixMessageQueueListener(String fileName, AFunction<void(String &msg)> callback, const size_t buffSize = 255, const bool recreate = true);
                virtual ~UnixMessageQueueListener();

                void destroyQueue() const;
        };
    }

#endif
