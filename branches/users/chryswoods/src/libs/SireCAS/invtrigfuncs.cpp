
#include "trigfuncs.h"
#include "invtrigfuncs.h"
#include "exp.h"
#include "identities.h"
#include "registerexpression.h"
#include "expression.h"
#include "complexvalues.h"

#include "SireMaths/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireCAS;

////////////
//////////// Register the functions
////////////

static const RegisterMetaType<ArcCos> r_arccos;
static const RegisterMetaType<ArcSin> r_arcsin;
static const RegisterMetaType<ArcTan> r_arctan;
static const RegisterMetaType<ArcCsc> r_arccsc;
static const RegisterMetaType<ArcSec> r_arcsec;
static const RegisterMetaType<ArcCot> r_arccot;

////////////
//////////// Implementation of Inverse-cosine
////////////

/** Null constructor */
ArcCos::ArcCos() : SingleFunc()
{}

/** Construct cos(expression) */
ArcCos::ArcCos(const Expression &expression) : SingleFunc(expression)
{}

/** Create cos(cos(expression)) */
ArcCos::ArcCos(const ArcCos &other) : SingleFunc(other)
{}

/** Destructor */
ArcCos::~ArcCos()
{}

/** Return the magic */
uint ArcCos::magic() const
{
    return r_arccos.magicID();
}

/** Comparison operator */
bool ArcCos::operator==(const ExBase &other) const
{
    const ArcCos *other_cos = dynamic_cast<const ArcCos*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** The differential of acos(x) = -1 / sqrt(1-x^2) */
Expression ArcCos::diff() const
{
    return -1 / sqrt( 1 - pow(x(),2) );
}

/** Integral of acos(x) = x acos(x) - sqrt(1-x^2) */
Expression ArcCos::integ() const
{
    return x()*ArcCos(x()) - sqrt(1 - pow(x(),2));
}

static RegisterExpression<ArcCos> RegisterArcCos;

////////////
//////////// Implementation of Inverse-sine
////////////

/** Null constructor */
ArcSin::ArcSin() : SingleFunc()
{}

/** Construct cos(expression) */
ArcSin::ArcSin(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
ArcSin::ArcSin(const ArcSin &other) : SingleFunc(other)
{}

/** Destructor */
ArcSin::~ArcSin()
{}

/** Return the magic */
uint ArcSin::magic() const
{
    return r_arcsin.magicID();
}

/** Comparison operator */
bool ArcSin::operator==(const ExBase &other) const
{
    const ArcSin *other_cos = dynamic_cast<const ArcSin*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** The differential of asin(x) = 1 / sqrt(1-x^2) */
Expression ArcSin::diff() const
{
    return 1 / sqrt( 1 - pow(x(),2) );
}

/** Integral of asin(x) = x asin(x) + sqrt(1-x^2) */
Expression ArcSin::integ() const
{
    return  x()*ArcSin(x()) + sqrt( 1 - pow(x(),2) );
}

static RegisterExpression<ArcSin> RegisterArcSin;

////////////
//////////// Implementation of Inverse-tangent
////////////

/** Null constructor */
ArcTan::ArcTan() : SingleFunc()
{}

/** Construct cos(expression) */
ArcTan::ArcTan(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
ArcTan::ArcTan(const ArcTan &other) : SingleFunc(other)
{}

/** Destructor */
ArcTan::~ArcTan()
{}

/** Return the magic */
uint ArcTan::magic() const
{
    return r_arctan.magicID();
}

/** Comparison operator */
bool ArcTan::operator==(const ExBase &other) const
{
    const ArcTan *other_cos = dynamic_cast<const ArcTan*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** The differential of atan(x) = 1 / (1+x^2) */
Expression ArcTan::diff() const
{
    return 1 / ( 1 + pow(x(),2) );
}

/** Integral of atan(x) = x atan(x) - 0.5 ln( 1 + x^2 ) */
Expression ArcTan::integ() const
{
    return x()*ArcTan(x()) - 0.5*Ln( 1 + pow(x(),2) );
}

static RegisterExpression<ArcTan> RegisterArcTan;

////////////
//////////// Implementation of Inverse-cosecant
////////////

/** Null constructor */
ArcCsc::ArcCsc() : SingleFunc()
{}

/** Construct cos(expression) */
ArcCsc::ArcCsc(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
ArcCsc::ArcCsc(const ArcCsc &other) : SingleFunc(other)
{}

/** Destructor */
ArcCsc::~ArcCsc()
{}

/** Return the magic */
uint ArcCsc::magic() const
{
    return r_arccsc.magicID();
}

/** Comparison operator */
bool ArcCsc::operator==(const ExBase &other) const
{
    const ArcCsc *other_cos = dynamic_cast<const ArcCsc*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** The differential of acsc(x) = -1 / (x^2 sqrt(1 - x^-2)) */
Expression ArcCsc::diff() const
{
    return -1 / ( pow(x(),2) * sqrt( 1 - pow(x(),-2) ) );
}

/** Integral of acsc(x) = x acsc(x) + Ln[ x ( 1 + sqrt( (x^2-1)/x^2 ) ) ] */
Expression ArcCsc::integ() const
{
    return x()*ArcCsc(x()) + Ln( x() * ( 1 + sqrt( (pow(x(),2)-1) / pow(x(),2) ) ) );
}

static RegisterExpression<ArcCsc> RegisterArcCsc;

////////////
//////////// Implementation of inverse-secant
////////////

/** Null constructor */
ArcSec::ArcSec() : SingleFunc()
{}

/** Construct cos(expression) */
ArcSec::ArcSec(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
ArcSec::ArcSec(const ArcSec &other) : SingleFunc(other)
{}

/** Destructor */
ArcSec::~ArcSec()
{}

/** Return the magic */
uint ArcSec::magic() const
{
    return r_arcsec.magicID();
}

/** Comparison operator */
bool ArcSec::operator==(const ExBase &other) const
{
    const ArcSec *other_cos = dynamic_cast<const ArcSec*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** The differential of asec(x) = 1 / ( x^2 sqrt( 1 - x^-2 ) ) */
Expression ArcSec::diff() const
{
    return ArcSec(x()) * ArcTan(x());
}

/** Integral of asec(x) = x asec(x) - ln( x ( 1 + sqrt( (x^2-1)/x^2 ) ) ) */
Expression ArcSec::integ() const
{
    return x()*ArcSec(x()) - Ln( x() * ( 1 + sqrt( (pow(x(),2) - 1) / pow(x(),2) ) ) );
}

static RegisterExpression<ArcSec> RegisterArcSec;

////////////
//////////// Implementation of Inverse-cotangent
////////////

/** Null constructor */
ArcCot::ArcCot() : SingleFunc()
{}

/** Construct cos(expression) */
ArcCot::ArcCot(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
ArcCot::ArcCot(const ArcCot &other) : SingleFunc(other)
{}

/** Destructor */
ArcCot::~ArcCot()
{}

/** Return the magic */
uint ArcCot::magic() const
{
    return r_arccot.magicID();
}

/** Comparison operator */
bool ArcCot::operator==(const ExBase &other) const
{
    const ArcCot *other_cos = dynamic_cast<const ArcCot*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** The differential of acot(x) = -1 / (1+x^2) */
Expression ArcCot::diff() const
{
    return -1 / (1 + pow(x(),2));
}

/** Integral of acot(x) = x acot(x) + 0.5*Ln(1+x^2) */
Expression ArcCot::integ() const
{
    return x()*ArcCot(x()) + 0.5 * Ln( 1 + pow(x(),2) );
}

static RegisterExpression<ArcCot> RegisterArcCot;
