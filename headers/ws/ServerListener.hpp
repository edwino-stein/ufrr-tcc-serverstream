#if !defined  WS_SERVER_LISTENER_HPP
    #define WS_SERVER_LISTENER_HPP

    #include "ws/SessionListener.hpp"

    namespace ws {

        class ServerListener : public ws::SessionListener {

            public:

                virtual ~ServerListener();

                virtual void onConnection(ws::Session * const session)=0;
                virtual bool onIsAcceptable(ws::HTTPRequest &request)=0;
                virtual void onAccept(ws::HTTPWSResponse &response)=0;
        };
    }

#endif
