#include "Application.hpp"
#include <iostream>

Application* Application::instance = NULL;
Application::Application(Application const&){}
Application& Application::operator=(Application const&){ return *(Application::instance); }

Application::Application(){}
Application::~Application(){}

Application& Application::app(){
    if(Application::instance == NULL) Application::instance = new Application();
    return *(Application::instance);
}

int Application::main(int argc, char const *argv[]){
    std::cout << "Application::main" << '\n';
    return 0;
}
