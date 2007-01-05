#ifndef SIRECAS_EXPRESSIONBASE_H
#define SIRECAS_EXPRESSIONBASE_H

#include <QString>

#include "exbase.h"

#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Complex;
}

namespace SireCAS
{
class ExpressionBase;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireCAS::ExpressionBase&);
QDataStream& operator>>(QDataStream&, SireCAS::ExpressionBase&);

namespace SireCAS
{

class ExBase;
class Expression;
class Expressions;
class Identities;
class Symbol;
class Symbols;
class Functions;
class Values;
class ComplexValues;

using SireMaths::Complex;
using SireBase::SharedPolyPointer;

/** This class provides implicitly shared access to ExBase objects, which
    are a virtual class hierarchy that provide all of the functionality of
    the expressions.

    @author Christopher Woods
*/
class SIRECAS_EXPORT ExpressionBase
{

friend QDataStream& ::operator<<(QDataStream&, const ExpressionBase&);
friend QDataStream& ::operator>>(QDataStream&, ExpressionBase&);

public:
    ExpressionBase();

    ExpressionBase(const ExBase &ex);

    ExpressionBase(const ExpressionBase &other);

    ~ExpressionBase();

    bool operator==(const ExpressionBase &other) const;
    bool operator!=(const ExpressionBase &other) const;

    ExpressionBase& operator=(const ExpressionBase &other);
    ExpressionBase& operator=(const ExBase &other);

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    Expression series(const Symbol &symbol, int n) const;

    Expression simplify(int options=0) const;

    Expression expand() const;
    Expression collapse() const;
    Expression conjugate() const;

    bool isFunction(const Symbol&) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;

    uint hash() const;

    const char* what() const;
    QString toString() const;

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression substitute(const Identities &identities) const;

    Symbols symbols() const;
    Functions functions() const;
    Expressions children() const;

    template<class T>
    bool isA() const
    {
        return d->isA<T>();
    }

    template<class T>
    const T& asA() const
    {
        return d->asA<T>();
    }

private:
    /** Shared pointer to the expression */
    SharedPolyPointer<ExBase> d;
};

}

/** Return a hash for an ExpressionBase */
inline uint qHash(const SireCAS::ExpressionBase &ex)
{
    return ex.hash();
}

Q_DECLARE_METATYPE(SireCAS::ExpressionBase)

SIRE_END_HEADER

#endif
