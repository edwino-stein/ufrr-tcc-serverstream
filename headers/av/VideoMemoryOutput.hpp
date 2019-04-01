#if !defined  _AV_VIDEO_MEMORY_OUTPUT_HPP
    #define _AV_VIDEO_MEMORY_OUTPUT_HPP

    #include "av/VideoFileOutput.hpp"

    namespace av {

        using memoryWriteHandle = AFunction<int(uint8_t * const, const int, av::VideoMemoryOutput * const)>;

        class VideoMemoryOutput : public av::VideoFileOutput {

            protected:
                av::memoryWriteHandle callback;

                size_t avioBufferSize;
                uint8_t *avioBufferCtx;
                AVIOContext *_avioCtx;

                virtual void openFile(String &fileName, AVDictionary **options = NULL) override;
                virtual void closeFile() override;

            public:

                AVIOContext * const &avioCtx;

                VideoMemoryOutput(av::memoryWriteHandle callback);
                virtual ~VideoMemoryOutput();

                virtual void init() override;
                virtual void init(JsonObject cfg) override;

                static int avioWrite(void *opaque, uint8_t *buf, int buf_size);
        };
    }

#endif
