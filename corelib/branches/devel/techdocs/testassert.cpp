
#include <iostream>

using namespace std;

class assert_even
{
public:
	assert_even(const int &v) : val(v)
	{
		if ( v % 2 != 0 )
			throw "not even!";
	}
	
	~assert_even()
	{}
	
	const int& v() const
	{
		return val;
	}
	
private:
	const int &val;
};

void printEven( const assert_even &even_value )
{
	cout << even_value.v() << endl;
}

int main(void)
{
	printEven(2);
	printEven(4);
	
	try
	{
		printEven(3);
	}
	catch(...)
	{
		cout << "Not even!\n";
	}

	return 0;
}
