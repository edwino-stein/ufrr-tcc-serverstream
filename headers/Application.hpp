#if !defined  _APPLICATION_HPP
    #define _APPLICATION_HPP

    #include "types.hpp"
    #include "av/libav.hpp"
    #include "ws/ws.hpp"
    #include "ws/ServerListener.hpp"

    class Application : public ws::ServerListener {

        private:
            //Singleton setup
            static Application* instance;
            Application();
            Application(Application const&);
            Application& operator=(Application const&);

            ws::Server *server;

            av::V4L2DeviceInput *videoIn;
            av::VideoMemoryOutput *videoOut;
            av::Transcoder *transcoder;

            void initVideo(json::JsonObject cfg);

        public:
            virtual ~Application();

            static Application& app();
            int main(int argc, char const *argv[]);

            void start();
            void stop();

            //Websocket callback
            void onMessage(ws::Session * const session, ws::IOBuffer &data) override;
            void onClose(ws::Session * const session, const int code) override;
            void onError(ws::Session * const session, exceptions::WsErrorException e) override;
            void onConnection(ws::Session * const session) override;
            bool onIsAcceptable(ws::HTTPRequest &request) override;
            void onAccept(ws::HTTPWSResponse &response) override;
    };

#endif
