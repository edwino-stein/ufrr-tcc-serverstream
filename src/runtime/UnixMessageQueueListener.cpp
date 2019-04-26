#include "runtime/UnixMessageQueueListener.hpp"

#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <iostream>
#include <sstream>

using namespace runtime;

UnixMessageQueueListener::UnixMessageQueueListener(key_t key, AFunction<void(String &msg)> callback, const size_t buffSize, const bool recreate):
LoopInside(), callback(callback), buffSize(buffSize){
    this->init(key, recreate);
}
UnixMessageQueueListener::UnixMessageQueueListener(String fileName, AFunction<void(String &msg)> callback, const size_t buffSize, const bool recreate):
LoopInside(), callback(callback), buffSize(buffSize){
    this->init(ftok(fileName.c_str(), 'G'), recreate);
}

UnixMessageQueueListener::~UnixMessageQueueListener(){
    this->stop(true);
}

void UnixMessageQueueListener::init(key_t key, const bool recreate){

    this->key = key;

    if(UnixMessageQueueListener::queueExists(this->key)){

        this->msqId = UnixMessageQueueListener::getQueue(this->key, 0);

        if(recreate){
            UnixMessageQueueListener::removeQueue(this->msqId);
            this->msqId = UnixMessageQueueListener::createQueue(this->key, 0666);
        }
    }
    else{
        this->msqId = UnixMessageQueueListener::createQueue(this->key, 0666);
    }
}

void UnixMessageQueueListener::loop(){

    struct MsgBuffer buf;
    std::ostringstream oss;

    if(msgrcv(this->msqId, &buf, this->buffSize + 1, 0, 0 | IPC_NOWAIT) == -1)
        return;

    buf.mtext[this->buffSize] = '\0';
    oss << buf.mtext;

    String str(oss.str());
    this->callback(str);
}

void UnixMessageQueueListener::destroyQueue() const {
    UnixMessageQueueListener::removeQueue(this->msqId);
}

int UnixMessageQueueListener::getQueue(key_t key, int flags){
    return msgget(key, flags);
}

int UnixMessageQueueListener::createQueue(key_t key, int flags){
    return UnixMessageQueueListener::getQueue(key, flags | IPC_CREAT | IPC_EXCL);
}

bool UnixMessageQueueListener::queueExists(key_t key){

    int msqId = UnixMessageQueueListener::createQueue(key, 0);

    if(msqId == -1){
        if(errno == EEXIST) return true;
        if(errno == EACCES) return true;
    }
    else{
        UnixMessageQueueListener::removeQueue(msqId);
    }

    return false;
}

void UnixMessageQueueListener::removeQueue(int msqId){
    msgctl(msqId, IPC_RMID, NULL);
}
