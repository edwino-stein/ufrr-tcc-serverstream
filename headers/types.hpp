#if !defined  _TYPES_HPP
    #define _TYPES_HPP

    #include <string>
    #include <vector>
    #include <functional>
    #include <mutex>
    #include <queue>

    #include <boost/asio.hpp>

    //Submodulos
    #include "ArduinoJson.h"

    using String = std::string;
    template <class T> using Vector = std::vector<T>;
    template <class T> using AFunction = std::function<T>;
    using Mutex = std::mutex;
    template <class T> using Queue = std::queue<T>;
    namespace json = ArduinoJson;

    //Boost.asio alias types
    namespace net {
        using tcp = boost::asio::ip::tcp;
        using IOContext = boost::asio::io_context;
        using Acceptor = tcp::acceptor;
        using EndPoint = tcp::endpoint;
        using TCPSocket = tcp::socket;
        using TcpResolver = tcp::resolver;
    };

#endif
