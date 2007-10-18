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

#ifndef SIREMM_RESDIHEDRALINFO_H
#define SIREMM_RESDIHEDRALINFO_H

#include "internaldatatypes.h"
#include "resinternalinfo.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class ResDihedralInfo;
}

QDataStream& operator<<(QDataStream&, const SireMM::ResDihedralInfo&);
QDataStream& operator>>(QDataStream&, SireMM::ResDihedralInfo&);

namespace SireMM
{

class MolDihedralInfo;

/**
This class holds the metadata about the dihedrals in a residue - the MolDihedralInfo class holds the information about the dihedrals in the molecule.

@author Christopher Woods
*/
class SIREMM_EXPORT ResDihedralInfo : public ResInternalInfo<Dihedral>
{
public:
    ResDihedralInfo();
    ResDihedralInfo(const MolDihedralInfo &molinfo, ResNum resnum);

    ResDihedralInfo(const ResInternalInfo<Dihedral> &other);

    ~ResDihedralInfo();

    int nDihedrals() const;
    
    int nIntraDihedrals() const;
    int nInterDihedrals() const;
    
    const_iterator dihedrals() const;
    const_iterator intraDihedrals() const;
    const_iterator interDihedrals() const;
};

}

Q_DECLARE_METATYPE(SireMM::ResDihedralInfo)

SIRE_END_HEADER

#endif
