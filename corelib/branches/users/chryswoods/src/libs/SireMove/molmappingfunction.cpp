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

#include "molmappingfunction.h"

#include "SireMol/moleculegroups.h"
#include "SireMol/moleculegroup.h"
#include "SireMol/molecules.h"
#include "SireMol/molecule.h"
#include "SireMol/moleculemover.h"

#include "SireSystem/querysystem.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMol;
using namespace SireSystem;
using namespace SireVol;
using namespace SireBase;
using namespace SireStream;

/////////////
///////////// Implementation of MolMappingFunctionBase
/////////////

static const RegisterMetaType<MolMappingFunctionBase> r_mmapbase(MAGIC_ONLY,
                                              "SireMove::MolMappingFunctionBase");

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const MolMappingFunctionBase &mmapbase)
{
    writeHeader(ds, r_mmapbase, 1);

    SharedDataStream sds(ds);
    sds << mmapbase.mapfunc;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        MolMappingFunctionBase &mmapbase)
{
    VersionID v = readHeader(ds, r_mmapbase);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> mmapbase.mapfunc;
    }
    else
        throw version_error(v, "1", r_mmapbase, CODELOC);

    return ds;
}

/** Construct a molecule mapping function that uses the passed MappingFunction
    to map CoordGroups from one space to another. */
MolMappingFunctionBase::MolMappingFunctionBase(const MappingFunction &mappingfunction)
                       : QSharedData(), mapfunc(mappingfunction)
{}

/** Copy constructor */
MolMappingFunctionBase::MolMappingFunctionBase(const MolMappingFunctionBase &other)
                       : QSharedData(), mapfunc(other.mapfunc)
{}

/** Destructor */
MolMappingFunctionBase::~MolMappingFunctionBase()
{}

/** Convienient syntax for "map" */
Molecules MolMappingFunctionBase::operator()(QuerySystem &system,
                                             const Space &new_space) const
{
    return map(system, new_space);
}

/////////////
///////////// Implementation of MapAsMolecules
/////////////

static const RegisterMetaType<MapAsMolecules> r_mapmols;

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const MapAsMolecules &mapmols)
{
    writeHeader(ds, r_mapmols, 1);
    
    SharedDataStream sds(ds);
    sds << mapmols.groupids 
        << static_cast<const MolMappingFunctionBase&>(mapmols);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MapAsMolecules &mapmols)
{
    VersionID v = readHeader(ds, r_mapmols);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> mapmols.groupids >> static_cast<MolMappingFunctionBase&>(mapmols);
    }
    else
        throw version_error(v, "1", r_mapmols, CODELOC);
    
    return ds;
}

/** Null constructor - this maps no molecules! */
MapAsMolecules::MapAsMolecules() : MolMappingFunctionBase( MappingFunction() )
{}

MapAsMolecules::MapAsMolecules(const MoleculeGroup &molgroup)
               : MolMappingFunctionBase( MappingFunction() )
{
    groupids.insert( molgroup.ID() );
}

/** Construct a mapper that maps the molecules from the group 'molgroup',
    using the mapping function 'mappingfunction' */
MapAsMolecules::MapAsMolecules(const MoleculeGroup &molgroup,
                               const MappingFunction &mappingfunction)
               : MolMappingFunctionBase(mappingfunction)
{
    groupids.insert( molgroup.ID() );
}

/** Construct a mapper that maps the molecules from the groups in 'molgroups'
    using the mapping function 'mappingfunction' */
MapAsMolecules::MapAsMolecules(const QList<MoleculeGroup> &molgroups,
                               const MappingFunction &mappingfunction)
               : MolMappingFunctionBase(mappingfunction)
{
    if (not molgroups.isEmpty())
    {
        groupids.reserve(molgroups.count());

        for (QList<MoleculeGroup>::const_iterator it = molgroups.begin();
             it != molgroups.end();
             ++it)
        {
            groupids.insert( it->ID() );
        }
    }
}

MapAsMolecules::MapAsMolecules(const QList<MoleculeGroup> &molgroups)
               : MolMappingFunctionBase( MappingFunction() )
{
    if (not molgroups.isEmpty())
    {
        groupids.reserve(molgroups.count());

        for (QList<MoleculeGroup>::const_iterator it = molgroups.begin();
             it != molgroups.end();
             ++it)
        {
            groupids.insert( it->ID() );
        }
    }
}

/** Construct a mapper that maps the molecules from the groups in 'molgroups'
    using the mapping function 'mappingfunction' */
MapAsMolecules::MapAsMolecules(const MoleculeGroups &molgroups,
                               const MappingFunction &mappingfunction)
               : MolMappingFunctionBase(mappingfunction),
                 groupids( molgroups.groupIDs() )
{}

/** Construct a mapper that maps the molecules from the groups in 'molgroups' */
MapAsMolecules::MapAsMolecules(const MoleculeGroups &molgroups)
               : MolMappingFunctionBase(MappingFunction()),
                 groupids( molgroups.groupIDs() )
{}

