#include "runtime/LoopInside.hpp"

using namespace runtime;

LoopInside::LoopInside() : Thread(){}

LoopInside::~LoopInside(){
    this->stop(true);
}

void LoopInside::run(){
    this->running = true;
    this->pThread = std::thread(LoopInside::entryPoint, this);
}

void LoopInside::entryPoint(LoopInside * const t){
    while(t->running) t->loop();
}
