
#include "system.h"
#include "simsystem.h"

using namespace SireSystem;

/** Construct a new simulation System. */
System::System(QString name) : nme(name)
{}

/** Destructor. This will delete everything in this system unless it is
    referenced elsewhere. */
System::~System()
{}

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
