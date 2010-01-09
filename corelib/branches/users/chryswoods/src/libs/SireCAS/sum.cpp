/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include <QMap>

#include "sum.h"
#include "expression.h"
#include "complexvalues.h"
#include "values.h"
#include "identities.h"
#include "factor.h"
#include "i.h"
#include "integrationconstant.h"

#include "SireMaths/complex.h"

#include "Siren/logger.h"
#include "Siren/tester.h"
#include "Siren/errors.h"
#include "Siren/stream.h"
#include "Siren/streamqt.h"

#include <QDebug>

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

static const RegisterObject<Sum> r_sum;

/** Construct an empty (zero) sum */
Sum::Sum() : Implements<Sum,CASNode>(), strtval(0)
{}

/** Construct the sum of two expressions */
Sum::Sum(const Expression &ex0, const Expression &ex1) 
    : Implements<Sum,CASNode>(), strtval(0)
{
    this->pvt_add(ex0);
    this->pvt_add(ex1);
}

/** Construct the sum of the expressions in 'expressions' */
Sum::Sum(const QList<Expression> &expressions) 
    : Implements<Sum,CASNode>(), strtval(0)
{
    int n = expressions.count();
    for (int i=0; i<n; ++i)
        this->pvt_add(expressions.at(i));
}

/** Copy constructor */
Sum::Sum(const Sum &other)
    : Implements<Sum,CASNode>(other), posparts(other.posparts), 
      negparts(other.negparts), strtval(other.strtval)
{}

/** Destructor */
Sum::~Sum()
{}

Sum& Sum::operator=(const Sum &other)
{
    if (this != &other)
    {
        posparts = other.posparts;
        negparts = other.negparts;
        strtval = other.strtval;
        CASNode::operator=(other);
    }
    
    return *this;
}

bool Sum::operator==(const Sum &other) const
{
    return this == &other or
           (strtval == other.strtval and posparts == other.posparts and
            negparts == other.negparts);
}

bool Sum::operator!=(const Sum &other) const
{
    return not Sum::operator==(other);
}

uint Sum::hashCode() const
{
    return qHash( Sum::typeName() ) + qHash( strtval ) +
           qHash( posparts.count() ) + qHash( negparts.count() );
}

/** Return a string representation of the sum */
QString Sum::toString() const
{
    if (posparts.count() == 0 and negparts.count() == 0)
        return QString::number(strtval);

    QString ret;

    int i = 0;

    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        if (i == 0)
            ret = it->toString();
        else
            ret = QString("%1 + %2").arg(ret,it->toString());

        ++i;
    }

    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        if (i == 0)
            ret = QString("-%1").arg(it->toString());
        else
            ret = QString("%1 - %2").arg(ret,it->toString());

        ++i;
    }

    if (not SireMaths::isZero(strtval))
    {
        if (i == 0)
            ret = QString::number(strtval);
        else if (strtval < 0)
            ret = QString("%1 - %2").arg(ret).arg(-strtval);
        else
            ret = QString("%1 + %2").arg(ret).arg(strtval);
    }

    return ret;
}

void Sum::stream(Stream &s)
{
    s.assertVersion<Sum>(1);
    
    Schema schema = s.item<Sum>();
    
    schema.data("constant") & strtval;
    schema.data("positive_parts") & posparts;
    schema.data("negative_parts") & negparts;
    
    CASNode::stream( schema.base() );
}

/** Remove the current version of 'ex', which will contain its current factor.
    Expression(0) will be returned if this expression is not in this Sum. */
Expression Sum::pvt_take(const CASNode &node)
{
    Expression ex(node);

    if (posparts.contains(ex))
        return posparts.take(ex);
    else if (negparts.contains(ex))
    {
        Expression negex = negparts.take(ex);
        return negex.negate();
    }
    else
        return Expression(0);
}

