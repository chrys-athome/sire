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

#include "simsystem.h"
#include "checkpoint.h"
#include "system.h"
#include "moves.h"
#include "move.h"

#include "SireFF/forcefield.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/moleculegroup.h"
#include "SireMol/moleculegroups.h"

#include "SireBase/property.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireFF;
using namespace SireBase;

/** Protected constructor used by derived classes to provide
    the SimSystem with the components of the System to be
    simulated */
SimSystem::SimSystem(SystemData &sysdata,
                     ForceFieldsBase &ffields,
                     SystemMonitors &monitors)
          : QuerySystem(sysdata, ffields, monitors)
{}

/** Destructor */
SimSystem::~SimSystem()
{}

/** Commit the current configuration of this system - this
    tells the monitors to update themselves using the current
    system configuration. */
void SimSystem::commit()
{
    sysmonitors.update(*this);
    sysdata.incrementMinorVersion();
}

/** Set the property 'name' in all forcefields to the value 'value'

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
void SimSystem::setProperty(const QString &name, const Property &value)
{
    ffields.setProperty(name, value);
    sysdata.incrementMinorVersion();
}

/** Set the property 'name' to the value 'value' in the
    forcefield with ID == ffid

    \throw SireFF::missing_forcefield
    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
void SimSystem::setProperty(ForceFieldID ffid, const QString &name,
                            const Property &value)
{
    ffields.setProperty(ffid, name, value);
    sysdata.incrementMinorVersion();
}

/** Set the property 'name' to the value 'value' in the
    forcefield whose IDs are in 'ffids'

    \throw SireFF::missing_forcefield
    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
void SimSystem::setProperty(const QSet<ForceFieldID> &ffids,
                            const QString &name, const Property &value)
{
    ffields.setProperty(ffids, name, value);
    sysdata.incrementMinorVersion();
}

/** Change the molecule 'molecule' in this system */
PartialMolecule SimSystem::change(const PartialMolecule &molecule)
{
    //change the molecule in a copy of the SystemData
    //(this is to maintain the invariant if something goes
    // wrong when we change the molecule in the forcefields)
    SystemData new_data(sysdata);

    //change the molecule in the data - this also maps it
    //into the system space
    PartialMolecule mapped_mol = new_data.change(molecule);

    //change the molecule in the forcefields
    if (ffields.change(mapped_mol))
        //increment the version of the system
        new_data.incrementMinorVersion();

    //everything was ok - copy the new data to the original
    sysdata = new_data;

    //return the mapped molecule
    return mapped_mol;
}

/** Change lots of molecules in this system */
QHash<MoleculeID,PartialMolecule>
SimSystem::change(const QHash<MoleculeID,PartialMolecule> &molecules)
{
    if (molecules.isEmpty())
        return QHash<MoleculeID,PartialMolecule>();
    else if (molecules.count() == 1)
    {
        QHash<MoleculeID,PartialMolecule> mapped_mols;

        const PartialMolecule &mol = *(molecules.constBegin());

        mapped_mols.insert( mol.ID(), this->change(mol) );

        return mapped_mols;
    }

    //change the molecules in a copy of the SystemData
    //(this is to maintain the invariant if something goes
    // wrong when we change the molecules in the forcefields)
    SystemData new_data(sysdata);

    QHash<MoleculeID,PartialMolecule> mapped_mols = new_data.change(molecules);

    //change the molecules in the forcefields
    if (ffields.change(mapped_mols))
        //increment the version of the system
        new_data.incrementMinorVersion();

    //everything was ok - copy the new data to the original
    sysdata = new_data;

    //return the mapped molecules
    return mapped_mols;
}

