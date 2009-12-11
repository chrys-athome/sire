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

#include "trigfuncs.h"
#include "invtrigfuncs.h"
#include "hyperbolicfuncs.h"
#include "invhyperbolicfuncs.h"
#include "exp.h"
#include "identities.h"
#include "expression.h"
#include "complexvalues.h"

#include "SireMaths/complex.h"
#include "SireMaths/errors.h"

#include "Siren/stream.h"

#ifndef HAVE_ASINH
    #include <boost/math/special_functions/asinh.hpp>
    #define asinh boost::math::asinh
#endif

using namespace SireCAS;
using namespace Siren;
using namespace SireMaths;

////////////
//////////// Implementation of Inverse-hyperbolic-cosine
////////////

static const RegisterObject<ArcCosh> r_arccosh;

/** Null constructor */
ArcCosh::ArcCosh() : Implements<ArcCosh,SingleFunc>()
{}

/** Construct cos(expression) */
ArcCosh::ArcCosh(const Expression &expression)
        : Implements<ArcCosh,SingleFunc>(expression)
{}

/** Create cos(cos(expression)) */
ArcCosh::ArcCosh(const ArcCosh &other) : Implements<ArcCosh,SingleFunc>(other)
{}

/** Destructor */
ArcCosh::~ArcCosh()
{}