/** Add fac * ex */
void Sum::pvt_add(double fac, const CASNode &node)
{
    if (node.isA<IntegrationConstant>())
    {
        //do not add these together - remove the current one...
        this->pvt_take(node);

        //add 1 IntegrationConstant to the sum
        posparts.insert( node, node );
    }
    else
    {
        //get any expression that is currently in this Sum with this base
        Expression current_ex = this->pvt_take(node);

        //get the current factor of this expression
        double current_factor = current_ex.factor();

        //calculate the new factor
        double new_factor = current_factor + fac;

        if (new_factor > 0)
            posparts.insert( node, new_factor * node );
        else if (new_factor < 0)
            negparts.insert( node, -new_factor * node );
    }
}

/** Add the expression 'ex' to this sum */
void Sum::pvt_add(const Expression &ex)
{
    //no need to add zero onto a sum!
    if (ex.isZero())
        return;
        
    else if (ex.node().isA<I>())
    {
        //this is a multiple of the complex number 'i'
        pvt_add(ex.factor(), ex.node());
    }
    else if (ex.isConstant())
    {
        Complex exval = ex.evaluate(ComplexValues());

        strtval += exval.real();

        if (not exval.isReal())
            this->pvt_add( exval.imag(), I() );
    }
    else if (ex.node().isA<Sum>())
    {
        //add the elements of the sum individually
        const Sum &sum = ex.node().asA<Sum>();

        if (posparts.count() == 0 and negparts.count() == 0)
        {
            if (ex.factor() == 1)
            {
                posparts = sum.posparts;
                negparts = sum.negparts;
                strtval += sum.strtval;
                
                return;
            }
            else if (ex.factor() == -1)
            {
                posparts = sum.negparts;
                negparts = sum.posparts;
                strtval -= sum.strtval;
                
                return;
            }
        }
            
        for (QHash<Expression,Expression>::const_iterator it = sum.posparts.begin();
             it != sum.posparts.end();
             ++it)
        {
           this->pvt_add( ex.factor()*it->factor(), it->node() );
        }

        for (QHash<Expression,Expression>::const_iterator it = sum.negparts.begin();
             it != sum.negparts.end();
             ++it)
        {
            this->pvt_add( -(ex.factor()*it->factor()), it->node() );
        }

        //add the start value to the sum
        strtval += ex.factor() * sum.strtval;
    }
    else
    {
        this->pvt_add(ex.factor(), ex.node());
    }
}

/** Reduce a Sum down to a simple form. This replaces the Sum with a single expression
    or a constant if this is no longer a Sum. It does not collapse together common
    factors - use 'collapse()' if you want to do this */
Expression Sum::reduce() const
{
    if (negparts.count() == 0 and posparts.count() == 0)
        return Expression(strtval);
    else if (SireMaths::isZero(strtval))
    {
        if (negparts.count() == 0 and posparts.count() == 1)
        {
            //sum contains only the positive part
            return posparts.values()[0];
        }
        else if (posparts.count() == 0 and negparts.count() == 1)
        {
            //sum contains only the negative part
            return negparts.values()[0].negate();
        }
    }

    return *this;
}

/** Simplify this sum */
Expression Sum::simplify(int options) const
{
    Sum ret;
    ret.strtval = strtval;

    //simplify the positive parts...
    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        ret.pvt_add( it->simplify(options) );
    }

    //now simplify the negative parts...
    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        ret.pvt_add( -(it->simplify(options)) );
    }

    return ret;
}

/** Return the conjugate of this sum */
Expression Sum::conjugate() const
{
    Sum ret;
    ret.strtval = strtval;

    //simplify the positive parts...
    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        ret.pvt_add( it->conjugate() );
    }

    //now simplify the negative parts...
    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        ret.pvt_add( -(it->conjugate()) );
    }

    return ret;
}

/** Evaluate the Sum for the values 'values'. Any missing values are assumed to be
    equal to zero. */
