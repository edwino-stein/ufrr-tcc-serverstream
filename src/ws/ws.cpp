#include "ws/ws.hpp"

#include <sstream>

ws::IOBuffer ws::stringToIOBuffer(String str){
    ws::IOBuffer buffer;
    boost::beast::ostream(buffer) << str;
    return buffer;
}

ws::IOBuffer ws::bytesToIoBuffer(unsigned char data[], const size_t length){

    ws::IOBuffer buffer;
    boost::asio::const_buffer cbf = boost::asio::buffer(data, length);
    size_t n = boost::asio::buffer_copy(buffer.prepare(length), cbf);
    buffer.commit(n);

    return buffer;
}

String ws::ioBufferToString(ws::IOBuffer &buffer){
    std::ostringstream oss;
    oss << boost::beast::make_printable(buffer.data());
    return oss.str();
}
