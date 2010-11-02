
#include <iostream>

using namespace std;

class A
{
public:
	A(int val=0) : i(val)
	{}
	
	A(const A &other) : i(other.i)
	{}
	
	~A()
	{}
	
	const A copy() const
	{
		return A(*this);
	}
	
	void setVal(int val)
	{
		i = val;
	}
	
	void print() const
	{
		cout << "A(" << i << ") _ const\n";
	}

	void print()
	{
		cout << "A(" << i << ") _ non-const\n";
	}

private:
	int i;
};


int main(void)
{
	A a;
	
	A a2 = a.copy();

	a.print();
	a2.print();
	
        const A* aptr = &a;

        aptr->print();

        const A* const aconstptr = &a;

        aconstptr->print();

        aptr = &a2;
//        aconstptr = &a2;

        aptr->print();
        aconstptr->print();

	return 0;
}