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

#include "power.h"
#include "exp.h"
#include "powerconstant.h"
#include "values.h"
#include "factor.h"
#include "integrationconstant.h"
#include "complexvalues.h"
#include "identities.h"

#include "SireCAS/errors.h"

#include "SireMaths/complex.h"

#include "Siren/stream.h"
#include "Siren/errors.h"

#include <QDebug>

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

////////////
//////////// Implementation of PowerFunction
////////////

static const RegisterObject<PowerFunction> r_powerfunc( VIRTUAL_CLASS );

/** Constructor */
PowerFunction::PowerFunction() : Extends<PowerFunction,CASNode>()
{}

/** Destructor */
PowerFunction::~PowerFunction()
{}

QString PowerFunction::typeName()
{
    return "SireCAS::PowerFunction";
}

QSet<Symbol> PowerFunction::symbols() const
{
    return core().symbols() + power().symbols();
}

bool PowerFunction::isCompound() const
{
    return false;
}

void PowerFunction::stream(Siren::Stream &s)
{
    s.assertVersion<PowerFunction>(1);
    
    Schema schema = s.item<PowerFunction>();
    
    CASNode::stream( schema.base() );
}

/** Return a string representation of this power */
QString PowerFunction::toString() const
{
    Expression pwr = power();

    if (pwr.isZero())
        return "1";
    else
    {
        Expression ex = core();

        if (pwr.isConstant() and ex.isConstant())
            return this->evaluate(ComplexValues()).toString();
        else if (pwr.isConstant())
        {
            Complex pwrval = pwr.evaluate(ComplexValues());

            if (pwrval.isReal())
            {
                double realpwr = pwrval.real();

                if ( SireMaths::areEqual(realpwr,1.0) )
                    return ex.toString();
                else if (ex.isCompound())
                    return QString("[%1]^%2").arg(ex.toString()).arg(realpwr);
                else
                    return QString("%1^%2").arg(ex.toString()).arg(realpwr);
            }
            else if ( SireMaths::isZero(pwrval.real()) )
            {
                if ( ex.isCompound() )
                    return QString("[%1]^%2").arg(ex.toString(),pwrval.toString());
                else
                    return QString("%1^%2").arg(ex.toString(),pwrval.toString());
            }
            else
            {
                if ( ex.isCompound() )
                    return QString("[%1]^[%2]").arg(ex.toString(),pwrval.toString());
                else
                    return QString("%1^[%2]").arg(ex.toString(),pwrval.toString());
            }
        }
        else
        {
            QString ret;
            if (ex.isConstant())
                ret = ex.evaluate(ComplexValues()).toString();
            else if (ex.isCompound())
                ret = QString("[%1]").arg(ex.toString());
            else
                ret = ex.toString();

            if (pwr.isCompound())
                return QString("%1^{%2}").arg(ret, pwr.toString());
            else
                return QString("%1^%2").arg(ret, pwr.toString());
        }
    }
}

/** Return this expression with the supplied substitutions */
Expression PowerFunction::substitute(const Identities &identities) const
{
    return Power( core().substitute(identities), power().substitute(identities) ).reduce();
}

/** Return the differential of this expression with respect to 'symbol' */
Expression PowerFunction::differentiate(const Symbol &symbol) const
{
    Expression ex = core();
    Expression pwr = power();

    //f(x)^0 == 1, do d f(x)^0 / dx = 0
    if (pwr.isZero())
        return Expression(0);
    //is the power a function of this symbol?
    else if (pwr.isFunction(symbol))
    {
        // d (a^x) / dx = a^x ln(a)

        if (ex.isFunction(symbol))
        {
            // use identity;
            // f(x)^g(x) = exp( g(x) * ln(f(x)) )

            return Exp( pwr * Ln(ex) ).differentiate(symbol);
        }
        else
            // d a^(f(x)) / dx = a^(f(x)) ln(a) * d f(x) / dx
            return Expression(*this) * Ln(ex) * pwr.differentiate(symbol);
    }
    else
    {
        //differentiate the base
        Expression diff = ex.differentiate(symbol);

        if (diff.isZero())
            return Expression(0);

        //constant power, so d (x^n) / dx = n x^(n-1)
        return diff * pwr * ex.pow( pwr - 1 );
    }
}

