#if !defined  _WS_HPP
    #define _WS_HPP

    #include "types.hpp"
    #include "boost/beast.hpp"

    namespace ws {

        //Boost.beast alias types
        namespace http = boost::beast::http;
        namespace websocket = boost::beast::websocket;
        using tcp = boost::asio::ip::tcp;
        using TCPSocket = tcp::socket;
        using WSocket = boost::beast::websocket::stream<tcp::socket>;
        using IOBuffer = boost::beast::multi_buffer;
        using IOContext = boost::asio::io_context;
        using EndPoint = tcp::endpoint;
        using Acceptor = tcp::acceptor;
        using HTTPRequest = http::request<http::string_body>;
        using HTTPWSResponse = boost::beast::websocket::response_type;
        using FlatBuffer = boost::beast::flat_buffer;

        //Enums
        enum class SessionLifeCycle {
            CONNECTING,
            OPEN,
            CLOSING,
            CLOSED
        };

        enum class SessionMessageType {
            TEXT,
            BINARY
        };

        //Functions

        //Classes
        class Session;

        //Interfaces
        class SessionListener;

    }

#endif
