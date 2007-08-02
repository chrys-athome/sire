
#include <iostream>

using namespace std;

class Base
{
public:
    Base()
    {}
    
    Base(float f, const int &i = 0)
    {
        cout << f << " " << i << endl;
    }
    
    Base(const Base&)
    {
        cout << "Base::Base(const Base&)\n";
    }
    
    virtual ~Base()
    {}
    
    virtual Base* clone() const=0;
    virtual Base& copy(const Base &other)=0;
    virtual bool compare(const Base &other) const=0;
    
    virtual const char* what() const=0;
    
    virtual Base& operator=(const Base &other)
    {
        cout << "Base::operator=(const Base&)\n";
        return *this;
    }
    
    virtual bool operator==(const Base &other) const
    {
        cout << "Base::operator==(const Base&) const\n";
        return false;
    }
    
    virtual bool operator!=(const Base &other) const
    {
        cout << "Base::operator==(const Base&) const\n";
        return false;
    }
};

template<class Derived, class Base>
class Leaf : public Base
{
public:
    Leaf() : Base()
    {}
    
    Leaf(const Leaf &other) : Base(other)
    {
        cout << "Leaf::Leaf(const Leaf&)\n";
    }

    template<class T0>
    Leaf(const T0 &t0) : Base(t0)
    {
        cout << "Leaf::Leaf(const T&)\n";
    }
    
    template<class T0, class T1>
    Leaf(const T0 &t0, const T1 &t1) : Base(t0,t1)
    {}
    
    ~Leaf()
    {}

    const char* what() const
    {
        return Derived::typeName();
    }
    
    Leaf<Derived,Base>* clone() const
    {
        return new Derived( dynamic_cast<const Derived&>(*this) );
    }
    
    Leaf<Derived,Base>& copy(const Base &other)
    {
        return Leaf<Derived,Base>::operator=(other);
    }
    
    bool compare(const Base &other) const
    {
        return Leaf<Derived,Base>::operator==(other);
    }
    
    Leaf<Derived,Base>& operator=(const Base &other)
    {
        cout << "Leaf::operator=(const Base&)\n";
    
        const Derived* other_t = dynamic_cast<const Derived*>(&other);
        
        if (!other_t)
        {
           cout << "Cannot copy incompatible classes!!! "
                << typeid(*this).name() << " " 
                << typeid(other).name() << endl;
        }
        else
        {
            static_cast<Derived*>(this)->operator=(*other_t);
        }
        
        return *this;
    }
    
    bool operator==(const Base &other) const
    {
        cout << "Leaf::operator==(const Base&) const\n";

        const Derived* other_t = dynamic_cast<const Derived*>(&other);
        
        if (other_t)
            return static_cast<const Derived*>(this)->operator==(*other_t);
        else
            return false;
    }

    bool operator!=(const Base &other) const
    {
        cout << "Leaf::operator!=(const Base&) const\n";

        const Derived* other_t = dynamic_cast<const Derived*>(&other);
        
        if (other_t)
            return static_cast<const Derived*>(this)->operator!=(*other_t);
        else
            return true;
    }

protected:

    Leaf<Derived,Base>& operator=(const Leaf<Derived,Base> &other)
    {
        cout << "Leaf::operator=(const Leaf&)\n";
        Base::operator=(other);
        return *this;
    }
};

class Derived : public Leaf<Derived, Base>
{
public:
    Derived() : Leaf<Derived,Base>()
    {}
    
    Derived(float f) : Leaf<Derived,Base>(f)
    {}
    
    Derived(const Derived &other) : Leaf<Derived,Base>(other)
    {
        cout << "Derived::Derived(const Derived&)\n";
    }
    
    ~Derived()
    {}
    
    using Base::operator=;
    using Base::operator==;
    using Base::operator!=;
    
    static const char* typeName()
    {
        return "Derived";
    }

    Derived& operator=(const Derived &other)
    {
        cout << "Derived::operator=(const Derived&)\n";
        Leaf<Derived,Base>::operator=(other);
        
        return *this;
    }
    
    bool operator==(const Derived &other) const
    {
        cout << "Derived::operator==(const Derived&) const\n";
        return true;
    }
    
    bool operator!=(const Derived &other) const
    {
        cout << "Derived::operator!=(const Derived&) const\n";
        return false;
    }
};

class Derived2 : public Leaf<Derived2, Base>
{
public:
    Derived2() : Leaf<Derived2,Base>()
    {}
    
    Derived2(float f, const int &i) : Leaf<Derived2,Base>(f,i)
    {}
    
    Derived2(const Derived2 &other) : Leaf<Derived2,Base>(other)
    {}
    
    ~Derived2()
    {}
    
    static const char* typeName()
    {
        return "Derived2";
    }
    
    using Base::operator=;
    using Base::operator==;
    using Base::operator!=;

    Derived2& operator=(const Derived2 &other)
    {
        cout << "Derived2::operator=(const Derived2&)\n";
        return *this;
    }
    
    bool operator==(const Derived2 &other) const
    {
        cout << "Derived2::operator==(const Derived2&) const\n";
        return true;
    }
    
    bool operator!=(const Derived2 &other) const
    {
        cout << "Derived2::operator!=(const Derived2&) const\n";
        return false;
    }
};

void copy(Base &base0, const Base &base1)
{
    base0 = base1;
    
    cout << (base0 == base1) << endl;
    cout << (base0 != base1) << endl;

    base0.copy(base1);
    
    cout << base0.compare(base1) << endl;
    
    cout << "Clone\n";
    base0.clone();
    
    cout << "Who are you?\n";
    cout << base0.what() << endl;
    cout << base1.what() << endl;
}

int main(void)
{
    Derived d0, d1(42);
    
    copy(d0, d1);
    
    Derived2 d2, d3(35,23);
    
    copy(d2, d3);
    
    copy(d0, d2);
    
    d0 = d1;
    
    cout << (d0 == d1) << endl;
    cout << (d0 != d1) << endl;
    
    d2 = d3;
    
    cout << (d2 == d3) << endl;
    cout << (d2 != d3) << endl;
    
    d0 = d2;
    
    cout << (d0 == d2) << endl;
    cout << (d0 != d2) << endl;
    
    d2 = d0;
    
    cout << (d2 == d0) << endl;
    cout << (d2 != d0) << endl;
    
    return 0;
}
