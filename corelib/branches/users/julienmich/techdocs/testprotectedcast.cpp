
#include <iostream>

using namespace std;

class A
{
public:
	A()
	{}
	
	virtual ~A()
	{}
};

class B : virtual protected A
{
public:
	B() : A()
	{}
	
	~B()
	{}
};

class C : virtual protected A
{
public:
	C() : A()
	{}
	
	~C()
	{}
};

class Base
{
public:
	Base()
	{}
	
	virtual ~Base()
	{}
};

class D : public Base, public B, public C
{
public:
	D() : Base(), B(), C()
	{}
	
	~D()
	{}
};

void test(const Base &base)
{
	const C *c = dynamic_cast<const C*>(&base);
	
	cout << c << endl;
}

int main(void)
{

	D d;

	test(d);	

	return 0;
}

