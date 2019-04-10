#if !defined  _WS_ERROR_EXCEPTION_HPP
    #define _WS_ERROR_EXCEPTION_HPP

    #include "exceptions/Exception.hpp"
    #include <boost/system/system_error.hpp>

    namespace exceptions {

        class WsErrorException : public exceptions::Exception {
            private:
                const boost::system::system_error wsException;
                String getWsError(const boost::system::system_error &wsException) const;

            public:
                WsErrorException(const boost::system::system_error wsException);
                virtual ~WsErrorException();

                const boost::system::system_error &getWsException() const;
        };
    }

#endif
