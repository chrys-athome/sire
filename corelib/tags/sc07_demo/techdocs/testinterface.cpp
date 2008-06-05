
#include <iostream>

#include <exception>

#include <boost/shared_ptr.hpp>

using namespace std;

class invalid_cast : public exception
{
public:
    invalid_cast() : exception()
    {}
    
    const char *what() const throw()
    {
        return "invalid_cast";
    }
};

template<class D>
class Value;

class ValueBase
{
public:
    ValueBase()
    {}
    
    virtual ~ValueBase()
    {}
    
    template<class D>
    void setResult(const D &value)
    {
        Value<D> *dval = dynamic_cast<Value<D>*>(this);
        if (dval)
            dval->r = value;
        else
            throw invalid_cast();
    }
    
};

typedef boost::shared_ptr<ValueBase> ValuePtr;

template<class D>
class Value : public ValueBase
{

friend class ValueBase;

public:
    explicit Value() : ValueBase()
    {}

    explicit Value(const D &value) : ValueBase(), r(value)
    {}
    
    ~Value()
    {}
    
    const D& value() const
    {
        return r;
    }
    
private:

    D r;
};  

template<class D>
class Result
{
public:
    Result() : dptr(new Value<D>()), v(dptr)
    {}

    explicit Result(const D &value) : dptr(new Value<D>(value)), v(dptr)
    {}
    
    Result(const Result<D> &value) : v(value.v)
    {}
    
    Result(const ValuePtr &ptr) : v(ptr)
    {
        //check sanity
        ValueBase *base = ptr.get();
        if (base)
        {
            dptr = dynamic_cast<Value<D>*>(base);
            if (not dptr)
                throw invalid_cast();
        }
    }
    
    ~Result()
    {}
    
    const D& value() const
    {
        return dptr->value();
    }
    
    operator D() const
    {
        return value();
    }

    operator ValuePtr() const
    {
        return v;
    }
    
private:

    Value<D> *dptr;
    ValuePtr v;
};

template<class T>
class Invoker
{
public:
    Invoker()
    {}
    
    ~Invoker()
    {}
    
    void call(int func, ValuePtr result)
    {
        switch(func)
        {
            case 1:
                callReturnInt(result);
                break;
            case 2: 
                callReturnDouble(result);
                break;
            default:
                throw "help";
        }
    }
    
private:
    
    void callReturnDouble(ValuePtr result)
    {
        result->setResult<double>( val.returnDouble() );
    }
    
    void callReturnInt(ValuePtr result)
    {
        result->setResult<int>( val.returnInt() );
    }
    
    T val;
};    

template<class T>
class Interface
{
public:
    Interface()
    {}
    
    ~Interface()
    {}
    
    template<class D>
    Result<D> call(int func)
    {
        Result<D> result;
        invoker.call(func, result);
        return result;
    }
    
    Result<double> returnDouble()
    {
        return call<double>(2);
    }
    
    Result<int> returnInt()
    {
        return call<int>(1);
    }
    
private:

    Invoker<T> invoker;
 
};

class A
{
public:
    A()
    {}
    
    ~A()
    {}
    
    double returnDouble()
    {
        return 5.4;
    }
    
    long returnInt()
    {
        return 424;
    }
};

int main(void)
{

    Interface<A> interface;
    
    Result<double> d = interface.call<double>(2);
    Result<int> i = interface.call<int>(1);
    
    cout << d << " " << i << endl;

    cout << interface.returnDouble() << " " << interface.returnInt() << endl;

}
