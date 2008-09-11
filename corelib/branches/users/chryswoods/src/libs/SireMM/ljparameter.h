/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMM_LJPARAMETER_H
#define SIREMM_LJPARAMETER_H

#include <QString>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class LJParameter;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMM::LJParameter&);
QDataStream& operator>>(QDataStream&, SireMM::LJParameter&);

quint32 qHash(const SireMM::LJParameter&);

namespace SireMM
{

class LJPair;

/**
An LJParameter holds Lennard Jones parameters (sigma and epsilon)

@author Christopher Woods
*/
class SIREMM_EXPORT LJParameter
{

friend QDataStream& ::operator<<(QDataStream&, const LJParameter&);
friend QDataStream& ::operator>>(QDataStream&, LJParameter&);

public:
    LJParameter();
    LJParameter(double sigma, double epsilon);
    LJParameter(const LJPair &ljpair);
    
    LJParameter(const LJParameter &param);
    
    ~LJParameter();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<LJParameter>() );
    }
    
    const char* what() const
    {
        return LJParameter::typeName();
    }
    
    bool isDummy() const;
    bool zeroLJ() const;
    
    double sigma() const;
    double epsilon() const;
    double sqrtEpsilon() const;
    double sqrtSigma() const;

    double A() const;
    double B() const;

    double rmin() const;

    QString toString() const;

    bool operator==(const LJParameter &other) const;
    bool operator!=(const LJParameter &other) const;

    static LJParameter dummy();
    static LJParameter fromSigmaAndEpsilon(double sigma, double epsilon);
    static LJParameter fromAAndB(double a, double b);
    static LJParameter fromRMinAndEpsilon(double rmin, double epsilon);

private:
    /** Square-root of the sigma parameter, in sqrt(Angstroms) */
    double sqrtsig;
    
    /** Square-root of the epsilon parameter. The square-root
        is stored to improve the efficiency of combining rules.
        The units are sqrt(kcal mol-1) */
    double sqrteps;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Return whether or not two LJParameters are equal */
inline bool LJParameter::operator==(const LJParameter &other) const
{
    return sqrtsig == other.sqrtsig and sqrteps == other.sqrteps;
}

/** Return whether or not two LJParameters are different */
inline bool LJParameter::operator!=(const LJParameter &other) const
{
    return not operator==(other);
}

/** Return whether or not this is a dummy LJ parameter */
inline bool LJParameter::isDummy() const
{
    //we only need to compare sqrtsig as this will be set to zero if 
    //sqrteps is zero
    return sqrtsig == 0.0;
}

/** Return whether or not this parameter has non-zero LJ parameters */
inline bool LJParameter::zeroLJ() const
{
    //we only need to compare sqrtsig as this will be set to zero if 
    //sqrteps is zero
    return sqrtsig == 0;
}

/** Return the sigma value of this parameter (in Angstroms) */
inline double LJParameter::sigma() const
{
    return sqrtsig*sqrtsig;
}

/** Return sqrt(sigma) */
inline double LJParameter::sqrtSigma() const
{
    return sqrtsig;
}

/** Return the epsilon value of this parameter (in kcal mol-1) */
inline double LJParameter::epsilon() const
{
    return sqrteps*sqrteps;
}

/** Return sqrt(epsilon) */
inline double LJParameter::sqrtEpsilon() const
{
    return sqrteps;
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_TYPEINFO(SireMM::LJParameter, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMM::LJParameter);

SIRE_EXPOSE_CLASS( SireMM::LJParameter )

SIRE_END_HEADER

#endif
