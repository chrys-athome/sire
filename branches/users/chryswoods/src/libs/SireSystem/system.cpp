
#include "SireSystem/qhash_siresystem.h"

#include "system.h"
#include "simsystem.h"
#include "moves.h"

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
     
    sds << system.sysdata << system.ffields;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, System &system)
{
    VersionID v = readHeader(ds, r_system);

    if (v == 1)
    {
        SharedDataStream sds(ds);
         
        sds >> system.sysdata >> system.ffields;
    }
    else
        throw version_error(v, "1", r_system, CODELOC);

    return ds;
}

/** Create a new, unnamed System */
System::System()
{}

/** Construct a new simulation System with a specified name */
System::System(const QString &name)
       : sysdata(name)
{}

/** Private constructor used by SimSystem to create a System from
    a SystemData and a collection of forcefields - it is up to
    you to ensure that the metadata and the forcefields are compatible! */
System::System(const SystemData &data,
               const ForceFields &forcefields)
       : sysdata(data), ffields(forcefields)
{}

/** Copy constructor */
System::System(const System &other)
       : sysdata(other.sysdata), ffields(other.ffields)
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
        sysdata = other.sysdata;
        ffields = other.ffields;
    }

    return *this;
}

/** Comparison operator - two systems are equal if they have the
    same ID and version */
bool System::operator==(const System &other) const
{
    return sysdata == other.sysdata;
}

/** Comparison operator - two systems are equal if they have the
    same ID and version */
bool System::operator!=(const System &other) const
{
    return sysdata != other.sysdata;
}

/** Prepare ourselves for simulation... */
void System::prepareForSimulation()
{
    //resolve all expressions etc.
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This performs 'nmoves' moves.
*/
Moves System::run(const Moves &moves, quint32 nmoves)
{
    //store the original version of the system 
    //in case something goes wrong...
    System orig_system = *this;
    
    try
    {
        SimSystem simsystem(*this);
        
        //work with a copy of the Moves
        Moves runmoves(moves);
        
        runmoves.run(simsystem, nmoves);
        
        //return the moves from after the simulation
        return runmoves;
    }
    catch(...)
    {
        //restore this system
        *this = orig_system;
        
        //rethrow the exception
        throw;
    }
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This performs one cycles of the supplied moves
    (e.g. moves.count() moves)
*/
Moves System::run(const Moves &moves)
{
    return this->run( moves, moves.count() );
}

/** Run this system in the local thread - this will only work
    if all of the forcefields can be run in the local thread.

    This runs the move 'nmoves' times.
*/
Moves System::run(const Move &move, quint32 nmoves)
{
    return this->run( Moves(move), nmoves );
}