double Sum::evaluate(const Values &values) const
{
    double result = strtval;

    for ( QHash<Expression,Expression>::const_iterator it = posparts.begin();
          it != posparts.end();
          ++it )
    {
        result += it->evaluate(values);
    }

    for ( QHash<Expression,Expression>::const_iterator it = negparts.begin();
          it != negparts.end();
          ++it )
    {
        result -= it->evaluate(values);
    }

    return result;
}

/** Evaluate the Sum for the values 'values'. Any missing values are assumed to be
    equal to zero. */
Complex Sum::evaluate(const ComplexValues &values) const
{
    Complex result(strtval);

    for ( QHash<Expression,Expression>::const_iterator it = posparts.begin();
          it != posparts.end();
          ++it )
    {
        result += it->evaluate(values);
    }

    for ( QHash<Expression,Expression>::const_iterator it = negparts.begin();
          it != negparts.end();
          ++it )
    {
        result -= it->evaluate(values);
    }

    return result;
}

/** Return the differential of this Sum with respect to 'symbol'. */
Expression Sum::differentiate(const Symbol &symbol) const
{
    Sum diff;

    //add the differentials of all of the positive parts...
    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        diff.pvt_add( it->differentiate(symbol) );
    }

    //now add the differentials of all of the negative parts...
    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        diff.pvt_add( -(it->differentiate(symbol)) );
    }

    return diff.reduce();
}

/** Return the integral of this Sum with respect to 'symbol'. The integral of
    a sum is the sum of the integrals */
Expression Sum::integrate(const Symbol &symbol) const
{
    Sum integ;

    //add the integrals of all of the positive parts...
    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        integ.pvt_add( it->integrate(symbol) );
    }

    //now add the integrals of all of the negative parts...
    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        integ.pvt_add( -(it->integrate(symbol)) );
    }

    //add strtval * symbol
    if (not SireMaths::isZero(strtval))
        integ.pvt_add( strtval * symbol );

    return integ.reduce();
}

/** Return a series expansion of this sum about 'symbol' to order 'n' */
Expression Sum::series(const Symbol &symbol, int n) const
{
    Sum s;

    //add the expansions of all of the positive parts...
    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        s.pvt_add( it->series(symbol,n) );
    }

    //now add the expansions of all of the negative parts...
    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        s.pvt_add( -(it->series(symbol,n)) );
    }

    s.strtval = strtval;

    return s.reduce();
}

/** Return an expression that is this expression with 'identities' substituted in */
Expression Sum::substitute(const Identities &identities) const
{
    if (isConstant())
        return Expression(*this);
    else
    {
        Sum subsum;
        subsum.strtval = strtval;

        for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
             it != posparts.end();
             ++it)
        {
            subsum.pvt_add( it->substitute(identities) );
        }

        for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
             it != negparts.end();
             ++it)
        {
            subsum.pvt_add( it->negate().substitute(identities) );
        }

        return subsum.reduce();
    }
}

/** Return whether or not this is constant */
bool Sum::isConstant() const
{
    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        if (not it->isConstant())
            return false;
    }

    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        if (not it->isConstant())
            return false;
    }

    return true;
}

/** Return whether or not this is a function of 'symbol' */
bool Sum::isFunction(const Symbol &symbol) const
{
    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        if (it->isFunction(symbol))
            return true;
    }

    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        if (it->isFunction(symbol))
            return true;
    }

    return false;
}

/** Return whether or not this function contains any complex parts */
bool Sum::isComplex() const
{
    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        if (it->isComplex())
            return true;
    }

    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        if (it->isComplex())
            return true;
    }

    return false;
}

/** Return all of the symbols involved in this sum (and all expressions in this sum) */
QSet<Symbol> Sum::symbols() const
{
    QSet<Symbol> syms;

    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        syms += it->symbols();
    }

    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        syms += it->symbols();
    }

    return syms;
}

