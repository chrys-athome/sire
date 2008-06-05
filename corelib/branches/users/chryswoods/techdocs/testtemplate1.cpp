
#include "testtemplate.h"

#include "testtemplate2.cpp"

int main(void)
{
    TestTemplate<float> f;
    //f.hello();
    
    TestTemplate<long> l;
    l.hello();
    
    testTemplate2Main();

}
