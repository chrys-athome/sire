
#include <iostream>

using namespace std;

typedef int MyInt1;
typedef int MyInt2;

void print(MyInt1 myint1)
{
    cout << "MyInt1 == " << myint1 << endl;
}

void print(MyInt2 myint2)
{
    cout << "MyInt2 == " << myint2 << endl;
}

//code should not compile as *cannot* overload using typedefs!!!

//This is an important lesson to learn as I have been working on 
//the assumption that I could overload using typedefs
//(e.g. selecing nAtoms(CutGroupID) vs. nAtoms(ResNum)
//  - which only works a CutGroupID is uint, while ResNum is int)

//( I need to convert these to custom classes.... )

int main(void)
{

    MyInt1 myint1 = 42;
    MyInt2 myint2 = 23;
    
    print(myint1);
    print(myint2);

    return 0;
}
