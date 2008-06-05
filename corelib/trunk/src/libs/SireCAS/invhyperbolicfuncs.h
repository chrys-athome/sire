#ifndef SIRECAS_INVHYPERBOLICFUNCS_H
#define SIRECAS_INVHYPERBOLICFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Inverse-hyperbolic-cosine */
class SIRECAS_EXPORT ArcCosh : public SingleFunc
{
public:
    ArcCosh();
    ArcCosh(const Expression &ex);
    
    ArcCosh(const ArcCosh &other);
    
    ~ArcCosh();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcCosh";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcCosh(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcCosh(arg).toExpression();
    }
    
    QString stringRep() const
    {
        return "arccosh";
    }
    
    uint magic() const;

    Expression diff() const;
    Expression integ() const;
    
};

/** Inverse-hyperbolic-sine */
class SIRECAS_EXPORT ArcSinh : public SingleFunc
{
public:
    ArcSinh();
    ArcSinh(const Expression &ex);
    
    ArcSinh(const ArcSinh &other);
    
    ~ArcSinh();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcSinh";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcSinh(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcSinh(arg).toExpression();
    }
    
    QString stringRep() const
    {
        return "arcsinh";
    }
    
    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-hyperbolic-tangent */
class SIRECAS_EXPORT ArcTanh : public SingleFunc
{
public:
    ArcTanh();
    ArcTanh(const Expression &ex);
    
    ArcTanh(const ArcTanh &other);
    
    ~ArcTanh();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcTanh";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcTanh(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcTanh(arg).toExpression();
    }
    
    QString stringRep() const
    {
        return "arctanh";
    }
    
    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-hyperbolic-secant */
class SIRECAS_EXPORT ArcSech : public SingleFunc
{
public:
    ArcSech();
    ArcSech(const Expression &ex);
    
    ArcSech(const ArcSech &other);
    
    ~ArcSech();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcSech";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcSech(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcSech(arg).toExpression();
    }
    
    QString stringRep() const
    {
        return "arcsech";
    }
    
    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-hyperbolic-cosecant */
class SIRECAS_EXPORT ArcCsch : public SingleFunc
{
public:
    ArcCsch();
    ArcCsch(const Expression &ex);
    
    ArcCsch(const ArcCsch &other);
    
    ~ArcCsch();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcCsch";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcCsch(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcCsch(arg).toExpression();
    }
    
    QString stringRep() const
    {
        return "arccsch";
    }
    
    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

/** Inverse-hyperbolic-cotangent */
class SIRECAS_EXPORT ArcCoth : public SingleFunc
{
public:
    ArcCoth();
    ArcCoth(const Expression &ex);
    
    ArcCoth(const ArcCoth &other);
    
    ~ArcCoth();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::ArcCoth";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new ArcCoth(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return ArcCoth(arg).toExpression();
    }
    
    QString stringRep() const
    {
        return "arccoth";
    }
    
    uint magic() const;

    Expression diff() const;
    Expression integ() const;
};

}

Q_DECLARE_METATYPE(SireCAS::ArcCosh)
Q_DECLARE_METATYPE(SireCAS::ArcSinh)
Q_DECLARE_METATYPE(SireCAS::ArcTanh)
Q_DECLARE_METATYPE(SireCAS::ArcCsch)
Q_DECLARE_METATYPE(SireCAS::ArcSech)
Q_DECLARE_METATYPE(SireCAS::ArcCoth)

SIRE_END_HEADER

#endif
