#ifndef SIREFF_ENERGYMATHS_HPP
#define SIREFF_ENERGYMATHS_HPP

#include "SireMaths/maths.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

/** This file contains the collection of helper functions and classes that are necessary
    to define the Energy classes as proper arithmetic classes, on which can have
    their limits queried, and which can be passed to some standard functions 
    (e.g. sqrt and abs) */

namespace std
{

/** Operators for the dummy, EnergyT<0> class */
inline SireFF::EnergyT<0> sqrt(const SireFF::EnergyT<0>&)
{
    return SireFF::EnergyT<0>();
}

/** Operators for the dummy, EnergyT<0> class */
inline SireFF::EnergyT<0> abs(const SireFF::EnergyT<0>&)
{
    return SireFF::EnergyT<0>();
}

/** The square root operator for any EnergyT type */
template<SireFF::EnergyID T>
SIRE_INLINE_TEMPLATE
SireFF::EnergyT<T> sqrt(const SireFF::EnergyT<T> &nrg)
{
    return SireFF::EnergyT<T>( std::sqrt(nrg.value()) );
}

/** The square root operator for the Energy type */
inline SireFF::Energy sqrt(const SireFF::Energy &nrg)
{
    return SireFF::Energy( std::sqrt(nrg.value()) );
}

/** The abs operator for any EnergyT type */
template<SireFF::EnergyID T>
SIRE_INLINE_TEMPLATE
SireFF::EnergyT<T> abs(const SireFF::EnergyT<T> &nrg)
{
    return SireFF::EnergyT<T>( std::abs(nrg.value()) );
}

/** The abs operator for the Energy type */
inline SireFF::Energy abs(const SireFF::Energy &nrg)
{
    return SireFF::Energy( std::abs(nrg.value()) );
}

}

namespace SireFF
{

template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_OUTOFLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::sqrt() const
{
    return EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>( std::sqrt(t1),
                                                   std::sqrt(t2),
                                                   std::sqrt(t3),
                                                   std::sqrt(t4),
                                                   std::sqrt(t5),
                                                   std::sqrt(t6),
                                                   std::sqrt(t7),
                                                   std::sqrt(t8),
                                                   std::sqrt(t9) );
}

template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_OUTOFLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::abs() const
{
    return EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>( std::abs(t1),
                                                   std::abs(t2),
                                                   std::abs(t3),
                                                   std::abs(t4),
                                                   std::abs(t5),
                                                   std::abs(t6),
                                                   std::abs(t7),
                                                   std::abs(t8),
                                                   std::abs(t9) );
}
}

namespace std
{

/** The square root operator for the EnergyPack types */
template<SireFF::EnergyID T1, SireFF::EnergyID T2, SireFF::EnergyID T3, 
         SireFF::EnergyID T4, SireFF::EnergyID T5, SireFF::EnergyID T6, 
         SireFF::EnergyID T7, SireFF::EnergyID T8, SireFF::EnergyID T9>
SIRE_INLINE_TEMPLATE
SireFF::EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> sqrt(
                            const SireFF::EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg)
{
    return nrg.sqrt();
}

/** The abs operator for the EnergyPack types */
template<SireFF::EnergyID T1, SireFF::EnergyID T2, SireFF::EnergyID T3, 
         SireFF::EnergyID T4, SireFF::EnergyID T5, SireFF::EnergyID T6, 
         SireFF::EnergyID T7, SireFF::EnergyID T8, SireFF::EnergyID T9>
SIRE_INLINE_TEMPLATE
SireFF::EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> abs(
                            const SireFF::EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg)
{
    return nrg.abs();
}

}

SIRE_END_HEADER

#endif
