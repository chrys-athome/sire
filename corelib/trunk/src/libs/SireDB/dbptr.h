#ifndef SIREDB_DBWrapper_H
#define SIREDB_DBWrapper_H

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
This is a thin wrapper class that is used to store pointers to the components of the forcefield database. This class is very low-level.

@author Christopher Woods
*/
class SIREDB_EXPORT DBWrapper
{
public:
    DBWrapper()
    {}

    virtual ~DBWrapper()
    {}

    virtual const char* what() const=0;
    virtual DBWrapper* clone() const=0;

    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }
    
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }
    
    template<class T>
    T& asA()
    {
        return dynamic_cast<T&>(*this);
    }
};

}

SIRE_END_HEADER

#endif
