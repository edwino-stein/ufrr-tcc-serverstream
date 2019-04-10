#if !defined  WS_SERVER_HPP
    #define WS_SERVER_HPP

    #include "runtime/LoopInside.hpp"
    #include "ws/ServerRequestAcceptor.hpp"

    namespace ws {

        class Server : public runtime::LoopInside, public ws::ServerRequestAcceptor {

            protected:

                Mutex sessionsMtx;
                Vector<Session *> sessions;

                virtual void loop() override;
                virtual void clearup();

            public:

                Server(const int port, ws::ServerListener * const listener);
                virtual ~Server();

                virtual void broadcast(IOBuffer &data);
                virtual void stop();
        };
    }

#endif
