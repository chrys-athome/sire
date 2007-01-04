
#include "hyperbolicfuncs.h"
#include "trigfuncs.h"
#include "exp.h"
#include "identities.h"
#include "registerexpression.h"
#include "expression.h"
#include "complexvalues.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireCAS;

////////////
//////////// Register the trig functions
////////////

static const RegisterMetaType<Cosh> r_cosh;
static const RegisterMetaType<Sinh> r_sinh;
static const RegisterMetaType<Tanh> r_tanh;
static const RegisterMetaType<Csch> r_csch;
static const RegisterMetaType<Sech> r_sech;
static const RegisterMetaType<Coth> r_coth;

////////////
//////////// Implementation of Hyperbolic Cosine
////////////

/** Null constructor */
Cosh::Cosh() : SingleFunc()
{}

/** Construct cos(expression) */
Cosh::Cosh(const Expression &expression) : SingleFunc(expression)
{}

/** Create cos(cos(expression)) */
Cosh::Cosh(const Cosh &other) : SingleFunc(other)
{}

/** Destructor */
Cosh::~Cosh()
{}

/** Return the magic */
uint Cosh::magic() const
{
    return r_cosh.magicID();
}

/** Comparison operator */
bool Cosh::operator==(const ExBase &other) const
{
    const Cosh *other_cos = dynamic_cast<const Cosh*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<Cosh> RegisterCosh;

////////////
//////////// Implementation of hyperbolic sine
////////////

/** Null constructor */
Sinh::Sinh() : SingleFunc()
{}

/** Construct cos(expression) */
Sinh::Sinh(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
Sinh::Sinh(const Sinh &other) : SingleFunc(other)
{}

/** Destructor */
Sinh::~Sinh()
{}

/** Return the magic */
uint Sinh::magic() const
{
    return r_sinh.magicID();
}

/** Comparison operator */
bool Sinh::operator==(const ExBase &other) const
{
    const Sinh *other_cos = dynamic_cast<const Sinh*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<Sinh> RegisterSinh;

////////////
//////////// Implementation of hyperbolic tangent
////////////

/** Null constructor */
Tanh::Tanh() : SingleFunc()
{}

/** Construct cos(expression) */
Tanh::Tanh(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
Tanh::Tanh(const Tanh &other) : SingleFunc(other)
{}

/** Destructor */
Tanh::~Tanh()
{}

/** Return the magic */
uint Tanh::magic() const
{
    return r_tanh.magicID();
}

/** Comparison operator */
bool Tanh::operator==(const ExBase &other) const
{
    const Tanh *other_cos = dynamic_cast<const Tanh*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<Tanh> RegisterTanh;

////////////
//////////// Implementation of hyperbolic cosecant
////////////

/** Null constructor */
Csch::Csch() : SingleFunc()
{}

/** Construct cos(expression) */
Csch::Csch(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
Csch::Csch(const Csch &other) : SingleFunc(other)
{}

/** Destructor */
Csch::~Csch()
{}

/** Return the magic */
uint Csch::magic() const
{
    return r_csch.magicID();
}

/** Comparison operator */
bool Csch::operator==(const ExBase &other) const
{
    const Csch *other_cos = dynamic_cast<const Csch*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<Csch> RegisterCsch;

////////////
//////////// Implementation of hyperbolic secant
////////////

/** Null constructor */
Sech::Sech() : SingleFunc()
{}

/** Construct cos(expression) */
Sech::Sech(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
Sech::Sech(const Sech &other) : SingleFunc(other)
{}

/** Destructor */
Sech::~Sech()
{}

/** Return the magic */
uint Sech::magic() const
{
    return r_sech.magicID();
}

/** Comparison operator */
bool Sech::operator==(const ExBase &other) const
{
    const Sech *other_cos = dynamic_cast<const Sech*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<Sech> RegisterSech;

////////////
//////////// Implementation of hyperbolic cotangent
////////////

/** Null constructor */
Coth::Coth() : SingleFunc()
{}

/** Construct cos(expression) */
Coth::Coth(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
Coth::Coth(const Coth &other) : SingleFunc(other)
{}

/** Destructor */
Coth::~Coth()
{}

/** Return the magic */
uint Coth::magic() const
{
    return r_coth.magicID();
}

/** Comparison operator */
bool Coth::operator==(const ExBase &other) const
{
    const Coth *other_cos = dynamic_cast<const Coth*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

static RegisterExpression<Coth> RegisterCoth;
