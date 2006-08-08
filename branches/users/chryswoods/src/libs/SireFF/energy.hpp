#ifndef SIREFF_ENERGY_HPP
#define SIREFF_ENERGY_HPP

#include "energy.h"

#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
typedef quint32 EnergyID;

template<EnergyID T>
class EnergyT;
}

template<SireFF::EnergyID T>
QDataStream& operator<<(QDataStream&, const SireFF::EnergyT<T>&);
template<SireFF::EnergyID T>
QDataStream& operator>>(QDataStream&, SireFF::EnergyT<T>&);

namespace SireFF
{

template<EnergyID T>
class EnergyT : public EnergyBase
{

friend QDataStream& ::operator<<<>(QDataStream&, const EnergyT<T>&);
friend QDataStream& ::operator>><>(QDataStream&, EnergyT<T>&);

public:

    static const EnergyID TypeID;

    EnergyT();
    EnergyT(const double &nrg);
    EnergyT(const EnergyT<T> &other);
    
    virtual ~EnergyT();

    QString toString() const;

    EnergyT<T>& operator=(const EnergyT<T> &other);
    EnergyT<T>& operator+=(const EnergyT<T> &other);
    EnergyT<T>& operator-=(const EnergyT<T> &other);
    EnergyT<T>& operator*=(const EnergyT<T> &other);
    EnergyT<T>& operator/=(const EnergyT<T> &other);
    EnergyT<T> operator-() const;
    
    bool operator==(const EnergyT<T> &other) const;
    bool operator!=(const EnergyT<T> &other) const;

};

/** Overload the general operator with a specific operator for Energy 
    components of the same type. */
template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T> operator+(const EnergyT<T> &nrg0, const EnergyT<T> &nrg1)
{
    return EnergyT<T>( nrg0.value() + nrg1.value() );
}

/** Overload the general operator with a specific operator for Energy 
    components of the same type. */
template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T> operator-(const EnergyT<T> &nrg0, const EnergyT<T> &nrg1)
{
    return EnergyT<T>( nrg0.value() - nrg1.value() );
}

/** Overload the general operator with a specific operator for Energy 
    components of the same type. */
template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T> operator*(const EnergyT<T> &nrg0, const EnergyT<T> &nrg1)
{
    return EnergyT<T>( nrg0.value() * nrg1.value() );
}

/** Overload the general operator with a specific operator for Energy 
    components of the same type. */
template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T> operator/(const EnergyT<T> &nrg0, const EnergyT<T> &nrg1)
{
    return EnergyT<T>( nrg0.value() / nrg1.value() );
}

/** We need to provide a specific overload for double so that the compiler makes the
    right choice! */
template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T> operator*(const EnergyT<T> &nrg, const double &scl)
{
    return EnergyT<T>(nrg.value() * scl);
}

/** We need to provide a specific overload for double so that the compiler makes the
    right choice! */
template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T> operator*(const double &scl, const EnergyT<T> &nrg)
{
    return EnergyT<T>(nrg.value() * scl);
}

/** We need to provide a specific overload for double so that the compiler makes the
    right choice! */
template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T> operator/(const EnergyT<T> &nrg, const double &scl)
{
    return EnergyT<T>(nrg.value() / scl);
}

/** Default function to return the name of the component 'T' */
template<EnergyID T>
SIRE_INLINE_TEMPLATE
QString getName(const EnergyT<T>&)
{
    return "Unnamed";
}

template<EnergyID T>
const EnergyID EnergyT<T>::TypeID(T);

template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T>::EnergyT() : EnergyBase()
{}

template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T>::EnergyT(const double &nrg) : EnergyBase(nrg)
{}

template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T>::EnergyT(const EnergyT<T> &other) : EnergyBase(other)
{}
    
template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T>::~EnergyT()
{}

template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T>& EnergyT<T>::operator=(const EnergyT<T> &other)
{
    nrg = other.nrg;
    return *this;
}

template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T>& EnergyT<T>::operator+=(const EnergyT<T> &other)
{
    nrg += other.nrg;
    return *this;
}

template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T>& EnergyT<T>::operator-=(const EnergyT<T> &other)
{
    nrg -= other.nrg;
    return *this;
}

template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T>& EnergyT<T>::operator*=(const EnergyT<T> &other)
{
    nrg *= other.nrg;
    return *this;
}

template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T>& EnergyT<T>::operator/=(const EnergyT<T> &other)
{
    nrg /= other.nrg;
    return *this;
}

template<EnergyID T>
SIRE_INLINE_TEMPLATE
EnergyT<T> EnergyT<T>::operator-() const
{
    return EnergyT<T>(-nrg);
}

template<EnergyID T>
SIRE_INLINE_TEMPLATE
bool EnergyT<T>::operator==(const EnergyT<T> &other) const
{
    return SireMaths::areEqual(nrg,other.nrg);
}

template<EnergyID T>
SIRE_INLINE_TEMPLATE
bool EnergyT<T>::operator!=(const EnergyT<T> &other) const
{
    return not this->operator==(other);
}

/** Return a string representation of this component */
template<EnergyID T>
SIRE_INLINE_TEMPLATE
QString EnergyT<T>::toString() const
{
    return QString("%1: %2").arg( getName(*this), EnergyBase::toString() );
}

}

/** Streaming operator for energy component T */
template<SireFF::EnergyID T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireFF::EnergyT<T> &energy)
{
    ds << energy.nrg;
    return ds;
}

/** Streaming operator for energy component T */
template<SireFF::EnergyID T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireFF::EnergyT<T> &energy)
{
    ds >> energy.nrg;
    return ds;
}

/** Null streaming operator for the null energy (type 0) */
template<>
SIRE_OUTOFLINE_TEMPLATE
inline QDataStream& operator<<(QDataStream &ds, const SireFF::EnergyT<0>&)
{
    return ds;
}

/** Null streaming operator for the null energy (type 0) */
template<>
SIRE_OUTOFLINE_TEMPLATE
inline QDataStream& operator>>(QDataStream &ds, SireFF::EnergyT<0>&)
{
    return ds;
}

SIRE_END_HEADER

#endif
