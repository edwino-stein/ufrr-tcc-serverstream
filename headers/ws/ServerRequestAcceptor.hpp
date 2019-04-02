#if !defined  WS_SERVER_REQUEST_ACCEPTOR_HPP
    #define WS_SERVER_REQUEST_ACCEPTOR_HPP

    #include "ws/ServerCallback.hpp"

    namespace ws {

        class ServerRequestAcceptor : public ws::ServerCallback {

            protected:

                ws::IOContext ioCtx;
                ws::EndPoint endPoint;
                ws::Acceptor acceptor;

                virtual bool isAcceptable(ws::HTTPRequest &request) const;
                virtual void onAccept(ws::HTTPWSResponse &response) const;
                virtual ws::Session *accept();
                virtual void close();

            public:

                ServerRequestAcceptor(const int port, ws::ServerListener * const listener);
                virtual ~ServerRequestAcceptor();
        };
    }

#endif
