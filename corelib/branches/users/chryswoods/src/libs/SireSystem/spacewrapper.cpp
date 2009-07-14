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

#include "spacewrapper.h"
#include "system.h"

#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/moleditor.h"

#include "SireVol/space.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireFF;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<SpaceWrapper> r_spacewrapper;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, 
                                          const SpaceWrapper &spacewrapper)
{
    writeHeader(ds, r_spacewrapper, 1);
    
    SharedDataStream sds(ds);
    
    sds << spacewrapper.wrap_point << spacewrapper.molgroup
        << spacewrapper.map
        << static_cast<const MoleculeConstraint&>(spacewrapper);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, SpaceWrapper &spacewrapper)
{
    VersionID v = readHeader(ds, r_spacewrapper);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> spacewrapper.wrap_point >> spacewrapper.molgroup
            >> spacewrapper.map
            >> static_cast<MoleculeConstraint&>(spacewrapper);
    }
    else
        throw version_error(v, "1", r_spacewrapper, CODELOC);
        
    return ds;
}

/** Constructor */
SpaceWrapper::SpaceWrapper() : ConcreteProperty<SpaceWrapper,MoleculeConstraint>()
{}

/** Construct to wrap all of the molecules in the group 'molgroup' 
    into the same periodic box as the point 'point' using the
    supplied property map to find the space and coordinate properties */
SpaceWrapper::SpaceWrapper(const Point &point,
                           const MoleculeGroup &wrap_group,
                           const PropertyMap &wrap_map)
             : ConcreteProperty<SpaceWrapper,MoleculeConstraint>(),
               wrap_point(point), molgroup(wrap_group), map(wrap_map)
{}

/** Copy constructor */
SpaceWrapper::SpaceWrapper(const SpaceWrapper &other)
             : ConcreteProperty<SpaceWrapper,MoleculeConstraint>(other),
               wrap_point(other.wrap_point), molgroup(other.molgroup),
               map(other.map)
{}

/** Destructor */
SpaceWrapper::~SpaceWrapper()
{}

/** Copy assignment operator */
SpaceWrapper& SpaceWrapper::operator=(const SpaceWrapper &other)
{
    if (this != &other)
    {
        wrap_point = other.wrap_point;
        molgroup = other.molgroup;
        map = other.map;
        MoleculeConstraint::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool SpaceWrapper::operator==(const SpaceWrapper &other) const
{
    return (this == &other) or
           (wrap_point == other.wrap_point and
            map == other.map and
            molgroup == other.molgroup and
            MoleculeConstraint::operator==(other));
}

/** Comparison operator */
bool SpaceWrapper::operator!=(const SpaceWrapper &other) const
{
    return not this->operator==(other);
}

const char* SpaceWrapper::typeName()
{
    return QMetaType::typeName( qMetaTypeId<SpaceWrapper>() );
}

/** Return the point that defines the center of the periodic box
    (the molecules will be wrapped so that they are in the same
    box as this point) */
const Point& SpaceWrapper::point() const
{
    return wrap_point.read();
}

/** Return the molecule group containing the molecules being wrapped */
const MoleculeGroup& SpaceWrapper::moleculeGroup() const
{
    return molgroup.read();
}

/** Return the property map used to find the coordinates and 
    space properties */
const PropertyMap& SpaceWrapper::propertyMap() const
{
    return map;
}

/** Return whether or not this constraint affects or is based
    on information from the molecule with number 'molnum' */
bool SpaceWrapper::involvesMolecule(MolNum molnum) const
{
    return wrap_point.read().contains(molnum) or molgroup.read().contains(molnum);
}

/** Return whether or not this constraint affects or is based
    on information from the molecules in 'molecules' */
bool SpaceWrapper::involvesMoleculesFrom(const Molecules &molecules) const
{
    if (wrap_point.read().usesMoleculesIn(molecules))
        return true;
        
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        if (molgroup.read().contains(it.key()))
            return true;
    }
    
    return false;
}

/** Internal function that checks and updates all of the 
    molecules from the system 'system' */
Molecules SpaceWrapper::updateAll(const System &system)
{
    SpaceWrapper old_state(*this);
    
    try
    {
        if (wrap_point.read().usesMoleculesIn(system))
            wrap_point.edit().update(system);
    
        molgroup = system[ molgroup.read().number() ];

        const Space &space = system.property( map["space"] ).asA<Space>();
    
        if (not space.isPeriodic())
            return Molecules();

        Molecules changed_mols;
        
        const Vector &center_point = wrap_point.read().point();
        
        const Molecules &molecules = molgroup.read().molecules();
        
        const PropertyName &coords_property = map["coordinates"];
        
        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            Molecule molecule = it->molecule();
        
            const AtomCoords &coords = molecule.property(coords_property)
                                               .asA<AtomCoords>();
                                                     
            //translate the molecule as a single entity (we don't want
            //molecules splitting over two sides of the box)
            CoordGroupArray new_coords = space.getMinimumImage(coords.array(),
                                                               center_point, true);
                                                               
            if (new_coords.constData() != coords.constData())
            {
                //the molecule has moved
                molecule = molecule.edit().setProperty(coords_property,
                                                       AtomCoords(new_coords))
                                          .commit();
                                          
                changed_mols.add(molecule);
            }
        }
        
        return changed_mols;
    }
    catch(...)
    {
        SpaceWrapper::operator=(old_state);
        throw;
    }
}

/** Internal function that checks and updates the molecule with 
    number 'molnum' */
