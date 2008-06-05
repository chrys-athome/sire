
#include <iostream>

using namespace std;

class Base
{
public:
   Base()
   {}
   
   virtual ~Base()
   {}
   
   virtual Base& operator=(const Base &other)=0;
};

class Derived : public Base
{
public:
   Derived() : Base()
   {}
   
   ~Derived()
   {}
   
   Derived& operator=(const Derived &other)
   {
       cout << "Derived::operator=(const Derived &other)\n";
       return *this;
   }
   
   Base& operator=(const Base &other)
   {
       cout << "Derived::operator=(const Base &other)\n";
       
       const Derived *d_other = dynamic_cast<const Derived*>(&other);
       
       if (d_other)
          return this->operator=(*d_other);
       else
          return *this;
   }
   
};


int main(void)
{
   Derived d0, d1;
   
   d0 = d1;

   return 0;
}
