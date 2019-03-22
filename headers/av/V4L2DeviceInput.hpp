#if !defined  _AV_V4L2_DEVICE_INPUT_HPP
    #define _AV_V4L2_DEVICE_INPUT_HPP

    #include "av/VideoInput.hpp"

    namespace av {

        class V4L2DeviceInput : public av::VideoInput {
            
            protected:
                String device;

            public:

                V4L2DeviceInput(String device);
                virtual ~V4L2DeviceInput();

                virtual void init() override;
                virtual void init(JsonObject cfg) override;
                virtual void close() override;
                virtual void dump() const override;
        };
    }

#endif