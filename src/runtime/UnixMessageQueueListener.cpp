#include "runtime/UnixMessageQueueListener.hpp"

#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>


using namespace runtime;

bool UnixMessageQueue::popQueue(int msqId, void *msgP, size_t msgSz, const bool noWait){
    int msgFlg = 0;
    if(noWait) msgFlg |= IPC_NOWAIT;
    return msgrcv(msqId, msgP, msgSz, 0, msgFlg) != -1;
}

key_t UnixMessageQueue::fileToKey(String fileName, int projId){
    return ftok(fileName.c_str(), projId);
}

int UnixMessageQueue::getQueue(key_t key, int flags){
    return msgget(key, flags);
}

int UnixMessageQueue::createQueue(key_t key, int flags){
    return UnixMessageQueue::getQueue(key, flags | IPC_CREAT | IPC_EXCL);
}

void UnixMessageQueue::removeQueue(int msqId){
    msgctl(msqId, IPC_RMID, NULL);
}

bool UnixMessageQueue::existsQueue(key_t key){

    int msqId = UnixMessageQueue::createQueue(key, 0);

    if(msqId == -1){
        if(errno == EEXIST) return true;
        if(errno == EACCES) return true;
    }
    else{
        UnixMessageQueue::removeQueue(msqId);
    }

    return false;
}
