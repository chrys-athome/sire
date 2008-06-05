
#include <iostream>
#include <list>
#include <vector>

using namespace std;

template<class V, class C = list<V> >
class Holder
{
public:
    Holder()
    {}
    
    ~Holder()
    {}
    
    void append(V value)
    {
        data.push_back(value);
    }
    
    void print()
    {
        for (typename C::iterator it = data.begin();  
             it != data.end();
             ++it)
            cout << *it << endl;
    }
    
private:

    C data;
};

int main(void)
{
    Holder<double> d;

    d.append(2.3);
    d.append(2.1);
    d.append(92.6);
    
    d.print();

    Holder<double, vector<double> > d2;
    d2.append(2.3);
    d2.append(2.1);
    d2.append(92.6);
    
    d2.print();
    

    return 0;
}
