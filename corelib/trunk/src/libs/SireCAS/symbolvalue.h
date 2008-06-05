#ifndef SIRECAS_SYMBOLVALUE_H
#define SIRECAS_SYMBOLVALUE_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Unique ID number given to all symbols */
typedef quint32 SymbolID;

/** Small class that holds a SymbolID number and an associated value */
class SIRECAS_EXPORT SymbolValue
{
public:
    SymbolValue(SymbolID id, double val) : _val(val), _id(id)
    {}
    
    ~SymbolValue()
    {}
    
    SymbolID ID() const
    {
        return _id;
    }
    
    double value() const
    {
        return _val;
    }
private:

    double _val;
    SymbolID _id;
};

}

SIRE_END_HEADER

#endif
