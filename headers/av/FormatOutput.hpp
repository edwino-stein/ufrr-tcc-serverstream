#if !defined  _AV_FORMAT_OUTPUT_HPP
    #define _AV_FORMAT_OUTPUT_HPP

    #include "av/Format.hpp"

    namespace av {

        class FormatOutput : public av::Format {

            protected:
                AVOutputFormat *_format;

            public:

                AVOutputFormat * const &format;

                FormatOutput();
                virtual ~FormatOutput();

                virtual void write(AVPacket *packet) const;
        };
    }

#endif