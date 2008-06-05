#ifndef SIRECAS_EXPRESSIONBASE_H
#define SIRECAS_EXPRESSIONBASE_H

#include <QString>

#include <boost/shared_ptr.hpp>

#include "sireglobal.h"

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
    
    ExpressionBase(const boost::shared_ptr<const ExBase> &ptr);
    ExpressionBase(const ExBase &ex);
    
    ~ExpressionBase();

    bool operator==(const ExpressionBase &other) const;
    bool operator!=(const ExpressionBase &other) const;
    
    ExpressionBase& operator=(const ExpressionBase &other);

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

    QString what() const;
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
        return dynamic_cast<const T*>(d.get()) != 0;
    }
    
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*d);
    }

private:
    boost::shared_ptr<const ExBase> d;
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
