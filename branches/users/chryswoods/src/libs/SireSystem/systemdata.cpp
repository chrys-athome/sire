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

#include "SireSystem/qhash_siresystem.h"

#include "systemdata.h"

#include "SireBase/incremint.h"

#include "SireMol/property.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/moleculegroup.h"
#include "SireMol/moleculegroups.h"

#include "SireFF/ffcomponent.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"
#include "SireFF/errors.h"

using namespace SireSystem;
using namespace SireStream;
using namespace SireBase;
using namespace SireCAS;
using namespace SireMol;
using namespace SireFF;

static const RegisterMetaType<SystemData> r_sysdata(MAGIC_ONLY,
                                                    "SireSystem::SystemData");

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const SystemData &sysdata)
{
    writeHeader(ds, r_sysdata, 1);

    SharedDataStream sds(ds);

    sds << sysdata.molgroups << sysdata.sys_space
        << sysdata.nme << sysdata.id_and_version;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, SystemData &sysdata)
{
    VersionID v = readHeader(ds, r_sysdata);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> sysdata.molgroups >> sysdata.sys_space
            >> sysdata.nme >> sysdata.id_and_version;
    }
    else
        throw version_error(v, "1", r_sysdata, CODELOC);

    return ds;
}

/** Static Incremint used to supply ID numbers to Systems */
static Incremint system_incremint;

/** Construct with an unspecified name */
SystemData::SystemData()
           : nme( QObject::tr("Unnamed") ), id_and_version(&system_incremint)
{}

/** Construct a SystemData with a specified name */
SystemData::SystemData(const QString &name)
           : nme(name), id_and_version(&system_incremint)
{}

/** Copy constructor */
SystemData::SystemData(const SystemData &other)
           : molgroups(other.molgroups),
             sys_space(other.sys_space),
             nme(other.nme),
             id_and_version(other.id_and_version)
{}

/** Destructor */
SystemData::~SystemData()
{}

/** Copy assignment operator */
SystemData& SystemData::operator=(const SystemData &other)
{
    molgroups = other.molgroups;
    sys_space = other.sys_space;
    nme = other.nme;
    id_and_version = other.id_and_version;
    
    return *this;
}

/** Comparison operator - two SystemData objects are equal if they
    have the same ID and version */
bool SystemData::operator==(const SystemData &other) const
{
    return ID() == other.ID() and version() == other.version();
}

/** Comparison operator - two SystemData objects are equal if they
    have the same ID and version */
bool SystemData::operator!=(const SystemData &other) const
{
    return ID() != other.ID() or version() != other.version();
}

/** Return the MoleculeGroup with ID == id

    \throw SireSystem::missing_group
*/
const MoleculeGroup& SystemData::group(MoleculeGroupID id) const
{
    return molgroups.group(id);
}

/** Add the MoleculeGroup 'group' to this System. */
void SystemData::add(const MoleculeGroup &group)
{
    molgroups.add(group);
    this->incrementMajorVersion();
}

/** Add lots of MoleculeGroups to this System */
void SystemData::add(const MoleculeGroups &groups)
{
    molgroups.add(groups);
}

/** Return whether we contain a group with ID == groupid */
bool SystemData::contains(MoleculeGroupID groupid) const
{
    return molgroups.contains(groupid);
}

/** Remove the MoleculeGroup 'group' from this System */
void SystemData::remove(const MoleculeGroup &group)
{
    if (molgroups.remove(group))
        this->incrementMajorVersion();
}

/** Remove the MoleculeGroup with ID == groupid from this System */
void SystemData::remove(MoleculeGroupID groupid)
{
    if (molgroups.remove(groupid))
        this->incrementMajorVersion();
}

/** Map the molecule 'molecule' into the system space (so that it is contained
    in the system space). The returned, mapped, molecule will contain 
    the union of the atoms selected in 'molecule' with the atoms that have
    had to be moved to accomplish the mapping */
