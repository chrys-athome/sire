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

#include "powerconstant.h"
#include "expression.h"
#include "symbol.h"
#include "values.h"
#include "complexvalues.h"
#include "identities.h"
#include "integrationconstant.h"

#include "SireMaths/errors.h"

#include "Siren/stream.h"

#include <QDebug>

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

//////////
////////// Implementation of PowerConstant
//////////

static const RegisterObject<PowerConstant> r_powerconstant;

/** Create a null PowerConstant (0^0) */
PowerConstant::PowerConstant() 
              : Implements<PowerConstant,PowerFunction>(), cre(0), pwr(1)
{}

/** Construct the PowerConstant val^power */
PowerConstant::PowerConstant(double val, const Expression &power)
              : Implements<PowerConstant,PowerFunction>(), cre(val), pwr(power)
{}

/** Copy constructor */
PowerConstant::PowerConstant(const PowerConstant &other)
              : Implements<PowerConstant,PowerFunction>(), cre(other.cre), pwr(other.pwr)
{}

/** Destructor */
PowerConstant::~PowerConstant()
{}

PowerConstant& PowerConstant::operator=(const PowerConstant &other)
{
    if (this != &other)
    {
        cre = other.cre;
        pwr = other.pwr;
        PowerFunction::operator=(other);
    }
    
    return *this;
}

bool PowerConstant::operator==(const PowerConstant &other) const
{
    return cre == other.cre and pwr == other.pwr;
}

bool PowerConstant::operator!=(const PowerConstant &other) const
{
    return not PowerConstant::operator==(other);
}

void PowerConstant::stream(Stream &s)
{
    s.assertVersion<PowerConstant>(1);
    
    Schema schema = s.item<PowerConstant>();
    
    schema.data("core") & cre;
    schema.data("power") & pwr;
    
    PowerFunction::stream( schema.base() );
}

Expression PowerConstant::core() const
{
    return cre;
}

Expression PowerConstant::power() const
{
    return pwr;
}

/** Evaluate this function */
double PowerConstant::evaluate(const Values &values) const
{
    return SireMaths::pow( cre, pwr.evaluate(values) );
}

/** Evaluate this function */
Complex PowerConstant::evaluate(const ComplexValues &values) const
{
    return SireMaths::pow( cre, pwr.evaluate(values) );
}

//////////
////////// Implementation of ConstantPower
//////////

//register a pure virtual class
static const RegisterObject<ConstantPower> r_constantpower( VIRTUAL_CLASS );

ConstantPower::ConstantPower() : Extends<ConstantPower,PowerFunction>()
{}

ConstantPower::ConstantPower(const Expression &expression)
              : Extends<ConstantPower,PowerFunction>(), ex(expression)
{}

ConstantPower::ConstantPower(const ConstantPower &other)
              : Extends<ConstantPower,PowerFunction>(other), ex(other.ex)
{}

ConstantPower::~ConstantPower()
{}

ConstantPower& ConstantPower::operator=(const ConstantPower &other)
{
    if (this != &other)
    {
        ex = other.ex;
        PowerFunction::operator=(other);
    }
    
    return *this;
}

bool ConstantPower::operator==(const ConstantPower &other) const
{
    return ex == other.ex;
}

bool ConstantPower::operator!=(const ConstantPower &other) const
{
    return ex != other.ex;
}

Expression ConstantPower::core() const
{
    return ex;
}

void ConstantPower::stream(Siren::Stream &s)
{
    s.assertVersion<ConstantPower>(1);
    
    Schema schema = s.item<ConstantPower>();
    
    schema.data("core") & ex;
    
    PowerFunction::stream( schema.base() );
}

//////////
////////// Implementation of IntegerPower
//////////

static const RegisterObject<IntegerPower> r_integerpower;

/** Null constructor */
IntegerPower::IntegerPower() : Implements<IntegerPower,ConstantPower>(), pwr(0)
{}

/** Construct expression^power */
IntegerPower::IntegerPower(const Expression &expression, int power)
             : Implements<IntegerPower,ConstantPower>(expression), pwr(power)
{}

/** Copy constructor */
IntegerPower::IntegerPower(const IntegerPower &other)
             : Implements<IntegerPower,ConstantPower>(other), pwr(other.pwr)
{}

/** Destructor */
IntegerPower::~IntegerPower()
{}

IntegerPower& IntegerPower::operator=(const IntegerPower &other)
{
    if (this != &other)
    {
        pwr = other.pwr;
        ConstantPower::operator=(other);
    }
    
    return *this;
}

bool IntegerPower::operator==(const IntegerPower &other) const
{
    return pwr == other.pwr and ConstantPower::operator==(other);
}

bool IntegerPower::operator!=(const IntegerPower &other) const
{
    return not IntegerPower::operator==(other);
}

void IntegerPower::stream(Stream &s)
{
    s.assertVersion<IntegerPower>(1);
    
    Schema schema = s.item<IntegerPower>();
    
    schema.data("power") & pwr;
    
    ConstantPower::stream( schema.base() );
}

/** Evaluate this power */
double IntegerPower::evaluate(const Values &values) const
{
    return SireMaths::pow( ex.evaluate(values), pwr );
}

/** Evaluate this power */
Complex IntegerPower::evaluate(const ComplexValues &values) const
{
    return SireMaths::pow( ex.evaluate(values), pwr );
}

//////////
////////// Implementation of RationalPower
//////////

static const RegisterObject<RationalPower> r_rationalpower;

