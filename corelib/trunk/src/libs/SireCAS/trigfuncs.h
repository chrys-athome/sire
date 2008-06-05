#ifndef SIRECAS_TRIGFUNCS_H
#define SIRECAS_TRIGFUNCS_H

#include "singlefunc.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Cosine */
class SIRECAS_EXPORT Cos : public SingleFunc
{
public:
    Cos();
    Cos(const Expression &ex);
    
    Cos(const Cos &other);
    
    ~Cos();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Cos";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Cos(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Cos(arg).toExpression();
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
public:
    Sin();
    Sin(const Expression &ex);
    
    Sin(const Sin &other);
    
    ~Sin();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Sin";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Sin(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Sin(arg).toExpression();
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
public:
    Tan();
    Tan(const Expression &ex);
    
    Tan(const Tan &other);
    
    ~Tan();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Tan";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Tan(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Tan(arg).toExpression();
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
public:
    Sec();
    Sec(const Expression &ex);
    
    Sec(const Sec &other);
    
    ~Sec();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Sec";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Sec(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Sec(arg).toExpression();
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
public:
    Csc();
    Csc(const Expression &ex);
    
    Csc(const Csc &other);
    
    ~Csc();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Csc";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Csc(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Csc(arg).toExpression();
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
public:
    Cot();
    Cot(const Expression &ex);
    
    Cot(const Cot &other);
    
    ~Cot();
    
    bool operator==(const ExBase &other) const;
    
    const char* what() const
    {
        return "SireCAS::Cot";
    }
    
    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
protected:
    ExBase* clone() const
    {
        return new Cot(*this);
    }
    
    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return Cot(arg).toExpression();
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
