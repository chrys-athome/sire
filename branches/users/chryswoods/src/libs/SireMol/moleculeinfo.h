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

#ifndef SIREMOL_MOLECULEINFO_H
#define SIREMOL_MOLECULEINFO_H

#include <QSharedDataPointer>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeInfo;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeInfo&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeInfo&);

namespace SireMol
{

class MoleculeInfoData;

/**
This class holds all of the metainfo about a molecule. This is information that 
allows mapping from an atom or residue-based ID to the actual information for the atom. 
Also it contains all non-coordinate information about the molecule. This information 
is separated off so that changing the coordinates will not cause any copying of 
the metainfo. In addition, this metainfo object can be used in other classes, 
e.g. the parameter table classes, where it can be combined with lists of other parameters.

The other main purpose of this class is to allow the many different ways of indexing 
an atom (e.g. by residue number and atom name, or by index into the molecule) to be 
mapped efficiently to the indexing method used internally by the code - namely CGAtomIdx.

@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeInfo
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeInfo&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeInfo&);

public:
    MoleculeInfo();

    MoleculeInfo(const MoleculeInfo &other);

    ~MoleculeInfo();

    MoleculeInfo& operator=(const MoleculeInfo &other);

private:
    /** Implicitly shared pointer to the data of this object */
    QSharedDataPointer<MoleculeInfoData> d;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeInfo)

SIRE_END_HEADER

#endif