/** Null constructor */
RationalPower::RationalPower() : Implements<RationalPower,ConstantPower>(), pwr(0)
{}

/** Construct expression^power */
RationalPower::RationalPower(const Expression &expression, const Rational &power)
              : Implements<RationalPower,ConstantPower>(expression), pwr(power)
{}

/** Copy constructor */
RationalPower::RationalPower(const RationalPower &other)
              : Implements<RationalPower,ConstantPower>(other), pwr(other.pwr)
{}

/** Destructor */
RationalPower::~RationalPower()
{}

RationalPower& RationalPower::operator=(const RationalPower &other)
{
    if (this != &other)
    {
        pwr = other.pwr;
        ConstantPower::operator=(other);
    }
    
    return *this;
}

bool RationalPower::operator==(const RationalPower &other) const
{
    return pwr == other.pwr and ConstantPower::operator==(other);
}

bool RationalPower::operator!=(const RationalPower &other) const
{
    return not RationalPower::operator==(other);
}

void RationalPower::stream(Stream &s)
{
    s.assertVersion<RationalPower>(1);
    
    Schema schema = s.item<RationalPower>();
    
    schema.data("power") & pwr;
    
    ConstantPower::stream( schema.base() );
}

Expression RationalPower::power() const
{
    return pwr;
}

/** Evaluate this power */
double RationalPower::evaluate(const Values &values) const
{
    return SireMaths::pow( ex.evaluate(values), pwr );
}

/** Evaluate this power */
Complex RationalPower::evaluate(const ComplexValues &values) const
{
    return SireMaths::pow( ex.evaluate(values), pwr );
}

//////////
////////// Implementation of RealPower
//////////

static const RegisterObject<RealPower> r_realpower;

/** Null constructor */
RealPower::RealPower() : Implements<RealPower,ConstantPower>(), pwr(0)
{}

/** Construct expression^power */
RealPower::RealPower(const Expression &expression, double power)
          : Implements<RealPower,ConstantPower>(expression), pwr(power)
{}

/** Copy constructor */
RealPower::RealPower(const RealPower &other)
          : Implements<RealPower,ConstantPower>(other), pwr(other.pwr)
{}

/** Destructor */
RealPower::~RealPower()
{}

RealPower& RealPower::operator=(const RealPower &other)
{
    if (this != &other)
    {
        pwr = other.pwr;
        ConstantPower::operator=(other);
    }
    
    return *this;
}

bool RealPower::operator==(const RealPower &other) const
{
    return pwr == other.pwr and ConstantPower::operator==(other);
}

bool RealPower::operator!=(const RealPower &other) const
{
    return not RealPower::operator==(other);
}

void RealPower::stream(Stream &s)
{
    s.assertVersion<RealPower>(1);
    
    Schema schema = s.item<RealPower>();
    
    schema.data("power") & pwr;
    
    ConstantPower::stream( schema.base() );
}

Expression RealPower::power() const
{
    return pwr;
}

/** Evaluate this power */
double RealPower::evaluate(const Values &values) const
{
    return SireMaths::pow( ex.evaluate(values), pwr );
}

/** Evaluate this power */
Complex RealPower::evaluate(const ComplexValues &values) const
{
    return SireMaths::pow( ex.evaluate(values), pwr );
}

//////////
////////// Implementation of ComplexPower
//////////

static const RegisterObject<ComplexPower> r_complexpower;

/** Null constructor */
ComplexPower::ComplexPower() : Implements<ComplexPower,ConstantPower>(), pwr(0)
{}

/** Construct expression^power */
ComplexPower::ComplexPower(const Expression &expression, const Complex &power)
             : Implements<ComplexPower,ConstantPower>(expression), pwr(power)
{}

/** Copy constructor */
ComplexPower::ComplexPower(const ComplexPower &other)
             : Implements<ComplexPower,ConstantPower>(other), pwr(other.pwr)
{}

/** Destructor */
ComplexPower::~ComplexPower()
{}

ComplexPower& ComplexPower::operator=(const ComplexPower &other)
{
    if (this != &other)
    {
        pwr = other.pwr;
        ConstantPower::operator=(other);
    }
    
    return *this;
}

bool ComplexPower::operator==(const ComplexPower &other) const
{
    return pwr == other.pwr and ConstantPower::operator==(other);
}

bool ComplexPower::operator!=(const ComplexPower &other) const
{
    return not ComplexPower::operator==(other);
}

void ComplexPower::stream(Stream &s)
{
    s.assertVersion<ComplexPower>(1);
    
    Schema schema = s.item<ComplexPower>();
    
    schema.data("power") & pwr;
    
    ConstantPower::stream( schema.base() );
}

Expression ComplexPower::power() const
{
    return pwr;
}

bool ComplexPower::isComplex() const
{
    return (not pwr.isReal()) or core().isComplex();
}

/** Evaluate this power */
double ComplexPower::evaluate(const Values &values) const
{
    //calculate the result...
    Complex val = SireMaths::pow( ex.evaluate(values), pwr );

    if (not val.isReal())
        throw SireMaths::domain_error(QObject::tr(
            "Raising the expression \"%1\" to the complex power \"%2\" has "
            "resulted in a complex value, \"%3\"")
                .arg(ex.toString(), pwr.toString(), val.toString()), CODELOC);

    return val.real();
}

/** Evaluate this power */
Complex ComplexPower::evaluate(const ComplexValues &values) const
{
    return SireMaths::pow( ex.evaluate(values), pwr );
}
