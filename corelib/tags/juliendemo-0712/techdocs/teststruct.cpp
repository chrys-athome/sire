
#include <iostream>

using namespace std;

typedef struct
{
double dat[2];
} complex;

class Complex : public complex
{
public:
	Complex()
	{}
	
	Complex(double a, double b)
	{
		dat[0] = a;
		dat[1] = b;
	}
	
	Complex(const complex &other) : complex(other)
	{}
	
	Complex(const Complex &other) : complex(other)
	{}
	
	~Complex()
	{}
	
	Complex& operator=(const Complex &other)
	{
		complex::operator=(other);
		return *this;
	}
	
	void printMe() const
	{
		cout << "Complex = " << dat[0] << " + " << dat[1] << " i\n";
	}

};

int main(void)
{
	complex c;
	c.dat[0] = 1.0;
	c.dat[1] = 4.5;
	
	Complex val(c);
	val.printMe();

	Complex val2(2.0,3.0);
	val2.printMe();
	
	val2 = val;
	val2.printMe();
	
	Complex val3(val2);
	val3.printMe();

	cout << "sizeof(complex) == " << sizeof(complex) << " " << 
	        "sizeof(Complex) == " << sizeof(Complex) << endl;
	
	return 0;
}
