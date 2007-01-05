#ifndef SIRECAS_INVTRIGFUNCS_H
#define SIRECAS_INVTRIGFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class ArcCos;
class ArcSin;
class ArcTan;
class ArcCsc;
class ArcSec;
class ArcCot;
}

QDataStream& operator<<(QDataStream&, const SireCAS::ArcCos&);
QDataStream& operator>>(QDataStream&, SireCAS::ArcCos&);

QDataStream& operator<<(QDataStream&, const SireCAS::ArcSin&);
QDataStream& operator>>(QDataStream&, SireCAS::ArcSin&);

QDataStream& operator<<(QDataStream&, const SireCAS::ArcTan&);
QDataStream& operator>>(QDataStream&, SireCAS::ArcTan&);

QDataStream& operator<<(QDataStream&, const SireCAS::ArcCsc&);
QDataStream& operator>>(QDataStream&, SireCAS::ArcCsc&);

QDataStream& operator<<(QDataStream&, const SireCAS::ArcSec&);
QDataStream& operator>>(QDataStream&, SireCAS::ArcSec&);

QDataStream& operator<<(QDataStream&, const SireCAS::ArcCot&);
QDataStream& operator>>(QDataStream&, SireCAS::ArcCot&);

namespace SireCAS
{

/** Inverse-cosine */
class SIRECAS_EXPORT ArcCos : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const ArcCos&);
friend QDataStream& ::operator>>(QDataStream&, ArcCos&);

public:
    ArcCos();
    ArcCos(const Expression &ex);

    ArcCos(const ArcCos &other);

    ~ArcCos();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::ArcCos";
    }

    const char* what() const
    {
        return ArcCos::typeName();
    }

    ArcCos* clone() const
    {
        return new ArcCos(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(ArcCos(arg));
    }

    QString stringRep() const
    {
        return "arccos";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;

};

/** Inverse-sine */
class SIRECAS_EXPORT ArcSin : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const ArcSin&);
friend QDataStream& ::operator>>(QDataStream&, ArcSin&);

public:
    ArcSin();
    ArcSin(const Expression &ex);

    ArcSin(const ArcSin &other);

    ~ArcSin();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::ArcSin";
    }

    const char* what() const
    {
        return ArcSin::typeName();
    }

    ArcSin* clone() const
    {
        return new ArcSin(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(ArcSin(arg));
    }

    QString stringRep() const
    {
        return "arcsin";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-tangent */
class SIRECAS_EXPORT ArcTan : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const ArcTan&);
friend QDataStream& ::operator>>(QDataStream&, ArcTan&);

public:
    ArcTan();
    ArcTan(const Expression &ex);

    ArcTan(const ArcTan &other);

    ~ArcTan();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::ArcTan";
    }

    const char* what() const
    {
        return ArcTan::typeName();
    }

    ArcTan* clone() const
    {
        return new ArcTan(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(ArcTan(arg));
    }

    QString stringRep() const
    {
        return "arctan";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-secant */
class SIRECAS_EXPORT ArcSec : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const ArcSec&);
friend QDataStream& ::operator>>(QDataStream&, ArcSec&);

public:
    ArcSec();
    ArcSec(const Expression &ex);

    ArcSec(const ArcSec &other);

    ~ArcSec();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::ArcSec";
    }

    const char* what() const
    {
        return ArcSec::typeName();
    }

    ArcSec* clone() const
    {
        return new ArcSec(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(ArcSec(arg));
    }

    QString stringRep() const
    {
        return "arcsec";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-cosecant */
class SIRECAS_EXPORT ArcCsc : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const ArcCsc&);
friend QDataStream& ::operator>>(QDataStream&, ArcCsc&);

public:
    ArcCsc();
    ArcCsc(const Expression &ex);

    ArcCsc(const ArcCsc &other);

    ~ArcCsc();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::ArcCsc";
    }

    const char* what() const
    {
        return ArcCsc::typeName();
    }

    ArcCsc* clone() const
    {
        return new ArcCsc(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(ArcCsc(arg));
    }

    QString stringRep() const
    {
        return "arccsc";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-cotangent */
class SIRECAS_EXPORT ArcCot : public SingleFunc
{

friend QDataStream& ::operator<<(QDataStream&, const ArcCot&);
friend QDataStream& ::operator>>(QDataStream&, ArcCot&);

public:
    ArcCot();
    ArcCot(const Expression &ex);

    ArcCot(const ArcCot &other);

    ~ArcCot();

    bool operator==(const ExBase &other) const;

    static const char* typeName()
    {
        return "SireCAS::ArcCot";
    }

    const char* what() const
    {
        return ArcCot::typeName();
    }

    ArcCot* clone() const
    {
        return new ArcCot(*this);
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return Expression(*this);
        else
            return Expression(ArcCot(arg));
    }

    QString stringRep() const
    {
        return "arccot";
    }

    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

}

Q_DECLARE_METATYPE(SireCAS::ArcCos)
Q_DECLARE_METATYPE(SireCAS::ArcSin)
Q_DECLARE_METATYPE(SireCAS::ArcTan)
Q_DECLARE_METATYPE(SireCAS::ArcCsc)
Q_DECLARE_METATYPE(SireCAS::ArcSec)
Q_DECLARE_METATYPE(SireCAS::ArcCot)


SIRE_END_HEADER

#endif
