#include "runtime/Thread.hpp"

using namespace runtime;

Thread::Thread() : pThread(std::thread()) {
    this->running = false;
}

Thread::~Thread(){
    this->stop(true);
}

void Thread::stop(const bool join){
    if(!this->running) return;
    this->running = false;
    if(join && this->isJoinable()) this->pThread.join();
}

bool Thread::isJoinable() const{
    return this->pThread.joinable();
}
