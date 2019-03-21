#if !defined  _AV_VIDEO_FILE_OUTPUT_HPP
    #define _AV_VIDEO_FILE_OUTPUT_HPP

    #include "av/VideoOutput.hpp"

    namespace av {

        class VideoFileOutput : public av::VideoOutput {
            
            protected:
                String fileName;

            public:

                VideoFileOutput(String fileName);
                virtual ~VideoFileOutput();

                virtual void init() override;
                virtual void init(JsonObject cfg) override;
                virtual void close() override;
                virtual void dump() const override;
        };
    }

#endif