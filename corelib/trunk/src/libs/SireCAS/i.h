#ifndef SIRECAS_I_H
#define SIRECAS_I_H

#include "constant.h"

#include "SireMaths/complex.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

using SireMaths::Complex;

/**
This is the complex number, i

@author Christopher Woods
*/
class SIRECAS_EXPORT I : public Constant
{
public:
    I();
    I(const I &other);
    
    ~I();

    bool operator==(const ExBase &other) const;
    
    uint hash() const;
    
    const char* what() const
    {
        return "SireCAS::I";
    }

    QString toString() const;
    double evaluate(const Values&) const;
    Complex evaluate(const ComplexValues&) const;

    Expression conjugate() const;

    bool isComplex() const;

protected:
    
    ExBase* clone() const
    {
        return new I();
    }

};

}

Q_DECLARE_METATYPE(SireCAS::I)

SIRE_END_HEADER

#endif
