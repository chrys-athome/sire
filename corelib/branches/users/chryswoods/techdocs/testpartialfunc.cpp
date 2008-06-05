
#include <iostream>

using namespace std;

class A
{
public:
    A()
    {}

    ~A()
    {}

    template<class T>
    void print(T value);
    
    template<class T>
    T get() const;

};

template<>
void A::print<int>(int value);

template<>
int A::get<int>() const;

template<class T>
void A::print(T value)
{
    cout << value << endl;
}

template<class T>
T A::get() const
{
    cout << typeid(T).name() << endl;
    return T();
}

template<>
void A::print<int>(int value)
{
   cout << "INT " << value << endl;
}

template<>
int A::get<int>() const
{
   cout << "** INT **\n";
   return 42;
}

int main(void)
{
    A a;

    a.print("hello world!");

    a.print(42);
 
    cout << a.get<int>() << endl;
    cout << a.get<double>() << endl;

    return 0;
}
