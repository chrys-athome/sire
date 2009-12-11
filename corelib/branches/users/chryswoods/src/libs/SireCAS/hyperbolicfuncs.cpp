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

#include "hyperbolicfuncs.h"
#include "trigfuncs.h"
#include "exp.h"
#include "identities.h"
#include "expression.h"
#include "complexvalues.h"

#include "SireMaths/complex.h"

#include "Siren/stream.h"

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

////////////
//////////// Implementation of Hyperbolic Cosine
////////////

static const RegisterObject<Cosh> r_cosh;

/** Null constructor */
Cosh::Cosh() : Implements<Cosh,SingleFunc>()
{}

/** Construct cosh(expression) */
Cosh::Cosh(const Expression &expression) : Implements<Cosh,SingleFunc>(expression)
{}

/** Copy constructor */
Cosh::Cosh(const Cosh &other) : Implements<Cosh,SingleFunc>(other)
{}

/** Destructor */
Cosh::~Cosh()
{}

Cosh& Cosh::operator=(const Cosh &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Cosh::operator==(const Cosh &other) const
{
    return SingleFunc::operator==(other);
}

bool Cosh::operator!=(const Cosh &other) const
{
    return SingleFunc::operator!=(other);
}

void Cosh::stream(Stream &s)
{
    s.assertVersion<Cosh>(1);
    
    Schema schema = s.item<Cosh>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Cosh::evaluate(const Values &values) const
{
    return std::cosh( x().evaluate(values) );
}

/** Complex evaluation */
Complex Cosh::evaluate(const ComplexValues &values) const
{
    return SireMaths::cosh( x().evaluate(values) );
}

Expression Cosh::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Cosh(arg);
}

QString Cosh::stringRep() const
{
    return QObject::tr("cosh");
}

/** The differential of cosh(x) = sinh(x) */
Expression Cosh::diff() const
{
    return Sinh(x());
}

/** Integral of cosh(x) = sinh(x) */
Expression Cosh::integ() const
{
    return Sinh(x());
}

////////////
//////////// Implementation of hyperbolic sine
////////////

static const RegisterObject<Sinh> r_sinh;

/** Null constructor */
Sinh::Sinh() : Implements<Sinh,SingleFunc>()
{}

/** Construct cos(expression) */
Sinh::Sinh(const Expression &expression) : Implements<Sinh,SingleFunc>(expression)
{}

/** Copy constructor */
Sinh::Sinh(const Sinh &other) : Implements<Sinh,SingleFunc>(other)
{}

/** Destructor */
Sinh::~Sinh()
{}

Sinh& Sinh::operator=(const Sinh &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Sinh::operator==(const Sinh &other) const
{
    return SingleFunc::operator==(other);
}

bool Sinh::operator!=(const Sinh &other) const
{
    return SingleFunc::operator!=(other);
}

void Sinh::stream(Stream &s)
{
    s.assertVersion<Sinh>(1);
    
    Schema schema = s.item<Sinh>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Sinh::evaluate(const Values &values) const
{
    return std::sinh( x().evaluate(values) );
}

/** Complex evaluation */
Complex Sinh::evaluate(const ComplexValues &values) const
{
    return SireMaths::sinh( x().evaluate(values) );
}

Expression Sinh::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Sinh(arg);
}

QString Sinh::stringRep() const
{
    return QObject::tr("sinh");
}

/** The differential of sinh(x) = cosh(x) */
Expression Sinh::diff() const
{
    return Cosh(x());
}

/** Integral of sinh(x) = cosh(x) */
Expression Sinh::integ() const
{
    return  -Cosh(x());
}

////////////
//////////// Implementation of hyperbolic tangent
////////////

static const RegisterObject<Tanh> r_tanh;

/** Null constructor */
Tanh::Tanh() : Implements<Tanh,SingleFunc>()
{}

/** Construct cos(expression) */
Tanh::Tanh(const Expression &expression) : Implements<Tanh,SingleFunc>(expression)
{}

/** Copy constructor */
Tanh::Tanh(const Tanh &other) : Implements<Tanh,SingleFunc>(other)
{}

/** Destructor */
Tanh::~Tanh()
{}

Tanh& Tanh::operator=(const Tanh &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Tanh::operator==(const Tanh &other) const
{
    return SingleFunc::operator==(other);
}

bool Tanh::operator!=(const Tanh &other) const
{
    return SingleFunc::operator!=(other);
}

void Tanh::stream(Stream &s)
{
    s.assertVersion<Tanh>(1);
    
    Schema schema = s.item<Tanh>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Tanh::evaluate(const Values &values) const
{
    return std::tanh( x().evaluate(values) );
}

/** Complex evaluation */
Complex Tanh::evaluate(const ComplexValues &values) const
{
    return SireMaths::tanh( x().evaluate(values) );
}

Expression Tanh::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Tanh(arg);
}

QString Tanh::stringRep() const
{
    return QObject::tr("tanh");
}

/** The differential of tanh(x) = sech^2(x) */
Expression Tanh::diff() const
{
    return pow( Sech(x()), 2 );
}

/** Integral of tan(x) = ln [ cosh(x) ] + C */
Expression Tanh::integ() const
{
    return Ln( Cosh(x()) );
}

////////////
//////////// Implementation of hyperbolic cosecant
////////////

static const RegisterObject<Csch> r_csch;

/** Null constructor */
Csch::Csch() : Implements<Csch,SingleFunc>()
{}

/** Construct cos(expression) */
Csch::Csch(const Expression &expression) : Implements<Csch,SingleFunc>(expression)
{}

/** Copy constructor */
Csch::Csch(const Csch &other) : Implements<Csch,SingleFunc>(other)
{}

/** Destructor */
Csch::~Csch()
{}

Csch& Csch::operator=(const Csch &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Csch::operator==(const Csch &other) const
{
    return SingleFunc::operator==(other);
}

bool Csch::operator!=(const Csch &other) const
{
    return SingleFunc::operator!=(other);
}

void Csch::stream(Stream &s)
{
    s.assertVersion<Csch>(1);
    
    Schema schema = s.item<Csch>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Csch::evaluate(const Values &values) const
{
    //csch = 1 / sinh
    return double(1.0) / std::sinh( x().evaluate(values) );
}

/** Complex evaluation */
Complex Csch::evaluate(const ComplexValues &values) const
{
    return SireMaths::csch( x().evaluate(values) );
}

Expression Csch::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Csch(arg);
}

QString Csch::stringRep() const
{
    return QObject::tr("csch");
}

/** The differential of csc(x) = -coth(x) csch(x) */
Expression Csch::diff() const
{
    return -Csch(x()) * Coth(x());
}

/** Integral of csc(x) = ln( sinh(x/2) ) - ln( cosh(x/2) )  */
Expression Csch::integ() const
{
    return Ln( Sinh(x()/2) ) - Ln( Cosh(x()/2) );
}

////////////
//////////// Implementation of hyperbolic secant
////////////

static const RegisterObject<Sech> r_sech;

/** Null constructor */
Sech::Sech() : Implements<Sech,SingleFunc>()
{}

/** Construct cos(expression) */
Sech::Sech(const Expression &expression) : Implements<Sech,SingleFunc>(expression)
{}

/** Copy constructor */
Sech::Sech(const Sech &other) : Implements<Sech,SingleFunc>(other)
{}

/** Destructor */
Sech::~Sech()
{}

Sech& Sech::operator=(const Sech &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Sech::operator==(const Sech &other) const
{
    return SingleFunc::operator==(other);
}

bool Sech::operator!=(const Sech &other) const
{
    return SingleFunc::operator!=(other);
}

void Sech::stream(Stream &s)
{
    s.assertVersion<Sech>(1);
    
    Schema schema = s.item<Sech>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Sech::evaluate(const Values &values) const
{
    //sech = 1 / cosh
    return double(1.0) / std::cosh( x().evaluate(values) );
}

/** Complex evaluation */
Complex Sech::evaluate(const ComplexValues &values) const
{
    return SireMaths::sech( x().evaluate(values) );
}

Expression Sech::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Sech(arg);
}

QString Sech::stringRep() const
{
    return QObject::tr("sech");
}

/** The differential of sec(x) = -sech(x) tanh(x) */
Expression Sech::diff() const
{
    return -Sech(x()) * Tanh(x());
}

/** Integral of sech(x) = 2 cot( tanh(x/2) ) */
Expression Sech::integ() const
{
    return 2 * Cot( Tanh(x()/2) );
}

////////////
//////////// Implementation of hyperbolic cotangent
////////////

static const RegisterObject<Coth> r_coth;

/** Null constructor */
Coth::Coth() : Implements<Coth,SingleFunc>()
{}

/** Construct cos(expression) */
Coth::Coth(const Expression &expression) : Implements<Coth,SingleFunc>(expression)
{}

/** Copy constructor */
Coth::Coth(const Coth &other) : Implements<Coth,SingleFunc>(other)
{}

/** Destructor */
Coth::~Coth()
{}

Coth& Coth::operator=(const Coth &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Coth::operator==(const Coth &other) const
{
    return SingleFunc::operator==(other);
}

bool Coth::operator!=(const Coth &other) const
{
    return SingleFunc::operator!=(other);
}

void Coth::stream(Stream &s)
{
    s.assertVersion<Coth>(1);
    
    Schema schema = s.item<Coth>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Coth::evaluate(const Values &values) const
{
    //coth = 1 / tanh
    return double(1.0) / std::tanh( x().evaluate(values) );
}

/** Complex evaluation */
Complex Coth::evaluate(const ComplexValues &values) const
{
    return SireMaths::coth( x().evaluate(values) );
}

Expression Coth::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Coth(arg);
}

QString Coth::stringRep() const
{
    return QObject::tr("coth");
}

/** The differential of coth(x) = -csch^2(x) */
Expression Coth::diff() const
{
    return -(pow( Csch(x()), 2 ));
}

/** Integral of coth(x) = ln( sinh(x) ) */
Expression Coth::integ() const
{
    return Ln( Sinh(x()) );
}
