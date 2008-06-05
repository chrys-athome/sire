
#include <iostream>

using namespace std;

#include <boost/shared_ptr.hpp>

template<class T>
class Wrapper : private boost::shared_ptr<T>
{

template<class D>
friend class Wrapper;

public:
    Wrapper() : boost::shared_ptr<T>(new T())
    {}
    
    Wrapper(T *ptr) : boost::shared_ptr<T>(ptr)
    {}
    
    Wrapper(const char *text) : boost::shared_ptr<T>(new T(text))
    {}
    
    Wrapper(const Wrapper<T> &other) : boost::shared_ptr<T>(other)
    {}
    
    template<class D>
    Wrapper(const Wrapper<D> &other) : boost::shared_ptr<T>(other)
    {}
    
    ~Wrapper()
    {}
    
    bool isValid() const
    {
        return boost::shared_ptr<T>::get() != 0;
    }
    
    T* operator->()
    {
        if (this->isValid())
            return boost::shared_ptr<T>::operator->();
        else
            throw "is null";
    }
    
    template<class D>
    Wrapper<D> dynamicCast() const
    {
        return Wrapper<D>(boost::dynamic_pointer_cast<D>(*this));
    }
    
    operator T*()
    {
        return this->get();
    }
    
private:

    Wrapper(const boost::shared_ptr<T> &other) : boost::shared_ptr<T>(other)
    {}
};
  
class SharedBase
{
public:
    SharedBase() : t(0)
    {}

    SharedBase(const char *text) : t(text)
    {}
    
    virtual ~SharedBase()
    {}
    
    virtual void printHello()
    {
        cout << "Hello World!\n";
        if (t)
            cout << t << endl;
    }

private:

    const char *t;
};

class HigherBase : public SharedBase
{
public:
    HigherBase() : SharedBase()
    {}
    
    ~HigherBase()
    {}
    
    void printHello()
    {
        cout << "Hello Higher!\n";
    }
};

typedef Wrapper<SharedBase> Shared;
typedef Wrapper<HigherBase> Higher;

void print(SharedBase *ptr)
{
    if (ptr)
        ptr->printHello();
}

int main(void)
{
    Shared s;
    
    s->printHello();

    Higher h;
    h->printHello();
    
    s = h;

    s->printHello();

    h = s.dynamicCast<HigherBase>();

    h->printHello();
    
    s = Shared();
    s->printHello();
    
    h = s.dynamicCast<HigherBase>();

    if (h.isValid())
        h->printHello();

    cout << "Implicit conversion...\n";

    print(s);
    print(h);

    h = Higher();
    
    print(h);

    cout << "Different constructor...\n";
    
    s = Shared("what is happening?");
    print(s);

    return 0;
}          
