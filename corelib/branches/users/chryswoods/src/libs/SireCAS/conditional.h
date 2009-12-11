/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#ifndef SIRECAS_CONDITIONAL_H
#define SIRECAS_CONDITIONAL_H

#include "casnode.h"
#include "expression.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** This is the base class of all conditional statements 
    (e.g. x < 2*y, or z == 3*w)
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT Condition : public Siren::Extends<Condition,CASNode>
{
public:
    Condition();
    Condition(const Expression &lhs, const Expression &rhs);
    
    Condition(const Condition &other);
    
    ~Condition();

    static QString typeName();

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    Expression simplify(int options=0) const;

    Expression conjugate() const;

    bool isFunction(const Symbol&) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;

    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);

    Expression substitute(const Identities &identities) const;

    QSet<Symbol> symbols() const;
    QList<Expression> children() const;

    QList<Factor> expand(const Symbol &symbol) const;
    
    const Expression& leftHandSide() const;
    const Expression& rightHandSide() const;

    virtual bool alwaysTrue() const=0;
    virtual bool alwaysFalse() const=0;
    
    bool evaluateCondition(const Values &values) const;
    bool evaluateCondition(const ComplexValues &values) const;

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;
    
protected:
    Condition& operator=(const Condition &other);
    
    bool operator==(const Condition &other) const;
    bool operator!=(const Condition &other) const;
    
    virtual bool compareValues(double val0, double val1) const=0;
    virtual bool compareValues(const SireMaths::Complex &val0, const SireMaths::Complex &val1) const=0;
    
    virtual QString operatorString() const=0;

private:
    /** The left hand side of the condition */
    Expression lhs;
    
    /** The right hand side of the condition */
    Expression rhs;
};

