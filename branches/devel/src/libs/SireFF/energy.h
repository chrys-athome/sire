#ifndef SIREFF_ENERGY_H
#define SIREFF_ENERGY_H

#include <QString>
#include <QDataStream>

#include "SireMaths/maths.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class Energy;
}

QDataStream& operator<<(QDataStream&, const SireFF::Energy&);
QDataStream& operator>>(QDataStream&, SireFF::Energy&);

namespace SireFF
{

class SIREFF_EXPORT EnergyBase
{
public:
    EnergyBase();
    EnergyBase(const double &nrg);
    EnergyBase(const EnergyBase &other);

    ~EnergyBase();

    QString toString() const;
       
    double value() const;
        
protected:

    /** The actual stored energy */
    double nrg;
};

/** Construct a zero energy */
inline EnergyBase::EnergyBase() : nrg(0.0)
{}

/** Construct an energy of 'nrg kcal mol-1' */
inline EnergyBase::EnergyBase(const double &n) : nrg(n)
{}

/** Copy constructor */
inline EnergyBase::EnergyBase(const EnergyBase &other) : nrg(other.nrg)
{}

/** Destructor */
inline EnergyBase::~EnergyBase()
{}

/** Return the energy in kcal mol-1 */
inline double EnergyBase::value() const
{
    return nrg;
}

/** 
An 'Energy' represents (shock-horror) an energy value. Three energy values are held; the energy in the reference state, and the same energy in the two perturbed states ('forwards' and 'backwards').

This Energy class is special in that it is returned whenever two different energy components
are added together, and functions exist to convert any component energy into this general
purpose Energy class. 
 
@author Christopher Woods
*/
class SIREFF_EXPORT Energy : public EnergyBase
{

friend QDataStream& ::operator<<(QDataStream&, const Energy&);
friend QDataStream& ::operator>>(QDataStream&, Energy&);

public:

    Energy();
    Energy(const double &nrg);
    Energy(const EnergyBase &other);

    ~Energy();

    Energy operator-() const;
    
    Energy& operator=(const EnergyBase &other);
    Energy& operator+=(const EnergyBase &other);
    Energy& operator-=(const EnergyBase &other);
    Energy& operator*=(const EnergyBase &other);
    Energy& operator/=(const EnergyBase &other);
};

/** General addition operator used to add together different energy component types. */
inline Energy operator+(const EnergyBase &nrg0, const EnergyBase &nrg1)
{
    return Energy(nrg0.value() + nrg1.value());
}

/** General subtraction operator used to subtract different energy component types. */
inline Energy operator-(const EnergyBase &nrg0, const EnergyBase &nrg1)
{
    return Energy(nrg0.value()-nrg1.value());
}

/** General multiplication operator used to multiple different energies together */
inline Energy operator*(const EnergyBase &nrg0, const EnergyBase &nrg1)
{
    return Energy(nrg0.value()*nrg1.value());
}

/** General division operator used to divide different energies */
inline Energy operator/(const EnergyBase &nrg0, const EnergyBase &nrg1)
{
    return Energy(nrg0.value()/nrg1.value());
}

inline double operator/(const double &val, const EnergyBase &nrg)
{
    return val / nrg.value();
}

inline Energy Energy::operator-() const
{
    return Energy( -nrg );
}
    
inline Energy& Energy::operator=(const EnergyBase &other)
{
    nrg = other.value();
    return *this;
}

inline Energy& Energy::operator+=(const EnergyBase &other)
{
    nrg += other.value();
    return *this;
}
    
inline Energy& Energy::operator-=(const EnergyBase &other)
{
    nrg -= other.value();
    return *this;
}
    
inline Energy& Energy::operator*=(const EnergyBase &other)
{
    nrg *= other.value();
    return *this;
}
    
inline Energy& Energy::operator/=(const EnergyBase &other)
{
    nrg /= other.value();
    return *this;
}
    
/** Compare one energy with another of any type. Only the general Energy class
    may be compared with any other Energy component (EnergyT or EnergyPack) class.
    The reason for this is that it would be wrong to compare CoulombEnergy with 
    LJEnergy, as they are of different types. Things are more complex for 
    the EnergyPack class, as it should not be compared to another EnergyPack
    that has different components (EnergyPacks are only equal if they have the
    same components, and the values of the components are all the same) */
inline bool operator==(const Energy &nrg0, const EnergyBase &nrg1)
{
    return SireMaths::areEqual(nrg0.value(), nrg1.value());
}

/** Compare one energy with another of any type. */
inline bool operator==(const EnergyBase &nrg0, const Energy &nrg1)
{
    return (nrg1 == nrg0);
}

/** Compare two energy values - this is used to remove the ambiguity of the overload */
inline bool operator==(const Energy &nrg0, const Energy &nrg1)
{
    return static_cast<const EnergyBase&>(nrg0) == nrg1;
}    

/** Compare one energy with another of any type. */
inline bool operator!=(const Energy &nrg0, const EnergyBase &nrg1)
{
    return not (nrg0 == nrg1);
}

/** Compare two energy values - this is used to remove the ambiguity of the overload */
inline bool operator!=(const Energy &nrg0, const Energy &nrg1)
{
    return not (nrg0 == nrg1);
}    

/** Compare one energy with another of any type. */
inline bool operator!=(const EnergyBase &nrg0, const Energy &nrg1)
{
    return not (nrg0 == nrg1);
}

/** Test if one energy is less than another. */
inline bool operator<(const EnergyBase &nrg0, const EnergyBase &nrg1)
{
    return nrg0.value() < nrg1.value();
}

/** Test if one energy is greater than another. */
inline bool operator>(const EnergyBase &nrg0, const EnergyBase &nrg1)
{
    return nrg0.value() > nrg1.value();
}

}

Q_DECLARE_TYPEINFO(SireFF::Energy, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireFF::Energy)

SIRE_END_HEADER

#endif
