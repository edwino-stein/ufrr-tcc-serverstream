#include "exceptions/Exception.hpp"

#include <iostream>
#include <sstream>

using namespace exceptions;
using stacktrace = boost::stacktrace::stacktrace;

Exception::Exception() : st(stacktrace()), message(_message), prev(_prev){
    this->init(
        "exceptions::Exception",
        NULL,
        2,
        1
    );
}

Exception::Exception(const Exception &e) : st(e.st), message(_message), prev(_prev) {
    this->init(
        e.message,
        e.prev != NULL ? new Exception(*(e.prev)) : NULL,
        e.stIgnoreBottom,
        e.stIgnoreTop
    );
}

Exception::Exception(String message, const Exception &prev) : st(stacktrace()), message(_message), prev(_prev) {
    this->init(message, new Exception(prev), 2, 1);
}

Exception::Exception(String message) : st(stacktrace()), message(_message), prev(_prev) {
    this->init(message, NULL, 2, 1);
}

Exception::Exception(const std::exception &e) : st(stacktrace()), message(_message), prev(_prev){
    this->init(e.what(), NULL, 2, 1);
}

Exception::Exception(int e) : st(stacktrace()), message(_message), prev(_prev){
    this->init("Exception with error code " + std::to_string(e) + ".", NULL, 2, 1);
}

Exception::~Exception(){
    if(this->prev != NULL) delete this->prev;
}

void Exception::init(String message, Exception *prev, const size_t stib, const size_t stit){
    this->_message = message;
    this->_prev = prev;
    this->stIgnoreBottom = stib;
    this->stIgnoreTop = stit;
}

String Exception::what() const {

    std::ostringstream w;
    size_t i = 0;

    w << std::endl << " ********** EXCEPTION STACK **********" << std::endl << std::endl;

    w << " #" << i++ << ": " << this->message << std::endl;
    w << "     Stacktrace:" << std::endl;
    w << this->decodeST() << std::endl;

    if(this->prev != NULL){
    
        Exception *prev = this->prev;
        while(prev != NULL){

            w << " #" << i++ << ": " << prev->message << std::endl;
            w << "     Stacktrace:" << std::endl;
            w << prev->decodeST() << std::endl;

            prev = prev->prev;
        }
    }

    return w.str();
}

String Exception::decodeST() const {
    
    std::ostringstream st;
    size_t count = 0;
    size_t totalFrames = this->st.size();
    size_t i = 0;

    for (boost::stacktrace::frame frame: this->st) {
        
        if(count >= totalFrames - this->stIgnoreBottom) break;
        if(count++ < this->stIgnoreTop) continue;

        st << "\t#" << i++ << ": ";
        st << "[" << frame.address() << "] " << frame.name() << std::endl;
    }
    
    return st.str();
}