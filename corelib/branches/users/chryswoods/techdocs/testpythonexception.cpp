
#include <Python.h>

#include <boost/python.hpp>

#include <exception>

class TestException : public std::exception
{
public:
    TestException() : std::exception()
    {}

    ~TestException() throw()
    {}
};

void exception_translator( const TestException &exception )
{
    PyErr_SetString(PyExc_UserWarning,"TestException!!!");
}

void throwException()
{
    throw TestException();
}

void export_TestPythonException()
{
    boost::python::def( "throwException", &throwException );

    boost::python::register_exception_translator<TestException>(&exception_translator);
}

BOOST_PYTHON_MODULE( testpythonexception )
{
    export_TestPythonException();
}
