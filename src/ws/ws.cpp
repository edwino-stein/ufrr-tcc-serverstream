#include "ws/ws.hpp"

#include <sstream>

ws::IOBuffer ws::stringToIOBuffer(String str){
    ws::IOBuffer buffer;
    boost::beast::ostream(buffer) << str;
    return buffer;
}


String ws::ioBufferToString(ws::IOBuffer &buffer){
    std::ostringstream oss;
    oss << boost::beast::buffers(buffer.data());
    return oss.str();
}
