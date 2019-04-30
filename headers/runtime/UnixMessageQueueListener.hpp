#if !defined  _UNIX_MESSAGE_QUEUE_HPP
    #define _UNIX_MESSAGE_QUEUE_HPP

    #include "types.hpp"
    #include "runtime/LoopInside.hpp"

    #include <sys/types.h>

    namespace runtime {

        namespace UnixMessageQueue {
            key_t fileToKey(String fileName, int projId);
            int getQueue(key_t key, int flags);
            int createQueue(key_t key, int flags);
            void removeQueue(int msqId);
            bool existsQueue(key_t key);
            bool popQueue(int msqId, void *msgP, size_t msgSz, const bool noWait);
        }

        template <size_t S>
        class UnixMessageQueueListener : public runtime::LoopInside {

            protected:

                struct MsgBuffer {
                    long mtype;
                    char mtext[S];
                };

                key_t key;
                AFunction<void(String &msg)> callback;
                int msqId;

                void init(key_t key, const bool recreate);
                void loop() override;

            public:

                UnixMessageQueueListener();
                UnixMessageQueueListener(key_t key, AFunction<void(String &msg)> callback, const bool recreate = true);
                UnixMessageQueueListener(String fileName, AFunction<void(String &msg)> callback, const bool recreate = true);
                virtual ~UnixMessageQueueListener();

                void destroyQueue() const;
        };


        template <size_t S>
        UnixMessageQueueListener<S>::UnixMessageQueueListener(key_t key, AFunction<void(String &msg)> callback, const bool recreate):
        LoopInside(), callback(callback){
            this->init(key, recreate);
        }

        template <size_t S>
        UnixMessageQueueListener<S>::UnixMessageQueueListener(String fileName, AFunction<void(String &msg)> callback, const bool recreate):
        LoopInside(), callback(callback){
            this->init(UnixMessageQueue::fileToKey(fileName, 'G'), recreate);
        }

        template <size_t S>
        UnixMessageQueueListener<S>::~UnixMessageQueueListener(){
            this->stop(true);
        }

        template <size_t S>
        void UnixMessageQueueListener<S>::init(key_t key, const bool recreate){

            this->key = key;

            if(UnixMessageQueue::existsQueue(this->key)){

                this->msqId = UnixMessageQueue::getQueue(this->key, 0);

                if(recreate){
                    UnixMessageQueue::removeQueue(this->msqId);
                    this->msqId = UnixMessageQueue::createQueue(this->key, 0666);
                }
            }
            else{
                this->msqId = UnixMessageQueue::createQueue(this->key, 0666);
            }
        }

        template <size_t S>
        void UnixMessageQueueListener<S>::destroyQueue() const {
            UnixMessageQueue::removeQueue(this->msqId);
        }

        template <size_t S>
        void UnixMessageQueueListener<S>::loop(){

            struct MsgBuffer buf;
            if(!UnixMessageQueue::popQueue(this->msqId, &buf, S + 1, true)) return;

            buf.mtext[S] = '\0';
            String str(buf.mtext);
            this->callback(str);
        }
    }

#endif
