#if !defined  _AV_FORMAT_INPUT_HPP
    #define _AV_FORMAT_INPUT_HPP

    #include "av/Format.hpp"

    namespace av {

        class FormatInput : public av::Format {

            protected:
                AVInputFormat *_format;

            public:

                AVInputFormat * const &format;

                FormatInput();
                virtual ~FormatInput();

                virtual void read(AVPacket *packet) const;
        };
    }

#endif