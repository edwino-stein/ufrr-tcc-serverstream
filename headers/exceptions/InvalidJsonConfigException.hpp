#if !defined  _INVALID_JSON_CONFIG_EXCEPTION_HPP
    #define _INVALID_JSON_CONFIG_EXCEPTION_HPP

    #include "exceptions/Exception.hpp"

    namespace exceptions {

        class InvalidJsonConfigException : public exceptions::Exception {

            public:
                InvalidJsonConfigException(String key, String expected, String butType);
                InvalidJsonConfigException(String key, String expected);
                virtual ~InvalidJsonConfigException();
        };
    }

#endif