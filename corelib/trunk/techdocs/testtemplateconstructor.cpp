
#include <iostream>
#include <vector>

using namespace std;

class Test
{
public:
    Test()
    {}

    template<class T>
    Test(const T &obj)
    {
        cout << obj << endl;
    }
    
    template<class T>
    Test(const vector<T> &vec)
    {
        cout << "Vector ";
        
        for (int i=0; i<vec.size(); ++i)
        {
            cout << vec[i] << " ";
        }
        
        cout << endl;
    }
    
    Test(int val)
    {
        cout << "Int " << val << endl;
    }
    
    ~Test()
    {}
};

int main(void)
{
    Test a(42);
    
    Test b("hello");
    
    Test c(45.4);
    
    vector<float> v;
    v.push_back(1.2);
    v.push_back(2.4);
    
    Test d(v);

    return 0;
}
