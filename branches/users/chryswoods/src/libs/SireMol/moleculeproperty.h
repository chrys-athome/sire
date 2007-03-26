/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREMOL_MOLECULEPROPERTY_H
#define SIREMOL_MOLECULEPROPERTY_H

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeProperty;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeProperty&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeProperty&);

namespace SireMol
{

class Molecule;

/** This is the base class of all properties that are specifically
    attached to molecules (e.g. AtomicProperties, BondProperties
    etc.)

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeProperty : public SireBase::PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeProperty&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeProperty&);

public:
    MoleculeProperty();

    MoleculeProperty(const MoleculeProperty &other);

    ~MoleculeProperty();

    virtual bool isCompatibleWith(const Molecule &molecule) const=0;

    void assertCompatibleWith(const Molecule &molecule) const;
};

}

SIRE_END_HEADER

#endif