/** Return the integral of this power with respect to 'symbol' */
Expression PowerFunction::integrate(const Symbol &symbol) const
{
    Expression pwr = power();
    Expression cre = core();

    bool cre_is_func = cre.isFunction(symbol);
    bool pwr_is_func = pwr.isFunction(symbol);

    if (cre_is_func and not pwr_is_func)
    {
        //power of form f(x)^n,  integral is 1/(f'(x)*(n+1)) * f(x)^n+1
        Expression newpower = pwr + 1;
        return Power(cre,newpower).reduce() / (newpower * cre.differentiate(symbol));
    }
    else if (pwr_is_func and not cre_is_func)
    {
        //what is integral of n^f(x)?
        return CASNode::integrate(symbol);
    }
    else if (cre_is_func and pwr_is_func)
    {
        //what is the integral of f(x)^g(x)?
        return CASNode::integrate(symbol);
    }
    else
        //this expression is constant with respect to symbol
        return *this * symbol;
}

/** Return the child expressions of this Power - this contains the core() and the power() */
QList<Expression> PowerFunction::children() const
{
    return core().children() + power().children();
}

/** Return whether this is a function of 'symbol' */
bool PowerFunction::isFunction(const Symbol &symbol) const
{
    return core().isFunction(symbol) or power().isFunction(symbol);
}

/** Return whether or not this is a constant */
bool PowerFunction::isConstant() const
{
    Expression pwr = power();

    return pwr.isConstant() and (pwr.isZero() or core().isConstant());
}

/** Reduce this Power to a simplified expression (if possible) */
Expression PowerFunction::reduce() const
{
    Expression ex = core();
    Expression pwr = power();

    if (ex.node().isA<PowerFunction>())
    {
        const PowerFunction &powerfunc = ex.node().asA<PowerFunction>();

        if (ex.factor() == 1)
        {
            //this is a power of a power. We can combine the powers together
            return Power( powerfunc.core(), pwr * powerfunc.power() ).reduce();
        }
        else
        {
            //this is ex.factor()^pwr * (power of a power)
            return PowerConstant(ex.factor(),pwr).reduce() *
                   Power(powerfunc.core(), pwr * powerfunc.power() ).reduce();
        }
    }
    else if (ex.node().isA<IntegrationConstant>())
    {
        if (pwr.isConstant())
            //IntegrationConstant ^ const_power  =  IntegrationConstant
            return ex.node();
        else
            return *this;
    }
    else
    {
        bool core_is_constant = ex.isConstant();
        bool power_is_constant = pwr.isConstant();

        if (core_is_constant and power_is_constant)
        {
            //this is a constant
            return Expression( this->evaluate(ComplexValues()) );
        }
        else if (power_is_constant)
        {
            //get the power as an irrational number
            Complex pwrval = pwr.evaluate(ComplexValues());

            if (pwrval.isZero())
            {
                return Expression(1);
            }
            else if (not pwrval.isReal())
            {
                return ComplexPower(ex, pwrval);
            }
            else
            {
                double realpower = pwrval.real();
                
                if (SireMaths::areEqual(realpower,1.0))
                {
                    return ex;
                }
                else if (SireMaths::isInteger(realpower))
                {
                    return IntegerPower(ex, int(realpower));
                }
                else if (Rational::isRational(realpower))
                {
                    return RationalPower(ex, Rational(realpower));
                }
                else
                {
                    return RealPower(ex, realpower);
                }
            }
        }
        else if (core_is_constant)
        {
            Complex val = ex.evaluate(ComplexValues());

            if (val.isReal())
            {
                double realval = val.real();

                if (SireMaths::areEqual(realval, SireMaths::e))
                    return Expression( Exp(pwr) );
                else
                    return Expression( PowerConstant(realval, pwr) );
            }
            else
                return *this;
        }
        else
        {
            return *this;
        }
    }
}

static QList<Factor> multiply(const QList<Factor> &f0s, const QList<Factor> &f1s)
{
    if (f0s.isEmpty())
        return f1s;
    else if (f1s.isEmpty())
        return f0s;

    QHash<Expression,Expression> factors;
    
    foreach (const Factor &f0, f0s)
    {
        foreach (const Factor &f1, f1s)
        {
            Expression power = f0.power() + f1.power();
        
            factors[power] = factors[power] + (f0.factor() * f1.factor());
        }
    }
    
    QList<Factor> ret;
    
    for (QHash<Expression,Expression>::const_iterator it = factors.constBegin();
         it != factors.constEnd();
         ++it)
    {
        ret.append( Factor( f0s.at(0).symbol(), it.value(), it.key() ) );
    }
    
    return ret;
}

