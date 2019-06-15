#if !defined  WS_SESSION_HPP
    #define WS_SESSION_HPP

    #include "ws/ws.hpp"
    #include "runtime/LoopInside.hpp"
    #include "ws/SessionMetaData.hpp"

    namespace ws {

        class Session : public runtime::LoopInside {

            protected:

                ws::SessionListener &listener;
                ws::WSocket socket;
                ws::SessionMetaData _metaData;

                ws::SessionLifeCycle _readyState;
                ws::SessionMessageType _type;

                virtual void loop() override;

            public:

                ws::SessionLifeCycle const &readyState;
                ws::SessionMessageType const &type;
                ws::SessionMetaData const &metaData;

                Session(ws::WSocket &&socket, ws::SessionListener &listener, ws::SessionMetaData md);
                virtual ~Session();

                virtual void send(ws::IOBuffer &data);
                virtual void setType(ws::SessionMessageType type);

                virtual void run() override;
                virtual void stop();
                virtual void close();
                virtual void close(ws::CloseCodes code);
        };
    }

#endif
