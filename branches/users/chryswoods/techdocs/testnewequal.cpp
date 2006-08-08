
#include <iostream>

using namespace std;

class B
{
public:
	B()
	{
		cout << "I am B!\n";
	}
	
	~B()
	{}
};

class A
{
public:

	A()
	{}
	
	~A()
	{}
	
	B operator==(double val)
	{
		cout << val << endl;
		return B();
	}
	
};

int main(void)
{
	A a;
	
	a == 3.4;
	
	return 0;
}
