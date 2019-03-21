#if !defined  _RETURN_VALUE_EXCEPTION_HPP
    #define _RETURN_VALUE_EXCEPTION_HPP

    #include "exceptions/Exception.hpp"

    namespace exceptions {

        class ReturnValueException : public exceptions::Exception {

            public:
                ReturnValueException(String functionName, const int rcode);
                ReturnValueException(String functionName, String rcustom);
                virtual ~ReturnValueException();
        };
    }

#endif