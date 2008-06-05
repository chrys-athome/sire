
#include <iostream>

using namespace std;

template<class T>
class Foo
{
public:
   Foo()
   {}
   
   ~Foo()
   {}
   
   template<class V>
   static V get()
   {
       cout << "Calling Foo<" << typeid(T).name() << ">::get<"
            << typeid(V).name() << ">() const\n";
	    
       return V();
   }

   static int getInt()
   {
       cout << "Calling Foo<" << typeid(T).name() << ">::getInt()\n";
	    
       return 42;
   }
};

template<class V>
V get(const Foo<double>&)
{
   cout << "Calling get<double, " << typeid(V).name() << ">()\n";
   return 42;
}

template<class T>
class Bar
{
public:
    Bar()
    {}
    
    ~Bar()
    {}
    
    template<class V>
    V get()
    {
	return ::get<V>(Foo<T>());
    }
};

int main(void)
{
    Foo<double> foo;

    cout << foo.get<int>() << endl;

    Bar<double> bar;
    
    cout << bar.get<int>() << endl;

    return 0;
}

