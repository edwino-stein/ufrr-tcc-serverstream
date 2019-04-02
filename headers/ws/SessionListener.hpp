#if !defined  WS_SESSION_LISTENER_HPP
    #define WS_SESSION_LISTENER_HPP

    #include "ws/ws.hpp"
    #include "exceptions/Exception.hpp"

    namespace ws {

        class SessionListener {

            public:

                virtual ~SessionListener();

                virtual void onMessage(ws::Session * const session, IOBuffer &data)=0;
                virtual void onClose(ws::Session * const session, const int code)=0;
                virtual void onError(ws::Session * const session, exceptions::Exception e)=0;
        };
    }

#endif
