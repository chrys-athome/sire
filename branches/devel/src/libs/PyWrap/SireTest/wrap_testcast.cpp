
/**
  * This file contains the boost::python wrapping of some test classes that are 
  * used to test the dynamic casting capabilities of boost::python.
  *
  * boost::python has a very excellent capability; if you return a derived class
  * via a pointer to the base class, then the pointer is automatically cast
  * into a python object reference to the derived class :-)
  *
  * This means that you can create factory functions in C++ that return pointers
  * to the common base class, and python will automatically see these created objects
  * as their correct types (as long as all of the types have been registered via
  * boost::python class_)
  *
  * This file contains four test classes (TestA -> TestD) and various functions
  * that either create them via a factory and specification (createType()), or
  * member functions that are specific to each type (TestA::functionA(),
  * TestD::functionD()). There is also a virtual function.
  *
  * Play around with the resulting classes and verify that you do indeed see this
  * very useful behaviour :-)
  *
  * There is a test script (test/python/test_python_cast.py) which uses these classes
  * and demonstrates/tests that the casting works.
  *
  * This behaviour is completely relied on by the FFWrapper class, as there is 
  * complete FFWrapper class hierarchy, and the FFWrappers are returned to 
  * python via boost::shared_ptr<FFWrapper>. This excellent dynamic casting 
  * means that even though the FFWrappers are returned via FFWrapper pointers, 
  * they are automatically available as their correct types, e.g. the user 
  * can write python code such as;
  *
  * sys.forcefield().component("CLJ").setCombineFunc( OPLSCombiningRules() )
  * sys.forcefield().component("QM").setMaxIterations(500)
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include <QDebug>

#include <boost/shared_ptr.hpp>

#include "SireTest/runtests.h"

using namespace boost::python;

namespace SireTest
{

class TestA
{
public:
    TestA() : v(0)
    {}
    
    TestA(int val) : v(val)
    {}
        
    virtual ~TestA()
    {}
    
    virtual void printMe()
    {
        qDebug() << QObject::tr("The value in TestA is %1").arg(v);
    }

    void functionA()
    {
        qDebug() << QObject::tr("This is a function in A");
    }

protected:
    
    int v;
};

class TestB : public TestA
{
public:
    TestB() : TestA()
    {}
    
    TestB(int val) : TestA(val)
    {}
    
    ~TestB()
    {}
    
    void printMe()
    {
        qDebug() << QObject::tr("The value in TestB is %1").arg(v);
    }

    void functionB()
    {
        qDebug() << QObject::tr("This is a function in B");
    }
};

class TestC : public TestA
{
public:
    TestC() : TestA()
    {}
    
    TestC(int val) : TestA(val)
    {}
    
    ~TestC()
    {}
    
    void printMe()
    {
        qDebug() << QObject::tr("The value in TestC is %1").arg(v);
    }
    
    void functionC()
    {
        qDebug() << QObject::tr("This is a function in C");
    }
};

class TestD : public TestB
{
public:
    TestD() : TestB()
    {}
    
    TestD(int val) : TestB(val)
    {}
    
    ~TestD()
    {}
    
    void printMe()
    {
        qDebug() << QObject::tr("The value in TestD is %1").arg(v);
    }
    
    void functionD()
    {
        qDebug() << QObject::tr("This is a function in D");
    }
};

boost::shared_ptr<TestA> castToA(const boost::shared_ptr<TestB> &ptr)
{
    return ptr;
}

boost::shared_ptr<TestA> createB()
{
    return boost::shared_ptr<TestA>(new TestB(5));
}

boost::shared_ptr<TestA> createC()
{
    return boost::shared_ptr<TestA>(new TestC(3));
}

boost::shared_ptr<TestA> createD()
{
    return boost::shared_ptr<TestA>(new TestD(6));
}

boost::shared_ptr<TestA> createType(const QString &typ, int value)
{
    if (typ == "A")
        return boost::shared_ptr<TestA>(new TestA(value));
    else if (typ == "B")
        return boost::shared_ptr<TestA>(new TestB(value));
    else if (typ == "C")
        return boost::shared_ptr<TestA>(new TestC(value));
    else if (typ == "D")
        return boost::shared_ptr<TestA>(new TestD(value));
    else
        return boost::shared_ptr<TestA>();
}

void  
SIRETEST_EXPORT
export_TestCast()
{
    def("castToA", &castToA);

    def("createB", &createB);
    def("createC", &createC);
    def("createD", &createD);

    def("createType", &createType);

    class_<RunTests>("RunTests", init<>())
                .def("run", &RunTests::run)
    ;
    
    class_<TestA, boost::shared_ptr<TestA> >("TestA", init<>())
             .def( init<int>() )
             .def( "printMe", &TestA::printMe )
             .def( "functionA", &TestA::functionA )
    ;
    
    class_<TestB, boost::shared_ptr<TestB>, bases<TestA> >("TestB", init<>())
                         .def( init<int>() )
                         .def( "functionB", &TestB::functionB )
    ;
    
    class_<TestC, boost::shared_ptr<TestC>, bases<TestA> >("TestC", init<>())
                         .def( init<int>() )
                         .def( "functionC", &TestC::functionC )
    ;
    
    class_<TestD, boost::shared_ptr<TestD>, bases<TestB> >("TestD", init<>())
                         .def( init<int>() )
                         .def( "functionD", &TestD::functionD )
    ;
}

}
