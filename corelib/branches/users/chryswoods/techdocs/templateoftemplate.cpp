
#include <iostream>

using namespace std;

template<class T>
class Holder
{
public: 
    Holder()
    {}
    
    Holder(const T &value) : val(value)
    {}
            
    ~Holder()
    {}
    
    void print()
    {
        cout << "Holder value is " << val << endl;
    }
    
private:
    T val;
};

template<class T, class Holder>
class TOfT
{
public:
    TOfT(const T &val) : h(val)
    {}
    
    ~TOfT()
    {}
    
    void print()
    {
        cout << "TOfT is printing..." << endl;
        h.print();
    }
private:
    Holder h;
};

int main(void)
{

    Holder<int> holder(3);
    holder.print();

    TOfT<int,Holder<int> > toft(5);
    toft.print();

    return 0;

}
