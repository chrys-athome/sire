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

#ifndef SIREMM_CHARGEPARAMETER_H
#define SIREMM_CHARGEPARAMETER_H

#include <QString>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class ChargeParameter;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMM::ChargeParameter&);
QDataStream& operator>>(QDataStream&, SireMM::ChargeParameter&);

namespace SireMM
{

/**
A ChargeParameter holds the partial charge parameter

@author Christopher Woods
*/
class SIREMM_EXPORT ChargeParameter
{
public:
    ChargeParameter();
    ChargeParameter(double charge);
    
    ChargeParameter(const ChargeParameter &other);
    
    ~ChargeParameter();
    
    ChargeParameter& operator=(const ChargeParameter &other);
    
    bool isDummy() const;
    bool zeroCharge() const;
    
    double charge() const;
    
    QString toString() const;
    
    bool operator==(const ChargeParameter &other) const;
    bool operator!=(const ChargeParameter &other) const;
    
    static ChargeParameter dummy();

private:
    /** The actual chage, in |e| */
    double chg;
};

/** Return whether or not this is a dummy charge (zero charge) */
inline bool ChargeParameter::isDummy() const
{
    return chg == 0;
}

/** Return whether or not this is a zero charge */
inline bool ChargeParameter::zeroCharge() const
{
    return chg == 0;
}

/** Return the actual charge */
inline double ChargeParameter::charge() const
{
    return chg;
}

}

Q_DECLARE_METATYPE(SireMM::ChargeParameter)
Q_DECLARE_TYPEINFO(SireMM::ChargeParameter, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
