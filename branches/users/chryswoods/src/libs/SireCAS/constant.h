#ifndef SIRECAS_CONSTANT_H
#define SIRECAS_CONSTANT_H

#include "exbase.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/**
This class represents a constant value (e.g. a number).

@author Christopher Woods
*/
class SIRECAS_EXPORT Constant : public ExBase
{
public:
    Constant();
    Constant(const Constant &other);

    ~Constant();

    ///////
    /////// Virtual functions - you may wish to override these
    /////// in your derived class
    ///////

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    ///////
    /////// Pure-virtual functions - these must be overridden
    /////// in your derived class
    ///////

    bool operator==(const ExBase &other) const;

    uint hash() const;

    static const char* typeName()
    {
        return "SireCAS::Constant";
    }

    const char* what() const
    {
        return Constant::typeName();
    }

    Constant* clone() const
    {
        return new Constant();
    }

    QString toString() const;

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression substitute(const Identities &identities) const;

    Symbols symbols() const;
    Functions functions() const;

    Expressions children() const;

};

}

Q_DECLARE_METATYPE(SireCAS::Constant)

SIRE_END_HEADER

#endif
