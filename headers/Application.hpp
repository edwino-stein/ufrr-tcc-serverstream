#if !defined  _APPLICATION_HPP
    #define _APPLICATION_HPP

    #include "types.hpp"

    class Application {

        private:
            //Singleton setup
            static Application* instance;
            Application();
            Application(Application const&);
            Application& operator=(Application const&);

        public:
            virtual ~Application();

            static Application& app();
            int main(int argc, char const *argv[]);
    };

#endif