/** Copy constructor */
MapAsMolecules::MapAsMolecules(const MapAsMolecules &other)
               : MolMappingFunctionBase(other),
                 groupids(other.groupids)
{}

/** Destructor */
MapAsMolecules::~MapAsMolecules()
{}

/** Return the molecules from 'system' as they have been mapped into 'new_space' */
Molecules MapAsMolecules::map(QuerySystem &system, const Space &new_space) const
{
    //get the current space from the system
    const Space &old_space = system.space();

    //extract the complete list of molecules to be mapped
    Molecules mapped_mols;

    if (groupids.count() == 1)
    {
        mapped_mols = system.group( *(groupids.constBegin()) ).molecules();
    }
    else
    {
        foreach (MoleculeGroupID groupid, groupids)
        {
            mapped_mols.add( system.group(groupid).molecules() );
        }
    }

    //loop over each molecule and map it to the new space
    foreach (MoleculeID molid, mapped_mols.moleculeIDs())
    {
        Molecule mol = mapped_mols.molecule(molid);

        //unite all of the CoordGroups into a single CoordGroup
        CoordGroup coords = CoordGroup::combine(mol.coordGroups());

        //now map the coordinates into the new space
        coords = mapfunc.map(coords, old_space, new_space);

        //split the coordinates back up into parts...
        QVector<CoordGroup> grouped_coords = CoordGroup::split(coords,
                                                               mol.coordGroups());

        //move the molecule
        mol = mol.move().setCoordinates(grouped_coords);

        //update the molecule in the list
        mapped_mols.change( PartialMolecule(mol) );
    }

    return mapped_mols;
}

/** Assert that this mapping function is compatible with the passed system */
void MapAsMolecules::assertCompatibleWith(QuerySystem &system) const
{
    foreach (MoleculeGroupID groupid, groupids)
    {
        system.assertContains(groupid);
    }
}

/////////////
///////////// Implementation of MapAsCutGroups
/////////////

static const RegisterMetaType<MapAsCutGroups> r_mapgroups;

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const MapAsCutGroups &mapgroups)
{
    writeHeader(ds, r_mapgroups, 1);
    
    SharedDataStream sds(ds);
    sds << mapgroups.groupids 
        << static_cast<const MolMappingFunctionBase&>(mapgroups);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MapAsCutGroups &mapgroups)
{
    VersionID v = readHeader(ds, r_mapgroups);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> mapgroups.groupids >> static_cast<MolMappingFunctionBase&>(mapgroups);
    }
    else
        throw version_error(v, "1", r_mapgroups, CODELOC);
    
    return ds;
}

/** Null constructor - this maps no molecules! */
MapAsCutGroups::MapAsCutGroups() : MolMappingFunctionBase( MappingFunction() )
{}

/** Construct a mapper that maps the molecules from the group 'molgroup',
    using the mapping function 'mappingfunction' */
MapAsCutGroups::MapAsCutGroups(const MoleculeGroup &molgroup,
                               const MappingFunction &mappingfunction)
               : MolMappingFunctionBase(mappingfunction)
{
    groupids.insert( molgroup.ID() );
}

/** Construct a mapper that maps the molecules from the group 'molgroup' */
MapAsCutGroups::MapAsCutGroups(const MoleculeGroup &molgroup)
               : MolMappingFunctionBase(MappingFunction())
{
    groupids.insert( molgroup.ID() );
}

/** Construct a mapper that maps the molecules from the groups in 'molgroups'
    using the mapping function 'mappingfunction' */
MapAsCutGroups::MapAsCutGroups(const QList<MoleculeGroup> &molgroups,
                               const MappingFunction &mappingfunction)
               : MolMappingFunctionBase(mappingfunction)
{
    if (not molgroups.isEmpty())
    {
        groupids.reserve(molgroups.count());

        for (QList<MoleculeGroup>::const_iterator it = molgroups.begin();
             it != molgroups.end();
             ++it)
        {
            groupids.insert( it->ID() );
        }
    }
}

/** Construct a mapper that maps the molecules from the groups in 'molgroups' */
MapAsCutGroups::MapAsCutGroups(const QList<MoleculeGroup> &molgroups)
               : MolMappingFunctionBase(MappingFunction())
{
    if (not molgroups.isEmpty())
    {
        groupids.reserve(molgroups.count());

        for (QList<MoleculeGroup>::const_iterator it = molgroups.begin();
             it != molgroups.end();
             ++it)
        {
            groupids.insert( it->ID() );
        }
    }
}

/** Construct a mapper that maps the molecules from the groups in 'molgroups'
    using the mapping function 'mappingfunction' */
MapAsCutGroups::MapAsCutGroups(const MoleculeGroups &molgroups,
                               const MappingFunction &mappingfunction)
               : MolMappingFunctionBase(mappingfunction),
                 groupids( molgroups.groupIDs() )
{}

