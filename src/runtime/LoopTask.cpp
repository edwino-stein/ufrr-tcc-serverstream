#include "runtime/LoopTask.hpp"

using namespace runtime;

LoopTask::LoopTask(TaskContext * const taskCtx, const TaskHandle task) : AsyncTask(taskCtx, task){
    this->keepLoop = false;
}

LoopTask::LoopTask(const runtime::TaskHandle task) : AsyncTask(task){
    this->keepLoop = false;
}

LoopTask::~LoopTask(){
    this->stop(true);
}

void LoopTask::run(){
    this->keepLoop = true;
    this->pThread = std::thread(LoopTask::entryPoint, this);
}

void LoopTask::stop(bool join){
    this->keepLoop = false;
    if(join) this->join();
}

void LoopTask::entryPoint(LoopTask * const t){
    while(t->keepLoop) t->task(t->taskCtx);
    std::this_thread::yield();
}
