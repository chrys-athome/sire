
#include <iostream>
#include <QtGlobal>

using namespace std;

template<class T>
class CharArray
{
public:
    CharArray()
    {
        for (int i=0; i<sizeof(T); ++i)
        {
            array()[i] = 0;
        }
    }

    CharArray(const CharArray &other) : data(other.data)
    {}
    
    ~CharArray()
    {}
    
    int count()
    {
        return sizeof(T);
    }
    
    quint8& operator[](int i)
    {
        return this->array()[i];
    }
    
    const quint8& operator[](int i) const
    {
        return this->array()[i];
    }

    operator T() const
    {
        return data;
    }

private:
    quint8* array()
    {
        return (quint8*)( &data );
    }
    
    const quint8* array() const
    {
        return (const quint8*)( &data );
    }
    
    T data;
};

int main(int argc, const char **argv)
{
    CharArray<quint32> u;

    cout << u.count() << endl;

    cout << quint32( u ) << endl;

    u[0] = 2;

    cout << quint32( u ) << endl;

    u[3] = 2;

    cout << quint32( u ) << endl;

    cout << (u[0] == 2) << (u[1] == 0) << (u[2] == 0) << (u[3] == 2) << endl;

    cout << (u[0] == 0) << (u[1] == 2) << (u[2] == 2) << (u[3] == 0) << endl;

    return 0;
}