/** This is a conditional function that returns whether or 
    not the first expression is greater than the second
    expression 
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT GreaterThan : public Siren::Implements<GreaterThan,Condition>
{
public:
    GreaterThan();
    GreaterThan(const Expression &left_hand_side, 
                const Expression &right_hand_side);
    
    GreaterThan(const GreaterThan &other);
    
    ~GreaterThan();
    
    GreaterThan& operator=(const GreaterThan &other);
    
    bool operator==(const GreaterThan &other) const;
    bool operator!=(const GreaterThan &other) const;
    
    void stream(Siren::Stream &s);

    bool alwaysTrue() const;
    bool alwaysFalse() const;

protected:
    bool compareValues(double val0, double val1) const;
    bool compareValues(const SireMaths::Complex &val0, const SireMaths::Complex &val1) const;

    QString operatorString() const;
};

/** This is a conditional function that returns whether or 
    not the first expression is less than the second
    expression
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT LessThan : public Siren::Implements<LessThan,Condition>
{
public:
    LessThan();
    LessThan(const Expression &left_hand_side, 
             const Expression &right_hand_side);
    
    LessThan(const LessThan &other);
    
    ~LessThan();
    
    LessThan& operator=(const LessThan &other);
    
    bool operator==(const LessThan &other) const;
    bool operator!=(const LessThan &other) const;

    void stream(Siren::Stream &s);

    bool alwaysTrue() const;
    bool alwaysFalse() const;

protected:
    bool compareValues(double val0, double val1) const;
    bool compareValues(const SireMaths::Complex &val0, const SireMaths::Complex &val1) const;

    QString operatorString() const;
};

/** This is a conditional function that returns whether or 
    not the first expression is greater than or equal to 
    the second expression 
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT GreaterOrEqualThan 
            : public Siren::Implements<GreaterOrEqualThan,Condition>
{
public:
    GreaterOrEqualThan();
    GreaterOrEqualThan(const Expression &left_hand_side, 
                       const Expression &right_hand_side);
    
    GreaterOrEqualThan(const GreaterOrEqualThan &other);
    
    ~GreaterOrEqualThan();
    
    GreaterOrEqualThan& operator=(const GreaterOrEqualThan &other);
    
    bool operator==(const GreaterOrEqualThan &other) const;
    bool operator!=(const GreaterOrEqualThan &other) const;

    void stream(Siren::Stream &s);

    bool alwaysTrue() const;
    bool alwaysFalse() const;

protected:
    bool compareValues(double val0, double val1) const;
    bool compareValues(const SireMaths::Complex &val0, const SireMaths::Complex &val1) const;

    QString operatorString() const;
};

/** This is a conditional function that returns whether or 
    not the first expression is less than or equal to 
    the second expression 
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT LessOrEqualThan 
            : public Siren::Implements<LessOrEqualThan,Condition>
{
public:
    LessOrEqualThan();
    LessOrEqualThan(const Expression &left_hand_side, 
                    const Expression &right_hand_side);
    
    LessOrEqualThan(const LessOrEqualThan &other);
    
    ~LessOrEqualThan();
    
    LessOrEqualThan& operator=(const LessOrEqualThan &other);
    
    bool operator==(const LessOrEqualThan &other) const;
    bool operator!=(const LessOrEqualThan &other) const;

    void stream(Siren::Stream &s);

    bool alwaysTrue() const;
    bool alwaysFalse() const;

protected:
    bool compareValues(double val0, double val1) const;
    bool compareValues(const SireMaths::Complex &val0, const SireMaths::Complex &val1) const;

    QString operatorString() const;
};

/** This is a conditional function that returns whether or 
    not the first expression is equal to the second
    expression 
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT EqualTo : public Siren::Implements<EqualTo,Condition>
{
public:
    EqualTo();
    EqualTo(const Expression &left_hand_side, 
            const Expression &right_hand_side);
    
    EqualTo(const EqualTo &other);
    
    ~EqualTo();
    
    EqualTo& operator=(const EqualTo &other);
    
    bool operator==(const EqualTo &other) const;
    bool operator!=(const EqualTo &other) const;

    void stream(Siren::Stream &s);

    bool alwaysTrue() const;
    bool alwaysFalse() const;

protected:
    bool compareValues(double val0, double val1) const;
    bool compareValues(const SireMaths::Complex &val0, const SireMaths::Complex &val1) const;

    QString operatorString() const;
};

/** This is a conditional function that returns whether or 
    not the first expression is not equal to the second
    expression 
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT NotEqualTo : public Siren::Implements<NotEqualTo,Condition>
{
public:
    NotEqualTo();
    NotEqualTo(const Expression &left_hand_side, 
               const Expression &right_hand_side);
    
    NotEqualTo(const NotEqualTo &other);
    
    ~NotEqualTo();
    
    NotEqualTo& operator=(const NotEqualTo &other);
    
    bool operator==(const NotEqualTo &other) const;
    bool operator!=(const NotEqualTo &other) const;

    void stream(Siren::Stream &s);

    bool alwaysTrue() const;
    bool alwaysFalse() const;

protected:
    bool compareValues(double val0, double val1) const;
    bool compareValues(const SireMaths::Complex &val0, const SireMaths::Complex &val1) const;

    QString operatorString() const;
};

/** This is an overloaded conditional that is always true */
class SIRECAS_EXPORT AlwaysTrue : public Siren::Implements<AlwaysTrue,Condition>
{
public:
    AlwaysTrue();
    AlwaysTrue(const AlwaysTrue &other);
    
    ~AlwaysTrue();
    
    AlwaysTrue& operator=(const AlwaysTrue &other);
    
    bool operator==(const AlwaysTrue &other) const;
    bool operator!=(const AlwaysTrue &other) const;

    QString toString() const;
    void stream(Siren::Stream &s);
    uint hashCode() const;
    
    Expression simplify(int options) const;

    bool alwaysTrue() const;
    bool alwaysFalse() const;

    bool isFunction(const Symbol&) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;

    Expression substitute(const Identities &identities) const;

    QSet<Symbol> symbols() const;
    QList<Expression> children() const;

    QList<Factor> expand(const Symbol &symbol) const;
    
    bool evaluateCondition(const Values &values) const;
    bool evaluateCondition(const ComplexValues &values) const;

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    bool compareValues(double val0, double val1) const;
    bool compareValues(const SireMaths::Complex &val0, const SireMaths::Complex &val1) const;

