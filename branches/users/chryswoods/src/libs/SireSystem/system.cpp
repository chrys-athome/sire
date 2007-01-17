
#include "system.h"
#include "simsystem.h"
#include "localsimsystem.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<System> r_system;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const System &system)
{
    writeHeader(ds, r_system, 1);

    SharedDataStream(ds) << system.ffields << system.tmp_expressions
                         << static_cast<const SystemData&>(system);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, System &system)
{
    VersionID v = readHeader(ds, r_system);

    if (v == 1)
    {
        SharedDataStream sds(ds) >> system.ffields >> system.tmp_expressions
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
System::System(QString name) : SystemData(name)
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
