#if !defined  _EXCEPTION_HPP
    #define _EXCEPTION_HPP

    #include "types.hpp" 
    #include <exception>
    #include <boost/stacktrace.hpp>

    namespace exceptions {

        class Exception {

            protected:

                String _message;
                Exception *_prev;
                boost::stacktrace::stacktrace st;
                size_t stIgnoreBottom;
                size_t stIgnoreTop;

                virtual void init(String message, Exception *prev, const size_t stib, const size_t stit);
                virtual String decodeST() const;


            public:

                String const &message;
                Exception * const &prev;

                Exception();
                Exception(const Exception &e);
                Exception(String message);
                Exception(String message, const Exception &prev);
                Exception(const std::exception &e);
                Exception(int e);

                virtual ~Exception();

                virtual String what() const;


        };
    }

#endif