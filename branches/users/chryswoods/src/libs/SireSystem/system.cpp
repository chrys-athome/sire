
#include "SireSystem/qhash_siresystem.h"

#include "system.h"
#include "simsystem.h"
#include "localsimsystem.h"

#include "SireMol/property.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireFF/errors.h"

using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<System> r_system;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const System &system)
{
    writeHeader(ds, r_system, 1);

    SharedDataStream sds(ds);
     
    sds << system.ffields << system.tmp_expressions
        << static_cast<const SystemData&>(system);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, System &system)
{
    VersionID v = readHeader(ds, r_system);

    if (v == 1)
    {
        SharedDataStream sds(ds);
         
        sds >> system.ffields >> system.tmp_expressions
            >> static_cast<SystemData&>(system);
    }
    else
        throw version_error(v, "1", r_system, CODELOC);

    return ds;
}

/** Create a new, unnamed System */
System::System() : SystemData()
{}

/** Construct a new simulation System with a specified name */
System::System(const QString &name) : SystemData(name)
{}

/** Private constructor used by SimSystem to create a System from
    a SystemData and a collection of forcefields - it is up to
    you to ensure that the metadata and the forcefields are compatible! */
System::System(const SystemData &data,
               const QHash<ForceFieldID,ForceField> &forcefields)
       : SystemData(data), ffields(forcefields)
{}

/** Copy constructor */
System::System(const System &other)
       : SystemData(other),
         ffields(other.ffields), tmp_expressions(other.tmp_expressions)
{}

/** Destructor. This will delete everything in this system unless it is
    referenced elsewhere. */
System::~System()
{}

/** Assignment operator */
System& System::operator=(const System &other)
{
    if (this != &other)
    {
        SystemData::operator=(other);
        ffields = other.ffields;
        tmp_expressions = other.tmp_expressions;
    }

    return *this;
}

/** Comparison operator - two systems are equal if they have the
    same ID and version */
bool System::operator==(const System &other) const
{
    return SystemData::operator==(other);
}

/** Comparison operator - two systems are equal if they have the
    same ID and version */
bool System::operator!=(const System &other) const
{
    return SystemData::operator!=(other);
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This runs the move 'nmoves' times.
*/
void System::run(const Move &move, quint32 nmoves)
{
    LocalSimSystem simsystem(*this, move);

    *this = simsystem.run(nmoves);
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This performs one cycles of the supplied moves
    (e.g. moves.count() moves)
*/
void System::run(const Moves &moves)
{
    LocalSimSystem simsystem(*this, moves);

    *this = simsystem.run(moves.count());
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This performs 'nmoves' moves.
*/
void System::run(const Moves &moves, quint32 nmoves)
{
    LocalSimSystem simsystem(*this, moves);

    *this = simsystem.run(nmoves);
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
ForceField& System::getForceField(ForceFieldID ffid)
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
Values System::getEnergyComponents(ForceFieldID ffid)
{
    return getForceField(ffid).energies();
}

/** Return the values of all of the energy components in each of the
    forcefields whose IDs are in 'ffids'
    
    \throw SireFF::missing_forcefield
*/
Values System::getEnergyComponents(const QSet<ForceFieldID> &ffids)
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
double System::getEnergyComponent(const FFComponent &component)
{
    return getForceField(component.forceFieldID()).energy(component);
}

/** Return all of the values of the energy components in 'components' 

    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
*/
Values System::getEnergyComponents(const QSet<FFComponent> &components)
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

/** Add the forcefield 'forcefield' to this System. This
    will replace any existing copy of the forcefield */
void System::add(const ForceField &ffield)
{
    ffields.insert( ffield.ID(), ffield );
}

/** Extract all of the equations from the SystemData into this object.
    This allows them to be manipulated at a time when they don't all
    have to resolve their dependencies on one another. */
void System::extractEquations()
{
    //need to move the list of expressions into the top list...
    QHash<Function,FFExpression> current_equations = this->equations();
    
    if (not current_equations.isEmpty())
    {
        if (tmp_expressions.isEmpty())
            tmp_expressions = current_equations;
        else
        {
            for (QHash<Function,FFExpression>::const_iterator 
                                          it = tmp_expressions.constBegin();
                 it != tmp_expressions.constEnd();
                 ++it)
            {
                current_equations.insert( it.key(), it.value() );
            }
        
            current_equations = tmp_expressions;
          
            SystemData::remove( tmp_expressions.keys().toSet() );
        }
    }
}

/** Remove the forcefield 'forcefield' from this System */
void System::remove(const ForceField &ffield)
{
    ffields.remove(ffield.ID());
    
    extractEquations();
}

/** Add the equation 'equation', used to calculate properties of the system.
    This will replace any other equation that has the same function as this one. */
void System::add(const FFExpression &ffexpression)
{
    tmp_expressions.insert(ffexpression.function(), ffexpression);
}

/** Remove the equation represented by the function 'func' */
void System::remove(const Function &func)
{
    if ( this->contains(func) )
    {
        extractEquations();
        tmp_expressions.remove(func);
    }
}

/** Remove the equation 'equation' */
void System::remove(const FFExpression &ffexpression)
{
    this->remove(ffexpression.function());
}

/** Add the MoleculeGroup 'group'. This will replace any existing
    molecule group with the same ID number. */
void System::add(const MoleculeGroup &group)
{
    SystemData::add(group);
}

/** Remove the MoleculeGroup 'group'. */
void System::remove(const MoleculeGroup &group)
{
    SystemData::remove(group);
}

/** Change the molecule 'molecule' */
void System::change(const Molecule &molecule)
{
    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.constEnd();
         ++it)
    {
        it->change(molecule);
    }
    
    SystemData::change(molecule);
}

/** Change the residue 'residue' */
void System::change(const Residue &residue)
{
    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.constEnd();
         ++it)
    {
        it->change(residue);
    }
    
    SystemData::change(residue);
}

/** Change the atom 'atom' */
void System::change(const NewAtom &atom)
{
    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.constEnd();
         ++it)
    {
        it->change(atom);
    }
    
    SystemData::change(atom);
}

/** Remove the molecule 'molecule' from this system */
void System::remove(const Molecule &molecule)
{
    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.constEnd();
         ++it)
    {
        it->remove(molecule);
    }
    
    SystemData::remove(molecule);
}
