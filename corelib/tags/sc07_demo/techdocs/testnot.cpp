
#include <iostream>

using namespace std;

class AS;

class A
{
public:
	A()
	{}
	
	~A()
	{}
	
	A operator!() const
	{
		cout << "Returning not A\n";
		return A();
	}
	
	AS operator&&(const A&) const;
};

class AS
{
public:
	AS()
	{}
	
	AS(const AS&)
	{}
	
	AS(const A&)
	{}
	
	~AS()
	{}
	
	AS operator!() const
	{
		cout << "not AS\n";
		return AS();
	}
	
	AS operator&&(const AS&) const
	{
		cout << "AS and AS\n";
		return AS();
	}
};

AS A::operator&&(const A &) const
{
	cout << "A and A\n";
	return AS();
}

int main(void)
{
	A a;
	
	A a2 = not a;
	
	A a3 = !a;

	AS a4 = a and not a2;
	AS a5 = a && a3;

	AS a6 = a and a2 and a3;

	AS a7 = not ( a and a2 and a3 );

	return 0;
}
