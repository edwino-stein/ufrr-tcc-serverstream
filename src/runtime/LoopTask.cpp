#include "runtime/LoopTask.hpp"

using namespace runtime;

LoopTask::LoopTask(TaskContext * const taskCtx, const TaskHandle task) : AsyncTask(taskCtx, task){}
LoopTask::LoopTask(const runtime::TaskHandle task) : AsyncTask(task){}

LoopTask::~LoopTask(){
    this->stop(true);
}

void LoopTask::run(){
    this->running = true;
    this->pThread = std::thread(LoopTask::entryPoint, this);
}

void LoopTask::entryPoint(LoopTask * const t){
    while(t->running) t->task(t->taskCtx);
    std::this_thread::yield();
}
