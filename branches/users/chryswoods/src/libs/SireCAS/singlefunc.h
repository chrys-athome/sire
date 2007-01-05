#ifndef SIRECAS_SINGLEFUNC_H
#define SIRECAS_SINGLEFUNC_H

#include "exbase.h"
#include "symbol.h"
#include "symbols.h"
#include "functions.h"
#include "expression.h"
#include "expressions.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class SingleFunc;
}

QDataStream& operator<<(QDataStream&, const SireCAS::SingleFunc&);
QDataStream& operator>>(QDataStream&, SireCAS::SingleFunc&);

namespace SireCAS
{

/** Base class of all single-expression functions (e.g. g( f(??) ))

    @author Christopher Woods
*/
class SIRECAS_EXPORT SingleFunc : public ExBase
{

friend QDataStream& ::operator<<(QDataStream&, const SingleFunc&);
friend QDataStream& ::operator>>(QDataStream&, SingleFunc&);

public:
    SingleFunc();
    SingleFunc(const Expression &ex);

    SingleFunc(const SingleFunc &other);

    ~SingleFunc();

    SingleFunc& operator=(const SingleFunc &other);

    uint hash() const;

    const Expression& argument() const;
    const Expression& x() const;

    Expression expand() const;
    Expression collapse() const;
    Expression conjugate() const;

    bool isFunction(const Symbol &symbol) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;

    QString toString() const;

    Expression substitute(const Identities &identities) const;
    Symbols symbols() const;
    Functions functions() const;
    Expressions children() const;

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

protected:

    virtual Expression functionOf(const Expression &arg) const=0;
    virtual QString stringRep() const=0;
    virtual uint magic() const=0;

    virtual Expression diff() const;
    virtual Expression integ() const;

    /** The expression that this function operates on */
    Expression ex;
};

/** Return the single argument to this function */
inline const Expression& SingleFunc::argument() const
{
    return ex;
}

/** Synonym for argument() - useful when doing calculus, and viewing
    the function as being a pure f(x) */
inline const Expression& SingleFunc::x() const
{
    return ex;
}

/** Return a has for the function */
inline uint SingleFunc::hash() const
{
    return (magic() << 16) | (ex.hash() & 0x0000FFFF);
}

/** To declare a new function, copy the below;

class MyFunc : public SingleFunc
{
public:
    MyFunc();
    MyFunc(const Expression &ex);

    MyFunc(const MyFunc &other);

    ~MyFunc();

    /// optional functions
    //Expression series(const Symbol &symbol, int n) const;
    //Expression simplify(int options=0) const;

    /// required functions
    bool operator==(const ExBase &other) const;

    const char* what() const
    {
        return "SireCAS::MyFunc";
    }

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

protected:
    //required functions
    ExBase* clone() const
    {
        return new MyFunc(*this);
    }

    Expression functionOf(const Expression &arg) const
    {
        if (arg == argument())
            return toExpression();
        else
            return MyFunc(arg).toExpression();
    }

    //optional
    Expression diff() const;
    Expression integ() const;

    QString stringRep() const
    {
        return "myfunc";
    }

    uint magic() const;
};

static RegisterExpression<MyFunc> RegisterMyFunc;

*/

}

SIRE_END_HEADER

#endif
