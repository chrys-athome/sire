#ifndef SIRECAS_EXP_H
#define SIRECAS_EXP_H

#include "power.h"
#include "singlefunc.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class Exp;
class Ln;
}

QDataStream& operator<<(QDataStream&, const SireCAS::Exp&);
QDataStream& operator>>(QDataStream&, SireCAS::Exp&);

QDataStream& operator<<(QDataStream&, const SireCAS::Ln&);
QDataStream& operator>>(QDataStream&, SireCAS::Ln&);

namespace SireCAS
{

/**
This is the exponential function, e^x

@author Christopher Woods
*/
class SIRECAS_EXPORT Exp : public PowerFunction
{

friend QDataStream& ::operator<<(QDataStream&, const Exp&);
friend QDataStream& ::operator>>(QDataStream&, Exp&);

public:
    Exp();
    Exp(const Expression &power);

    Exp(const Exp &other);

    ~Exp();

    bool operator==(const ExBase &other) const;

    uint hash() const;

    static const char* typeName()
    {
        return "SireCAS::Exp";
    }

    const char* what() const
    {
        return Exp::typeName();
    }

    Exp* clone() const
    {
        return new Exp(*this);
    }

    QString toString() const;

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    Expression core() const;
    Expression power() const;

private:

    /** The expression to which 'e' is raised to */
    Expression pwr;
};

/** This is the natural logarithm (ln) function

@author Christopher Woods
*/
class SIRECAS_EXPORT Ln : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Ln&);
friend QDataStream& ::operator>>(QDataStream&, Ln&);

public:
    Ln();
    Ln(const Expression &expression);

    Ln(const Ln &other);
    ~Ln();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Ln";
    }

    const char* what() const
    {
        return Ln::typeName();
    }

    Ln* clone() const
    {
        return new Ln(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:

    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Ln(arg));
    }

    QString stringRep() const
    {
        return "ln";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

}

Q_DECLARE_METATYPE(SireCAS::Exp)
Q_DECLARE_METATYPE(SireCAS::Ln)

SIRE_END_HEADER

#endif
