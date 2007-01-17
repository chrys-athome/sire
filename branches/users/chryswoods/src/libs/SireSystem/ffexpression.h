#ifndef SIRESYSTEM_FFEXPRESSION_H
#define SIRESYSTEM_FFEXPRESSION_H

#include "SireCAS/values.h"
#include "SireCAS/expression.h"
#include "SireCAS/symbol.h"
#include "SireCAS/symbols.h"
#include "SireCAS/function.h"
#include "SireCAS/functions.h"

#include "SireFF/forcefieldid.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class FFExpression;
}

QDataStream& operator<<(QDataStream&, const SireSystem::FFExpression&);
QDataStream& operator>>(QDataStream&, SireSystem::FFExpression&);

namespace SireSystem
{

using SireCAS::Symbol;
using SireCAS::Expression;
using SireCAS::Function;
using SireCAS::Functions;
using SireCAS::Values;

using SireFF::ForceFieldID;

/** This class is used by System to hold metainformation
    about an energy expression.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT FFExpression
{
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

    const Function& function() const;

    const Expression& expression() const;

    double evaluate(const Values &values) const;

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

}

Q_DECLARE_METATYPE(SireSystem::FFExpression);

SIRE_END_HEADER

#endif
