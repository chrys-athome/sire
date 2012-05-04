
#include <iostream>

using namespace std;

enum Test { A = 0x0001,
            B = 0x0002
          };
        
enum Test2 { I = 0x0001,
             J = 0x0002
           };        
          
template<typename T>
class EnumTemplate
{
public:
    explicit EnumTemplate(T t) : val(t)
    {}

    explicit EnumTemplate(int t) : val(t)
    {}
    
    ~EnumTemplate()
    {}

    bool operator==(const T &v) const
    {
        return val == v;
    }

protected:

    int val;
};

int main(void)
{
    EnumTemplate<Test> TestEnum( A );
    
    if ( TestEnum == A )
        cout << "I am an A\n";
    
    if ( not (TestEnum == B) )
        cout << "I am not a B\n";
    
    if ( TestEnum == B )
        cout << "ERROR!\n";
    
    EnumTemplate<Test2> Test2Enum( I );
    
    //this test below should cause a compile error!
    if (Test2Enum == A)
        cout << "Error!\n";
    
    return 0;

}
