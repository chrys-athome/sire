#ifndef SIRECAS_EXPRESSION_H
#define SIRECAS_EXPRESSION_H

#include "SireMaths/rational.h"

#include "expressionbase.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class Expression;
}

QDataStream& operator<<(QDataStream&, const SireCAS::Expression&);
QDataStream& operator>>(QDataStream&, SireCAS::Expression&);

namespace SireCAS
{

using SireMaths::Rational;

class Identities;
class Symbols;
class Functions;

/**
An Expression is the base class of all algebraic entities.

@author Christopher Woods
*/
class SIRECAS_EXPORT Expression
{

friend QDataStream& ::operator<<(QDataStream&, const Expression&);
friend QDataStream& ::operator>>(QDataStream&, Expression&);

public:
    Expression();
    
    Expression(int constant);
    Expression(const Rational &constant);
    Expression(double constant);
    Expression(const Complex &constant);
    
    Expression(const ExpressionBase &base);
    Expression(const ExBase &base);
    
    Expression(const Expression &other);
    
    ~Expression();

    bool operator==(const Expression &other) const;
    bool operator!=(const Expression &other) const;
    
    Expression& operator=(const Expression &other);

    Expression& operator+=(const Expression &other);
    Expression& operator-=(const Expression &other);
    Expression& operator*=(const Expression &other);
    Expression& operator/=(const Expression &other);

    double operator()(const Values &values) const;
    Complex operator()(const ComplexValues &values) const;

    Expression operator-() const;

    Expression add(const Expression &ex) const;
    Expression subtract(const Expression &ex) const;
    Expression multiply(const Expression &ex) const;
    Expression divide(const Expression &ex) const;
    
    Expression multiply(double val) const;
    Expression divide(double val) const;
    
    Expression multiply(const Complex &val) const;
    Expression divide(const Complex &val) const;
    
    Expression negate() const;
    Expression invert() const;
    Expression conjugate() const;
    
    Expression pow(int n) const;
    Expression squared() const;
    Expression cubed() const;

    Expression pow(const Rational &n) const;
    Expression pow(double n) const;
    Expression pow(const Complex &n) const;
    Expression pow(const Expression &n) const;

    Expression root(int n) const;

    Expression substitute(const Identities &identities) const;
    Expression simplify(int options=0) const;

    Expression expand() const;
    Expression collapse() const;

    double factor() const;

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
    Expression differentiate(const Symbol &symbol, int level=1) const;
    Expression integrate(const Symbol &symbol) const;
    
    Expression diff(const Symbol &symbol, int level=1) const;
    Expression integ(const Symbol &symbol) const;
    
    Expression series(const Symbol &symbol, int order) const;
    
    bool isZero() const;
    bool isConstant() const;
    bool isFunction(const Symbol &symbol) const;
    bool isCompound() const;
    bool isComplex() const;
    
    QString toString() const;
    
    uint hash() const
    {
        return exbase.hash();
    }
    
    const ExpressionBase& base() const;

    Symbols symbols() const;
    Functions functions() const;
    Expressions children() const;

private:
    
    /** The base of this expression */
    ExpressionBase exbase;

    /** The factor of the expression */
    double fac;
};

/** Comparison operator */
inline bool Expression::operator==(const Expression &other) const
{
    return fac == other.fac and exbase == other.exbase;
}

/** Comparison operator */
inline bool Expression::operator!=(const Expression &other) const
{
    return fac != other.fac or exbase != other.exbase;
}

/** Addition operator */
inline Expression operator+(const Expression &ex0, 
                            const Expression &ex1)
{
    return ex0.add(ex1);
}

/** Subtraction operator */
inline Expression operator-(const Expression &ex0,
                            const Expression &ex1)
{
    return ex0.subtract(ex1);
}

/** Multiplication operator */
inline Expression operator*(const Expression &ex0,
                            const Expression &ex1)
{
    return ex0.multiply(ex1);
}

/** Multiplication operator */
inline Expression operator*(double val, const Expression &ex)
{
    return ex.multiply(val);
}

/** Multiplication operator */
inline Expression operator*(const Expression &ex, double val)
{
    return ex.multiply(val);
}

/** Multiplication operator */
inline Expression operator*(const Complex &val, const Expression &ex)
{
    return ex.multiply(val);
}

/** Multiplication operator */
inline Expression operator*(const Expression &ex, const Complex &val)
{
    return ex.multiply(val);
}

/** Division operator */
inline Expression operator/(const Expression &ex0,
                            const Expression &ex1)
{
    return ex0.divide(ex1);
}

/** Raise an expression to the nth power */
inline Expression pow(const Expression &ex0, int n)
{
    return ex0.pow(n);
}

/** Raise an expression to a rational power */
inline Expression pow(const Expression &ex0, 
                      const SireMaths::Rational &n)
{
    return ex0.pow(n);
}

/** Raise an expression to a real power */
inline Expression pow(const Expression &ex0, double n)
{
    return ex0.pow(n);
}

/** Raise an expression to a functional power */
inline Expression pow(const Expression &ex0,
                      const Expression &n)
{
    return ex0.pow(n);
}

/** Raise an expression to a complex power */
inline Expression pow(const Expression &ex0, const Complex &n)
{
    return ex0.pow(n);
}

/** Take the nth root of an expression */
inline Expression root(const Expression &ex0, int n)
{
    return ex0.root(n);
}

/** Take the square root of an expression */
inline Expression sqrt(const Expression &ex0)
{
    return ex0.root(2);
}

/** Take the cube root of an expression */
inline Expression cbrt(const Expression &ex0)
{
    return ex0.root(3);
}

}

/** Return a hash for an expression */
inline uint qHash(const SireCAS::Expression &ex)
{
    return ex.hash();
}

Q_DECLARE_METATYPE(SireCAS::Expression)

SIRE_END_HEADER

#endif
