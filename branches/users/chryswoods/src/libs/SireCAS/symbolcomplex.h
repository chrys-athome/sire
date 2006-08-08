#ifndef SIRECAS_SYMBOLCOMPLEX_H
#define SIRECAS_SYMBOLCOMPLEX_H

#include "symbolvalue.h"

#include "SireMaths/complex.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

using SireMaths::Complex;

/** Small class that holds a SymbolID number and an associated complex value */
class SIRECAS_EXPORT SymbolComplex
{
public:
    SymbolComplex(SymbolID id, const Complex &val) : _val(val), _id(id)
    {}
    
    SymbolComplex(SymbolID id, double val) : _val(val), _id(id)
    {}
    
    SymbolComplex(const SymbolValue &val) : _val(val.value()), _id(val.ID())
    {}
    
    ~SymbolComplex()
    {}
    
    SymbolID ID() const
    {
        return _id;
    }
    
    const Complex& value() const
    {
        return _val;
    }
private:

    Complex _val;
    SymbolID _id;
};

}

SIRE_BEGIN_HEADER

#endif