Molecules SpaceWrapper::updateMol(const System &system, MolNum molnum)
{
    const MoleculeGroup &newgroup = system[ molgroup.read().number() ];
    
    if (not newgroup.contains(molnum))
    {
        //the group has changed, but it is not the expected molecule
        return this->updateAll(system);
    }
    
    Molecule molecule = newgroup[molnum].molecule();
    
    const Space &space = system.property( map["space"] ).asA<Space>();
    
    Molecules changed_mols;
    
    if (space.isPeriodic())
    {
        const Vector &center_point = wrap_point.read().point();
        
        const PropertyName &coords_property = map["coordinates"];

        const AtomCoords &coords = molecule.property(coords_property)
                                           .asA<AtomCoords>();
                                                 
        //translate the molecule as a single entity (we don't want
        //molecules splitting over two sides of the box)
        CoordGroupArray new_coords = space.getMinimumImage(coords.array(),
                                                           center_point, true);
                                                           
        if (new_coords.constData() != coords.constData())
        {
            //the molecule has moved
            molecule = molecule.edit().setProperty(coords_property,
                                                   AtomCoords(new_coords))
                                      .commit();
                                      
            changed_mols.add(molecule);
        }
    }
    
    molgroup = newgroup;
    
    return changed_mols;
}

/** Internal function that checks and updates the molecules 
    in 'molecules' */
Molecules SpaceWrapper::updateMols(const System &system, const Molecules &molecules)
{
    const MoleculeGroup &newgroup = system[ molgroup.read().number() ];

    const Space &space = system.property( map["space"] ).asA<Space>();

    Molecules changed_mols;

    if (space.isPeriodic())
    {
        const Vector &center_point = wrap_point.read().point();
        const PropertyName &coords_property = map["coordinates"];

        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            if (newgroup.contains(it.key()))
            {
                Molecule molecule = newgroup[it.key()].molecule();

                const AtomCoords &coords = molecule.property(coords_property)
                                                   .asA<AtomCoords>();
                                                 
                //translate the molecule as a single entity (we don't want
                //molecules splitting over two sides of the box)
                CoordGroupArray new_coords = space.getMinimumImage(coords.array(),
                                                                   center_point, true);
                                                           
                if (new_coords.constData() != coords.constData())
                {
                    //the molecule has moved
                    molecule = molecule.edit().setProperty(coords_property,
                                                           AtomCoords(new_coords))
                                              .commit();
                                      
                    changed_mols.add(molecule);
                }
            }
        }
    }
    
    molgroup = newgroup;
    
    return changed_mols;
}

/** Update this constraint from the passed system and return the molecules
    that need to change to maintain this constraint */
Molecules SpaceWrapper::update(const System &system)
{
    Molecules changed_mols;

    if (system.UID() == this->sysUID() and system.version() == this->sysVersion())
        return changed_mols;
        
    else if (system.UID() != this->sysUID() or
             system.version().majorVersion() != this->sysVersion().majorVersion())
    {
        //the space may have changed
        changed_mols = this->updateAll(system);
    }
    else
    {
        const MoleculeGroup &newgroup = system[ molgroup.read().number() ];
    
        if (molgroup.read().version() != newgroup.version())
            changed_mols = this->updateAll(system);
    }

    this->updatedFrom(system);
    
    return changed_mols;
}

/** Update this constraint from the passed system, returning the 
    molecules that need to be changed to maintain this constraint, and
    also supplying the hint that *only* the molecule with number 'molnum'
    has changed since this constraint was last updated */
Molecules SpaceWrapper::update(const System &system, MolNum molnum)
{
    Molecules changed_mols;

    if (system.UID() == this->sysUID() and system.version() == this->sysVersion())
        return changed_mols;
        
    else if (system.UID() != this->sysUID() or
             system.version().majorVersion() != this->sysVersion().majorVersion())
    {
        //the space may have changed
        changed_mols = this->updateAll(system);
    }
    else if (wrap_point.read().contains(molnum))
    {
        //everything may have changed as the wrap point has changed
        changed_mols = this->updateAll(system);
    }
    else
    {
        const MoleculeGroup &newgroup = system[ molgroup.read().number() ];
    
        if (molgroup.read().version() != newgroup.version())
        {
            if (molgroup.read().version().majorVersion() != 
                newgroup.version().majorVersion())
            {
                //more than just a molecule has changed
                changed_mols = this->updateAll(system);
            }
            else
            {
                //only the specified molecule has changed (supposedly!)
                changed_mols = this->updateMol(system, molnum);
            }
        }
    }

    this->updatedFrom(system);
    
    return changed_mols;
}

/** Update this constraint from the passed system, returning the 
    molecules that need to be changed to maintain this constraint, and
    also supplying the hint that *only* the molecules in 'molecules'
    have changed since this constraint was last updated */
Molecules SpaceWrapper::update(const System &system, const Molecules &molecules)
{
    Molecules changed_mols;

    if (system.UID() == this->sysUID() and system.version() == this->sysVersion())
        return changed_mols;
        
    else if (system.UID() != this->sysUID() or
             system.version().majorVersion() != this->sysVersion().majorVersion())
    {
        //the space may have changed
        changed_mols = this->updateAll(system);
    }
    else if (wrap_point.read().usesMoleculesIn(molecules))
    {
        //everything may have changed as the wrap point has changed
        changed_mols = this->updateAll(system);
    }
    else
    {
        const MoleculeGroup &newgroup = system[ molgroup.read().number() ];
    
        if (molgroup.read().version() != newgroup.version())
        {
            if (molgroup.read().version().majorVersion() != 
                newgroup.version().majorVersion())
            {
                //more than just a molecule has changed
                changed_mols = this->updateAll(system);
            }
            else
            {
                //only the specified molecule has changed (supposedly!)
                changed_mols = this->updateMols(system, molecules);
            }
        }
    }

    this->updatedFrom(system);
    
    return changed_mols;
}
