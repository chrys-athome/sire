#ifndef SIRECAS_EXBASE_H
#define SIRECAS_EXBASE_H

#include <QString>
#include <QSharedData>

#include "SireMaths/rational.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class ExBase;
}

QDataStream& operator<<(QDataStream&, const SireCAS::ExBase&);
QDataStream& operator>>(QDataStream&, SireCAS::ExBase&);

namespace SireMaths
{
class Complex;
}

namespace SireCAS
{

class Values;
class ComplexValues;
class Symbol;
class Symbols;
class Functions;
class Expression;
class Expressions;
class Identities;

class ExpressionBase;
class Expression;

using SireMaths::Complex;
using SireMaths::Rational;

/**
Pure-virtual base class of all of the parts of mathematical expressions.

This class provides the 'atom' of SireCAS. ExBase objects are combined together
to form complete expressions. All constants, functions and symbols are derived
from this object.

This class is an example of an implicitly shared, self-managed object, and
is designed so that it can be held by SharedPolyPointer (indeed,
ExpressionBase is just a proxy for SharedPolyPointer<ExBase>).

@author Christopher Woods
*/
class SIRECAS_EXPORT ExBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const ExBase&);
friend QDataStream& ::operator>>(QDataStream&, ExBase&);

public:
    ExBase();

    ExBase(const ExBase &other);

    virtual ~ExBase();

    ///////
    /////// Non-virtual functions
    ///////

    bool operator!=(const ExBase &other) const;
    Expression operator-() const;

    /** Return whether or not this is an ExBase of type 'T' */
    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }

    /** Return this ExBase cast as a const reference to type 'T'.
        Note that this is only safe if 'isA<T>()' returns true. */
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }

    ///////
    /////// Virtual functions - you may wish to override these
    /////// in your derived class
    ///////

    virtual Expression differentiate(const Symbol &symbol) const;
    virtual Expression integrate(const Symbol &symbol) const;

    virtual Expression series(const Symbol &symbol, int n) const;

    virtual Expression simplify(int options=0) const;

    virtual Expression expand() const;
    virtual Expression collapse() const;
    virtual Expression conjugate() const;

    virtual bool isFunction(const Symbol&) const;
    virtual bool isConstant() const;
    virtual bool isComplex() const;
    virtual bool isCompound() const;

    ///////
    /////// Pure-virtual functions - these must be overridden
    /////// in your derived class
    ///////

    /** Comparison operator - only return true if these are the same
        class and contain the same data. */
    virtual bool operator==(const ExBase &other) const=0;

    /** Return a hash of this object - return a combination of the
        identifying magic for the class and a hash for its contents. */
    virtual uint hash() const=0;

    /** Return the name of the type of this ExBase object */
    virtual const char* what() const=0;

    /** Return the name of this class type */
    static const char* typeName()
    {
        return "SireCAS::ExBase";
    }

    /** Return a string representation of this object */
    virtual QString toString() const=0;

    /** Return a clone of this object */
    virtual ExBase* clone() const=0;

    /** Evaluate this ExBase using values 'values'. Any
        missing symbols are assumed to equal zero.

        Note that an exception will be thrown if the result of the
        evaluation of this, or one of its children, is complex.

        \throw SireMaths::domain_error
    */
    virtual double evaluate(const Values &values) const=0;

    /** Evaluate this ExBase using the complex values 'values'.
        Any missing symbols are assumed to equal zero. */
    virtual Complex evaluate(const ComplexValues &values) const=0;

    /** Return an expression that has the identities in 'identities'
        substituted into this expression */
    virtual Expression substitute(const Identities &identities) const=0;

    /** Return the set of Symbols that appear in this ExBase */
    virtual Symbols symbols() const=0;

    /** Return the set of Functions that appear in this ExBase */
    virtual Functions functions() const=0;

    /** Return the child expressions of this Expression */
    virtual Expressions children() const=0;

private:

    /** You cannot assign-copy an ExBase */
    ExBase& operator=(const ExBase&)
    {
        return *this;
    }
};

Expression operator+(const ExBase &base0, const ExBase &base1);
Expression operator-(const ExBase &base0, const ExBase &base1);
Expression operator*(const ExBase &base0, const ExBase &base1);
Expression operator/(const ExBase &base0, const ExBase &base1);

Expression operator*(const ExBase &base, double val);
Expression operator*(double val, const ExBase &base);
Expression operator*(const ExBase &base, const Complex &val);
Expression operator*(const Complex &val, const ExBase &base);

Expression operator+(const ExBase &base, const Expression &ex);
Expression operator-(const ExBase &base, const Expression &ex);
Expression operator*(const ExBase &base, const Expression &ex);
Expression operator/(const ExBase &base, const Expression &ex);
Expression operator+(const Expression &ex, const ExBase &base);
Expression operator-(const Expression &ex, const ExBase &base);
Expression operator*(const Expression &ex, const ExBase &base);
Expression operator/(const Expression &ex, const ExBase &base);

Expression pow(const ExBase &base, int n);
Expression pow(const ExBase &base, const Rational &n);
Expression pow(const ExBase &base, double n);
Expression pow(const ExBase &base, const Complex &n);
Expression pow(const ExBase &base, const Expression &n);
Expression pow(const ExBase &base, const ExBase &n);

}

/** Return a hash of an ExBase object */
inline uint qHash(const SireCAS::ExBase &ex)
{
    return ex.hash();
}

SIRE_END_HEADER

#endif
