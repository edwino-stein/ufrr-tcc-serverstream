#include "runtime/AsyncTask.hpp"

using namespace runtime;

AsyncTask::AsyncTask(TaskContext * const taskCtx, const TaskHandle task) : Task(taskCtx, task), Thread(){}
AsyncTask::AsyncTask(const runtime::TaskHandle task) : Task(task), Thread(){}

AsyncTask::~AsyncTask(){
    this->stop(true);
}

void AsyncTask::run(){
    this->running = true;
    this->pThread = std::thread(AsyncTask::entryPoint, this);
}

void AsyncTask::entryPoint(AsyncTask * const t){
    t->task(t->taskCtx);
    std::this_thread::yield();
}
