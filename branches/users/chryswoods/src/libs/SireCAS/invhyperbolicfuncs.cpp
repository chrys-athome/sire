
#include "trigfuncs.h"
#include "invtrigfuncs.h"
#include "hyperbolicfuncs.h"
#include "invhyperbolicfuncs.h"
#include "exp.h"
#include "identities.h"
#include "registerexpression.h"
#include "expression.h"
#include "complexvalues.h"

#include "SireMaths/errors.h"

#include "SireStream/datastream.h"

#ifndef HAVE_ASINH
#define asinh gsl_asinh
#endif

using namespace SireCAS;
using namespace SireStream;

////////////
//////////// Register the functions
////////////

static const RegisterMetaType<ArcCosh> r_arccosh;
static const RegisterMetaType<ArcSinh> r_arcsinh;
static const RegisterMetaType<ArcTanh> r_arctanh;
static const RegisterMetaType<ArcCsch> r_arccsch;
static const RegisterMetaType<ArcSech> r_arcsech;
static const RegisterMetaType<ArcCoth> r_arccoth;

////////////
//////////// Implementation of Inverse-hyperbolic-cosine
////////////

/** Null constructor */
ArcCosh::ArcCosh() : SingleFunc()
{}

/** Construct cos(expression) */
ArcCosh::ArcCosh(const Expression &expression) : SingleFunc(expression)
{}

/** Create cos(cos(expression)) */
ArcCosh::ArcCosh(const ArcCosh &other) : SingleFunc(other)
{}

/** Destructor */
ArcCosh::~ArcCosh()
{}

/** Return the magic */
uint ArcCosh::magic() const
{
    return r_arccosh.magicID();
}

/** Comparison operator */
bool ArcCosh::operator==(const ExBase &other) const
{
    const ArcCosh *other_cos = dynamic_cast<const ArcCosh*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<ArcCosh> RegisterArcCosh;

////////////
//////////// Implementation of Inverse-hyperbolic-sine
////////////

/** Null constructor */
ArcSinh::ArcSinh() : SingleFunc()
{}

/** Construct cos(expression) */
ArcSinh::ArcSinh(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
ArcSinh::ArcSinh(const ArcSinh &other) : SingleFunc(other)
{}

/** Destructor */
ArcSinh::~ArcSinh()
{}

/** Return the magic */
uint ArcSinh::magic() const
{
    return r_arcsinh.magicID();
}

/** Comparison operator */
bool ArcSinh::operator==(const ExBase &other) const
{
    const ArcSinh *other_cos = dynamic_cast<const ArcSinh*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<ArcSinh> RegisterArcSinh;

////////////
//////////// Implementation of Inverse-hyperbolic-tangent
////////////

/** Null constructor */
ArcTanh::ArcTanh() : SingleFunc()
{}

/** Construct cos(expression) */
ArcTanh::ArcTanh(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
ArcTanh::ArcTanh(const ArcTanh &other) : SingleFunc(other)
{}

/** Destructor */
ArcTanh::~ArcTanh()
{}

/** Return the magic */
uint ArcTanh::magic() const
{
    return r_arctanh.magicID();
}

/** Comparison operator */
bool ArcTanh::operator==(const ExBase &other) const
{
    const ArcTanh *other_cos = dynamic_cast<const ArcTanh*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<ArcTanh> RegisterArcTanh;

////////////
//////////// Implementation of Inverse-hyperbolic-cosecant
////////////

/** Null constructor */
ArcCsch::ArcCsch() : SingleFunc()
{}

/** Construct cos(expression) */
ArcCsch::ArcCsch(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
ArcCsch::ArcCsch(const ArcCsch &other) : SingleFunc(other)
{}

/** Destructor */
ArcCsch::~ArcCsch()
{}

/** Return the magic */
uint ArcCsch::magic() const
{
    return r_arccsch.magicID();
}

/** Comparison operator */
bool ArcCsch::operator==(const ExBase &other) const
{
    const ArcCsch *other_cos = dynamic_cast<const ArcCsch*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<ArcCsch> RegisterArcCsch;

////////////
//////////// Implementation of inverse-secant
////////////

/** Null constructor */
ArcSech::ArcSech() : SingleFunc()
{}

/** Construct cos(expression) */
ArcSech::ArcSech(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
ArcSech::ArcSech(const ArcSech &other) : SingleFunc(other)
{}

/** Destructor */
ArcSech::~ArcSech()
{}

/** Return the magic */
uint ArcSech::magic() const
{
    return r_arcsech.magicID();
}

/** Comparison operator */
bool ArcSech::operator==(const ExBase &other) const
{
    const ArcSech *other_cos = dynamic_cast<const ArcSech*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<ArcSech> RegisterArcSech;

////////////
//////////// Implementation of Inverse-hyperbolic-cotangent
////////////

/** Null constructor */
ArcCoth::ArcCoth() : SingleFunc()
{}

/** Construct cos(expression) */
ArcCoth::ArcCoth(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
ArcCoth::ArcCoth(const ArcCoth &other) : SingleFunc(other)
{}

/** Destructor */
ArcCoth::~ArcCoth()
{}

/** Return the magic */
uint ArcCoth::magic() const
{
    return r_arccoth.magicID();
}

/** Comparison operator */
bool ArcCoth::operator==(const ExBase &other) const
{
    const ArcCoth *other_cos = dynamic_cast<const ArcCoth*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<ArcCoth> RegisterArcCoth;
