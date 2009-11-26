
#include <iostream>

using namespace std;

template<class T>
class Container;

class Contents;

typedef Container<Contents> MyContainer;

void sortContents(const MyContainer &c);

typedef Container<Contents> MyContainer;

template<class T>
class Container
{
public:
    Container()
    {}
    
    ~Container()
    {}
    
    void hello() const
    {
        T::hello();
    }
};

class Contents
{
public:
    static void hello()
    {
        cout << "Hello World!\n";
    }
};

void sortContents(const MyContainer &c)
{
    c.hello();
}

int main(void)
{
    MyContainer c;
    
    sortContents(c);
}
