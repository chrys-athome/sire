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

#ifndef SIREMM_CLJPARAMETER_H
#define SIREMM_CLJPARAMETER_H

#include "chargeparameter.h"
#include "ljparameter.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJParameter;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJParameter&);
QDataStream& operator>>(QDataStream&, SireMM::CLJParameter&);

namespace SireMM
{

/**
A CLJParameter holds charge and Lennard Jones (sigma and epsilon) parameters. 
 
@author Christopher Woods
*/
class SIREMM_EXPORT CLJParameter : public ChargeParameter, public LJParameter
{
public:
    CLJParameter();
    CLJParameter(const ChargeParameter &charge, const LJParameter &lj);
    CLJParameter(const ChargeParameter &charge, double sigma, double epsilon);
    
    CLJParameter(const CLJParameter &other);
    
    ~CLJParameter();
    
    bool operator==(const CLJParameter &other) const;
    bool operator!=(const CLJParameter &other) const;
    
    QString toString() const;
    
    bool isDummy() const;
    
    static CLJParameter dummy();
};

/** Is this a dummy parameter? */
inline bool CLJParameter::isDummy() const
{
    return ChargeParameter::isDummy() and LJParameter::isDummy();
}

}

Q_DECLARE_TYPEINFO(SireMM::CLJParameter, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMM::CLJParameter)

SIRE_END_HEADER

#endif