/** Change lots of molecules in this system */
QHash<MoleculeID,PartialMolecule>
SimSystem::change(const QList<PartialMolecule> &molecules)
{
    if (molecules.isEmpty())
        return QHash<MoleculeID,PartialMolecule>();
    else if (molecules.count() == 1)
    {
        QHash<MoleculeID,PartialMolecule> mapped_mols;

        const PartialMolecule &mol = molecules.first();

        mapped_mols.insert( mol.ID(), this->change(mol) );

        return mapped_mols;
    }

    //change the molecules in a copy of the SystemData
    //(this is to maintain the invariant if something goes
    // wrong when we change the molecules in the forcefields)
    SystemData new_data(sysdata);

    QHash<MoleculeID,PartialMolecule> mapped_mols = new_data.change(molecules);

    //change the molecules in the forcefields
    if (ffields.change(mapped_mols))
        //increment the version of the system
        new_data.incrementMinorVersion();

    //everything was ok - copy the new data to the original
    sysdata = new_data;

    //return the mapped molecules
    return mapped_mols;
}

/** Add the molecule 'molecule' to the molecule groups whose
    IDs are in 'molgroupids'

    \throw SireMol::missing_group
*/
PartialMolecule SimSystem::add(const PartialMolecule &molecule,
                               const QSet<MoleculeGroupID> &molgroupids)
{
    //add this molecule to the system
    return sysdata.add(molecule, molgroupids);
}

/** Add the molecule 'molecule' to the molecule groups whose
    IDs are in 'molgroupids' and to the forcefield groups
    whose IDs are in ffgroupids

    \throw SireMol::missing_group
    \throw SireFF::missing_forcefield
    \throw SireFF::invalid_group
*/
PartialMolecule SimSystem::add(const PartialMolecule &molecule,
                               const QSet<FFGroupID> &ffgroupids,
                               const QSet<MoleculeGroupID> &molgroupids)
{
    if (ffgroupids.isEmpty())
        return this->add(molecule, molgroupids);
    else
    {
        SystemData new_data(sysdata);

        PartialMolecule mapped_mol = new_data.add(molecule,
                                                  molgroupids);

        if (ffields.add(mapped_mol, ffgroupids))
            new_data.incrementMinorVersion();

        sysdata = new_data;

        return mapped_mol;
    }
}

/** Add lots of molecules to the molecule groups whose
    IDs are in 'molgroupids'

    \throw SireMol::missing_group
*/
QHash<MoleculeID,PartialMolecule
SimSystem::add(const QHash<MoleculeID,PartialMolecule> &molecules,
               const QSet<MoleculeGroupID> &molgroupids)
{
    return sysdata.add(molecules, molgroupids);
}

/** Add lots of molecules to the molecule groups whose
    IDs are in 'molgroupids' and to the forcefield groups
    whose IDs are in ffgroupids

    \throw SireMol::missing_group
    \throw SireFF::missing_forcefield
    \throw SireFF::invalid_group
*/
QHash<MoleculeID,PartialMolecule
SimSystem::add(const QHash<MoleculeID,PartialMolecule> &molecules,
               const QSet<FFGroupID> &ffgroupids,
               const QSet<MoleculeGroupID> &molgroupids)
{
    if (molecules.isEmpty())
        return QHash<MoleculeID,PartialMolecule>();
    else if (molecules.count() == 1)
        return this->add( *(molecules.constBegin()), ffgroupids, molgroupids );
    else if (ffgroupids.isEmpty())
        return this->add(molecules, molgroupids);
    else
    {
        SystemData new_data = sysdata;

        QHash<MoleculeID,PartialMolecule> mapped_mols = new_data.add(molecules);

        if (ffields.add(mapped_mols))
            new_data.incrementMinorVersion();

        sysdata = new_data;

        return mapped_mols;
    }
}

/** Add lots of molecules to the molecule groups whose
    IDs are in 'molgroupids'

    \throw SireMol::missing_group
*/
QHash<MoleculeID,PartialMolecule>
SimSystem::add(const QList<PartialMolecule> &molecules,
               const QSet<MoleculeGroupID> &molgroupids)
{
    return sysdata.add(molecules, molgroupids);
}

