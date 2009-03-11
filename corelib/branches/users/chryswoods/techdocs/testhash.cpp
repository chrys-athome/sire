
#include <QHash>
#include <QString>

#include <iostream>

using namespace std;

const char* test(bool val)
{
    if (val)
        return "true";
    else
        return "false";
}

int main(void)
{
    QHash<QString,int> hash;
    
    QHash<QString,int>::const_iterator it = hash.begin();
    
    cout << test(it == hash.begin()) << endl;
    cout << test(it == hash.end()) << endl;

    it = hash.end();
    cout << test(it == hash.begin()) << endl;
    cout << test(it == hash.end()) << endl;


    hash["hello"] = 42;

    it = hash.end();
    --it;
    cout << *it << endl;
    
    cout << test(it == hash.begin()) << endl;

    cout << test(it == hash.begin()) << endl;
    cout << test(it == hash.end()) << endl;
    
    --it;
    cout << *it << endl;

    it = hash.begin();
    cout << *it << endl;
    
    hash.remove("hello");
    
    cout << *it << endl;
    cout << hash.count() << endl;
    
    cout << test(it == hash.begin()) << endl;

    return 0;
}
