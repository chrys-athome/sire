#ifndef SIRECAS_HYPERBOLICFUNCS_H
#define SIRECAS_HYPERBOLICFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class Cosh;
class Sinh;
class Tanh;
class Csch;
class Sech;
class Coth;
}

QDataStream& operator<<(QDataStream&, const SireCAS::Cosh&);
QDataStream& operator>>(QDataStream&, SireCAS::Cosh&);

QDataStream& operator<<(QDataStream&, const SireCAS::Sinh&);
QDataStream& operator>>(QDataStream&, SireCAS::Sinh&);

QDataStream& operator<<(QDataStream&, const SireCAS::Tanh&);
QDataStream& operator>>(QDataStream&, SireCAS::Tanh&);

QDataStream& operator<<(QDataStream&, const SireCAS::Csch&);
QDataStream& operator>>(QDataStream&, SireCAS::Csch&);

QDataStream& operator<<(QDataStream&, const SireCAS::Sech&);
QDataStream& operator>>(QDataStream&, SireCAS::Sech&);

QDataStream& operator<<(QDataStream&, const SireCAS::Coth&);
QDataStream& operator>>(QDataStream&, SireCAS::Coth&);

namespace SireCAS
{

/** Hyperbolic cosine */
class SIRECAS_EXPORT Cosh : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Cosh&);
friend QDataStream& ::operator>>(QDataStream&, Cosh&);

public:
    Cosh();
    Cosh(const Expression &ex);

    Cosh(const Cosh &other);

    ~Cosh();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Cosh";
    }

    const char* what() const
    {
        return Cosh::typeName();
    }

    Cosh* clone() const
    {
        return new Cosh(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Cosh(arg));
    }

    QString stringRep() const
    {
        return "cosh";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;

};

/** Hyperbolic sine */
class SIRECAS_EXPORT Sinh : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Sinh&);
friend QDataStream& ::operator>>(QDataStream&, Sinh&);

public:
    Sinh();
    Sinh(const Expression &ex);

    Sinh(const Sinh &other);

    ~Sinh();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Sinh";
    }

    const char* what() const
    {
        return Sinh::typeName();
    }

    Sinh* clone() const
    {
        return new Sinh(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Sinh(arg));
    }

    QString stringRep() const
    {
        return "sinh";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Hyperbolic tangent */
class SIRECAS_EXPORT Tanh : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Tanh&);
friend QDataStream& ::operator>>(QDataStream&, Tanh&);

public:
    Tanh();
    Tanh(const Expression &ex);

    Tanh(const Tanh &other);

    ~Tanh();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Tanh";
    }

    const char* what() const
    {
        return Tanh::typeName();
    }

    Tanh* clone() const
    {
        return new Tanh(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Tanh(arg));
    }

    QString stringRep() const
    {
        return "tanh";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Hyperbolic secant */
class SIRECAS_EXPORT Sech : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Sech&);
friend QDataStream& ::operator>>(QDataStream&, Sech&);

public:
    Sech();
    Sech(const Expression &ex);

    Sech(const Sech &other);

    ~Sech();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Sech";
    }

    const char* what() const
    {
        return Sech::typeName();
    }

    Sech* clone() const
    {
        return new Sech(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Sech(arg));
    }

    QString stringRep() const
    {
        return "sech";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Hyperbolic cosecant */
class SIRECAS_EXPORT Csch : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Csch&);
friend QDataStream& ::operator>>(QDataStream&, Csch&);

public:
    Csch();
    Csch(const Expression &ex);

    Csch(const Csch &other);

    ~Csch();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Csch";
    }

    const char* what() const
    {
        return Csch::typeName();
    }

    Csch* clone() const
    {
        return new Csch(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Csch(arg));
    }

    QString stringRep() const
    {
        return "csch";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Hyperbolic cotangent */
class SIRECAS_EXPORT Coth : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Coth&);
friend QDataStream& ::operator>>(QDataStream&, Coth&);

public:
    Coth();
    Coth(const Expression &ex);

    Coth(const Coth &other);

    ~Coth();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Coth";
    }

    const char* what() const
    {
        return Coth::typeName();
    }

    Coth* clone() const
    {
        return new Coth(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Coth(arg));
    }

    QString stringRep() const
    {
        return "coth";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

}

Q_DECLARE_METATYPE(SireCAS::Cosh)
Q_DECLARE_METATYPE(SireCAS::Sinh)
Q_DECLARE_METATYPE(SireCAS::Tanh)
Q_DECLARE_METATYPE(SireCAS::Csch)
Q_DECLARE_METATYPE(SireCAS::Sech)
Q_DECLARE_METATYPE(SireCAS::Coth)

SIRE_BEGIN_HEADER

#endif
