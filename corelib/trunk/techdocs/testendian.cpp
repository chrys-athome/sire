
#include <iostream>

#include <qglobal.h>

using namespace std;

void printValue(quint32 n)
{
    quint32 t;
    uchar first,second,third,fourth;
    
    t = n & 0xFF000000;
    fourth = t >> 24;   

    t = n & 0x00FF0000;
    third = t >> 16;
    
    t = n & 0x0000FF00;
    second = t >> 8;
    
    t = n & 0x000000FF;
    first = t;

    cout << "\"" << n << "\" = (" << (int)first << "," << (int)second
                 << "," << (int)third << "," << (int)fourth << ")\n";
}

quint32 pack(quint32 val0, quint32 val1)
{

    cout << "pack(" << val0 << "," << val1 << ")\n";
    
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    #warning Big Endian
#elif Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    #warning Little Endian
#else
    #error Why is Q_BYTE_ORDER no correctly defined?
#endif
    
    return (val0 << 16) | (val1 & 0x0000FFFF);
}

quint32 pack3(quint32 val0, quint32 val1, quint32 val2)
{
    cout << "pack3(" << val0 << "," << val1 << "," << val2 << ")\n";
    
    return (val0 << 16) | ( (val1 << 8) & 0x0000FF00) | ( val2 & 0x000000FF);
}

quint32 pack4(quint32 val0, quint32 val1, 
              quint32 val2, quint32 val3)
{
    cout << "pack4(" << val0 << "," << val1 << "," << val2 
         << "," << val3 << ")\n";

    return (val0 << 24) | ( (val1 << 16) & 0x00FF0000) 
            | ( (val2 << 8) & 0x0000FF00) | ( val3 & 0x000000FF );
}

int main(void)
{
    cout << "*** printValue ****\n";
    printValue(0);
    printValue(1);
    printValue(2);
    printValue(255);
    printValue(256);
    
    
    cout << "\n*** printValue shifted\n";
    printValue( quint32(255) << 1 );
    printValue( quint32(255) << 2 );
    printValue( quint32(255) << 4 );
    printValue( quint32(255) << 8 );
    printValue( quint32(255) << 16 );
    printValue( quint32(255) << 20 );
    printValue( 255 << 24 );
    printValue( quint32(255) << 28 );
    printValue( quint32(255) << 32 );
    

    cout << "\n*** pack ***\n";
    printValue( pack(1,1) );
    printValue( pack(255,255) );
    printValue( pack(65535, 255) );
    printValue( pack(65536, 255) );
    printValue( pack(65537, 255) );
    
    printValue( pack(65535, 65535) );
    printValue( pack(65535, 65536) );
    printValue( pack(65536, 65536) );
    
    
    cout << "\n*** pack3 ***\n";
    printValue( pack3(1,1,1) );
    printValue( pack3(255,1,1) );
    printValue( pack3(256,1,1) );
    printValue( pack3(256,256,256) );
    printValue( pack3(128,128,65535) );
    printValue( pack3(65535,128,128) );
    printValue( pack3(65536,128,128) );
    
    cout << "\n*** pack4 ***\n";
    printValue( pack4(1,1,1,1) );
    printValue( pack4(255,1,1,1) );
    printValue( pack4(256,1,1,1) );
    printValue( pack4(1,255,1,1) );
    printValue( pack4(1,256,1,1) );
    printValue( pack4(2,2,255,1) );
    printValue( pack4(2,2,2,255) );
    
    return 0;
}
