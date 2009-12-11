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
#include "exp.h"
#include "identities.h"
#include "expression.h"
#include "complexvalues.h"

#include "SireMaths/complex.h"
#include "SireMaths/errors.h"

#include "Siren/stream.h"

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

////////////
//////////// Implementation of Inverse-cosine
////////////

static const RegisterObject<ArcCos> r_arccos;

/** Null constructor */
ArcCos::ArcCos() : Implements<ArcCos,SingleFunc>()
{}

/** Construct cos(expression) */
ArcCos::ArcCos(const Expression &expression) : Implements<ArcCos,SingleFunc>(expression)
{}

/** Create cos(cos(expression)) */
ArcCos::ArcCos(const ArcCos &other) : Implements<ArcCos,SingleFunc>(other)
{}

/** Destructor */
ArcCos::~ArcCos()
{}

ArcCos& ArcCos::operator=(const ArcCos &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcCos::operator==(const ArcCos &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcCos::operator!=(const ArcCos &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcCos::stream(Stream &s)
{
    s.assertVersion<ArcCos>(1);
    
    Schema schema = s.item<ArcCos>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcCos::evaluate(const Values &values) const
{
    Complex val = SireMaths::arccos_real( x().evaluate(values) );

    if (not val.isReal())
        throw SireMaths::domain_error(QObject::tr(
            "arccos(%1) is complex (%2): %3")
                .arg(x().evaluate(values)).arg(val.toString(), toString()), CODELOC);

    return val.real();
}

/** Complex evaluation */
Complex ArcCos::evaluate(const ComplexValues &values) const
{
    return SireMaths::arccos( x().evaluate(values) );
}

Expression ArcCos::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcCos(arg);
}

QString ArcCos::stringRep() const
{
    return QObject::tr("arccos");
}

/** The differential of acos(x) = -1 / sqrt(1-x^2) */
Expression ArcCos::diff() const
{
    return -1 / sqrt( 1 - SireCAS::pow(x(),2) );
}

/** Integral of acos(x) = x acos(x) - sqrt(1-x^2) */
Expression ArcCos::integ() const
{
    return x()*ArcCos(x()) - sqrt(1 - SireCAS::pow(x(),2));
}

////////////
//////////// Implementation of Inverse-sine
////////////

static const RegisterObject<ArcSin> r_arcsin;

/** Null constructor */
ArcSin::ArcSin() : Implements<ArcSin,SingleFunc>()
{}

/** Construct cos(expression) */
ArcSin::ArcSin(const Expression &expression) : Implements<ArcSin,SingleFunc>(expression)
{}

/** Copy constructor */
ArcSin::ArcSin(const ArcSin &other) : Implements<ArcSin,SingleFunc>(other)
{}

/** Destructor */
ArcSin::~ArcSin()
{}

ArcSin& ArcSin::operator=(const ArcSin &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcSin::operator==(const ArcSin &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcSin::operator!=(const ArcSin &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcSin::stream(Stream &s)
{
    s.assertVersion<ArcSin>(1);
    
    Schema schema = s.item<ArcSin>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcSin::evaluate(const Values &values) const
{
    Complex val = SireMaths::arcsin_real( x().evaluate(values) );

    if (not val.isReal())
        throw SireMaths::domain_error(QObject::tr(
            "arcsin(%1) is complex (%2): %3")
                .arg(x().evaluate(values)).arg(val.toString(), toString()), CODELOC);

    return val.real();
}

/** Complex evaluation */
Complex ArcSin::evaluate(const ComplexValues &values) const
{
    return SireMaths::arcsin( x().evaluate(values) );
}

Expression ArcSin::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcSin(arg);
}

QString ArcSin::stringRep() const
{
    return QObject::tr("arcsin");
}

/** The differential of asin(x) = 1 / sqrt(1-x^2) */
Expression ArcSin::diff() const
{
    return 1 / sqrt( 1 - SireCAS::pow(x(),2) );
}

/** Integral of asin(x) = x asin(x) + sqrt(1-x^2) */
Expression ArcSin::integ() const
{
    return  x()*ArcSin(x()) + sqrt( 1 - SireCAS::pow(x(),2) );
}

////////////
//////////// Implementation of Inverse-tangent
////////////

static const RegisterObject<ArcTan> r_arctan;

/** Null constructor */
ArcTan::ArcTan() : Implements<ArcTan,SingleFunc>()
{}

/** Construct cos(expression) */
ArcTan::ArcTan(const Expression &expression) : Implements<ArcTan,SingleFunc>(expression)
{}

/** Copy constructor */
ArcTan::ArcTan(const ArcTan &other) : Implements<ArcTan,SingleFunc>(other)
{}

/** Destructor */
ArcTan::~ArcTan()
{}

ArcTan& ArcTan::operator=(const ArcTan &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcTan::operator==(const ArcTan &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcTan::operator!=(const ArcTan &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcTan::stream(Stream &s)
{
    s.assertVersion<ArcTan>(1);
    
    Schema schema = s.item<ArcTan>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcTan::evaluate(const Values &values) const
{
    return std::atan( x().evaluate(values) );
}

/** Complex evaluation */
Complex ArcTan::evaluate(const ComplexValues &values) const
{
    return SireMaths::arctan( x().evaluate(values) );
}

Expression ArcTan::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcTan(arg);
}

QString ArcTan::stringRep() const
{
    return QObject::tr("arctan");
}

/** The differential of atan(x) = 1 / (1+x^2) */
Expression ArcTan::diff() const
{
    return 1 / ( 1 + SireCAS::pow(x(),2) );
}

/** Integral of atan(x) = x atan(x) - 0.5 ln( 1 + x^2 ) */
Expression ArcTan::integ() const
{
    return x()*ArcTan(x()) - 0.5*Ln( 1 + SireCAS::pow(x(),2) );
}

////////////
//////////// Implementation of Inverse-cosecant
////////////

static const RegisterObject<ArcCsc> r_arccsc;

/** Null constructor */
ArcCsc::ArcCsc() : Implements<ArcCsc,SingleFunc>()
{}

/** Construct cos(expression) */
ArcCsc::ArcCsc(const Expression &expression) : Implements<ArcCsc,SingleFunc>(expression)
{}

/** Copy constructor */
ArcCsc::ArcCsc(const ArcCsc &other) : Implements<ArcCsc,SingleFunc>(other)
{}

/** Destructor */
ArcCsc::~ArcCsc()
{}

ArcCsc& ArcCsc::operator=(const ArcCsc &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcCsc::operator==(const ArcCsc &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcCsc::operator!=(const ArcCsc &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcCsc::stream(Stream &s)
{
    s.assertVersion<ArcCsc>(1);
    
    Schema schema = s.item<ArcCsc>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcCsc::evaluate(const Values &values) const
{
    Complex val = SireMaths::arccsc_real( x().evaluate(values) );

    if (not val.isReal())
        throw SireMaths::domain_error(QObject::tr(
            "arccsc(%1) is complex (%2): %3")
                .arg(x().evaluate(values)).arg(val.toString(), toString()), CODELOC);

    return val.real();
}

/** Complex evaluation */
Complex ArcCsc::evaluate(const ComplexValues &values) const
{
    return SireMaths::arccsc( x().evaluate(values) );
}

Expression ArcCsc::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcCsc(arg);
}

QString ArcCsc::stringRep() const
{
    return QObject::tr("arccsc");
}

/** The differential of acsc(x) = -1 / (x^2 sqrt(1 - x^-2)) */
Expression ArcCsc::diff() const
{
    return -1 / ( SireCAS::pow(x(),2) * sqrt( 1 - SireCAS::pow(x(),-2) ) );
}

/** Integral of acsc(x) = x acsc(x) + Ln[ x ( 1 + sqrt( (x^2-1)/x^2 ) ) ] */
Expression ArcCsc::integ() const
{
    return x()*ArcCsc(x()) + 
           Ln( x() * ( 1 + sqrt( (SireCAS::pow(x(),2)-1) / SireCAS::pow(x(),2) ) ) );
}

////////////
//////////// Implementation of inverse-secant
////////////

static const RegisterObject<ArcSec> r_arcsec;

/** Null constructor */
ArcSec::ArcSec() : Implements<ArcSec,SingleFunc>()
{}

/** Construct cos(expression) */
ArcSec::ArcSec(const Expression &expression) : Implements<ArcSec,SingleFunc>(expression)
{}

/** Copy constructor */
ArcSec::ArcSec(const ArcSec &other) : Implements<ArcSec,SingleFunc>(other)
{}

/** Destructor */
ArcSec::~ArcSec()
{}

ArcSec& ArcSec::operator=(const ArcSec &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcSec::operator==(const ArcSec &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcSec::operator!=(const ArcSec &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcSec::stream(Stream &s)
{
    s.assertVersion<ArcSec>(1);
    
    Schema schema = s.item<ArcSec>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcSec::evaluate(const Values &values) const
{
    Complex val = SireMaths::arcsec_real( x().evaluate(values) );

    if (not val.isReal())
        throw SireMaths::domain_error(QObject::tr(
            "arcsec(%1) is complex (%2): %3")
                .arg(x().evaluate(values)).arg(val.toString(), toString()), CODELOC);

    return val.real();
}

/** Complex evaluation */
Complex ArcSec::evaluate(const ComplexValues &values) const
{
    return SireMaths::arcsec( x().evaluate(values) );
}

Expression ArcSec::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcSec(arg);
}

QString ArcSec::stringRep() const
{
    return QObject::tr("arcsec");
}

/** The differential of asec(x) = 1 / ( x^2 sqrt( 1 - x^-2 ) ) */
Expression ArcSec::diff() const
{
    return ArcSec(x()) * ArcTan(x());
}

/** Integral of asec(x) = x asec(x) - ln( x ( 1 + sqrt( (x^2-1)/x^2 ) ) ) */
Expression ArcSec::integ() const
{
    return x()*ArcSec(x()) - 
           Ln( x() * ( 1 + sqrt( (SireCAS::pow(x(),2) - 1) / SireCAS::pow(x(),2) ) ) );
}

////////////
//////////// Implementation of Inverse-cotangent
////////////

static const RegisterObject<ArcCot> r_arccot;

/** Null constructor */
ArcCot::ArcCot() : Implements<ArcCot,SingleFunc>()
{}

/** Construct cos(expression) */
ArcCot::ArcCot(const Expression &expression) : Implements<ArcCot,SingleFunc>(expression)
{}

/** Copy constructor */
ArcCot::ArcCot(const ArcCot &other) : Implements<ArcCot,SingleFunc>(other)
{}

/** Destructor */
ArcCot::~ArcCot()
{}

ArcCot& ArcCot::operator=(const ArcCot &other)
{
    SingleFunc::operator=(other);
    return *this;
}

bool ArcCot::operator==(const ArcCot &other) const
{
    return SingleFunc::operator==(other);
}

bool ArcCot::operator!=(const ArcCot &other) const
{
    return SingleFunc::operator!=(other);
}

void ArcCot::stream(Stream &s)
{
    s.assertVersion<ArcCot>(1);
    
    Schema schema = s.item<ArcCot>();
    
    SingleFunc::stream( schema.base() );
}

/** Evaluate this function */
double ArcCot::evaluate(const Values &values) const
{
    return std::atan( 1.0 / x().evaluate(values) );
}

/** Complex evaluation */
Complex ArcCot::evaluate(const ComplexValues &values) const
{
    return SireMaths::arccot( x().evaluate(values) );
}

Expression ArcCot::functionOf(const Expression &arg) const
{
    if (arg == argument())
        return *this;
    else
        return ArcCot(arg);
}

QString ArcCot::stringRep() const
{
    return QObject::tr("arccot");
}

/** The differential of acot(x) = -1 / (1+x^2) */
Expression ArcCot::diff() const
{
    return -1 / (1 + SireCAS::pow(x(),2));
}

/** Integral of acot(x) = x acot(x) + 0.5*Ln(1+x^2) */
Expression ArcCot::integ() const
{
    return x()*ArcCot(x()) + 0.5 * Ln( 1 + SireCAS::pow(x(),2) );
}

