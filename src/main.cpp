#include "Application.hpp"

int main(int argc, char const *argv[]){
    int r = Application::app().main(argc, argv);
    return Application::exit(r);
}
