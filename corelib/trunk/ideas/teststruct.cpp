
#include <iostream>

using namespace std;

struct tester
{
        tester(double m1, double m2, double m3) : l1(m1), l2(m2), l3(m3)
        {}
        
        bool operator==(const tester &other) const
        {
            return l1 == other.l1 and l2 == other.l2 and l3 == other.l3;
        }
        
	double l1;
	double l2;
	double l3;
};

int main(void)
{
	tester t1(1.0, 2.0, 3.0);

	tester t2(2.0, 3.0, 4.0);

	cout << (t1 == t2) << endl;

        tester t3 = t2;
        
        cout << (t1 == t3) << endl;
        cout << (t2 == t3) << endl;

	return 0;
}

