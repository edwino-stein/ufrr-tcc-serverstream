#if !defined  WS_SERVER_CALLBACK_HPP
    #define WS_SERVER_CALLBACK_HPP

    #include "ws/ws.hpp"
    #include "ws/SessionListener.hpp"

    namespace ws {

        class ServerCallback : public ws::SessionListener {

            protected:
                ws::ServerListener * const listener;

            public:

                ServerCallback(ws::ServerListener * const listener);
                virtual ~ServerCallback();

                //Session proxy events
                virtual void onMessage(ws::Session * const session, IOBuffer &data) override;
                virtual void onClose(ws::Session * const session, const int code) override;
                virtual void onError(ws::Session * const session, exceptions::WsErrorException e) override;
        };
    }

#endif
