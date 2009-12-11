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

#include "conditional.h"
#include "identities.h"
#include "values.h"
#include "complexvalues.h"
#include "factor.h"

#include "SireCAS/errors.h"

#include "Siren/objref.h"
#include "Siren/stream.h"
#include "Siren/errors.h"

#include "SireMaths/complex.h"
#include "SireMaths/errors.h"

using namespace SireCAS;
using namespace SireMaths;
using namespace Siren;

///////////
/////////// Implementation of Condition
///////////

static const RegisterObject<Condition> r_condition( VIRTUAL_CLASS );

/** Null constructor */
Condition::Condition() : Extends<Condition,CASNode>()
{}

/** Construct with the passed left and right hand side of the condition */
Condition::Condition(const Expression &lhs_, const Expression &rhs_)
          : Extends<Condition,CASNode>(), lhs(lhs_), rhs(rhs_)
{}

/** Copy constructor */
Condition::Condition(const Condition &other)
          : Extends<Condition,CASNode>(other), lhs(other.lhs), rhs(other.rhs)
{}

/** Destructor */
Condition::~Condition()
{}

/** Copy assignment operator */
Condition& Condition::operator=(const Condition &other)
{
    lhs = other.lhs;
    rhs = other.rhs;
    CASNode::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool Condition::operator==(const Condition &other) const
{
    return this == &other or
           (lhs == other.lhs and rhs == other.rhs);
}

/** Comparison operator */
bool Condition::operator!=(const Condition &other) const
{
    return not Condition::operator==(other);
}

/** Differentiate this expression */
Expression Condition::differentiate(const Symbol &symbol) const
{
    throw SireCAS::unavailable_differential( QObject::tr(
              "No differential is available for the condition \"%1\"")
                   .arg(this->toString()), CODELOC );

    return Expression();
}

/** Integrate this expression */
Expression Condition::integrate(const Symbol &symbol) const
{
    throw SireCAS::unavailable_integral( QObject::tr(
            "No integral is available for the condition \"%1\"")
                .arg(this->toString()), CODELOC );
                    
    return Expression();
}

/** Simplify this condition */
Expression Condition::simplify(int options) const
{
    CASNodePtr ret = *this;
    
    Condition &c = ret.edit().asA<Condition>();
    
    c.lhs = lhs.simplify(options);
    c.rhs = rhs.simplify(options);
    
    if (c.alwaysTrue())
        return AlwaysTrue();
    else if (c.alwaysFalse())
        return AlwaysFalse();
    else
        return c;
}

/** The complex conjugate of a condition is the condition */
Expression Condition::conjugate() const
{
    return *this;
}

/** Return whether or not this function is a function of 'symbol' */
bool Condition::isFunction(const Symbol &symbol) const
{
    return lhs.isFunction(symbol) or rhs.isFunction(symbol);
}

/** Return whether or not this is a constant */
bool Condition::isConstant() const
{
    return (lhs.isConstant() and rhs.isConstant()) or (lhs == rhs);
}

/** Return whether or not this is a complex condition */
bool Condition::isComplex() const
{
    return lhs.isComplex() or rhs.isComplex();
}

/** Return whether or not this is a compound expression */
bool Condition::isCompound() const
{
    return not this->isConstant();
}

/** Substitute in the passed identities */
Expression Condition::substitute(const Identities &identities) const
{
    CASNodePtr ret( *this );
    
    Condition &c = ret.edit().asA<Condition>();
    
    c.lhs = lhs.substitute(identities);
    c.rhs = rhs.substitute(identities);
    
    return c;
}

/** Return the symbols used in this expression */
QSet<Symbol> Condition::symbols() const
{
    if (this->alwaysTrue())
        return lhs.symbols();
    else if (this->alwaysFalse())
        return rhs.symbols();
    else
        return lhs.symbols() + rhs.symbols();
}

/** Return all of the child expressions in this condition */
QList<Expression> Condition::children() const
{
    return lhs.children() + rhs.children();
}

/** Expand this condition into factors of the passed symbol */
QList<Factor> Condition::expand(const Symbol &symbol) const
{
    throw SireCAS::rearrangement_error( QObject::tr(
        "The conditional expression \"%1\" cannot be expanded in terms "
        "of %2.")
            .arg(this->toString(), symbol.toString()), CODELOC );
                
    return QList<Factor>();
}

/** Return the left hand side of this condition */
const Expression& Condition::leftHandSide() const
{
    return lhs;
}

/** Return the right hand side of this condition */
const Expression& Condition::rightHandSide() const
{
    return rhs;
}

/** Return a string representation of this expression */
QString Condition::toString() const
{
    return QObject::tr("%1 %2 %3")
                .arg(lhs.toString(), this->operatorString(), rhs.toString());
}

uint Condition::hashCode() const
{
    return qHash(this->what()) + qHash(lhs) + qHash(rhs);
}

void Condition::stream(Stream &s)
{
    s.assertVersion<Condition>(1);
    
    Schema schema = s.item<Condition>();
    
    schema.data("left-hand-side") & lhs;
    schema.data("right-hand-side") & rhs;
    
    CASNode::stream( schema.base() );
}

/** Evalute this condition, returning whether or not it is true or false */
bool Condition::evaluateCondition(const Values &values) const
{
    double lhs_value = lhs.evaluate(values);
    double rhs_value = rhs.evaluate(values);
    
    return this->compareValues(lhs_value, rhs_value);
}

/** Evalute this condition, returning whether or not it is true or false */
bool Condition::evaluateCondition(const ComplexValues &values) const
{
    Complex lhs_value = lhs.evaluate(values);
    Complex rhs_value = rhs.evaluate(values);
    
    return this->compareValues(lhs_value, rhs_value);
}

/** Evaluate this expression - this returns '1' if it is true, else it
    returns '0' if it is false */
double Condition::evaluate(const Values &values) const
{
    if (this->evaluateCondition(values))
        return 1;
    else
        return 0;
}

/** Evaluate this expression - this returns '1' if it is true, else
    it returns '0' if it is false */
Complex Condition::evaluate(const ComplexValues &values) const
{
    if (this->evaluateCondition(values))
        return Complex(1);
    else
        return Complex(0);
}

///////////
/////////// Implementation of GreaterThan
///////////

static const RegisterObject<GreaterThan> r_greaterthan;

/** Constructor */
GreaterThan::GreaterThan() : Implements<GreaterThan,Condition>()
{}

/** Construct to compare 'left_hand_side' with 'right_hand_side' */
GreaterThan::GreaterThan(const Expression &left_hand_side,
                         const Expression &right_hand_side)
            : Implements<GreaterThan,Condition>(left_hand_side, right_hand_side)
{}

/** Copy constructor */
GreaterThan::GreaterThan(const GreaterThan &other)
            : Implements<GreaterThan,Condition>(other)
{}

/** Destructor */
GreaterThan::~GreaterThan()
{}

/** Copy assignment operator */
GreaterThan& GreaterThan::operator=(const GreaterThan &other)
{
    Condition::operator=(other);
    return *this;
}

/** Comparison operator */
bool GreaterThan::operator==(const GreaterThan &other) const
{
    return Condition::operator==(other);
}

/** Comparison operator */
bool GreaterThan::operator!=(const GreaterThan &other) const
{
    return Condition::operator!=(other);
}

void GreaterThan::stream(Stream &s)
{
    s.assertVersion<GreaterThan>(1);
    
    Schema schema = s.item<GreaterThan>();
    
    Condition::stream( schema.base() );
}

/** Compare the values 'val0' and 'val1' */
bool GreaterThan::compareValues(double val0, double val1) const
{
    return val0 > val1;
}

/** Compare the values 'val0' and 'val1' 

    \throw SireMaths::domain_error
*/
bool GreaterThan::compareValues(const Complex &val0, const Complex &val1) const
{
    if (val0 == val1)
        return false;
    
    if (val0.isReal() and val1.isReal())
        return compareValues( val0.real(), val1.real() );
    
    else if (val0.isPurelyComplex() and val1.isPurelyComplex())
        return compareValues( val0.imag(), val1.imag() );
        
    else
        throw SireMaths::domain_error( QObject::tr(
                "Cannot compare the values of complex numbers %1 and %2.")
                    .arg(val0.toString(), val1.toString()), CODELOC );
    
    return false;
}

/** Return the string representation of this operator */
QString GreaterThan::operatorString() const
{
    return QObject::tr(">");
}

/** Return whether or not we can be absolutely sure that this
    condition will always be true. Note that this doesn't try
    too hard, so some things that are always true may not
    be reported here as being always true, e.g. x + 1 > x */
bool GreaterThan::alwaysTrue() const
{
    if (leftHandSide() == rightHandSide())
        return false;
    else if (this->isConstant())
        return this->evaluateCondition( ComplexValues() );
    else
        return false;
}

/** Return whether or not we can be absolutely sure that this
    condition will always be false. Note that this doesn't try
    too hard, so some things that are always false may not
    be reported here as being always false, e.g. x > x + 1 */
bool GreaterThan::alwaysFalse() const
{
    if (leftHandSide() == rightHandSide())
        return true;
    else if (this->isConstant())
        return not (this->evaluateCondition( ComplexValues() ));
    else
        return false;
}

///////////
/////////// Implementation of LessThan
///////////

static const RegisterObject<LessThan> r_lessthan;

/** Constructor */
LessThan::LessThan() : Implements<LessThan,Condition>()
{}

/** Construct to compare 'left_hand_side' with 'right_hand_side' */
LessThan::LessThan(const Expression &left_hand_side,
                   const Expression &right_hand_side)
         : Implements<LessThan,Condition>(left_hand_side, right_hand_side)
{}

/** Copy constructor */
LessThan::LessThan(const LessThan &other)
         : Implements<LessThan,Condition>(other)
{}

/** Destructor */
LessThan::~LessThan()
{}

/** Copy assignment operator */
LessThan& LessThan::operator=(const LessThan &other)
{
    Condition::operator=(other);
    return *this;
}

/** Comparison operator */
bool LessThan::operator==(const LessThan &other) const
{
    return Condition::operator==(other);
}

/** Comparison operator */
bool LessThan::operator!=(const LessThan &other) const
{
    return Condition::operator!=(other);
}

void LessThan::stream(Stream &s)
{
    s.assertVersion<LessThan>(1);
    
    Schema schema = s.item<LessThan>();
    
    Condition::stream( schema.base() );
}

/** Compare the values 'val0' and 'val1' */
bool LessThan::compareValues(double val0, double val1) const
{
    return val0 < val1;
}

/** Compare the values 'val0' and 'val1' 

    \throw SireMaths::domain_error
*/
bool LessThan::compareValues(const Complex &val0, const Complex &val1) const
{
    if (val0 == val1)
        return false;
    
    if (val0.isReal() and val1.isReal())
        return compareValues( val0.real(), val1.real() );
    
    else if (val0.isPurelyComplex() and val1.isPurelyComplex())
        return compareValues( val0.imag(), val1.imag() );
        
    else
        throw SireMaths::domain_error( QObject::tr(
                "Cannot compare the values of complex numbers %1 and %2.")
                    .arg(val0.toString(), val1.toString()), CODELOC );
    
    return false;
}

/** Return the string representation of this operator */
QString LessThan::operatorString() const
{
    return QObject::tr("<");
}

/** Return whether or not we can be absolutely sure that this
    condition will always be true. Note that this doesn't try
    too hard, so some things that are always true may not
    be reported here as being always true, e.g. x < x + 1 */
bool LessThan::alwaysTrue() const
{
    if (leftHandSide() == rightHandSide())
        return false;
    else if (this->isConstant())
        return this->evaluateCondition( ComplexValues() );
    else
        return false;
}

/** Return whether or not we can be absolutely sure that this
    condition will always be false. Note that this doesn't try
    too hard, so some things that are always false may not
    be reported here as being always false, e.g. x + 1 < x */
bool LessThan::alwaysFalse() const
{
    if (leftHandSide() == rightHandSide())
        return true;
    else if (this->isConstant())
        return not (this->evaluateCondition( ComplexValues() ));
    else
        return false;
}

///////////
/////////// Implementation of GreaterOrEqualThan
///////////

static const RegisterObject<GreaterOrEqualThan> r_gethan;

/** Constructor */
GreaterOrEqualThan::GreaterOrEqualThan() : Implements<GreaterOrEqualThan,Condition>()
{}

/** Construct to compare 'left_hand_side' with 'right_hand_side' */
GreaterOrEqualThan::GreaterOrEqualThan(const Expression &left_hand_side,
                                       const Expression &right_hand_side)
        : Implements<GreaterOrEqualThan,Condition>(left_hand_side, right_hand_side)
{}

/** Copy constructor */
GreaterOrEqualThan::GreaterOrEqualThan(const GreaterOrEqualThan &other)
                   : Implements<GreaterOrEqualThan,Condition>(other)
{}

/** Destructor */
GreaterOrEqualThan::~GreaterOrEqualThan()
{}

/** Copy assignment operator */
GreaterOrEqualThan& GreaterOrEqualThan::operator=(const GreaterOrEqualThan &other)
{
    Condition::operator=(other);
    return *this;
}

/** Comparison operator */
bool GreaterOrEqualThan::operator==(const GreaterOrEqualThan &other) const
{
    return Condition::operator==(other);
}

/** Comparison operator */
bool GreaterOrEqualThan::operator!=(const GreaterOrEqualThan &other) const
{
    return Condition::operator!=(other);
}

void GreaterOrEqualThan::stream(Stream &s)
{
    s.assertVersion<GreaterOrEqualThan>(1);
    
    Schema schema = s.item<GreaterOrEqualThan>();
    
    Condition::stream( schema.base() );
}

/** Compare the values 'val0' and 'val1' */
bool GreaterOrEqualThan::compareValues(double val0, double val1) const
{
    return val0 >= val1;
}

/** Compare the values 'val0' and 'val1' 

    \throw SireMaths::domain_error
*/
bool GreaterOrEqualThan::compareValues(const Complex &val0, const Complex &val1) const
{
    if (val0 == val1)
        return true;
    
    if (val0.isReal() and val1.isReal())
        return compareValues( val0.real(), val1.real() );
    
    else if (val0.isPurelyComplex() and val1.isPurelyComplex())
        return compareValues( val0.imag(), val1.imag() );
        
    else
        throw SireMaths::domain_error( QObject::tr(
                "Cannot compare the values of complex numbers %1 and %2.")
                    .arg(val0.toString(), val1.toString()), CODELOC );
    
    return false;
}

/** Return the string representation of this operator */
QString GreaterOrEqualThan::operatorString() const
{
    return QObject::tr(">=");
}

/** Return whether or not we can be absolutely sure that this
    condition will always be true. Note that this doesn't try
    too hard, so some things that are always true may not
    be reported here as being always true, e.g. x + 1 > x */
bool GreaterOrEqualThan::alwaysTrue() const
{
    if (leftHandSide() == rightHandSide())
        return true;
    else if (this->isConstant())
        return this->evaluateCondition( ComplexValues() );
    else
        return false;
}

/** Return whether or not we can be absolutely sure that this
    condition will always be false. Note that this doesn't try
    too hard, so some things that are always false may not
    be reported here as being always false, e.g. x > x + 1 */
bool GreaterOrEqualThan::alwaysFalse() const
{
    if (leftHandSide() == rightHandSide())
        return false;
    else if (this->isConstant())
        return not (this->evaluateCondition( ComplexValues() ));
    else
        return false;
}

///////////
/////////// Implementation of LessOrEqualThan
///////////

static const RegisterObject<LessOrEqualThan> r_lethan;

/** Constructor */
LessOrEqualThan::LessOrEqualThan() : Implements<LessOrEqualThan,Condition>()
{}

/** Construct to compare 'left_hand_side' with 'right_hand_side' */
LessOrEqualThan::LessOrEqualThan(const Expression &left_hand_side,
                                 const Expression &right_hand_side)
                : Implements<LessOrEqualThan,Condition>(left_hand_side, right_hand_side)
{}

/** Copy constructor */
LessOrEqualThan::LessOrEqualThan(const LessOrEqualThan &other)
                : Implements<LessOrEqualThan,Condition>(other)
{}

/** Destructor */
LessOrEqualThan::~LessOrEqualThan()
{}

/** Copy assignment operator */
LessOrEqualThan& LessOrEqualThan::operator=(const LessOrEqualThan &other)
{
    Condition::operator=(other);
    return *this;
}

/** Comparison operator */
bool LessOrEqualThan::operator==(const LessOrEqualThan &other) const
{
    return Condition::operator==(other);
}

/** Comparison operator */
bool LessOrEqualThan::operator!=(const LessOrEqualThan &other) const
{
    return Condition::operator!=(other);
}

void LessOrEqualThan::stream(Stream &s)
{
    s.assertVersion<LessOrEqualThan>(1);
    
    Schema schema = s.item<LessOrEqualThan>();
    
    Condition::stream( schema.base() );
}

/** Compare the values 'val0' and 'val1' */
bool LessOrEqualThan::compareValues(double val0, double val1) const
{
    return val0 <= val1;
}

/** Compare the values 'val0' and 'val1' 

    \throw SireMaths::domain_error
*/
bool LessOrEqualThan::compareValues(const Complex &val0, const Complex &val1) const
{
    if (val0 == val1)
        return true;
    
    if (val0.isReal() and val1.isReal())
        return compareValues( val0.real(), val1.real() );
    
    else if (val0.isPurelyComplex() and val1.isPurelyComplex())
        return compareValues( val0.imag(), val1.imag() );
        
    else
        throw SireMaths::domain_error( QObject::tr(
                "Cannot compare the values of complex numbers %1 and %2.")
                    .arg(val0.toString(), val1.toString()), CODELOC );
    
    return false;
}

/** Return the string representation of this operator */
QString LessOrEqualThan::operatorString() const
{
    return QObject::tr("<=");
}

/** Return whether or not we can be absolutely sure that this
    condition will always be true. Note that this doesn't try
    too hard, so some things that are always true may not
    be reported here as being always true, e.g. x + 1 > x */
bool LessOrEqualThan::alwaysTrue() const
{
    if (leftHandSide() == rightHandSide())
        return true;
    else if (this->isConstant())
        return this->evaluateCondition( ComplexValues() );
    else
        return false;
}

/** Return whether or not we can be absolutely sure that this
    condition will always be false. Note that this doesn't try
    too hard, so some things that are always false may not
    be reported here as being always false, e.g. x > x + 1 */
bool LessOrEqualThan::alwaysFalse() const
{
    if (leftHandSide() == rightHandSide())
        return false;
    else if (this->isConstant())
        return not (this->evaluateCondition( ComplexValues() ));
    else
        return false;
}

///////////
/////////// Implementation of EqualTo
///////////

static const RegisterObject<EqualTo> r_equalto;

/** Constructor */
EqualTo::EqualTo() : Implements<EqualTo,Condition>()
{}

/** Construct to compare 'left_hand_side' with 'right_hand_side' */
EqualTo::EqualTo(const Expression &left_hand_side,
                 const Expression &right_hand_side)
        : Implements<EqualTo,Condition>(left_hand_side, right_hand_side)
{}

/** Copy constructor */
EqualTo::EqualTo(const EqualTo &other)
        : Implements<EqualTo,Condition>(other)
{}

/** Destructor */
EqualTo::~EqualTo()
{}

/** Copy assignment operator */
EqualTo& EqualTo::operator=(const EqualTo &other)
{
    Condition::operator=(other);
    return *this;
}

/** Comparison operator */
bool EqualTo::operator==(const EqualTo &other) const
{
    return Condition::operator==(other);
}

/** Comparison operator */
bool EqualTo::operator!=(const EqualTo &other) const
{
    return Condition::operator!=(other);
}

void EqualTo::stream(Stream &s)
{
    s.assertVersion<EqualTo>(1);
    
    Schema schema = s.item<EqualTo>();
    
    Condition::stream( schema.base() );
}

/** Compare the values 'val0' and 'val1' */
bool EqualTo::compareValues(double val0, double val1) const
{
    return val0 == val1;
}

/** Compare the values 'val0' and 'val1' 

    \throw SireMaths::domain_error
*/
bool EqualTo::compareValues(const Complex &val0, const Complex &val1) const
{
    if (val0 == val1)
        return true;
    
    if (val0.isReal() and val1.isReal())
        return compareValues( val0.real(), val1.real() );
    
    else if (val0.isPurelyComplex() and val1.isPurelyComplex())
        return compareValues( val0.imag(), val1.imag() );
        
    else
        throw SireMaths::domain_error( QObject::tr(
                "Cannot compare the values of complex numbers %1 and %2.")
                    .arg(val0.toString(), val1.toString()), CODELOC );
    
    return false;
}

/** Return the string representation of this operator */
QString EqualTo::operatorString() const
{
    return QObject::tr("==");
}

/** Return whether or not we can be absolutely sure that this
    condition will always be true. Note that this doesn't try
    too hard, so some things that are always true may not
    be reported here as being always true, e.g. x + 1 > x */
bool EqualTo::alwaysTrue() const
{
    if (leftHandSide() == rightHandSide())
        return true;
    else if (this->isConstant())
        return this->evaluateCondition( ComplexValues() );
    else
        return false;
}

/** Return whether or not we can be absolutely sure that this
    condition will always be false. Note that this doesn't try
    too hard, so some things that are always false may not
    be reported here as being always false, e.g. x > x + 1 */
bool EqualTo::alwaysFalse() const
{
    if (leftHandSide() == rightHandSide())
        return false;
    else if (this->isConstant())
        return not (this->evaluateCondition( ComplexValues() ));
    else
        return false;
}

///////////
/////////// Implementation of NotEqualTo
///////////

static const RegisterObject<NotEqualTo> r_notequalto;

/** Constructor */
NotEqualTo::NotEqualTo() : Implements<NotEqualTo,Condition>()
{}

/** Construct to compare 'left_hand_side' with 'right_hand_side' */
NotEqualTo::NotEqualTo(const Expression &left_hand_side,
                       const Expression &right_hand_side)
           : Implements<NotEqualTo,Condition>(left_hand_side, right_hand_side)
{}

/** Copy constructor */
NotEqualTo::NotEqualTo(const NotEqualTo &other)
           : Implements<NotEqualTo,Condition>(other)
{}

/** Destructor */
NotEqualTo::~NotEqualTo()
{}

/** Copy assignment operator */
NotEqualTo& NotEqualTo::operator=(const NotEqualTo &other)
{
    Condition::operator=(other);
    return *this;
}

/** Comparison operator */
bool NotEqualTo::operator==(const NotEqualTo &other) const
{
    return Condition::operator==(other);
}

/** Comparison operator */
bool NotEqualTo::operator!=(const NotEqualTo &other) const
{
    return Condition::operator!=(other);
}

void NotEqualTo::stream(Stream &s)
{
    s.assertVersion<NotEqualTo>(1);
    
    Schema schema = s.item<NotEqualTo>();
    
    Condition::stream( schema.base() );
}

/** Compare the values 'val0' and 'val1' */
bool NotEqualTo::compareValues(double val0, double val1) const
{
    return val0 != val1;
}

/** Compare the values 'val0' and 'val1' 

    \throw SireMaths::domain_error
*/
bool NotEqualTo::compareValues(const Complex &val0, const Complex &val1) const
{
    if (val0 == val1)
        return false;
    
    if (val0.isReal() and val1.isReal())
        return compareValues( val0.real(), val1.real() );
    
    else if (val0.isPurelyComplex() and val1.isPurelyComplex())
        return compareValues( val0.imag(), val1.imag() );
        
    else
        throw SireMaths::domain_error( QObject::tr(
                "Cannot compare the values of complex numbers %1 and %2.")
                    .arg(val0.toString(), val1.toString()), CODELOC );
    
    return false;
}

/** Return the string representation of this operator */
QString NotEqualTo::operatorString() const
{
    return QObject::tr("!=");
}

/** Return whether or not we can be absolutely sure that this
    condition will always be true. Note that this doesn't try
    too hard, so some things that are always true may not
    be reported here as being always true, e.g. x + 1 > x */
bool NotEqualTo::alwaysTrue() const
{
    if (leftHandSide() == rightHandSide())
        return false;
    else if (this->isConstant())
        return this->evaluateCondition( ComplexValues() );
    else
        return false;
}

/** Return whether or not we can be absolutely sure that this
    condition will always be false. Note that this doesn't try
    too hard, so some things that are always false may not
    be reported here as being always false, e.g. x > x + 1 */
bool NotEqualTo::alwaysFalse() const
{
    if (leftHandSide() == rightHandSide())
        return true;
    else if (this->isConstant())
        return not (this->evaluateCondition( ComplexValues() ));
    else
        return false;
}

///////////
/////////// Implementation of AlwaysTrue
///////////

static const RegisterObject<AlwaysTrue> r_alwaystrue;

/** Constructor */
AlwaysTrue::AlwaysTrue() : Implements<AlwaysTrue,Condition>()
{}

/** Copy constructor */
AlwaysTrue::AlwaysTrue(const AlwaysTrue &other) : Implements<AlwaysTrue,Condition>(other)
{}

/** Destructor */
AlwaysTrue::~AlwaysTrue()
{}

/** Copy assignment operator */
AlwaysTrue& AlwaysTrue::operator=(const AlwaysTrue &other)
{
    Condition::operator=(other);
    return *this;
}

/** Comparison operator */
bool AlwaysTrue::operator==(const AlwaysTrue&) const
{
    return true;
}

/** Comparison operator */
bool AlwaysTrue::operator!=(const AlwaysTrue &other) const
{
    return false;
}

void AlwaysTrue::stream(Stream &s)
{
    s.assertVersion<AlwaysTrue>(1);
    
    Schema schema = s.item<AlwaysTrue>();
    
    Condition::stream( schema.base() );
}

/** Return a string representation of truth */
QString AlwaysTrue::toString() const
{
    return QObject::tr( "True" );
}
    
/** This cannot be further simplified */
Expression AlwaysTrue::simplify(int) const
{
    return *this;
}

/** AlwaysTrue is always true */
bool AlwaysTrue::alwaysTrue() const
{
    return true;
}

/** AlwaysTrue is never false */
bool AlwaysTrue::alwaysFalse() const
{
    return false;
}

/** This is not a function of anything */
bool AlwaysTrue::isFunction(const Symbol&) const
{
    return false;
}

/** Truth is always constant */
bool AlwaysTrue::isConstant() const
{
    return true;
}

/** Truth is never complex */
bool AlwaysTrue::isComplex() const
{
    return false;
}

/** Truth is always simple */
bool AlwaysTrue::isCompound() const
{
    return false;
}

/** Hash truth */
uint AlwaysTrue::hashCode() const
{
    return 1;
}

/** There is no substituting the truth */
Expression AlwaysTrue::substitute(const Identities&) const
{
    return *this;
}

/** There are no symbols in truth */
QSet<Symbol> AlwaysTrue::symbols() const
{
    return QSet<Symbol>();
}

/** The truth has no children */
QList<Expression> AlwaysTrue::children() const
{
    return QList<Expression>();
}

/** The truth cannot be expanded */
QList<Factor> AlwaysTrue::expand(const Symbol &symbol) const
{
    QList<Factor> ret;
    ret.append( Factor(symbol, *this, 0) );
    
    return ret;
}

/** Truth is always true */
bool AlwaysTrue::evaluateCondition(const Values&) const
{
    return true;
}

/** Truth is always true */
bool AlwaysTrue::evaluateCondition(const ComplexValues&) const
{
    return true;
}

/** Truth is always true */
double AlwaysTrue::evaluate(const Values&) const
{
    return 1;
}

/** Truth is always true */
Complex AlwaysTrue::evaluate(const ComplexValues&) const
{
    return Complex(1);
}

QString AlwaysTrue::operatorString() const
{
    return QString();
}

bool AlwaysTrue::compareValues(double, double) const
{
    return true;
}

bool AlwaysTrue::compareValues(const Complex&, const Complex&) const
{
    return true;
}

///////////
/////////// Implementation of AlwaysFalse
///////////

static const RegisterObject<AlwaysFalse> r_alwaysfalse;

/** Constructor */
AlwaysFalse::AlwaysFalse() : Implements<AlwaysFalse,Condition>()
{}

/** Copy constructor */
AlwaysFalse::AlwaysFalse(const AlwaysFalse &other) 
            : Implements<AlwaysFalse,Condition>(other)
{}

/** Destructor */
AlwaysFalse::~AlwaysFalse()
{}

/** Copy assignment operator */
AlwaysFalse& AlwaysFalse::operator=(const AlwaysFalse &other)
{
    Condition::operator=(other);
    return *this;
}

/** Comparison operator */
bool AlwaysFalse::operator==(const AlwaysFalse&) const
{
    return true;
}

/** Comparison operator */
bool AlwaysFalse::operator!=(const AlwaysFalse &other) const
{
    return false;
}

void AlwaysFalse::stream(Stream &s)
{
    s.assertVersion<AlwaysFalse>(1);
    
    Schema schema = s.item<AlwaysFalse>();
    
    Condition::stream( schema.base() );
}

/** Return a string representation of truth */
QString AlwaysFalse::toString() const
{
    return QObject::tr( "False" );
}
    
/** This cannot be further simplified */
Expression AlwaysFalse::simplify(int) const
{
    return *this;
}

/** AlwaysFalse is never true */
bool AlwaysFalse::alwaysTrue() const
{
    return false;
}

/** AlwaysFalse is always false */
bool AlwaysFalse::alwaysFalse() const
{
    return true;
}

/** This is not a function of anything */
bool AlwaysFalse::isFunction(const Symbol&) const
{
    return false;
}

/** Truth is always constant */
bool AlwaysFalse::isConstant() const
{
    return true;
}

/** False is never complex */
bool AlwaysFalse::isComplex() const
{
    return false;
}

/** False is always simple */
bool AlwaysFalse::isCompound() const
{
    return false;
}

/** Hash false */
uint AlwaysFalse::hashCode() const
{
    return 0;
}

/** There is no substituting false */
Expression AlwaysFalse::substitute(const Identities&) const
{
    return *this;
}

/** There are no symbols in false */
QSet<Symbol> AlwaysFalse::symbols() const
{
    return QSet<Symbol>();
}

/** False has no children */
QList<Expression> AlwaysFalse::children() const
{
    return QList<Expression>();
}

/** False cannot be expanded */
QList<Factor> AlwaysFalse::expand(const Symbol &symbol) const
{
    QList<Factor> ret;
    ret.append( Factor(symbol, *this, 0) );
    
    return ret;
}

/** False is never true */
bool AlwaysFalse::evaluateCondition(const Values&) const
{
    return false;
}

/** False is never true */
bool AlwaysFalse::evaluateCondition(const ComplexValues&) const
{
    return false;
}

/** False is never true */
double AlwaysFalse::evaluate(const Values&) const
{
    return 0;
}

/** False is never true */
Complex AlwaysFalse::evaluate(const ComplexValues&) const
{
    return Complex(0);
}

QString AlwaysFalse::operatorString() const
{
    return QString();
}

bool AlwaysFalse::compareValues(double, double) const
{
    return false;
}

bool AlwaysFalse::compareValues(const Complex&, const Complex&) const
{
    return false;
}

///////////
/////////// Implementation of Conditional
///////////

static const RegisterObject<Conditional> r_conditional;

/** Null constructor */
Conditional::Conditional() : Implements<Conditional,CASNode>(), cond(AlwaysTrue())
{}

/** Construct a conditional where if 'condition' is true, then 
    'true_expression' is evaluated, while if 'condition' is false,
    then 'false_expression' is evaluated */
Conditional::Conditional(const Condition &condition, 
                         const Expression &t_expression,
                         const Expression &f_expression)
            : Implements<Conditional,CASNode>(),
              cond(condition),
              true_expression(t_expression),
              false_expression(f_expression)
{
    if (condition.isConstant())
    {
        cond = AlwaysTrue();
    
        if (condition.alwaysTrue())
        {
            false_expression = Expression();
        }
        else
        {
            true_expression = false_expression;
            false_expression = Expression();
        }
    }
}
            
/** Copy constructor */
Conditional::Conditional(const Conditional &other)
            : Implements<Conditional,CASNode>(other), cond(other.cond),
              true_expression(other.true_expression),
              false_expression(other.false_expression)
{}

/** Destructor */
Conditional::~Conditional()
{}

/** Copy assignment operator */
Conditional& Conditional::operator=(const Conditional &other)
{
    if (this != &other)
    {
        CASNode::operator=(other);
        cond = other.cond;
        true_expression = other.true_expression;
        false_expression = other.false_expression;
    }

    return *this;
}

/** Comparison operator */
bool Conditional::operator==(const Conditional &other) const
{
    return this == &other or
           (cond.equals(other.cond) and 
            true_expression == other.true_expression and
            false_expression == other.false_expression );
}

/** Comparison operator */
bool Conditional::operator!=(const Conditional &other) const
{
    return not Conditional::operator==(other);
}

void Conditional::stream(Stream &s)
{
    s.assertVersion<Conditional>(1);
    
    Schema schema = s.item<Conditional>();
    
    schema.data("true_expression") & true_expression;
    schema.data("false_expression") & false_expression;
    schema.data("condition") & cond;
    
    CASNode::stream( schema.base() );
}

/** Return the condition */
const Condition& Conditional::condition() const
{
    return cond->asA<Condition>();
}

/** Return the expression to be evaluated if the condition is true */
const Expression& Conditional::trueExpression() const
{
    return true_expression;
}

/** Return the expression to be evaluated if the condition is false */
const Expression& Conditional::falseExpression() const
{
    return false_expression;
}

/** Return the series expansion of this product with respect to 'symbol', to order 'n'*/
Expression Conditional::series(const Symbol &symbol, int n) const
{
    if (condition().alwaysTrue())
        return true_expression.series(symbol, n);
    else
        return *this;
}

/** Try to simplify this condition */
Expression Conditional::simplify(int options) const
{
    if (condition().isConstant())
    {
        if (condition().alwaysTrue())
            return true_expression.simplify(options);
        else
            return false_expression.simplify(options);
    }
    else
    {
        Conditional ret;
        
        Expression simple_cond = cond->simplify(options);
        
        if (simple_cond.isConstant())
        {
            if (simple_cond.isZero())
                return false_expression.simplify(options);
            else
                return true_expression.simplify(options);
        }
        
        if (not simple_cond.node().isA<Condition>())
            throw Siren::program_bug( QObject::tr(
                    "How have we lost the condition %1 and got %2 instead???")
                        .arg(cond->toString(), simple_cond.toString()), CODELOC );
        
        ret.cond = simple_cond.node().asA<Condition>();
        ret.true_expression = true_expression.simplify(options);
        ret.false_expression = false_expression.simplify(options);
        
        return ret;
    }
}

/** Return the complex conjugate of this expression */
Expression Conditional::conjugate() const
{
    if (condition().alwaysTrue())
        return true_expression.conjugate();
    else
    {
        Conditional ret;
        
        ret.cond = cond;
        ret.true_expression = true_expression.conjugate();
        ret.false_expression = false_expression.conjugate();
        
        return ret;
    }
}

/** Return the differential of this expression */
Expression Conditional::differentiate(const Symbol &symbol) const
{
    if (condition().alwaysTrue())
        return true_expression.differentiate(symbol);
    else
    {
        if (true_expression.isFunction(symbol) or 
            false_expression.isFunction(symbol))
        {
            throw SireCAS::unavailable_differential( QObject::tr(
                    "Cannot differentiate the condition %1 with respect to %2.")
                        .arg(this->toString(), symbol.toString()), CODELOC );
        }
        
        return 0;
    }
}

/** Return the integral of this expression */
Expression Conditional::integrate(const Symbol &symbol) const
{
    if (condition().alwaysTrue())
        return true_expression.integrate(symbol);
    else
    {
        if (true_expression.isFunction(symbol) or 
            false_expression.isFunction(symbol))
        {
            throw SireCAS::unavailable_integral( QObject::tr(
                    "Cannot integrate the condition %1 with respect to %2.")
                        .arg(this->toString(), symbol.toString()), CODELOC );
        }
        
        return *this * symbol;
    }
}

/** Return whether or not this is a function of 'symbol' */
bool Conditional::isFunction(const Symbol &symbol) const
{
    return true_expression.isFunction(symbol) or 
           false_expression.isFunction(symbol);
}

/** Return whether or not this is constant */
bool Conditional::isConstant() const
{
    return cond->isConstant() and 
           true_expression.isConstant() and false_expression.isConstant();
}

/** Is this a complex expression? */
bool Conditional::isComplex() const
{
    return condition().isComplex() or 
           true_expression.isComplex() or false_expression.isComplex();
}

/** Is this a compound expression? */
bool Conditional::isCompound() const
{
    if (condition().alwaysTrue())
        return true_expression.isCompound();
    else
        return true;
}

/** Hash this conditional */
uint Conditional::hashCode() const
{
    return qHash( *cond ) + qHash(true_expression) + qHash(false_expression);
}

/** Return a string representation of this conditional */
QString Conditional::toString() const
{
    if (condition().alwaysTrue())
    {
        return true_expression.toString();
    }
    else
    {
        return QObject::tr( "if ( %1 ){ %2 ? %3 }" )
                    .arg(cond->toString(),
                         true_expression.toString(),
                         false_expression.toString());
    }
}

/** Evaluate this expression for the passed values */
double Conditional::evaluate(const Values &values) const
{
    if (cond->asA<Condition>().evaluateCondition(values))
    {
        return true_expression.evaluate(values);
    }
    else
    {
        return false_expression.evaluate(values);
    }
}

/** Evaluate this expresion for the passed values */
Complex Conditional::evaluate(const ComplexValues &values) const
{
    if (cond->asA<Condition>().evaluateCondition(values))
    {
        return true_expression.evaluate(values);
    }
    else
    {
        return false_expression.evaluate(values);
    }
}

/** Substitute 'identities' into this expression */
Expression Conditional::substitute(const Identities &identities) const
{
    Conditional ret;

    Expression new_cond = cond->substitute(identities);
    
    if (new_cond.node().isA<Condition>())
        ret.cond = new_cond.node().asA<Condition>();
    else
    {
        if (new_cond.isConstant())
        {
            if (new_cond.isZero())
                ret.cond = AlwaysFalse();
            else
                ret.cond = AlwaysTrue();
        }
        else
            throw Siren::program_bug( QObject::tr(
                    "How did the condition %1 turn into %2???")
                        .arg(cond->toString(), new_cond.toString()), CODELOC );
    }

    if (ret.cond->isConstant())
    {
        if (ret.cond->asA<Condition>().alwaysTrue())
        {
            ret.true_expression = true_expression.substitute(identities);
            ret.false_expression = Expression();
        }
        else
        {
            ret.true_expression = false_expression.substitute(identities);
            ret.false_expression = Expression();
        }
        
        ret.cond = AlwaysTrue();
    }
    else
    {
        ret.true_expression = true_expression.substitute(identities);
        ret.false_expression = false_expression.substitute(identities);
    }
    
    return ret;
}

/** Return the symbols used in this expression */
QSet<Symbol> Conditional::symbols() const
{
    if (condition().alwaysTrue())
        return true_expression.symbols();
    else
    {
        return condition().symbols() + 
               true_expression.symbols() + false_expression.symbols();
    }
}

/** Return the children of this expression */
QList<Expression> Conditional::children() const
{
    if (condition().alwaysTrue())
        return true_expression.children();
    else
    {
        return true_expression.children() + false_expression.children() +
               cond->children();
    }
}

/** Expand this expression in terms of 'symbol' */
QList<Factor> Conditional::expand(const Symbol &symbol) const
{
    if (condition().alwaysTrue())
        return true_expression.expand(symbol);
        
    else if (this->isFunction(symbol))
        throw SireCAS::rearrangement_error( QObject::tr(
                 "Cannot expand the conditional %1 in terms of %2.")
                    .arg(this->toString(), symbol.toString()), CODELOC );
                    
    QList<Factor> ret;
    
    ret.append( Factor(symbol, *this, 0) );
    
    return ret;
}
