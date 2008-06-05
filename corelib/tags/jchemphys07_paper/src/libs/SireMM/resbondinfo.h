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

#ifndef SIREMM_RESBONDINFO_H
#define SIREMM_RESBONDINFO_H

#include "internaldatatypes.h"
#include "resinternalinfo.hpp"
#include "SireMol/bond.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class ResBondInfo;
}

QDataStream& operator<<(QDataStream&, const SireMM::ResBondInfo&);
QDataStream& operator>>(QDataStream&, SireMM::ResBondInfo&);

namespace SireMM
{

class MolBondInfo;

/**
This class is to MolBondInfo what ResidueInfo is to MoleculeInfo, namely this contains the bonding metadata for a residue, while MolBondInfo contains the bonding metadata for the molecule.

@author Christopher Woods
*/
class SIREMM_EXPORT ResBondInfo : public ResInternalInfo<Bond>
{
public:
    ResBondInfo();
    ResBondInfo(const MolBondInfo &molinfo, ResNum resnum);

    ResBondInfo(const ResInternalInfo<Bond> &other);

    ~ResBondInfo();

    int nBonds() const;
    
    int nIntraBonds() const;
    int nInterBonds() const;
    
    const_iterator bonds() const;
    const_iterator intraBonds() const;
    const_iterator interBonds() const;
};

}

Q_DECLARE_METATYPE(SireMM::ResBondInfo)

SIRE_END_HEADER

#endif
