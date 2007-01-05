#ifndef SIRECAS_TRIGFUNCS_H
#define SIRECAS_TRIGFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class Cos;
class Sin;
class Tan;
class Csc;
class Sec;
class Cot;
}

QDataStream& operator<<(QDataStream&, const SireCAS::Cos&);
QDataStream& operator>>(QDataStream&, SireCAS::Cos&);

QDataStream& operator<<(QDataStream&, const SireCAS::Sin&);
QDataStream& operator>>(QDataStream&, SireCAS::Sin&);

QDataStream& operator<<(QDataStream&, const SireCAS::Tan&);
QDataStream& operator>>(QDataStream&, SireCAS::Tan&);

QDataStream& operator<<(QDataStream&, const SireCAS::Csc&);
QDataStream& operator>>(QDataStream&, SireCAS::Csc&);

QDataStream& operator<<(QDataStream&, const SireCAS::Sec&);
QDataStream& operator>>(QDataStream&, SireCAS::Sec&);

QDataStream& operator<<(QDataStream&, const SireCAS::Cot&);
QDataStream& operator>>(QDataStream&, SireCAS::Cot&);

namespace SireCAS
{

/** Cosine */
class SIRECAS_EXPORT Cos : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Cos&);
friend QDataStream& ::operator>>(QDataStream&, Cos&);

public:
    Cos();
    Cos(const Expression &ex);

    Cos(const Cos &other);

    ~Cos();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Cos";
    }

    const char* what() const
    {
        return Cos::typeName();
    }

    Cos* clone() const
    {
        return new Cos(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Cos(arg));
    }

    QString stringRep() const
    {
        return "cos";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;

};

/** Sine */
class SIRECAS_EXPORT Sin : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Sin&);
friend QDataStream& ::operator>>(QDataStream&, Sin&);

public:
    Sin();
    Sin(const Expression &ex);

    Sin(const Sin &other);

    ~Sin();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Sin";
    }

    const char* what() const
    {
        return Sin::typeName();
    }

    Sin* clone() const
    {
        return new Sin(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Sin(arg));
    }

    QString stringRep() const
    {
        return "sin";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Tangent */
class SIRECAS_EXPORT Tan : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Tan&);
friend QDataStream& ::operator>>(QDataStream&, Tan&);

public:
    Tan();
    Tan(const Expression &ex);

    Tan(const Tan &other);

    ~Tan();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Tan";
    }

    const char* what() const
    {
        return Tan::typeName();
    }

    Tan* clone() const
    {
        return new Tan(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Tan(arg));
    }

    QString stringRep() const
    {
        return "tan";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Secant */
class SIRECAS_EXPORT Sec : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Sec&);
friend QDataStream& ::operator>>(QDataStream&, Sec&);

public:
    Sec();
    Sec(const Expression &ex);

    Sec(const Sec &other);

    ~Sec();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Sec";
    }

    const char* what() const
    {
        return Sec::typeName();
    }

    Sec* clone() const
    {
        return new Sec(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Sec(arg));
    }

    QString stringRep() const
    {
        return "sec";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Cosecant */
class SIRECAS_EXPORT Csc : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Csc&);
friend QDataStream& ::operator>>(QDataStream&, Csc&);

public:
    Csc();
    Csc(const Expression &ex);

    Csc(const Csc &other);

    ~Csc();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Csc";
    }

    const char* what() const
    {
        return Csc::typeName();
    }

    Csc* clone() const
    {
        return new Csc(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Csc(arg));
    }

    QString stringRep() const
    {
        return "csc";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Cotangent */
class SIRECAS_EXPORT Cot : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const Cot&);
friend QDataStream& ::operator>>(QDataStream&, Cot&);

public:
    Cot();
    Cot(const Expression &ex);

    Cot(const Cot &other);

    ~Cot();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::Cot";
    }

    const char *what() const
    {
        return Cot::typeName();
    }

    Cot* clone() const
    {
        return new Cot(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(Cot(arg));
    }

    QString stringRep() const
    {
        return "cot";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

}

Q_DECLARE_METATYPE(SireCAS::Cos)
Q_DECLARE_METATYPE(SireCAS::Sin)
Q_DECLARE_METATYPE(SireCAS::Tan)
Q_DECLARE_METATYPE(SireCAS::Csc)
Q_DECLARE_METATYPE(SireCAS::Sec)
Q_DECLARE_METATYPE(SireCAS::Cot)


SIRE_END_HEADER

#endif
