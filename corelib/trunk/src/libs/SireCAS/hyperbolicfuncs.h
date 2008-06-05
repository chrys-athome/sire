#ifndef SIRECAS_HYPERBOLICFUNCS_H
#define SIRECAS_HYPERBOLICFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Hyperbolic cosine */
class SIRECAS_EXPORT Cosh : public SingleFunc
{
public:
    Cosh();
    Cosh(const Expression &ex);
    
    Cosh(const Cosh &other);
    
    ~Cosh();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Cosh";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Cosh(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Cosh(arg).toExpression();
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
public:
    Sinh();
    Sinh(const Expression &ex);
    
    Sinh(const Sinh &other);
    
    ~Sinh();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Sinh";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Sinh(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Sinh(arg).toExpression();
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
public:
    Tanh();
    Tanh(const Expression &ex);
    
    Tanh(const Tanh &other);
    
    ~Tanh();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Tanh";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Tanh(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Tanh(arg).toExpression();
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
public:
    Sech();
    Sech(const Expression &ex);
    
    Sech(const Sech &other);
    
    ~Sech();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Sech";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Sech(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Sech(arg).toExpression();
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
public:
    Csch();
    Csch(const Expression &ex);
    
    Csch(const Csch &other);
    
    ~Csch();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Csch";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Csch(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Csch(arg).toExpression();
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
public:
    Coth();
    Coth(const Expression &ex);
    
    Coth(const Coth &other);
    
    ~Coth();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Coth";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Coth(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Coth(arg).toExpression();
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