QList<Factor> PowerFunction::expand(const Symbol &symbol) const
{
    QList<Factor> ret;

    if (not this->isFunction(symbol))
    {
        ret.append( Factor(symbol, *this, 0) );
        return ret;
    }

    //get the core
    Expression ex = core();
    
    //expand the core in powers of this symbol
    QList<Factor> core_factors = ex.expand(symbol);

    //get the power...
    Expression pwr = power();

    //we cannot expand if the power is a function of the symbol
    if (pwr.isFunction(symbol))
        throw SireCAS::rearrangement_error( QObject::tr(
            "The expression %1 cannot be rearranged in terms of powers of %2 "
            "as this power is itself a function of %2.")
                .arg(this->toString(), symbol.toString()), CODELOC );

    //if there is only a single power of the symbol, then it doesn't
    //matter what the power is
    if (core_factors.count() == 1)
    {
        core_factors[0] = Factor( core_factors[0].symbol(),
                                  core_factors[0].factor(),
                                  pwr * core_factors[0].power() );
                                  
        return core_factors;
    }

    //we can only expand if the power is a constant integer...
    Complex pwrval = pwr.evaluate(ComplexValues());

    if (pwrval.isZero())
    {
        //this expression is just equal to 1
        ret.append( Factor(symbol,1,0) );
        return ret;
    }
    else if (pwrval.isReal() and SireMaths::isInteger(pwrval.real()))
    {
        int int_power = int(pwrval.real());

        if (int_power == 1)
            return core_factors;
        else if (int_power == 0)
        {
            QList<Factor> ret;
            ret.append( Factor(symbol,1,0) );
            return ret;
        }
        else if (int_power < 0)
        {
            if (core_factors.count() > 1)
                throw SireCAS::rearrangement_error( QObject::tr(
                    "The expression %1 cannot be rearranged in terms of powers of %2 "
                    "because multiple powers of %2 are raised to a negative power.")
                        .arg(this->toString(), symbol.toString()), CODELOC );
                        
            core_factors[0] = Factor(  core_factors[0].symbol(),
                                       core_factors[0].factor(), 
                                       int_power * core_factors[0].power() );
                                      
            return core_factors;
        }
        else
        {
            QList<Factor> ret = core_factors;
            
            for (int i=1; i<int_power; ++i)
            {
                ret = ::multiply(ret, core_factors);
            }
            
            return ret;
        }
    }
    else
        throw SireCAS::rearrangement_error( QObject::tr(
            "The expression %1 cannot be rearranged in terms of powers of %2 "
            "because it involves a non-integer power of a compound expression "
            "involving %2.")
                .arg(this->toString(), symbol.toString()), CODELOC );
    
    return ret;
}

////////////
//////////// Implementation of Power
////////////

static const RegisterObject<Power> r_power;

/** Construct a null power */
Power::Power() : Implements<Power,PowerFunction>()
{}

/** Construct a power that represents core^power */
Power::Power(const Expression &core, const Expression &power)
      : Implements<Power,PowerFunction>(), ex(core), pwr(power)
{}

/** Copy constructor */
Power::Power(const Power &other)
      : Implements<Power,PowerFunction>(other), ex(other.ex), pwr(other.pwr)
{}

/** Destructor */
Power::~Power()
{}

Power& Power::operator=(const Power &other)
{
    if (this != &other)
    {
        ex = other.ex;
        pwr = other.pwr;
        PowerFunction::operator=(other);
    }
    
    return *this;
}

bool Power::operator==(const Power &other) const
{
    return ex == other.ex and pwr == other.pwr;
}

bool Power::operator!=(const Power &other) const
{
    return not Power::operator==(other);
}

uint Power::hashCode() const
{
    return qHash(this->what()) + qHash(ex) + qHash(pwr);
}

void Power::stream(Stream &s)
{
    s.assertVersion<Power>(1);
    
    Schema schema = s.item<Power>();
    
    schema.data("core") & ex;
    schema.data("power") & ex;
    
    PowerFunction::stream( schema.base() );
}

bool Power::isCompound() const
{
    return pwr.isCompound();
}

/** Evaluate this power - this could be dodgy for negative bases with
    non-integer powers */
double Power::evaluate(const Values &values) const
{
    double pwrval = pwr.evaluate(values);
    if (SireMaths::isZero(pwrval))
        return 1.0;
    else
    {
        double val = ex.evaluate(values);
        if (SireMaths::isZero(val))
            return val;
        else
            return SireMaths::pow(val, pwrval);
    }
}

/** Evaluate this power - this could be dodgy for negative bases with
    non-integer powers */
Complex Power::evaluate(const ComplexValues &values) const
{
    Complex pwrval = pwr.evaluate(values);

    if (pwrval.isZero())
        return Complex(1);
    else
    {
        Complex val = ex.evaluate(values);
        if (val.isZero())
            return val;
        else
            return SireMaths::pow(val, pwrval);
    }
}