/** Construct a mapper that maps the molecules from the groups in 'molgroups' */
MapAsCutGroups::MapAsCutGroups(const MoleculeGroups &molgroups)
               : MolMappingFunctionBase(MappingFunction()),
                 groupids( molgroups.groupIDs() )
{}

/** Copy constructor */
MapAsCutGroups::MapAsCutGroups(const MapAsCutGroups &other)
               : MolMappingFunctionBase(other),
                 groupids(other.groupids)
{}

/** Destructor */
MapAsCutGroups::~MapAsCutGroups()
{}

/** Return the molecules from 'system' as they have been mapped into 'new_space' */
Molecules MapAsCutGroups::map(QuerySystem &system, const Space &new_space) const
{
    //get the current space from the system
    const Space &old_space = system.space();

    //extract the complete list of molecules to be mapped
    Molecules mapped_mols;

    if (groupids.count() == 1)
    {
        mapped_mols = system.group( *(groupids.constBegin()) ).molecules();
    }
    else
    {
        foreach (MoleculeGroupID groupid, groupids)
        {
            mapped_mols.add( system.group(groupid).molecules() );
        }
    }

    //loop over each molecule and map it to the new space
    foreach (MoleculeID molid, mapped_mols.moleculeIDs())
    {
        Molecule mol = mapped_mols.molecule(molid);

        //now map the coordinates into the new space
        QVector<CoordGroup> new_coords = mapfunc.map(mol.coordGroups(),
                                                     old_space, new_space);

        //move the molecule
        mol = mol.move().setCoordinates(new_coords);

        //update the molecule in the list
        mapped_mols.change( PartialMolecule(mol) );
    }

    return mapped_mols;
}

/** Assert that this mapping function is compatible with the passed system */
void MapAsCutGroups::assertCompatibleWith(QuerySystem &system) const
{
    foreach (MoleculeGroupID groupid, groupids)
    {
        system.assertContains(groupid);
    }
}

/////////////
///////////// Implementation of MolMappingFunction
/////////////

static SharedPolyPointer<MolMappingFunctionBase> shared_null;

static SharedPolyPointer<MolMappingFunctionBase> getSharedNull()
{
    if (shared_null.constData() == 0)
        shared_null = MapAsMolecules();

    return shared_null;
}

static const RegisterMetaType<MolMappingFunction> r_mapfunc;

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const MolMappingFunction &mapfunc)
{
    writeHeader(ds, r_mapfunc, 1);
    
    SharedDataStream sds(ds);
    sds << mapfunc.d;
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        MolMappingFunction &mapfunc)
{
    VersionID v = readHeader(ds, r_mapfunc);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> mapfunc.d;
    }
    else
        throw version_error(v, "1", r_mapfunc, CODELOC);
    
    return ds;
}

/** Constructor */
MolMappingFunction::MolMappingFunction()
                   : d( getSharedNull() )
{}

MolMappingFunction::MolMappingFunction(const MoleculeGroup &molgroup)
                   : d( new MapAsMolecules(molgroup) )
{}

MolMappingFunction::MolMappingFunction(const QList<MoleculeGroup> &molgroups)
                   : d( new MapAsMolecules(molgroups) )
{}

MolMappingFunction::MolMappingFunction(const MoleculeGroups &molgroups)
                   : d( new MapAsMolecules(molgroups) )
{}

/** Construct from the passed mapping function */
MolMappingFunction::MolMappingFunction(const MolMappingFunctionBase &mapfunc)
                   : d( mapfunc )
{}

/** Copy constructor */
MolMappingFunction::MolMappingFunction(const MolMappingFunction &other)
                   : d( other.d )
{}

/** Destructor */
MolMappingFunction::~MolMappingFunction()
{}

/** Assign from a mapping function */
MolMappingFunction& MolMappingFunction::operator=(const MolMappingFunctionBase &mapfunc)
{
    d = mapfunc;
    return *this;
}

/** Copy assignment operator */
MolMappingFunction& MolMappingFunction::operator=(const MolMappingFunction &other)
{
    d = other.d;
    return *this;
}

/** Convienient syntax for 'map' */
Molecules MolMappingFunction::operator()(QuerySystem &system,
                                         const Space &new_space) const
{
    return d->operator()(system, new_space);
}

/** Return the typename of this mapping function */
const char* MolMappingFunction::what() const
{
    return d->what();
}

/** Return the base class of this mapping function */
const MolMappingFunctionBase& MolMappingFunction::base() const
{
    return *d;
}

/** Return the molecules from 'system' that have been mapped into 'new_space' */
Molecules MolMappingFunction::map(QuerySystem &system, const Space &new_space) const
{
    return d->map(system, new_space);
}

/** Assert that this mapping function is compatible with the passed system */
void MolMappingFunction::assertCompatibleWith(QuerySystem &system) const
{
    d->assertCompatibleWith(system);
}
