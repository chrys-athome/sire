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

#ifndef SIREMOL_PROPERTYEXTRACTOR_H
#define SIREMOL_PROPERTYEXTRACTOR_H

#include <QVector>
#include <QHash>

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class PropertyExtractor;
}

QDataStream& operator<<(QDataStream&, const SireMol::PropertyExtractor&);
QDataStream& operator>>(QDataStream&, SireMol::PropertyExtractor&);

namespace SireBase
{
class Property;
}

namespace SireVol
{
class CoordGroup;
}

namespace SireMol
{

class CutGroup;
class CutGroupID;
class Element;
class PartialMolecule;

using SireBase::Property;
using SireVol::CoordGroup;

/** This class is used to add a nice API to the MoleculeView based classes to
    allow the extraction of various properties of the molecule (without the
    need to clutter up the moleculeview classes APIs).

    e.g. can type mol.extract().elements() rather than mol.elements().

    In addition, this allows me to have the property extraction code
    all in one place, without the need to update multiple MoleculeView
    derived classes APIs

    @author Christopher Woods
*/
class SIREMOL_EXPORT PropertyExtractor : public MolDataView
{

friend QDataStream& ::operator<<(QDataStream&, const PropertyExtractor&);
friend QDataStream& ::operator>>(QDataStream&, PropertyExtractor&);

public:
    PropertyExtractor();

    PropertyExtractor(const MoleculeView &molecule);
    
    PropertyExtractor(const MoleculeView &molecule,
                      const SelectionFromMol &selection);

    PropertyExtractor(const PropertyExtractor &other);

    ~PropertyExtractor();

    PropertyExtractor& operator=(const PropertyExtractor &other);

    QHash<CutGroupID,quint32> cutGroupIndex() const;

    QVector< CutGroup > cutGroups() const;
    QVector< CoordGroup > coordGroups() const;

    QVector< QVector<Element> > elements() const;

    Vector geometricCenter() const;

    Property property(const QString &name) const;
};

}

Q_DECLARE_METATYPE(SireMol::PropertyExtractor)

SIRE_END_HEADER

#endif
