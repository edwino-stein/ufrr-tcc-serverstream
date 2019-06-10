#if !defined  _APPLICATION_HPP
    #define _APPLICATION_HPP

    #include "types.hpp"
    #include "ws/ServerListener.hpp"
    #include "ffmpeg/FfmpegListener.hpp"

    class Application : public ws::ServerListener,  public ffmpeg::FfmpegListener {

        private:

            bool loop;
            Mutex broadcastQueueMtx;
            Queue<ws::IOBuffer> broadcastQueue;
            bool isValidPort(const unsigned int port) const;

        public:

            Application();
            virtual ~Application();
            int main(int argc, char const *argv[]);

            //FFMPEG callback
            void onFfmpegReceive(unsigned char data[], const size_t length) override;

            //Websocket callback
            void onMessage(ws::Session &session, ws::IOBuffer &data) override;
            void onClose(ws::Session &session, const int code) override;
            void onError(ws::Session &session, exceptions::WsErrorException e) override;
            void onConnection(ws::Session &session) override;
            bool onIsAcceptable(ws::HTTPRequest &request) override;
            void onAccept(ws::HTTPWSResponse &response) override;
    };

#endif
