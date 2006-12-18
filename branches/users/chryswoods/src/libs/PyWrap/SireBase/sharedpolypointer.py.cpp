
#include <boost/python.hpp>

#include "SireBase/sharedpolypointer.hpp"

#include <QDebug>

using namespace boost::python;

namespace SireBase
{

class SharedBase : public QSharedData
{
public:
    SharedBase() : QSharedData()
    {
        qDebug() << "SharedBase::SharedBase()";
    }
    
    SharedBase(const SharedBase&) : QSharedData()
    {
        qDebug() << "SharedBase::SharedBase(const SharedBase&)";
    }
    
    virtual ~SharedBase()
    {}
    
    virtual void helloWorld() const=0;
    
    virtual SharedBase* clone() const=0;
};

class SharedFoo : public SharedBase
{
public:
    SharedFoo() : SharedBase()
    {
        qDebug() << "SharedFoo::SharedFoo";
    }
    
    SharedFoo(const SharedFoo &other) :  SharedBase(other)
    {
        qDebug() << "SharedFoo::SharedFoo(const SharedFoo&)";
    }
    
    ~SharedFoo()
    {}
    
    void helloWorld() const
    {
        qDebug() << "Hello World from Foo!";
    }
    
    SharedFoo* clone() const
    {
        return new SharedFoo(*this);
    }
};

class SharedBar : public SharedBase
{
public:
    SharedBar() : SharedBase()
    {
        qDebug() << "SharedBar::SharedBar";
    }
    
    SharedBar(const SharedBar &other) :  SharedBase(other)
    {
        qDebug() << "SharedBar::SharedBar(const SharedBar&)";
    }
    
    ~SharedBar()
    {}
    
    void helloWorld() const
    {
        qDebug() << "Hello World from Bar!";
    }
    
    SharedBar* clone() const
    {
        return new SharedBar(*this);
    }
    
    void goodbye() const
    {
        qDebug() << "See ya!";
    }
};

SharedPolyPointer<SharedBase> createFoo()
{
    return SharedPolyPointer<SharedBase>( new SharedFoo() );
}

SharedPolyPointer<SharedBase> createBar()
{
    return SharedPolyPointer<SharedBase>( new SharedBar() );
}

void SIREBASE_EXPORT export_SharedPolyPointer()
{
    class_<SharedBase, boost::noncopyable>("SharedBase", no_init)
        .def( "helloWorld", &SharedBase::helloWorld )
    ;


    class_<SharedFoo, bases<SharedBase> >("SharedFoo", init<>() )
        .def( init<const SharedFoo&>() )
    ;
    
    class_<SharedBar, bases<SharedBase> >("SharedBar", init<>() )
        .def( init<const SharedBar&>() )
        .def( "goodbye", &SharedBar::goodbye )
    ;
    
    def( "createFoo", &createFoo );
    def( "createBar", &createBar );
    
    register_ptr_to_python< SharedPolyPointer<SharedBase> >();
}

}
