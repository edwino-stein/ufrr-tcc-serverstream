#if !defined  _AV_VIDEO_FILE_INPUT_HPP
    #define _AV_VIDEO_FILE_INPUT_HPP

    #include "av/VideoInput.hpp"

    namespace av {

        class VideoFileInput : public av::VideoInput {
            
            protected:
                String fileName;

            public:

                VideoFileInput(String fileName);
                virtual ~VideoFileInput();

                virtual void init() override;
                virtual void init(JsonObject cfg) override;
                virtual void close() override;
                virtual void dump() const override;
        };
    }

#endif