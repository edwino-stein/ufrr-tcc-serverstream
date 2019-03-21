#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test suite exceptions
#include <boost/test/unit_test.hpp>

#include "av/libav.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/AvErrorException.hpp"
#include "exceptions/ReturnValueException.hpp"
#include "exceptions/InvalidJsonConfigException.hpp"

using namespace exceptions;

void throwException(const Exception &e){
    throw e;
}

bool checkExceptionOutput(const Exception &e){
    BOOST_TEST_MESSAGE("*********************** OUTPUT e.what() ***********************");
    BOOST_TEST_MESSAGE(e.what());
    BOOST_TEST_MESSAGE("***************************************************************");
    return true;
}

BOOST_AUTO_TEST_CASE(simple_exception){
    BOOST_CHECK_THROW(throwException(Exception("Test Exception")), Exception);
}

BOOST_AUTO_TEST_CASE(exception_output_format){
    BOOST_CHECK_EXCEPTION(throwException(Exception("Test Exception")), Exception, checkExceptionOutput);
}

BOOST_AUTO_TEST_CASE(exception_output_format_int){
    BOOST_CHECK_EXCEPTION(throwException(Exception(0)), Exception, checkExceptionOutput);
}

BOOST_AUTO_TEST_CASE(exception_output_format_std_exception){
    BOOST_CHECK_EXCEPTION(throwException(Exception(std::exception())), Exception, checkExceptionOutput);
}

BOOST_AUTO_TEST_CASE(exception_output_format_other_exeption){
    BOOST_CHECK_EXCEPTION(throwException(Exception("second exception", Exception("first exception"))), Exception, checkExceptionOutput);
}

BOOST_AUTO_TEST_CASE(av_error_exception_output_format){
    BOOST_CHECK_EXCEPTION(throwException(AvErrorException(-2)), Exception, checkExceptionOutput);
}

BOOST_AUTO_TEST_CASE(av_error_exception_output_format_other_exeption){
    BOOST_CHECK_EXCEPTION(throwException(AvErrorException(-2, ReturnValueException("testfunc", -2))), Exception, checkExceptionOutput);
}

BOOST_AUTO_TEST_CASE(av_error_exception_invalid_json){
    BOOST_CHECK_EXCEPTION(throwException(InvalidJsonConfigException("theKey", "expectedValue")), Exception, checkExceptionOutput);
}

BOOST_AUTO_TEST_CASE(av_error_exception_invalid_json2){
    BOOST_CHECK_EXCEPTION(throwException(InvalidJsonConfigException("theKey", "expectedType", "otherType")), Exception, checkExceptionOutput);
}
