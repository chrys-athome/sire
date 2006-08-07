#ifndef SIRECAS_INVTRIGFUNCS_H
#define SIRECAS_INVTRIGFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Inverse-cosine */
class SIRECAS_EXPORT ArcCos : public SingleFunc
{
public:
    ArcCos();
    ArcCos(const Expression &ex);
    
    ArcCos(const ArcCos &other);
    
    ~ArcCos();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcCos";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcCos(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcCos(arg).toExpression();
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
public:
    ArcSin();
    ArcSin(const Expression &ex);
    
    ArcSin(const ArcSin &other);
    
    ~ArcSin();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcSin";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcSin(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcSin(arg).toExpression();
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
public:
    ArcTan();
    ArcTan(const Expression &ex);
    
    ArcTan(const ArcTan &other);
    
    ~ArcTan();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcTan";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcTan(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcTan(arg).toExpression();
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
public:
    ArcSec();
    ArcSec(const Expression &ex);
    
    ArcSec(const ArcSec &other);
    
    ~ArcSec();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcSec";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcSec(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcSec(arg).toExpression();
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
public:
    ArcCsc();
    ArcCsc(const Expression &ex);
    
    ArcCsc(const ArcCsc &other);
    
    ~ArcCsc();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcCsc";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcCsc(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcCsc(arg).toExpression();
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
public:
    ArcCot();
    ArcCot(const Expression &ex);
    
    ArcCot(const ArcCot &other);
    
    ~ArcCot();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcCot";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcCot(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcCot(arg).toExpression();
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
