#if !defined  WS_SESSION_HPP
    #define WS_SESSION_HPP

    #include "ws/ws.hpp"
    #include "runtime/LoopInside.hpp"

    namespace ws {

        class Session : public runtime::LoopInside {

            protected:

                ws::SessionListener * const listener;
                ws::WSocket socket;

                ws::SessionLifeCycle _readyState;
                ws::SessionMessageType _type;

                virtual void loop() override;

            public:

                ws::SessionLifeCycle const &readyState;
                ws::SessionMessageType const &type;

                Session(ws::WSocket &&socket, ws::SessionListener * const listener);
                virtual ~Session();

                virtual void send(IOBuffer &data);
                virtual void setType(ws::SessionMessageType type);

                virtual void run() override;
                virtual void stop();
                virtual void close();
                virtual void close(ws::CloseCodes code);
        };
    }

#endif
