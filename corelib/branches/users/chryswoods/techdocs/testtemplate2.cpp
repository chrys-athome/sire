
#include "testtemplate.h"

template<>
class TestTemplate<float>
{
public:
    TestTemplate()
    {
        cout << "Special float version!\n";
    }
    
    ~TestTemplate()
    {}
};

void testTemplate2Main()
{
    TestTemplate<float> f; //creating f should use above constructor (doesn't on g++ 3.4)

//    f.hello();  // special float version has no 'hello' function
                  // so uncommenting this should cause a compile error

    TestTemplate<long> l;
    l.hello();
}