ArcCosh& ArcCosh::operator=(const ArcCosh &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcCosh::operator==(const ArcCosh &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcCosh::operator!=(const ArcCosh &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcCosh::stream(Stream &s)
{
    s.assertVersion<ArcCosh>(1);
    
    Schema schema = s.item<ArcCosh>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcCosh::evaluate(const Values &values) const
{
    Complex val = SireMaths::arccosh_real( x().evaluate(values) );

    if (not val.isReal())
        throw SireMaths::domain_error(QObject::tr(
            "arccosh(%1) is complex (%2): %3")
                .arg(x().evaluate(values)).arg(val.toString(), toString()), CODELOC);

    return val.real();
}

/** Complex evaluation */
Complex ArcCosh::evaluate(const ComplexValues &values) const
{
    return SireMaths::arccosh( x().evaluate(values) );
}

Expression ArcCosh::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcCosh(arg);
}

QString ArcCosh::stringRep() const
{
    return QObject::tr("arccosh");
}

/** The differential of acosh(x) = 1 / [sqrt(x-1)*sqrt(x+1)] */
Expression ArcCosh::diff() const
{
    return -1 / ( sqrt(x() - 1) * sqrt(x()+1) );
}

/** Integral of acosh(x) = x acosh(x) - (1+x) * [ sqrt(x-1)/sqrt(x+1) ] */
Expression ArcCosh::integ() const
{
    return x()*ArcCosh(x()) - (1+x())*( sqrt(x()-1) / sqrt(x()+1) );
}

////////////
//////////// Implementation of Inverse-hyperbolic-sine
////////////

static const RegisterObject<ArcSinh> r_arcsinh;

/** Null constructor */
ArcSinh::ArcSinh() : Implements<ArcSinh,SingleFunc>()
{}

/** Construct cos(expression) */
ArcSinh::ArcSinh(const Expression &expression) 
        : Implements<ArcSinh,SingleFunc>(expression)
{}

/** Copy constructor */
ArcSinh::ArcSinh(const ArcSinh &other) : Implements<ArcSinh,SingleFunc>(other)
{}

/** Destructor */
ArcSinh::~ArcSinh()
{}

ArcSinh& ArcSinh::operator=(const ArcSinh &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcSinh::operator==(const ArcSinh &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcSinh::operator!=(const ArcSinh &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcSinh::stream(Stream &s)
{
    s.assertVersion<ArcSinh>(1);
    
    Schema schema = s.item<ArcSinh>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcSinh::evaluate(const Values &values) const
{
    return asinh( x().evaluate(values) );
}

/** Complex evaluation */
Complex ArcSinh::evaluate(const ComplexValues &values) const
{
    return SireMaths::arcsinh( x().evaluate(values) );
}

Expression ArcSinh::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcSinh(arg);
}

QString ArcSinh::stringRep() const
{
    return QObject::tr("arcsinh");
}

/** The differential of asinh(x) = 1 / sqrt(1+x^2) */
Expression ArcSinh::diff() const
{
    return 1 / sqrt( 1 + pow(x(),2) );
}

/** Integral of asinh(x) = x*asinh(x) - sqrt(1+x^2) */
Expression ArcSinh::integ() const
{
    return  x()*ArcSinh(x()) - sqrt( 1 + pow(x(),2) );
}

////////////
//////////// Implementation of Inverse-hyperbolic-tangent
////////////

static const RegisterObject<ArcTanh> r_arctanh;

/** Null constructor */
ArcTanh::ArcTanh() : Implements<ArcTanh,SingleFunc>()
{}

/** Construct cos(expression) */
ArcTanh::ArcTanh(const Expression &expression) 
        : Implements<ArcTanh,SingleFunc>(expression)
{}

/** Copy constructor */
ArcTanh::ArcTanh(const ArcTanh &other) : Implements<ArcTanh,SingleFunc>(other)
{}

/** Destructor */
ArcTanh::~ArcTanh()
{}

ArcTanh& ArcTanh::operator=(const ArcTanh &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcTanh::operator==(const ArcTanh &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcTanh::operator!=(const ArcTanh &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcTanh::stream(Stream &s)
{
    s.assertVersion<ArcTanh>(1);
    
    Schema schema = s.item<ArcTanh>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcTanh::evaluate(const Values &values) const
{
    Complex val = SireMaths::arctanh_real( x().evaluate(values) );

    if (not val.isReal())
        throw SireMaths::domain_error(QObject::tr(
            "arctanh(%1) is complex (%2): %3")
                .arg(x().evaluate(values)).arg(val.toString(), toString()), CODELOC);

    return val.real();
}

/** Complex evaluation */
Complex ArcTanh::evaluate(const ComplexValues &values) const
{
    return SireMaths::arctanh( x().evaluate(values) );
}

Expression ArcTanh::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcTanh(arg);
}

QString ArcTanh::stringRep() const
{
    return QObject::tr("arctanh");
}

/** The differential of atanh(x) = 1 / (1-x^2) */
Expression ArcTanh::diff() const
{
    return 1 / ( 1 - pow(x(),2) );
}

/** Integral of atanh(x) = x arctanh(x) + 0.5 ln( x^2 - 1 ) */
Expression ArcTanh::integ() const
{
    return x()*ArcTanh(x()) + 0.5*Ln( pow(x(),2) - 1 );
}

////////////
//////////// Implementation of Inverse-hyperbolic-cosecant
////////////

static const RegisterObject<ArcCsch> r_arccsch;

/** Null constructor */
ArcCsch::ArcCsch() : Implements<ArcCsch,SingleFunc>()
{}

/** Construct cos(expression) */
ArcCsch::ArcCsch(const Expression &expression) 
        : Implements<ArcCsch,SingleFunc>(expression)
{}

/** Copy constructor */
ArcCsch::ArcCsch(const ArcCsch &other) : Implements<ArcCsch,SingleFunc>(other)
{}

/** Destructor */
ArcCsch::~ArcCsch()
{}

ArcCsch& ArcCsch::operator=(const ArcCsch &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcCsch::operator==(const ArcCsch &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcCsch::operator!=(const ArcCsch &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcCsch::stream(Stream &s)
{
    s.assertVersion<ArcCsch>(1);
    
    Schema schema = s.item<ArcCsch>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcCsch::evaluate(const Values &values) const
{
    Complex val = SireMaths::arccsch( Complex(x().evaluate(values), 0) );

    if (not val.isReal())
        throw SireMaths::domain_error(QObject::tr(
            "arccsch(%1) is complex (%2): %3")
                .arg(x().evaluate(values)).arg(val.toString(), toString()), CODELOC);

    return val.real();
}

/** Complex evaluation */
Complex ArcCsch::evaluate(const ComplexValues &values) const
{
    return SireMaths::arccsch( x().evaluate(values) );
}

Expression ArcCsch::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcCsch(arg);
}

QString ArcCsch::stringRep() const
{
    return QObject::tr("arccsch");
}

/** The differential of acsch(x) = -1 / (x^2 sqrt(1 + x^-2)) */
Expression ArcCsch::diff() const
{
    return -1 / ( pow(x(),2) * sqrt( 1 + pow(x(),-2) ) );
}

/** Integral of acsch(x) = x acsch(x) + Ln[ x ( 1 + sqrt( (x^2+1)/x^2 ) ) ] */
Expression ArcCsch::integ() const
{
    return x()*ArcCsch(x()) + Ln( x() * ( 1 + sqrt( (pow(x(),2)+1) / pow(x(),2) ) ) );
}

////////////
//////////// Implementation of inverse-secant
////////////

static const RegisterObject<ArcSech> r_arcsech;

/** Null constructor */
ArcSech::ArcSech() : Implements<ArcSech,SingleFunc>()
{}

/** Construct cos(expression) */
ArcSech::ArcSech(const Expression &expression) 
        : Implements<ArcSech,SingleFunc>(expression)
{}

/** Copy constructor */
ArcSech::ArcSech(const ArcSech &other) : Implements<ArcSech,SingleFunc>(other)
{}

/** Destructor */
ArcSech::~ArcSech()
{}

ArcSech& ArcSech::operator=(const ArcSech &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcSech::operator==(const ArcSech &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcSech::operator!=(const ArcSech &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcSech::stream(Stream &s)
{
    s.assertVersion<ArcSech>(1);
    
    Schema schema = s.item<ArcSech>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcSech::evaluate(const Values &values) const
{
    Complex val = SireMaths::arcsech( Complex(x().evaluate(values), 0) );

    if (not val.isReal())
        throw SireMaths::domain_error(QObject::tr(
            "arcsech(%1) is complex (%2): %3")
                .arg(x().evaluate(values)).arg(val.toString(), toString()), CODELOC);

    return val.real();
}

/** Complex evaluation */
Complex ArcSech::evaluate(const ComplexValues &values) const
{
    return SireMaths::arcsech( x().evaluate(values) );
}

Expression ArcSech::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcSech(arg);
}

QString ArcSech::stringRep() const
{
    return QObject::tr("arcsech");
}

/** The differential of asech(x) = 1 / [x (x+1) sqrt( (1-x)/(1+x) )] */
Expression ArcSech::diff() const
{
    return 1 / ( x() * (x()+1) * sqrt( (1-x())/(1+x()) ) );
}

/** Integral of asech(x) = x arcsech(x) - arctan[ (x/(x-1)) * sqrt( (1-x)/(1+x) )] */
Expression ArcSech::integ() const
{
    return x()*ArcSech(x()) - ArcTan( (x()/(x()-1)) * sqrt( (1-x())/(1+x())) );
}

////////////
//////////// Implementation of Inverse-hyperbolic-cotangent
////////////

static const RegisterObject<ArcCoth> r_arccoth;

/** Null constructor */
ArcCoth::ArcCoth() : Implements<ArcCoth,SingleFunc>()
{}

/** Construct cos(expression) */
ArcCoth::ArcCoth(const Expression &expression) 
        : Implements<ArcCoth,SingleFunc>(expression)
{}

/** Copy constructor */
ArcCoth::ArcCoth(const ArcCoth &other) : Implements<ArcCoth,SingleFunc>(other)
{}

/** Destructor */
ArcCoth::~ArcCoth()
{}

ArcCoth& ArcCoth::operator=(const ArcCoth &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcCoth::operator==(const ArcCoth &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcCoth::operator!=(const ArcCoth &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcCoth::stream(Stream &s)
{
    s.assertVersion<ArcCoth>(1);
    
    Schema schema = s.item<ArcCoth>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcCoth::evaluate(const Values &values) const
{
    Complex val = SireMaths::arccoth( Complex(x().evaluate(values), 0) );

    if (not val.isReal())
        throw SireMaths::domain_error(QObject::tr(
            "arccoth(%1) is complex (%2): %3")
                .arg(x().evaluate(values)).arg(val.toString(), toString()), CODELOC);

    return val.real();
}

/** Complex evaluation */
Complex ArcCoth::evaluate(const ComplexValues &values) const
{
    return SireMaths::arccoth( x().evaluate(values) );
}

Expression ArcCoth::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcCoth(arg);
}

QString ArcCoth::stringRep() const
{
    return QObject::tr("arccoth");
}

/** The differential of acoth(x) = 1 / (1-x^2) */
Expression ArcCoth::diff() const
{
    return 1 / (1 - pow(x(),2));
}

/** Integral of acoth(x) = x acoth(x) + (1/2) ln( x^2 - 1 ) */
Expression ArcCoth::integ() const
{
    return x()*ArcCoth(x()) + 0.5*Ln( pow(x(),2) - 1 );
}
