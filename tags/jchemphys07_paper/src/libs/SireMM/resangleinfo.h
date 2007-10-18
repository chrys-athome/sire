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

#ifndef SIREMM_RESANGLEINFO_H
#define SIREMM_RESANGLEINFO_H

#include "internaldatatypes.h"
#include "resinternalinfo.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class ResAngleInfo;
}

QDataStream& operator<<(QDataStream&, const SireMM::ResAngleInfo&);
QDataStream& operator>>(QDataStream&, SireMM::ResAngleInfo&);

namespace SireMM
{

class MolAngleInfo;

/**
This class holds the metadata about the angles in a residue - the MolAngleInfo class holds the information about the angles in the molecule.

@author Christopher Woods
*/
class SIREMM_EXPORT ResAngleInfo : public ResInternalInfo<Angle>
{
public:
    ResAngleInfo();
    ResAngleInfo(const MolAngleInfo &molinfo, ResNum resnum);

    ResAngleInfo(const ResInternalInfo<Angle> &other);

    ~ResAngleInfo();

    int nAngles() const;
    
    int nIntraAngles() const;
    int nInterAngles() const;
    
    const_iterator angles() const;
    const_iterator intraAngles() const;
    const_iterator interAngles() const;
};

}

Q_DECLARE_METATYPE(SireMM::ResAngleInfo)

SIRE_END_HEADER

#endif
