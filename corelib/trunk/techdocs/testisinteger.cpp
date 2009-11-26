
#include <iostream>
#include <limits>

using namespace std;

const int largest_int = std::numeric_limits<int>::max();
const int smallest_int = std::numeric_limits<int>::min();

const double largest = std::numeric_limits<double>::max();
const double smallest = std::numeric_limits<double>::min();

void isInteger( double val )
{

	if (smallest_int <= val and val <= largest_int 
	       and val == double(int(val)))
		cout << val << " is an integer :-)\n";
	else
		cout << val << " is a double :-(\n";

}

int main(void)
{

	cout << "Largest = " << largest_int << " and smallest = " 
	     << smallest_int << endl;

	cout << "Largest = " << largest << " and smallest = " 
	     << smallest << endl;

	isInteger(1.0);
	isInteger(1.1);
	isInteger(4.9/4.9);
	isInteger(2.5*2.0);
	
	isInteger(largest);
	isInteger(smallest);

	isInteger( double(largest_int) );
	isInteger( double(largest_int) + 1 );
	
	
	for (int i=0; i<=100; ++i)
		isInteger(i*0.1);

	return 0;
}