/** Add lots of molecules to the molecule groups whose
    IDs are in 'molgroupids' and to the forcefield groups
    whose IDs are in ffgroupids

    \throw SireMol::missing_group
    \throw SireFF::missing_forcefield
    \throw SireFF::invalid_group
*/
QHash<MoleculeID,PartialMolecule>
SimSystem::add(const QList<PartialMolecule> &molecules,
               const QSet<FFGroupID> &ffgroupids,
               const QSet<MoleculeGroupID> &molgroupids)
{
    if (molecules.isEmpty())
        return QHash<MoleculeID,PartialMolecule>();
    else if (molecules.count() == 1)
        return this->add( molecules.first(), ffgroupids, molgroupids );
    else if (ffgroupids.isEmpty())
        return this->add(molecules, molgroupids);
    else
    {
        SystemData new_data = sysdata;

        QHash<MoleculeID,PartialMolecule> mapped_mols = new_data.add(molecules);

        if (ffields.add(mapped_mols))
            new_data.incrementMinorVersion();

        sysdata = new_data;

        return mapped_mols;
    }
}

/** Completely remove all selected atoms of any version
    of the molecule 'molecule' from the system */
void SimSystem::remove(const PartialMolecule &molecule)
{
    SystemData new_data( sysdata );

    new_data.remove(molecule);
    ffields.remove(molecule);

    new_data.incrementMinorVersion();
    sysdata = newdata;
}

/** Completely remove all selected atoms of any version
    of the molecules in 'molecules' */
void SimSystem::remove(const QList<PartialMolecule> &molecules)
{
    if (not molecules.isEmpty())
    {
        SystemData new_data( sysdata );

        new_data.remove(molecules);
        ffields.remove(molecules);

        new_data.incrementMinorVersion();
        sysdata = newdata;
    }
}

/** Completely remove all selected atoms from any version
    of the molecule 'molecule' from all of the molecule groups
    whose IDs are in 'molgroupids'

    \throw SireMol::missing_group
*/
void SimSystem::remove(const PartialMolecule &molecule,
                       const QSet<MoleculeGroupID> &molgroupids)
{
    if (not molgroupids.isEmpty())
    {
        new_data.remove(molecule, molgroupids);
        new_data.incrementMinorVersion();
    }
}

/** Completely remove all selected atoms from any version
    of the molecule 'molecule' from all of the molecule groups
    and forcefields whose IDs are in 'ffgroupids' and
    'molgroupids'

    \throw SireFF::missing_forcefield
    \throw SireFF::invalid_group
    \throw SireMol::missing_group
*/
void SimSystem::remove(const PartialMolecule &molecule,
                       const QSet<FFGroupID> &ffgroupids,
                       const QSet<MoleculeGroupID> &molgroupids)
{
    if (ffgroupids.isEmpty())
        this->remove(molecule, molgroupids);
    else
    {
        SystemData new_data(sysdata);
        new_data.remove(molecule, molgroupids);
        ffields.remove(molecule, ffgroupids);

        new_data.incrementMinorVersion();
        sysdata = new_data;
    }
}

/** Completely remove all selected atoms from any version
    of the molecules in 'molecules' from all of the molecule groups
    whose IDs are in 'molgroupids'

    \throw SireMol::missing_group
*/
void SimSystem::remove(const QList<PartialMolecule> &molecules,
                       const QSet<MoleculeGroupID> &molgroupids)
{
    if ( not (molecules.isEmpty() or molgroupids.isEmpty()) )
    {
        sysdata.remove(molecules, molgroupids);
        sysdata.incrementMinorVersion();
    }
}

/** Completely remove all selected atoms from any version
    of the molecules in 'molecules' from all of the molecule groups
    and forcefields whose IDs are in 'ffgroupids' and
    'molgroupids'

    \throw SireFF::missing_forcefield
    \throw SireFF::invalid_group
    \throw SireMol::missing_group
*/
void SimSystem::remove(const QList<PartialMolecule> &molecules,
                       const QSet<FFGroupID> &ffgroupids,
                       const QSet<MoleculeGroupID> &molgroupids)
{
    if (molecules.isEmpty())
        return;
    else if (ffgroupids.isEmpty())
        this->remove(molecules,molgroupids);
    else
    {
        SystemData new_data(sysdata);

        new_data.remove(molecules, molgroupids);
        ffields.remove(molecules, ffgroupids);

        new_data.incrementMinorVersion();
        sysdata = new_data;
    }
}
