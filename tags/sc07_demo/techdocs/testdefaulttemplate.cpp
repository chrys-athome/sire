
#include <iostream>

using namespace std;

class Foo
{
public:
      Foo()
      {}
      
      ~Foo()
      {}
      
      template<class T>
      T value() const;
      
      int value() const
      {
          return 42;
      }
};

template<>
double Foo::value() const
{
    return 3.5;
}

template<>
bool Foo::value() const
{
    return false;
}

template<class T>
T Foo::value() const
{
    return T();
}

int main(void)
{
    Foo f;
    
    cout << f.value() << endl;
    
    cout << f.value<double>() << endl;
    
    cout << f.value<bool>() << endl;
    
    return 0;
}
