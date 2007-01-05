
#include "trigfuncs.h"
#include "exp.h"
#include "identities.h"
#include "expression.h"
#include "complexvalues.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireCAS;

////////////
//////////// Register the trig functions
////////////

static const RegisterMetaType<Cos> r_cos;
static const RegisterMetaType<Sin> r_sin;
static const RegisterMetaType<Tan> r_tan;
static const RegisterMetaType<Csc> r_csc;
static const RegisterMetaType<Sec> r_sec;
static const RegisterMetaType<Cot> r_cot;

////////////
//////////// Stream the trig functions
////////////

/** Serialise to a binary datastream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const Cos &cos)
{
    writeHeader(ds, r_cos, 1) << static_cast<const SingleFunc&>(cos);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, Cos &cos)
{
    VersionID v = readHeader(ds, r_cos);

    if (v == 1)
    {
        ds >> static_cast<SingleFunc&>(cos);
    }
    else
        throw version_error(v, "1", r_cos, CODELOC);

    return ds;
}

/** Serialise to a binary datastream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const Sin &sin)
{
    writeHeader(ds, r_sin, 1) << static_cast<const SingleFunc&>(sin);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, Sin &sin)
{
    VersionID v = readHeader(ds, r_sin);

    if (v == 1)
    {
        ds >> static_cast<SingleFunc&>(sin);
    }
    else
        throw version_error(v, "1", r_sin, CODELOC);

    return ds;
}

/** Serialise to a binary datastream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const Tan &tan)
{
    writeHeader(ds, r_tan, 1) << static_cast<const SingleFunc&>(tan);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, Tan &tan)
{
    VersionID v = readHeader(ds, r_tan);

    if (v == 1)
    {
        ds >> static_cast<SingleFunc&>(tan);
    }
    else
        throw version_error(v, "1", r_tan, CODELOC);

    return ds;
}

/** Serialise to a binary datastream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const Csc &csc)
{
    writeHeader(ds, r_csc, 1) << static_cast<const SingleFunc&>(csc);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, Csc &csc)
{
    VersionID v = readHeader(ds, r_csc);

    if (v == 1)
    {
        ds >> static_cast<SingleFunc&>(csc);
    }
    else
        throw version_error(v, "1", r_csc, CODELOC);

    return ds;
}

/** Serialise to a binary datastream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const Sec &sec)
{
    writeHeader(ds, r_sec, 1) << static_cast<const SingleFunc&>(sec);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, Sec &sec)
{
    VersionID v = readHeader(ds, r_sec);

    if (v == 1)
    {
        ds >> static_cast<SingleFunc&>(sec);
    }
    else
        throw version_error(v, "1", r_sec, CODELOC);

    return ds;
}

/** Serialise to a binary datastream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const Cot &cot)
{
    writeHeader(ds, r_cot, 1) << static_cast<const SingleFunc&>(cot);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, Cot &cot)
{
    VersionID v = readHeader(ds, r_cot);

    if (v == 1)
    {
        ds >> static_cast<SingleFunc&>(cot);
    }
    else
        throw version_error(v, "1", r_cot, CODELOC);

    return ds;
}

////////////
//////////// Implementation of Cosine
////////////

/** Null constructor */
Cos::Cos() : SingleFunc()
{}

/** Construct cos(expression) */
Cos::Cos(const Expression &expression) : SingleFunc(expression)
{}

/** Create cos(cos(expression)) */
Cos::Cos(const Cos &other) : SingleFunc(other)
{}

/** Destructor */
Cos::~Cos()
{}

/** Return the magic */
uint Cos::magic() const
{
    return r_cos.magicID();
}

/** Comparison operator */
bool Cos::operator==(const ExBase &other) const
{
    const Cos *other_cos = dynamic_cast<const Cos*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** Null constructor */
Sin::Sin() : SingleFunc()
{}

/** Construct cos(expression) */
Sin::Sin(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
Sin::Sin(const Sin &other) : SingleFunc(other)
{}

/** Destructor */
Sin::~Sin()
{}

/** Return the magic */
uint Sin::magic() const
{
    return r_sin.magicID();
}

/** Comparison operator */
bool Sin::operator==(const ExBase &other) const
{
    const Sin *other_cos = dynamic_cast<const Sin*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** Null constructor */
Tan::Tan() : SingleFunc()
{}

/** Construct cos(expression) */
Tan::Tan(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
Tan::Tan(const Tan &other) : SingleFunc(other)
{}

/** Destructor */
Tan::~Tan()
{}

/** Return the magic */
uint Tan::magic() const
{
    return r_tan.magicID();
}

/** Comparison operator */
bool Tan::operator==(const ExBase &other) const
{
    const Tan *other_cos = dynamic_cast<const Tan*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** Null constructor */
Csc::Csc() : SingleFunc()
{}

/** Construct cos(expression) */
Csc::Csc(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
Csc::Csc(const Csc &other) : SingleFunc(other)
{}

/** Destructor */
Csc::~Csc()
{}

/** Return the magic */
uint Csc::magic() const
{
    return r_csc.magicID();
}

/** Comparison operator */
bool Csc::operator==(const ExBase &other) const
{
    const Csc *other_cos = dynamic_cast<const Csc*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** Null constructor */
Sec::Sec() : SingleFunc()
{}

/** Construct cos(expression) */
Sec::Sec(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
Sec::Sec(const Sec &other) : SingleFunc(other)
{}

/** Destructor */
Sec::~Sec()
{}

/** Return the magic */
uint Sec::magic() const
{
    return r_sec.magicID();
}

/** Comparison operator */
bool Sec::operator==(const ExBase &other) const
{
    const Sec *other_cos = dynamic_cast<const Sec*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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

/** Null constructor */
Cot::Cot() : SingleFunc()
{}

/** Construct cos(expression) */
Cot::Cot(const Expression &expression) : SingleFunc(expression)
{}

/** Copy constructor */
Cot::Cot(const Cot &other) : SingleFunc(other)
{}

/** Destructor */
Cot::~Cot()
{}

/** Return the magic */
uint Cot::magic() const
{
    return r_cot.magicID();
}

/** Comparison operator */
bool Cot::operator==(const ExBase &other) const
{
    const Cot *other_cos = dynamic_cast<const Cot*>(&other);

    return other_cos != 0 and typeid(other).name() == typeid(*this).name()
                 and this->argument() == other_cos->argument();
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
