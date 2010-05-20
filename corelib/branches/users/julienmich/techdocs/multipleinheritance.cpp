
#include <iostream>

using namespace std;

class ParentBase
{
public:
    ParentBase()
    {}
    virtual ~ParentBase()
    {}
    
    void callParent() const
    {
        cout << "The parent is being called\n";
    }
};

class ChildBase
{
public:
    ChildBase() : prnt(0)
    {
        cout << "ChildBase constructor\n";
    }
    virtual ~ChildBase()
    {
        cout << "ChildBase destructor\n";
    }
    
    const ParentBase& parent() const
    {
        return const_cast<ChildBase*>(this)->parent();
    }
    
    ParentBase& parent()
    {
        if (not prnt)
        {
            //try to get the parent now...
            this->registerParent();
        
            if (prnt)
                return *prnt;
        
            cout << "I am not a parent!!!\n";
            throw "help!";
        }
        else
            return *prnt;
    }        

protected:
    void registerParent()
    {
        prnt = dynamic_cast<ParentBase*>(this);
        
        if (not prnt)
            cout << "I am not part of a parent!\n";
        else
            cout << "I am part of the parent :-)\n";
    }
    
    ParentBase *prnt;
};

class ChildA : public virtual ChildBase
{
public:
    ChildA() : ChildBase()
    {
        cout << "ChildA constructor\n";
    }
    ~ChildA()
    {
        cout << "ChildA destructor\n";
    }
};

class ChildB : public virtual ChildBase
{
public:
    ChildB() : ChildBase()
    {
        cout << "ChildB constructor\n";
    }
    ~ChildB()
    {
        cout << "ChildB destructor\n";
    }
};

class ChildC : public virtual ChildBase
{
public:
    ChildC() : ChildBase()
    {
        cout << "ChildC constructor\n";
    }
    ~ChildC()
    {
        cout << "ChildC destructor\n";
    }
};

class Parent : public ParentBase, public ChildA, public ChildB
{
public:
    Parent() : ParentBase(), ChildA(), ChildB()
    {
        cout << "Parent constructor\n";
    }
    
    ~Parent()
    {
        cout << "Parent destructor\n";
    }
};

class Parent2 : public ParentBase, public ChildA, public ChildC
{
public:
    Parent2() : ParentBase(), ChildA(), ChildC()
    {
        cout << "Parent2 constructor\n";
    }
    
    ~Parent2()
    {
        cout << "Parent2 destructor\n";
    }
};

void doStuffWithA(const ChildA &a)
{
    cout << "Doing stuff with an 'A' class!\n";
    a.parent().callParent();
}

void doStuffWithB(const ChildB &b)
{
    cout << "Doing stuff with a 'B' class!\n";
    b.parent().callParent();
}

void doStuffWithC(const ChildC &c)
{
    cout << "Doing stuff with a 'C' class!\n";
    c.parent().callParent();
}

void polymorph(const ParentBase &parent)
{
    const ChildA &a = dynamic_cast<const ChildA&>(parent);
    doStuffWithA(a);
    
    const ChildB &b = dynamic_cast<const ChildB&>(parent);
    doStuffWithB(b);
}

void failPolymorph(ParentBase &parent)
{
    try
    {
        ChildC &c = dynamic_cast<ChildC&>(parent);
        doStuffWithC(c);
    }
    catch( std::bad_cast )
    {
        cout << "This is not a type of C!\n";
    }
}

int main(void)
{
 
    Parent p;
    
    polymorph(p);
    
    cout << "Overload..\n";
    
    doStuffWithA(p);
    doStuffWithB(p);
 
    cout << "Fail...\n";
    failPolymorph(p);

    Parent2 p2;
    failPolymorph(p2);
 
    cout << "Overload2..\n";
    doStuffWithA(p2);
    doStuffWithC(p2);
    
    return 0;
}
