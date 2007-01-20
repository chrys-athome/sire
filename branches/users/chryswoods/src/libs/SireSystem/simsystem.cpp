
#include "simsystem.h"
#include "system.h"
#include "moleculegroup.h"
#include "moves.h"
#include "move.h"

#include "SireFF/forcefield.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireFF;

//////////
////////// Implementation of SimSystem
//////////

/** Construct from a System object */
SimSystem::SimSystem(System &system)
          : boost::noncopyable(),
            sysdata( system.sysdata ),
            ffields( system.ffields )
{
    //ensure that the system is ready to be simulated
    system.prepareForSimulation();
}

/** Construct from the passed SystemData and ForceFields - this
    must be from the same System or else weird things will happen! */
SimSystem::SimSystem(SystemData &systemdata, 
                     ForceFieldsBase &forcefields)
          : boost::noncopyable(),
            sysdata(systemdata), ffields(forcefields)
{}

/** Destructor */
SimSystem::~SimSystem()
{}

/** Return the energy of the component 'component' */
double SimSystem::energy(const Function &component)
{
    return ffields.energy(component);
}

/** Return the energy of the component 'component' */
double SimSystem::energy(const FFComponent &component)
{
    return ffields.energy(component);
}

/** Return a checkpoint of the system in its current state */
System SimSystem::checkpoint() const
{
    return System(sysdata, ffields.forceFields());
}

/** Set the system to be simulated - this is used both
    to change the system, but also to roll back to 
    a previous checkpoint. */
void SimSystem::setSystem(System &system)
{
    if (system.ID() != this->ID())
    {
        //completely changing the system, therefore
        //completely changing the forcefields.
        ffields.setEqualTo(system.forceFields());
    }
    else if (system.version().major() != this->version().major())
    {
        //same system, but major version change - some
        //forcefields may have been added, others may
        //have been removed
        ffields.majorUpdateTo(system.forceFields());
    }
    else if (system.version().minor() != this->version().minor())
    {
        //same system with the same forcefields, but some
        //of the molecules in those forcefields may have
        //changed
        ffields.minorUpdateTo(system.forceFields());
    }
    else
        //else identical ID and version, so systems are identical
        //and there is nothing to do :-)
        return;
    
    //now that we have updated the forcefields, 
    //copy the system data.
    sysdata = checkpoint.info();
}

/** Return the molecule group with ID == groupid 

    \throw SireMol::missing_group
*/
const MoleculeGroup& SimSystem::group(MoleculeGroupID groupid) const
{
    return sysdata.group(groupid);
}

/** Return the copy of the group 'molgroup' that is in this 
    system.
    
    \throw SireMol::missing_group
*/
const MoleculeGroup& SimSystem::group(const MoleculeGroup &molgroup) const
{
    return this->group(molgroup.ID());
}

/** Return a hash of all of the MoleculeGroups in this system,
    indexed by their MoleculeGroupID */
const QHash<MoleculeGroupID,MoleculeGroup>& SimSystem::groups() const
{
    return sysdata.groups();
}

/** Return the ID number of this system */
SystemID SimSystem::ID() const
{
    return sysdata.ID();
}

/** Return the version number of this system */
Version SimSystem::version()
{
    return sysdata.version();
}

/** Change the molecule 'mol'. */
void SimSystem::change(const Molecule &mol)
{
    bool in_sysdata = sysdata.contains(mol.ID());
    bool in_ffields = ffields.contains(mol.ID());

    if ( in_sysdata and in_ffields )
    {
        //remember the current state of the system data object
        // - this is necessary in case ffields throws an exception
        //   and we need to restore the old state
        SystemData orig_sysdata = sysdata;
        
        //update the system - this increments the minor ID number
        //and also moves the molecule into the simulation box - 
        // the moved molecule is returned
        Molecule mol_in_box = sysdata.change(mol);
        
        //now change the molecule in the forcefields
        try
        {
            ffields.change(mol_in_box);
        }
        catch(...)
        {
            //the forcefields are already back in their 
            //original state - we need to restore the system data
            sysdata = orig_sysdata;
            
            //rethrow the exception
            throw;
        }
    }
    else if ( in_sysdata )
    {
        //this molecule is only present in the system info
        // - no need to update the forcefields
        sysdata.change(mol);
    }
    else if ( in_ffields )
    {
        //this molecule is only present in the forcefields
        // - no need to update the system data!
        
        //do need to move this molecule into our simulation box
        Molecule mol_in_box = sysdata.moveIntoBox(mol);
        
        //change the molecule in the forcefields (this is atomic)
        ffields.change(mol_in_box);
        
        //increment the minor ID number of the system as a molecule has changed
        sysdata.incrementMinorID();
    }
}

/** Change the residue 'residue' */
void SimSystem::change(const Residue &residue)
{
    bool in_sysdata = sysdata.contains(residue.ID());
    bool in_ffields = ffields.contains(residue.ID());

    if (in_sysdata and in_ffields)
    {
        SystemData orig_sysdata = sysdata;
        
        Molecule mol_in_box = sysdata.change( residue.molecule() );
        
        try
        {
            if (mol_in_box.version() != residue.version())
            {
                //the change of residue has pushed the molecule outside
                //the simulation box, and it has had to be pushed back in
                //  - we are therefore changing the entire molecule
                ffields.change(mol_in_box);
            }
            else
                ffields.change(residue);
        }
        catch(...)
        {
            sysdata = orig_sysdata;
            throw;
        }
    }
    else if (in_sysdata)
    {
        sysdata.change(residue.molecule());
    }
    else if (in_ffields)
    {
        Molecule mol_in_box = sysdata.moveIntoBox(residue.molecule());
        
        if (mol_in_box.version() != residue.version())
            ffields.change(mol_in_box);
        else
            ffields.change(residue);
            
        sysdata.incrementMinorID();
    }
}

/** Change the atom 'atom' */
void SimSystem::change(const NewAtom &atom)
{
    bool in_sysdata = sysdata.contains(atom.ID());
    bool in_ffields = ffields.contains(atom.ID());
    
    if (in_sysdata and in_ffields)
    {
        SystemData orig_sysdata = sysdata;
        
        Molecule mol_in_box = sysdata.change(atom.molecule());
        
        try
        {
            if (mol_in_box.version() != atom.version())
                ffields.change(mol_in_box);
            else
                ffields.change(atom);
        }
        catch(...)
        {
            sysdata = orig_sysdata;
            throw;
        }
    }
    else if (in_sysdata)
    {
        sysdata.change(atom.molecule());
    }
    else if (in_ffields)
    {
        Molecule mol_in_box = sysdata.moveIntoBox(atom.molecule());
        
        if (mol_in_box.version() != atom.version())
            ffields.change(mol_in_box);
        else
            ffields.change(atom);
            
        sysdata.incrementMinorID();
    }
}

void SimSystem::remove(const Molecule &molecule);
