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

#include <cmath>

#include "SireMaths/complex.h"

#include "trigfuncs.h"
#include "exp.h"
#include "identities.h"
#include "expression.h"
#include "complexvalues.h"

#include "Siren/stream.h"

using namespace Siren;
using namespace SireCAS;

////////////
//////////// Implementation of Cosine
////////////

static const RegisterObject<Cos> r_cos;

/** Null constructor */
Cos::Cos() : Implements<Cos,SingleFunc>()
{}

/** Construct cos(expression) */
Cos::Cos(const Expression &expression) : Implements<Cos,SingleFunc>(expression)
{}

/** Create cos(cos(expression)) */
Cos::Cos(const Cos &other) : Implements<Cos,SingleFunc>(other)
{}

/** Destructor */
Cos::~Cos()
{}

Cos& Cos::operator=(const Cos &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Cos::operator==(const Cos &other) const
{
    return SingleFunc::operator==(other);
}

bool Cos::operator!=(const Cos &other) const
{
    return SingleFunc::operator!=(other);
}

void Cos::stream(Stream &s)
{
    s.assertVersion<Cos>(1);
    
    Schema schema = s.item<Cos>();
    
    SingleFunc::stream(schema.base());
}

/** Evaluate this function */
double Cos::evaluate(const Values &values) const
{
    return std::cos( x().evaluate(values) );
}

/** Complex evaluation */
Complex Cos::evaluate(const ComplexValues &values) const
{
    return SireMaths::cos( x().evaluate(values) );
}

Expression Cos::functionOf(const Expression &arg) const
{
    if (arg == this->argument())
        return *this;
    else
        return Cos(arg);
}

QString Cos::stringRep() const
{
    return QObject::tr("cos");
}

/** The differential of cos(x) = -sin(x) */
Expression Cos::diff() const
{
    return -Sin(x());
}

/** Integral of cos(x) = sin(x) */
Expression Cos::integ() const
{
    return Sin(x());
}

////////////
//////////// Implementation of Sine
////////////

static const RegisterObject<Sin> r_sin;

/** Null constructor */
Sin::Sin() : Implements<Sin,SingleFunc>()
{}

/** Construct cos(expression) */
Sin::Sin(const Expression &expression) : Implements<Sin,SingleFunc>(expression)
{}

/** Copy constructor */
Sin::Sin(const Sin &other) : Implements<Sin,SingleFunc>(other)
{}

/** Destructor */
Sin::~Sin()
{}

Sin& Sin::operator=(const Sin &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Sin::operator==(const Sin &other) const
{
    return SingleFunc::operator==(other);
}

bool Sin::operator!=(const Sin &other) const
{
    return SingleFunc::operator!=(other);
}

void Sin::stream(Stream &s)
{
    s.assertVersion<Sin>(1);
    
    Schema schema = s.item<Sin>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Sin::evaluate(const Values &values) const
{
    return std::sin( x().evaluate(values) );
}

/** Complex evaluation */
Complex Sin::evaluate(const ComplexValues &values) const
{
    return SireMaths::sin( x().evaluate(values) );
}

Expression Sin::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Sin(arg);
}

QString Sin::stringRep() const
{
    return QObject::tr("sin");
}

/** The differential of sin(x) = cos(x) */
Expression Sin::diff() const
{
    return Cos(x());
}

/** Integral of sin(x) = -cos(x) */
Expression Sin::integ() const
{
    return  -Cos(x());
}

////////////
//////////// Implementation of Tangent
////////////

static const RegisterObject<Tan> r_tan;

/** Null constructor */
Tan::Tan() : Implements<Tan,SingleFunc>()
{}

/** Construct cos(expression) */
Tan::Tan(const Expression &expression) : Implements<Tan,SingleFunc>(expression)
{}

/** Copy constructor */
Tan::Tan(const Tan &other) : Implements<Tan,SingleFunc>(other)
{}

/** Destructor */
Tan::~Tan()
{}

Tan& Tan::operator=(const Tan &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Tan::operator==(const Tan &other) const
{
    return SingleFunc::operator==(other);
}

bool Tan::operator!=(const Tan &other) const
{
    return SingleFunc::operator!=(other);
}

void Tan::stream(Stream &s)
{
    s.assertVersion<Tan>(1);
    
    Schema schema = s.item<Tan>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Tan::evaluate(const Values &values) const
{
    return std::tan( x().evaluate(values) );
}

/** Complex evaluation */
Complex Tan::evaluate(const ComplexValues &values) const
{
    return SireMaths::tan( x().evaluate(values) );
}

Expression Tan::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Tan(arg);
}

QString Tan::stringRep() const
{
    return QObject::tr("tan");
}

/** The differential of tan(x) = sec^2(x) */
Expression Tan::diff() const
{
    return pow( Sec(x()), 2 );
}

/** Integral of tan(x) = ln [ sec(x) ] + C */
Expression Tan::integ() const
{
    return Ln( Sec(x()) );
}

////////////
//////////// Implementation of Cosecant
////////////

static const RegisterObject<Csc> r_csc;

/** Null constructor */
Csc::Csc() : Implements<Csc,SingleFunc>()
{}

/** Construct cos(expression) */
Csc::Csc(const Expression &expression) : Implements<Csc,SingleFunc>(expression)
{}

/** Copy constructor */
Csc::Csc(const Csc &other) : Implements<Csc,SingleFunc>(other)
{}

/** Destructor */
Csc::~Csc()
{}

Csc& Csc::operator=(const Csc &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Csc::operator==(const Csc &other) const
{
    return SingleFunc::operator==(other);
}

bool Csc::operator!=(const Csc &other) const
{
    return SingleFunc::operator!=(other);
}

void Csc::stream(Stream &s)
{
    s.assertVersion<Csc>(1);
    
    Schema schema = s.item<Csc>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Csc::evaluate(const Values &values) const
{
    //csc = 1 / sin
    return double(1.0) / std::sin( x().evaluate(values) );
}

/** Complex evaluation */
Complex Csc::evaluate(const ComplexValues &values) const
{
    return SireMaths::csc( x().evaluate(values) );
}

Expression Csc::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Csc(arg);
}

QString Csc::stringRep() const
{
    return QObject::tr("csc");
}

/** The differential of csc(x) = -csc(x) cot(x) */
Expression Csc::diff() const
{
    return -Csc(x()) * Cot(x());
}

/** Integral of csc(x) = ln( sin(x/2) ) - ln( cos(x/2) ) + C */
Expression Csc::integ() const
{
    return Ln( Sin(x()/2) ) - Ln( Cos(x()/2) );
}

////////////
//////////// Implementation of Secant
////////////

static const RegisterObject<Sec> r_sec;

/** Null constructor */
Sec::Sec() : Implements<Sec,SingleFunc>()
{}

/** Construct cos(expression) */
Sec::Sec(const Expression &expression) : Implements<Sec,SingleFunc>(expression)
{}

/** Copy constructor */
Sec::Sec(const Sec &other) : Implements<Sec,SingleFunc>(other)
{}

/** Destructor */
Sec::~Sec()
{}

Sec& Sec::operator=(const Sec &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Sec::operator==(const Sec &other) const
{
    return SingleFunc::operator==(other);
}

bool Sec::operator!=(const Sec &other) const
{
    return SingleFunc::operator!=(other);
}

void Sec::stream(Stream &s)
{
    s.assertVersion<Sec>(1);
    
    Schema schema = s.item<Sec>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Sec::evaluate(const Values &values) const
{
    //sec = 1 / cos
    return double(1.0) / std::cos( x().evaluate(values) );
}

/** Complex evaluation */
Complex Sec::evaluate(const ComplexValues &values) const
{
    return SireMaths::sec( x().evaluate(values) );
}

Expression Sec::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Sec(arg);
}

QString Sec::stringRep() const
{
    return QObject::tr("sec");
}

/** The differential of sec(x) = sec(x) tan(x) */
Expression Sec::diff() const
{
    return Sec(x()) * Tan(x());
}

/** Integral of sec(x) = ln( cos(x/2) + sin(x/2) ) - ln( cos(x/2) - sin(x/2) ) + C  */
Expression Sec::integ() const
{
    return Ln( Cos(x()/2) + Sin(x()/2) ) - Ln( Cos(x()/2) - Sin(x()/2) );
}

////////////
//////////// Implementation of Cotangent
////////////

static const RegisterObject<Cot> r_cot;

/** Null constructor */
Cot::Cot() : Implements<Cot,SingleFunc>()
{}

/** Construct cos(expression) */
Cot::Cot(const Expression &expression) : Implements<Cot,SingleFunc>(expression)
{}

/** Copy constructor */
Cot::Cot(const Cot &other) : Implements<Cot,SingleFunc>(other)
{}

/** Destructor */
Cot::~Cot()
{}

Cot& Cot::operator=(const Cot &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool Cot::operator==(const Cot &other) const
{
    return SingleFunc::operator==(other);
}

bool Cot::operator!=(const Cot &other) const
{
    return SingleFunc::operator!=(other);
}

void Cot::stream(Stream &s)
{
    s.assertVersion<Cot>(1);
    
    Schema schema = s.item<Cot>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double Cot::evaluate(const Values &values) const
{
    //cot = 1 / tan
    return double(1.0) / std::tan( x().evaluate(values) );
}

/** Complex evaluation */
Complex Cot::evaluate(const ComplexValues &values) const
{
    return SireMaths::cot( x().evaluate(values) );
}

Expression Cot::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return Cot(arg);
}

QString Cot::stringRep() const
{
    return QObject::tr("cot");
}

/** The differential of cot(x) = -csc^2(x) */
Expression Cot::diff() const
{
    return -(pow( Csc(x()), 2 ));
}

/** Integral of cot(x) = ln( sin(x) ) */
Expression Cot::integ() const
{
    return Ln( Sin(x()) );
}
