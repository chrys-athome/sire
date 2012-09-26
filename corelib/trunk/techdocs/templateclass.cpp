
#include <iostream>
#include <string>

using namespace std;

class Base
{
public:
    Base()
    {}
    
    virtual ~Base()
    {}
    
    void addBase(const Base &other)
    {
        cout << "Adding a Base...\n";
    }
};

template<unsigned int T>
class ClassTyp : public Base
{
public:
    
    ClassTyp() : Base()
    {}
    
    ~ClassTyp()
    {}
    
    void add(const ClassTyp<T> &other)
    {
        cout << "Adding another " << T << endl;
        Base::addBase(other);
    }
    
    static const unsigned int TypeID;
};

template<unsigned int T>
const unsigned int ClassTyp<T>::TypeID(T);

template<>
class ClassTyp<0>
{
public:
    ClassTyp<0>()
    {
        cout << "building null class\n";
    }
    ~ClassTyp<0>()
    {}
};

template<unsigned int T, unsigned int U>
class PackTyp : public Base
{
public:
    PackTyp() : Base()
    {}
    
    ~PackTyp()
    {}
   
    template<unsigned int A>
    const ClassTyp<A>& get() const
    {
        switch(A)
        {
            case T:
                return reinterpret_cast< const ClassTyp<A>& >(classt);
            case U:
                return reinterpret_cast< const ClassTyp<A>& >(classu);
            default:
                throw "BadCast!!!";
        }
    }
    
    void add(const ClassTyp<T> &other)
    {
            classt.add(other);
    }
    
    void add(const ClassTyp<U> &other)
    {
            classu.add(other);
    }

    template<unsigned int A>
    void add(const ClassTyp<A> &other)
    {
        //this is not a T or U, so skip it!
        cout << "Skipping add as incompatible! " << other.TypeID << "\n";
    }
   
    template<unsigned int A, unsigned int B>
    void add(const PackTyp<A,B> &other)
    {
        add( other.get<A>() );
        add( other.get<B>() );
    }
   
protected:
    ClassTyp<T> classt;
    ClassTyp<U> classu;
};

const int COULOMB = 1;
const int LJ = 2;

typedef ClassTyp<COULOMB> MyType1;
typedef ClassTyp<LJ> MyType2;

int main(void)
{
    MyType1 test1;
    MyType2 test2;
    
    cout << typeid(test1).name() << endl;
    cout << typeid(test2).name() << endl;
        
    PackTyp<COULOMB,LJ> pack;
    
    cout << typeid(pack).name() << endl;
    
    pack.add(test1);
    pack.add(test2);

    cout << typeid(pack.get<COULOMB>()).name() << endl;
    cout << typeid(pack.get<LJ>()).name() << endl;
    
    pack.add( ClassTyp<3>() );

    PackTyp<4,COULOMB> pack2;
    
    pack.add(pack2);

    PackTyp<COULOMB,0> pack3;

    return 0;
}

    