/** Return all of the child expressions in this Sum */
QList<Expression> Sum::children() const
{
    QList<Expression> exps;

    for (QHash<Expression,Expression>::const_iterator it = posparts.begin();
         it != posparts.end();
         ++it)
    {
        exps.append( *it );
    }

    for (QHash<Expression,Expression>::const_iterator it = negparts.begin();
         it != negparts.end();
         ++it)
    {
        exps.append( it->negate() );
    }

    return exps;
}

/** Return whether or not this is compound (needs brakets when printed) */
bool Sum::isCompound() const
{
    return posparts.count() + negparts.count() 
                        >= ( 2 - (not SireMaths::isZero(strtval)) );
}

QList<Factor> Sum::expand(const Symbol &symbol) const
{
    QHash<Expression, Expression> factors;
    
    for (QHash<Expression, Expression>::const_iterator it = posparts.constBegin();
         it != posparts.constEnd();
         ++it)
    {
        QList<Factor> facs = it->expand(symbol);

        foreach (const Factor &fac, facs)
        {
            factors[fac.power()] = factors[fac.power()] + fac.factor();
        }
    }

    for (QHash<Expression, Expression>::const_iterator it = negparts.constBegin();
         it != negparts.constEnd();
         ++it)
    {
        QList<Factor> facs = it->expand(symbol);

        foreach (const Factor &fac, facs)
        {
            factors[fac.power()] = factors[fac.power()] - fac.factor();
        }
    }
    
    QList<Factor> ret;
    
    for (QHash<Expression,Expression>::const_iterator it = factors.constBegin();
         it != factors.constEnd();
         ++it)
    {
        ret.append( Factor( symbol, it.value(), it.key() ) );
    }
    
    if (strtval != 0)
    {
        ret.append( Factor(symbol, strtval, 0) );
    }
    
    return ret;
}

bool Sum::test(Logger &logger) const
{
    Tester tester(*this, logger);
    
    #ifndef SIREN_DISABLE_TESTS
    
    try
    {
        Symbol x("x");
        Symbol y("y");
        Symbol z("z");
        
        // test 1
        {
            tester.nextTest();
            
            Sum sum( x, y );
            
            tester.expect_true( QObject::tr("x + y contains x"),
                                CODELOC,
                                sum.symbols().contains(x) );

            tester.expect_true( QObject::tr("x + y contains y"),
                                CODELOC,
                                sum.symbols().contains(y) );
                                
            tester.expect_false( QObject::tr("x + y doesn't contain z"),
                                 CODELOC,
                                 sum.symbols().contains(z) );
                                 
            tester.expect_equal( QObject::tr("d (x+y) / dx = 1"),
                                 CODELOC,
                                 sum.differentiate(x), Expression(1) );
                                 
            tester.expect_equal( QObject::tr("d (x+y) / dy = 1"),
                                 CODELOC,
                                 sum.differentiate(y), Expression(1) );
                                 
            tester.expect_equal( QObject::tr("d (x+y) / dz = 0"),
                                 CODELOC,
                                 sum.differentiate(z), Expression(0) );
                                 
            tester.expect_equal( QObject::tr("x + y = 5 if x == 3 and y == 2"),
                                 CODELOC,
                                 sum.evaluate( (x==3)+(y==2) ), double(5) );
        }
        
        // test 2
        {
            tester.nextTest();
            
            Expression f = 3 + x - y;
            
            tester.expect_equal( QObject::tr("3 + x - y = 10 if x == 11 and y = 4"),
                                 CODELOC,
                                 f( (x==11)+(y==4) ), double(10) );
        }
    }
    catch(const Siren::exception &e)
    {
        tester.unexpected_error(e);
    }
    catch(...)
    {
        tester.unexpected_error( unknown_error(CODELOC) );
    }
    
    #endif // SIREN_DISABLE_TESTS
    
    return tester.allPassed();
}
