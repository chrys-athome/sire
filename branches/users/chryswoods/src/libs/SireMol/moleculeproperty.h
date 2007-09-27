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

#ifndef SIREMOL_MOLECULARPROPERTY_H
#define SIREMOL_MOLECULARPROPERTY_H

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MolecularProperty;
}

QDataStream& operator<<(QDataStream&, const SireMol::MolecularProperty&);
QDataStream& operator>>(QDataStream&, SireMol::MolecularProperty&);

XMLStream& operator<<(XMLStream&, const SireMol::MolecularProperty&);
XMLStream& operator>>(XMLStream&, SireMol::MolecularProperty&);

namespace SireMol
{

class MoleculeInfo;
class AtomSelection;

/** This is the base class of all properties that are specifically
    attached to molecules (e.g. AtomicProperties, BondProperties
    etc.)

    @author Christopher Woods
*/
class SIREMOL_EXPORT MolecularProperty : public SireBase::PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const MolecularProperty&);
friend QDataStream& ::operator>>(QDataStream&, MolecularProperty&);

friend XMLStream& ::operator<<(XMLStream&, const MolecularProperty&);
friend XMLStream& ::operator>>(XMLStream&, MolecularProperty&);

public:
    MolecularProperty();

    MolecularProperty(const MolecularProperty &other);

    ~MolecularProperty();

    static const char* typeName()
    {
        return "SireMol::MolecularProperty";
    }

    virtual bool isCompatibleWith(const MoleculeInfo &molinfo) const=0;

    virtual SireBase::Property mask(const AtomSelection &selected_atoms) const=0;

    void assertCompatibleWith(const MoleculeInfo &info) const;
};

}

SIRE_END_HEADER

#endif
