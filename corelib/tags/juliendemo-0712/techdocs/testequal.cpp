
#include <iostream>

using namespace std;

class A
{
public:
	A()
	{}
	
	virtual ~A()
	{}
	
	bool operator==(const A &other) const
	{
		return typeid(other).name() == typeid(*this).name();
	}

	bool operator!=(const A &other) const
	{
		return not operator==(other);
	}
};

class B : public A
{
public:
	B() : A()
	{}
	
	~B()
	{}
	
};

const char* test(bool val)
{
	if (val)
		return "true\n";
	else
		return "false\n";
}

int main(void)
{
	A a;
	B b;
	
	cout << test( a == a );
	cout << test( a == b );
	cout << test( b == b );
	cout << test( b == a );
	cout << test( a == (A&)b );
	cout << test( (A&)b == a );
	cout << test( (A&)b == (A&)b );
	
	cout << "Invert\n";
	
	cout << test( a != a );
	cout << test( a != b );
	cout << test( b != b );
	cout << test( b != a );
	cout << test( a != (A&)b );
	cout << test( (A&)b != a );
	cout << test( (A&)b != (A&)b );
	
	
	return 0;
}
