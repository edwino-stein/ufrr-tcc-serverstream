#if !defined  WS_SESSION_META_DATA_HPP
    #define WS_SESSION_META_DATA_HPP

    #include "ws/ws.hpp"

    namespace ws {

        class SessionMetaData {

            protected:

                String _userAgent;
                String _secWsKey;
                String _clientIp;


            public:

                String const &userAgent;
                String const &secWsKey;
                String const &clientIp;

                SessionMetaData(ws::WSocket &socket, ws::HTTPRequest &request);
                SessionMetaData(const ws::SessionMetaData& md);
                virtual ~SessionMetaData();

                String toString() const;
        };
    }

#endif
