#ifndef SIRESYSTEM_LOCALSIMSYSTEM_H
#define SIRESYSTEM_LOCALSIMSYSTEM_H

#include "simsystem.h"
#include "system.h"

#include "moves.h"

#include "moleculegroupid.h"
#include "moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

/** This class provides a SimSystem that is used to perform
    the entire simulation in the local process. This class
    is designed to allow the user to quickly run local simulations
    without the hassle of setting up processors/cluster and using
    SireSim

    @author Christopher Woods
*/
class LocalSimSystem : public SimSystem, protected SystemData
{
public:
    LocalSimSystem(const System &system, const Moves &moves);

    ~LocalSimSystem();
    
    System run(quint32 nmoves);
    
    double energy(const Function &component);
    double energy(const FFComponent &component);
    
    double energy(const FFExpression &expression);

    System checkpoint();
    void rollback(const System &checkpoint);

    MoleculeGroup group(MoleculeGroupID id);
    
    QHash<MoleculeGroupID,MoleculeGroup> groups();

    SystemID ID();
    Version version();

    void change(const Molecule &molecule);
    void change(const Residue &residue);
    void change(const NewAtom &atom);

private:
    /** The forcefields in this system */
    QHash<ForceFieldID, ForceField> ffields;

    /** The moves that will be performed on this system */
    Moves mvs;
};

/** Return the energy of the component represented by the
    function 'component' 
    
    \throw SireFF::missing_component
*/
inline double LocalSimSystem::energy(const Function &component)
{
    return SystemData::energy(component);
}

/** Return the energy of the component represented by the 
    function 'component'
    
    \throw SireFF::missing_component
*/
inline double LocalSimSystem::energy(const FFComponent &component)
{
    return SystemData::energy(component);
}
    
/** Return the energy calculated using the passed expression.

    \throw SireFF::missing_component
*/
inline double LocalSimSystem::energy(const FFExpression &expression)
{
    return SystemData::energy(expression);
}

/** Return the MoleculeGroup with ID == id 

    \throw SireSystem::missing_group
*/
inline MoleculeGroup LocalSimSystem::group(MoleculeGroupID id)
{
    return SystemData::group(id);
}
    
/** Return all of the MoleculeGroups in this System, indexed
    by thier MoleculeGroupID */
inline QHash<MoleculeGroupID,MoleculeGroup> LocalSimSystem::groups()
{
    return SystemData::groups();
}

/** Return the ID number of the System */
inline SystemID LocalSimSystem::ID()
{
    return SystemData::ID();
}

/** Return the version number of the System */
inline Version LocalSimSystem::version()
{
    return SystemData::version();
}

}

SIRE_END_HEADER

#endif
