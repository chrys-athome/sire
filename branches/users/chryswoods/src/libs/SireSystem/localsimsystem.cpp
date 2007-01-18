
#include "localsimsystem.h"

#include "SireMol/property.h"

#include "SireFF/errors.h"
#include "SireError/errors.h"

using namespace SireSystem;
using namespace SireMol;

/** Construct a simulation that performs the moves 'moves' on
    the system 'system' */
LocalSimSystem::LocalSimSystem(const System &system, const Moves &moves)
               : SimSystem(), SystemData(system),
                 mvs(moves)
{
    //initialise the moves with the system - this ensures
    //that the moves are compatible with the system, thus
    //throwing any exceptions now, rather than after a weeks
    //fruitless simulation ;-)
    mvs.initialise(*this);
}

/** Destructor */
LocalSimSystem::~LocalSimSystem()
{}

/** Return a checkpoint of the System being simulated */
System LocalSimSystem::checkpoint()
{
    return this->makeSystem(*this, ffields);
}

/** Rollback to the supplied checkpoint. The checkpoint
    must have a lower version number than the current
    system, or else that implies that the checkpoint
    has been modified outside of the simulation.

    \throw SireError::incompatible_error
    \throw SireError::version_error
*/
void LocalSimSystem::rollback(const System &checkpoint)
{
    //the ID numbers must match - we cannot change to
    //a new system during a running simulation!
    if (this->ID() != checkpoint.ID())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot roll back to a different simulation system! "
            "(current == \"%1\" (%2), checkpoint == \"%3\" (%4))")
                .arg(this->name()).arg(this->ID())
                .arg(checkpoint.name()).arg(checkpoint.ID()),
                    CODELOC );

    //the checkpoint must have a lower or equal version number
    //to the current system, or that implies that the
    //checkpoint has been modified outside of this
    //simulation
    else if (this->version() < checkpoint.version())
        throw SireError::version_error( QObject::tr(
            "The version number of the checkpoint (%1) is "
            "greater than that of the simulation (%2)! "
            "This implies that the checkpoint has been "
            "modified outside of this simulation.")
                .arg(checkpoint.version().toString(),
                     this->version().toString()), CODELOC );

    SystemData::operator=(checkpoint);
    ffields = checkpoint.forceFields();
}

/** Run 'nmoves' moves on the system. Return a copy
    of the System after the moves. */
System LocalSimSystem::run(quint32 nmoves)
{
    mvs.run(*this, nmoves);

    return this->makeSystem(*this, ffields);
}

/** Change the molecule 'molecule' in this System */
void LocalSimSystem::change(const Molecule &molecule)
{
    //should I make sure that this maintains the invariant?
    
    //change the molecule in all forcefields
    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        it->change(molecule);
    }
         
    //now change the molecule in the SystemData
    SystemData::change(molecule);
}

/** Change the residue 'residue' in this System */
void LocalSimSystem::change(const Residue &residue)
{
    //should I make sure that this maintains the invariant?
    
    //change the residue in all forcefields
    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        it->change(residue);
    }
         
    //now change the residue in the SystemData
    SystemData::change(residue);
}

/** Change the atom 'atom' in this System */
void LocalSimSystem::change(const NewAtom &atom)
{
    //should I make sure that this maintains the invariant?
    
    //change the atom in all forcefields
    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        it->change(atom);
    }
         
    //now change the atom in the SystemData
    SystemData::change(atom);
}

inline QString getString(const QList<ForceFieldID> &ffids)
{
    QStringList l;
    
    foreach (ForceFieldID ffid, ffids)
    {
        l.append( QString::number(ffid) );
    }
    
    return QString("[ %1 ]").arg( l.join(", ") );
}

/** Get a modifiable reference to the forcefield with ID == ffid
 
    \throw SireFF::missing_forcefield
*/
ForceField& LocalSimSystem::getForceField(ForceFieldID ffid)
{
    QHash<ForceFieldID,ForceField>::iterator it = ffields.find(ffid);

    if (it == ffields.constEnd())
        throw SireFF::missing_forcefield( QObject::tr(
            "The System \"%1\" (%2 %3) does not contain a forcefield "
            "with ID == %4 (contained forcefields are %5)")
                .arg(name()).arg(ID()).arg(version().toString())
                .arg(ffid).arg( getString(ffields.keys()) ), CODELOC );

    return *it;
}

/** Return the values of all of the energy components in the
    forcefield with ID == ffid
    
    \throw SireFF::missing_forcefield
*/
Values LocalSimSystem::getEnergyComponents(ForceFieldID ffid)
{
    return getForceField(ffid).energies();
}

/** Return the values of all of the energy components in each of the
    forcefields whose IDs are in 'ffids'
    
    \throw SireFF::missing_forcefield
*/
Values LocalSimSystem::getEnergyComponents(const QSet<ForceFieldID> &ffids)
{
    if (ffids.isEmpty())
        return Values();
    else if (ffids.count() == 1)
    {
        return this->getEnergyComponents( *(ffids.begin()) );
    }
    else
    {
        Values vals;
        
        for (QSet<ForceFieldID>::const_iterator it = ffids.begin();
             it != ffids.end();
             ++it)
        {
            vals += this->getEnergyComponents(*it);
        }
        
        return vals;
    }
}

/** Get the energy component 'component'

    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
*/
double LocalSimSystem::getEnergyComponent(const FFComponent &component)
{
    return getForceField(component.forceFieldID()).energy(component);
}

/** Return all of the values of the energy components in 'components' 

    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
*/
Values LocalSimSystem::getEnergyComponents(const QSet<FFComponent> &components)
{
    if (components.isEmpty())
        return Values();
        
    Values vals;
    vals.reserve(components.count());
    
    //sort the components by forcefield...
    QHash< ForceFieldID,QSet<FFComponent> > sorted_components;
    
    for (QSet<FFComponent>::const_iterator it = components.begin();
         it != components.end();
         ++it)
    {
        sorted_components[it->forceFieldID()].insert(*it);
    }
    
    //now add the energy of each component
    for (QHash< ForceFieldID,QSet<FFComponent> >::const_iterator 
                                    it = sorted_components.constBegin();
         it != sorted_components.constEnd();
         ++it)
    {
        const QSet<FFComponent> &ffcomps = *it;
        
        if (ffcomps.count() == 1)
        {
            const FFComponent &ffcomp = *(ffcomps.begin());
            vals.set( ffcomp, this->getEnergyComponent(ffcomp) );
        }
        else
        {
            vals += this->getForceField(it.key()).energies(ffcomps);
        }
    }

    return vals;
}
