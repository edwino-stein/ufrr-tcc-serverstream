#include "runtime/Task.hpp"

using namespace runtime;

Task::Task(TaskContext * const taskCtx, const TaskHandle task) : taskCtx(taskCtx), task(task){}
Task::Task(const runtime::TaskHandle task) :taskCtx(NULL), task(task){}

Task::~Task(){}
void Task::run(){ this->task(this->taskCtx); }
