#ifndef TESTTEMPLATE_H
#define TESTTEMPLATE_H

#include <iostream>
using namespace std;

void testTemplate2Main();

template<class T>
class TestTemplate
{
public:
    TestTemplate()
    {
        cout << typeid(T()).name() << endl;
    }
    
    ~TestTemplate()
    {}
    
    void hello() const
    {
        cout << "Hello World\n";
    }
};
    
#endif
