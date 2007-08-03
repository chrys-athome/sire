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

#ifndef SIREMOL_ATOMICPROPERTIES_H
#define SIREMOL_ATOMICPROPERTIES_H

#include "moleculeproperty.h"

SIRE_BEGIN_HEADER

class QVariant;

namespace SireMol
{
class AtomicProperties;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomicProperties&);
QDataStream& operator>>(QDataStream&, SireMol::AtomicProperties&);

XMLStream& operator<<(XMLStream&, const SireMol::AtomicProperties&);
XMLStream& operator>>(XMLStream&, SireMol::AtomicProperties&);

namespace SireMol
{

class CutGroupID;
class CGAtomID;

/** This class provides the interface which must be supplied
    by all properties that claim to be AtomicProperties
    (i.e. have one value for each atom in the molecule)

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomicProperties : public MoleculeProperty
{

friend QDataStream& ::operator<<(QDataStream&, const AtomicProperties&);
friend QDataStream& ::operator>>(QDataStream&, AtomicProperties&);

friend XMLStream& ::operator<<(XMLStream&, const AtomicProperties&);
friend XMLStream& ::operator>>(XMLStream&, AtomicProperties&);

public:
    AtomicProperties();
    AtomicProperties(const AtomicProperties &other);

    ~AtomicProperties();

    static const char* typeName()
    {
        return "SireMol::AtomicProperties";
    }

    virtual QVariant value(const CGAtomID &cgatomid) const=0;

protected:
    void throwMissingCutGroup(CutGroupID cgid, int ncg) const;
    void throwMissingAtom(const CGAtomID &cgatomid, int nats) const;
};

}

SIRE_END_HEADER

#endif