    QString operatorString() const;
};

/** This is an overloaded conditional that is always false */
class SIRECAS_EXPORT AlwaysFalse : public Siren::Implements<AlwaysFalse,Condition>
{
public:
    AlwaysFalse();
    AlwaysFalse(const AlwaysFalse &other);
    
    ~AlwaysFalse();
    
    AlwaysFalse& operator=(const AlwaysFalse &other);
    
    bool operator==(const AlwaysFalse &other) const;
    bool operator!=(const AlwaysFalse &other) const;

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    Expression simplify(int options) const;

    bool alwaysTrue() const;
    bool alwaysFalse() const;

    bool isFunction(const Symbol&) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;

    Expression substitute(const Identities &identities) const;

    QSet<Symbol> symbols() const;
    QList<Expression> children() const;

    QList<Factor> expand(const Symbol &symbol) const;
    
    bool evaluateCondition(const Values &values) const;
    bool evaluateCondition(const ComplexValues &values) const;

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

protected:
    bool compareValues(double val0, double val1) const;
    bool compareValues(const SireMaths::Complex &val0, const SireMaths::Complex &val1) const;

    QString operatorString() const;
};

/** This is a conditional expression. If the condition is true,
    then true_expression is evaluated, else if the condition
    is false then false_expression is evaluate
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT Conditional : public Siren::Implements<Conditional,CASNode>
{
public:
    Conditional();
    
    Conditional(const Condition &condition, 
                const Expression &true_expression,
                const Expression &false_expression);
                
    Conditional(const Conditional &other);
    
    ~Conditional();
    
    Conditional& operator=(const Conditional &other);
    
    bool operator==(const Conditional &other) const;
    bool operator!=(const Conditional &other) const;

    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);

    const Condition& condition() const;
    
    const Expression& trueExpression() const;
    const Expression& falseExpression() const;

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    Expression series(const Symbol &symbol, int n) const;

    Expression simplify(int options=0) const;

    Expression conjugate() const;

    bool isFunction(const Symbol&) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

    Expression substitute(const Identities &identities) const;

    QSet<Symbol> symbols() const;
    QList<Expression> children() const;

    QList<Factor> expand(const Symbol &symbol) const;

private:
    /** The condition to be evaluated */
    CASNodePtr cond;
    
    /** The expression to evaluate if the condition is true */
    Expression true_expression;
    
    /** The expression to evaluate if the condition is false */
    Expression false_expression;
};

}

Q_DECLARE_METATYPE( SireCAS::Conditional )

Q_DECLARE_METATYPE( SireCAS::GreaterThan )
Q_DECLARE_METATYPE( SireCAS::LessThan )
Q_DECLARE_METATYPE( SireCAS::GreaterOrEqualThan )
Q_DECLARE_METATYPE( SireCAS::LessOrEqualThan )
Q_DECLARE_METATYPE( SireCAS::EqualTo )
Q_DECLARE_METATYPE( SireCAS::NotEqualTo )
Q_DECLARE_METATYPE( SireCAS::AlwaysTrue )
Q_DECLARE_METATYPE( SireCAS::AlwaysFalse )

SIRE_EXPOSE_CLASS( SireCAS::Conditional )

SIRE_EXPOSE_CLASS( SireCAS::Condition )
SIRE_EXPOSE_CLASS( SireCAS::GreaterThan )
SIRE_EXPOSE_CLASS( SireCAS::LessThan )
SIRE_EXPOSE_CLASS( SireCAS::GreaterOrEqualThan )
SIRE_EXPOSE_CLASS( SireCAS::LessOrEqualThan )
SIRE_EXPOSE_CLASS( SireCAS::EqualTo )
SIRE_EXPOSE_CLASS( SireCAS::NotEqualTo )
SIRE_EXPOSE_CLASS( SireCAS::AlwaysTrue )
SIRE_EXPOSE_CLASS( SireCAS::AlwaysFalse )

#endif
