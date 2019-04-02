#include "runtime/Task.hpp"

using namespace runtime;

Task::Task() : taskCtx(NULL){}
Task::Task(TaskContext * const taskCtx, const TaskHandle task) : taskCtx(taskCtx), task(task){}
Task::Task(const runtime::TaskHandle task) :taskCtx(NULL), task(task){}

Task::~Task(){}
void Task::run(){ this->task(this->taskCtx); }

Task &Task::operator=(const Task& other){

    if(this != &other){

        this->taskCtx = other.taskCtx;
        this->task = other.task;
    }

    return *this;
}
