#ifndef SIREFF_FFEXPRESSION_H
#define SIREFF_FFEXPRESSION_H

#include "SireCAS/values.h"
#include "SireCAS/expression.h"
#include "SireCAS/symbol.h"
#include "SireCAS/symbols.h"
#include "SireCAS/function.h"
#include "SireCAS/functions.h"

#include "forcefieldid.h"
#include "ffcomponent.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFExpression;
}

QDataStream& operator<<(QDataStream&, const SireFF::FFExpression&);
QDataStream& operator>>(QDataStream&, SireFF::FFExpression&);

namespace SireFF
{

using SireCAS::Symbol;
using SireCAS::SymbolID;
using SireCAS::Expression;
using SireCAS::Function;
using SireCAS::Functions;
using SireCAS::Values;

/** This class is used by System to hold metainformation
    about an energy expression.

    @author Christopher Woods
*/
class SIREFF_EXPORT FFExpression
{

friend QDataStream& ::operator<<(QDataStream&, const FFExpression&);
friend QDataStream& ::operator>>(QDataStream&, FFExpression&);

public:
    FFExpression();

    FFExpression(const QString &name, const Expression &expression);

    FFExpression(const Function &func);

    FFExpression(const FFExpression &other);

    ~FFExpression();

    bool operator==(const FFExpression &other) const;
    bool operator!=(const FFExpression &other) const;

    const QSet<ForceFieldID>& forceFieldIDs() const;
    const QSet<Function>& dependencies() const;

    QSet<FFComponent> components() const;

    const Function& function() const;

    SymbolID ID() const;

    const Expression& expression() const;

    double evaluate(const Values &values) const;

    QString toString() const;

private:
    /** Initialise this object from the expression 'expression' */
    void initialise(const Expression &expression);

    /** The expression */
    Expression ex;

    /** The function that represents this expression */
    Function func;

    /** The IDs of the forcefields on which this
        expression depends */
    QSet<ForceFieldID> ffids;

    /** The functions on which this forcefield depends
        (not including energy components) */
    Functions deps;
};

/** Return the ID numbers of the forcefields that contain
    components on which this expression depends */
inline const QSet<ForceFieldID>& FFExpression::forceFieldIDs() const
{
    return ffids;
}

/** Return the non-forcefield functions on which this
    expression depends */
inline const QSet<Function>& FFExpression::dependencies() const
{
    return deps;
}

/** Return the function that can be used to refer to this
    expression */
inline const Function& FFExpression::function() const
{
    return func;
}

/** Return the actual expression */
inline const Expression& FFExpression::expression() const
{
    return ex;
}

/** Evaluate the value of this expression given the passed
    values of all of the symbols and functions. */
inline double FFExpression::evaluate(const Values &values) const
{
    return ex.evaluate(values);
}

/** Return a string representation of this function */
inline QString FFExpression::toString() const
{
    return ex.toString();
}

/** Return the ID of the symbol representing this expression */
inline SymbolID FFExpression::ID() const
{
    return func.ID();
}

}

Q_DECLARE_METATYPE(SireFF::FFExpression);

SIRE_END_HEADER

#endif
