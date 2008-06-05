
#include <iostream>

using namespace std;

class Foo
{
public:
    Foo() : a(12), b(42), c(48)
    {}
    
    ~Foo()
    {}
    
    double A() const
    {
        return a;
    }
    
    double B() const
    {
        return b;
    }
    
    double C() const
    {
        return c;
    }

    void setA(double val)
    {
        a = val;
    }
    
private:
    double a, b, c;

};

class Bar
{
public:
    Bar()
    {}
    
    ~Bar()
    {}

    const Foo& top() const
    {
        return footop;
    }
    
    const Foo& bottom() const
    {
        return foobottom;
    }

private:
    Foo footop, foobottom;
};

class ContainerBase
{
public:
    ContainerBase(int size) : sz(size)
    {}
    
    ~ContainerBase()
    {}
    
    int size() const
    {
        return sz;
    }
    
    const Bar& bar() const
    {
        return b;
    }

private:
    int sz;
    Bar b;
};

class Container
{
public:

    const char* memory() const
    {
        return storage;
    }

    Foo* data()
    {
        return (Foo*)( memory() + sizeof(ContainerBase) );
    }

    const ContainerBase& container() const
    {
        return *((ContainerBase*)(memory()));
    }

    Container(int size)
    {
        cout << "Constructing..\n";
    
        storage = new char[ sizeof(ContainerBase) +
                            size * sizeof(Foo) ];
    
        new (storage) ContainerBase(size);
    
        Foo *array = data();
        
        for (int i=0; i<size; ++i)
        {
            new ( &(array[i]) ) Foo();
        }
        
        cout << "Done!\n";
    }
    
    Container(const Container &other)
    {
        cout << "Copying...\n";
        
        int size = sizeof(ContainerBase) +
                   other.size() * sizeof(Foo);
        
        storage = new char[size];
                            
        const char *other_storage = other.memory();
        
        for (int i=0; i<size; ++i)
        {
            storage[i] = other_storage[i];
        }
    }
    
    ~Container()
    {
        cout << "Deleting...\n";
        
        //invoke the destructors of all of 
        //the foos
        int sz = this->size();
        Foo *array = data();
        
        for (int i=sz-1; i>=0; --i)
        {
            array[i].~Foo();
        }
        
        //invoke the destructor of the container
        container().~ContainerBase();

        cout << "Deleting storage...\n";
        delete[] storage;
        cout << "Done!\n";
    }
    
    Foo& operator[](int i)
    {
        return data()[i];
    }
    
    int size() const
    {
       return container().size();
    }

private:
    char *storage;
};

int main(void)
{
    cout << sizeof(Foo) << endl;
    cout << sizeof(Bar) << endl;
    cout << sizeof(ContainerBase) << endl;

    cout << sizeof(Container) << endl;
    
    Container c(100);
    
    cout << c.size() << endl;
    cout << c.container().bar().top().A() << endl;
    cout << c.container().bar().bottom().A() << endl;
    cout << c[10].B() << endl;

    cout << &(c.container()) << " "
         << &(c[0]) << " "
         << &(c[1]) << " "
         << &(c[2]) << " "
         << &(c[3]) << " "
         << &(c[100]) << endl;

    Container c2(c);
    
    cout << c2.size() << endl;
    cout << c2.container().bar().top().A() << endl;
    cout << c2.container().bar().bottom().A() << endl;
    cout << c2[10].B() << endl;

    cout << &(c2.container()) << " "
         << &(c2[0]) << " "
         << &(c2[1]) << " "
         << &(c2[2]) << " "
         << &(c2[3]) << " "
         << &(c2[100]) << endl;
         
    return 0;
}
