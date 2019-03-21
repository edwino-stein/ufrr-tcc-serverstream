#if !defined  _AV_ERROR_EXCEPTION_HPP
    #define _AV_ERROR_EXCEPTION_HPP

    #include "exceptions/Exception.hpp"

    namespace exceptions {

        class AvErrorException : public exceptions::Exception {
            private:
                String getAvError(const int errnum) const;

            public:
                AvErrorException(const int errnum);
                AvErrorException(const int errnum, const exceptions::Exception &prev);
                virtual ~AvErrorException();
        };
    }

#endif