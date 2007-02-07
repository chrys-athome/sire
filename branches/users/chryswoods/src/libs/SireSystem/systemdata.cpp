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

/** Return whether or not this object contains information about
    the molecule 'mol' */
bool SystemData::contains(const Molecule &molecule) const
{
    return this->contains(molecule.ID());
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

/** Remove the molecule 'molecule' from the system */
void SystemData::remove(const Molecule &molecule)
{
    if (molgroups.remove(molecule))
    {
        this->incrementMinorVersion();
    }
}

/** Apply constraints to this molecule, and any molecules that have
    constraints based on this molecule, and return copies of the
    affected changed molecules. If no molecule is affected by the
    constraint then return an empty hash. */
QHash<MoleculeID,Molecule> SystemData::applyConstraints(const Molecule &molecule)
{
    //apply constraint in MoleculeConstraints...


    //apply space constraint on resulting molecules...


    return QHash<MoleculeID,Molecule>();
}

/** Change the molecule 'molecule' */
QHash<MoleculeID,Molecule> SystemData::change(const Molecule &molecule)
{
    QHash<MoleculeID,Molecule> constrained_mols =
                            this->applyConstraints(molecule);

    bool changed = false;

    if (constrained_mols.isEmpty())
    {
        changed = molgroups.change(molecule);
    }
    else if (constrained_mols.count() == 1)
    {
        changed = molgroups.change( *(constrained_mols.constBegin()) );
    }
    else
    {
        changed = molgroups.change(constrained_mols);
    }

    if (changed)
        this->incrementMinorVersion();

    return constrained_mols;
}

/** Change the residue 'residue' */
QHash<MoleculeID,Molecule> SystemData::change(const Residue &residue)
{
    return this->change( residue.molecule() );
}

/** Change the atom 'atom' */
QHash<MoleculeID,Molecule> SystemData::change(const NewAtom &atom)
{
    return this->change( atom.molecule() );
}
