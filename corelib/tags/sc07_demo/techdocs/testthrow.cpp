
#include <iostream>
#include <exception>

using namespace std;

class My_exception : public exception
{
public:

   My_exception() : exception()
   {}
   
   ~My_exception() throw()
   {}
   
   const char* what() const throw()
   {
      return "My_exception";
   }
};

int main(void)
{

      int a = 42;

      int old_a = a;

      try
      {

      try
      {
          cout << "Changing state...\n";
          a = 94;
          
          throw My_exception();
      }
      catch(...)
      {
          cout << "Caught an error - resetting state!\n";
          a = old_a;
          
          throw;
      }
      
      }
      catch(...)
      {
          cout << "A should equal 42: It equals " << a << endl;
          cout << "You should now terminate with a 'My_exception'\n";
          throw;
      }

      return 0;
}
