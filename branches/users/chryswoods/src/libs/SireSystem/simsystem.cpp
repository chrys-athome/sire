
#include "simsystem.h"
#include "system.h"
#include "moves.h"
#include "move.h"

#include "SireFF/forcefield.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"

#include "SireMol/moleculegroup.h"
#include "SireMol/moleculegroups.h"

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
    return System(sysdata, ffields);
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
        ffields.majorUpdate(system.forceFields());
    }
    else if (system.version().minor() != this->version().minor())
    {
        //same system with the same forcefields, but some
        //of the molecules in those forcefields may have
        //changed
        ffields.minorUpdate(system.forceFields());
    }
    else
        //else identical ID and version, so systems are identical
        //and there is nothing to do :-)
        return;
    
    //now that we have updated the forcefields, 
    //copy the system data.
    sysdata = system.info();
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
const MoleculeGroups& SimSystem::groups() const
{
    return sysdata.groups();
}

/** Return the ID number of this system */
SystemID SimSystem::ID() const
{
    return sysdata.ID();
}

/** Return the version number of this system */
const Version &SimSystem::version() const
{
    return sysdata.version();
}

/** Change the object 'obj' */
template<class T>
inline void SimSystem::_pvt_change(const T &obj)
{
    bool in_sysdata = sysdata.contains(obj.ID());
    bool in_ffields = ffields.contains(obj.ID());

    if (in_sysdata and in_ffields)
    {
        SystemData orig_sysdata = sysdata;
        
        QHash<MoleculeID,Molecule> constrained_mols = 
                                    sysdata.change( Molecule(obj) );
        
        try
        {
            if (constrained_mols.isEmpty())
                ffields.change(obj);
            else if (constrained_mols.count() == 1)
                ffields.change( *(constrained_mols.constBegin()) );
            else
                ffields.change(constrained_mols);
        }
        catch(...)
        {
            sysdata = orig_sysdata;
            throw;
        }
    }
    else if (in_sysdata)
    {
        sysdata.change( Molecule(obj) );
    }
    else if (in_ffields)
    {
        QHash<MoleculeID,Molecule> constrained_mols = 
                                      sysdata.applyConstraints( Molecule(obj) );
        
        if (constrained_mols.isEmpty())
            ffields.change(obj);
        else if (constrained_mols.count() == 1)
            ffields.change( *(constrained_mols.constBegin()) );
        else
            ffields.change(constrained_mols);
            
        sysdata.incrementMinorVersion();
    }
}

/** Change the molecule 'mol'. */
void SimSystem::change(const Molecule &mol)
{
    this->_pvt_change<Molecule>(mol);
}

/** Change the residue 'residue' */
void SimSystem::change(const Residue &residue)
{
    this->_pvt_change<Residue>(residue);
}

/** Change the atom 'atom' */
void SimSystem::change(const NewAtom &atom)
{
    this->_pvt_change<NewAtom>(atom);
}

/** Remove the molecule 'molecule' */
void SimSystem::remove(const Molecule &molecule)
{
    bool in_sysdata = sysdata.contains(molecule.ID());
    bool in_ffields = ffields.contains(molecule.ID());
    
    if (in_sysdata and in_ffields)
    {
        SystemData orig_sysdata = sysdata;
        
        sysdata.remove(molecule);
        
        try
        {
            ffields.remove(molecule);
        }
        catch(...)
        {
            sysdata = orig_sysdata;
            throw;
        }
    }
    else if (in_sysdata)
    {
        sysdata.remove(molecule);
    }
    else if (in_ffields)
    {
        ffields.remove(molecule);
        sysdata.incrementMinorVersion();
    }
}
