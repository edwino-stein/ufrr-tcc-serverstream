#include "runtime/AsyncTask.hpp"

using namespace runtime;

AsyncTask::AsyncTask(TaskContext * const taskCtx, const TaskHandle task) : Task(taskCtx, task), pThread(std::thread()){}
AsyncTask::AsyncTask(const runtime::TaskHandle task) : Task(task), pThread(std::thread()){}


AsyncTask::~AsyncTask(){
    if(this->isJoinable()) this->join();
}


void AsyncTask::run(){
    this->pThread = std::thread(AsyncTask::entryPoint, this);
}

void AsyncTask::join(){
    this->pThread.join();
}

bool AsyncTask::isJoinable() const{
    return this->pThread.joinable();
}

void AsyncTask::entryPoint(AsyncTask * const t){
    t->task(t->taskCtx);
    std::this_thread::yield();
}