PartialMolecule SystemData::mapIntoSystemSpace(const PartialMolecule &molecule) const
{
    PartialMolecule mapped_mol = molecule;
    
    QSet<CutGroupID> cgids_that_were_mapped = mapped_mol.mapIntoSpace(spce);
    
    mapped_mol.selectAll(cgids_that_were_mapped);
    
    return mapped_mol;
}

/** Map the molecule in 'molecules' into the system space (so that they are contained
    in the system space). The returned, mapped, molecules will contain 
    the union of the atoms selected in each molecule with the atoms that have
    had to be moved to accomplish the mapping */
QHash<MoleculeID,PartialMolecule> SystemData::mapIntoSystemSpace(
                    const QHash<MoleculeID,PartialMolecule> &molecules) const
{
    QHash<MoleculeID,PartialMolecule> mapped_mols = molecules;
    
    for (QHash<MoleculeID,PartialMolecule>::iterator it = mapped_mols.begin();
         it != mapped_mols.end();
         ++it)
    {
        *it = this->mapIntoSystemSpace(*it);
    }
    
    return mapped_mols;
}

/** Map the molecule in 'molecules' into the system space (so that they are contained
    in the system space). The returned, mapped, molecules will contain 
    the union of the atoms selected in each molecule with the atoms that have
    had to be moved to accomplish the mapping */
QHash<MoleculeID,PartialMolecule>
SystemData::mapIntoSystemSpace(const QList<PartialMolecule> &molecules) const
{
    QHash<MoleculeID,PartialMolecule> mapped_mols;
    
    mapped_mols.reserve(molecules.count());
    
    for (QList<PartialMolecule>::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        PartialMolecule mapped_mol = this->mapIntoSystemSpace(*it);
        
        if (mapped_mols.contains(it->ID()))
        {
            //we have already mapped this molecule...
            //we have to do it again, and create the union of selected atoms
            AtomSelection current_selection = it->selectedAtoms();
            
            mapped_mol.selectAll(current_selection);
        }
        
        mapped_mols.insert(mapped_mol.ID(), mapped_mol);
    }
    
    return mapped_mols;
}

/** Change the molecule 'molecule' */
void SystemData::change(const PartialMolecule &molecule)
{
    if (groups.change(molecule))
        this->incrementMinorVersion();
}

/** Change a whole load of molecules */
void SystemData::change(const QList<PartialMolecule> &molecules)
{
    if (groups.change(molecules))
        this->incrementMinorVersion();
}

/** Change a whole load of molecules */
void SystemData::change(const QHash<MoleculeID,PartialMolecule> &molecules)
{
    if (groups.change(molecules))
        this->incrementMinorVersion();
}

/** Add the molecule to the groups whose IDs are in 'groupids' */
void SystemData::add(const PartialMolecule &molecule,
                     const QSet<MoleculeGroupID> &groupids)
{
    if (groups.add(molecule, groupids))
        this->incrementMinorVersion();
}

/** Add lots of molecules to the groups whose IDs are in 'groupids' */
void SystemData::add(const QHash<MoleculeID,PartialMolecule> &molecules,
                     const QSet<MoleculeGroupID> &groupids)
{
    if (groups.add(molecules, groupids))
        this->incrementMinorVersion();
}

void SystemData::add(const QList<PartialMolecule> &molecules,
           const QSet<MoleculeGroupID> &groupids);

void SystemData::remove(const PartialMolecule &molecule,
              const QSet<MoleculeGroupID> &groupids);

void SystemData::remove(const QHash<MoleculeID,PartialMolecule> &molecules,
              const QSet<MoleculeGroupID> &groupids);

void SystemData::remove(const QList<PartialMolecule> &molecules,
              const QSet<MoleculeGroupID> &groupids);

void SystemData::remove(const PartialMolecule &molecule);
void SystemData::remove(const QHash<MoleculeID,PartialMolecule> &molecules);
void SystemData::remove(const QList<PartialMolecule> &molecules);
