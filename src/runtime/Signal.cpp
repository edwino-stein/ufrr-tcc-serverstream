#include "runtime/Signal.hpp"

using namespace runtime;

std::map<runtime::SIG, runtime::Task> Signal::callbacks;

Signal::Signal(){}

void Signal::attach(SIG sig, runtime::Task task){
    Signal::detach(sig);
    Signal::callbacks[sig] = task;
    std::signal((int) sig, Signal::entryPoint);
}

void Signal::detach(runtime::SIG sig){
    if(Signal::hasAttached(sig)) Signal::callbacks.erase(sig);
    std::signal((int) sig, SIG_DFL);
}

void Signal::trigger(runtime::SIG sig){
    std::raise((int) sig);
}

void Signal::entryPoint(int sig){
    if(Signal::hasAttached((runtime::SIG) sig)) Signal::callbacks[(runtime::SIG) sig].run();
}

bool Signal::hasAttached(runtime::SIG sig){
    std::map<runtime::SIG, Task>::iterator it = Signal::callbacks.find(sig);
    return it != Signal::callbacks.end();
}
