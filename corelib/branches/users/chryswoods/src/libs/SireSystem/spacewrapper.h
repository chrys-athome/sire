/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIRESYSTEM_SPACEWRAPPER_H
#define SIRESYSTEM_SPACEWRAPPER_H

#include "moleculeconstraint.h"

#include "SireFF/point.h"
#include "SireMol/moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class SpaceWrapper;
}

QDataStream& operator<<(QDataStream&, const SireSystem::SpaceWrapper&);
QDataStream& operator>>(QDataStream&, SireSystem::SpaceWrapper&);

namespace SireSystem
{

using SireBase::PropertyMap;

using SireMol::MoleculeGroup;

using SireFF::Point;

/** This is a molecule constraint that constrains
    a group of molecules to lie within the same 
    periodic box as a specified point - the molecules
    are wrapped into the box (i.e. they are moved into
    the opposite side of the box that they leave)
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SpaceWrapper 
        : public SireBase::ConcreteProperty<SpaceWrapper,MoleculeConstraint>
{

friend QDataStream& ::operator<<(QDataStream&, const SpaceWrapper&);
friend QDataStream& ::operator>>(QDataStream&, SpaceWrapper&);

public:
    SpaceWrapper();
    SpaceWrapper(const Point &point,
                 const MoleculeGroup &molgroup,
                 const PropertyMap &map = PropertyMap());
    
    SpaceWrapper(const SpaceWrapper &other);
    
    ~SpaceWrapper();
    
    SpaceWrapper& operator=(const SpaceWrapper &other);
    
    bool operator==(const SpaceWrapper &other) const;
    bool operator!=(const SpaceWrapper &other) const;
    
    static const char* typeName();

    const Point& point() const;
    const MoleculeGroup& moleculeGroup() const;
    const PropertyMap& propertyMap() const;
    
    bool involvesMolecule(MolNum molnum) const;
    bool involvesMoleculesFrom(const Molecules &molecules) const;
    
    Molecules update(const System &system);
    Molecules update(const System &system, MolNum molnum);
    Molecules update(const System &system, const Molecules &molecules);

private:
    Molecules updateAll(const System &system);
    Molecules updateMol(const System &system, MolNum molnum);
    Molecules updateMols(const System &system, const Molecules &molecules);

    /** The point that defines the center of the box */
    SireFF::PointPtr wrap_point;

    /** The molecule group containing the molecules to be wrapped */
    SireMol::MolGroupPtr molgroup;
    
    /** The property map specifying the locations of the space
        and coordinates properties */
    PropertyMap map;
};

}

Q_DECLARE_METATYPE( SireSystem::SpaceWrapper )

SIRE_EXPOSE_CLASS( SireSystem::SpaceWrapper )

SIRE_END_HEADER

#endif
