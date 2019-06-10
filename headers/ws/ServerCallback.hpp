#if !defined  WS_SERVER_CALLBACK_HPP
    #define WS_SERVER_CALLBACK_HPP

    #include "ws/ws.hpp"
    #include "ws/SessionListener.hpp"

    namespace ws {

        class ServerCallback : public ws::SessionListener {

            protected:
                ws::ServerListener &listener;

            public:

                ServerCallback(ws::ServerListener &listener);
                virtual ~ServerCallback();

                //Session proxy events
                virtual void onMessage(ws::Session &session, ws::IOBuffer &data) override;
                virtual void onClose(ws::Session &session, const int code) override;
                virtual void onError(ws::Session &session, exceptions::WsErrorException e) override;
        };
    }

#endif
