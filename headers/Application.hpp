#if !defined  _APPLICATION_HPP
    #define _APPLICATION_HPP

    #include "types.hpp"
    #include "av/libav.hpp"
    #include "ws/ws.hpp"
    #include "ws/ServerListener.hpp"
    #include "runtime/UnixMessageQueueListener.hpp"

    #define QUEUE_MSG_BS 512
    #define JSON_BS 1024

    class Application : public ws::ServerListener {

        private:

            enum STREAM_STATE {
                STOPPED,
                RUNNING,
                ERROR
            };

            //Singleton setup
            static Application* instance;
            Application();
            Application(Application const&);
            Application& operator=(Application const&);

            ws::Server *server;
            av::V4L2DeviceInput *videoIn;
            av::VideoMemoryOutput *videoOut;
            av::Transcoder *transcoder;

            enum STREAM_STATE state = STREAM_STATE::STOPPED;
            runtime::UnixMessageQueueListener<QUEUE_MSG_BS> *mql;

            void initVideo(json::JsonObject cfg);
            void initWs(json::JsonObject cfg);

            void stopVideo();
            void stopWs();

            void onQueueMessage(String &msg);
            void onStart(json::JsonObject extra);

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
