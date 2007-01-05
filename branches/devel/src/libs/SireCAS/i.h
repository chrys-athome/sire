#ifndef SIRECAS_I_H
#define SIRECAS_I_H

#include "constant.h"

#include "SireMaths/complex.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class I;
}

QDataStream& operator<<(QDataStream&, const SireCAS::I&);
QDataStream& operator>>(QDataStream&, SireCAS::I&);

namespace SireCAS
{

using SireMaths::Complex;

/**
This is the complex number, i

@author Christopher Woods
*/
class SIRECAS_EXPORT I : public Constant
{

friend QDataStream& ::operator<<(QDataStream&, const I&);
friend QDataStream& ::operator>>(QDataStream&, I&);

public:
    I();
    I(const I &other);

    ~I();

    bool operator==(const ExBase &other) const;

    uint hash() const;

    static const char* typeName()
    {
        return "SireCAS::I";
    }

    const char* what() const
    {
        return I::typeName();
    }

    I* clone() const
    {
        return new I();
    }

    QString toString() const;
    double evaluate(const Values&) const;
    Complex evaluate(const ComplexValues&) const;

    Expression conjugate() const;

    bool isComplex() const;
};

}

Q_DECLARE_METATYPE(SireCAS::I)

SIRE_END_HEADER

#endif
