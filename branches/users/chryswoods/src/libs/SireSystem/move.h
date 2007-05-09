/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIRESYSTEM_MOVE_H
#define SIRESYSTEM_MOVE_H

#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class MoveBase;
class Move;
}

QDataStream& operator<<(QDataStream&, const SireSystem::MoveBase&);
QDataStream& operator>>(QDataStream&, SireSystem::MoveBase&);

QDataStream& operator<<(QDataStream&, const SireSystem::Move&);
QDataStream& operator>>(QDataStream&, SireSystem::Move&);

namespace SireSim
{
class Simulation;
}

namespace SireSystem
{

class SimSystem;
class QuerySystem;

using SireSim::Simulation;

/** This is the virtual base class of all moves that
    can be applied to a SimSystem

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MoveBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MoveBase&);
friend QDataStream& ::operator>>(QDataStream&, MoveBase&);

public:
    virtual ~MoveBase();

    MoveBase& operator=(const MoveBase &other);

    static const char* typeName()
    {
        return "SireMove::MoveBase";
    }

    virtual const char* what() const=0;

    virtual MoveBase* clone() const=0;

    virtual void initialise(QuerySystem &system)=0;

    virtual void move(SimSystem &system)=0;

protected:
    MoveBase();
    MoveBase(const MoveBase &other);
};

/** This is a convienient wrapper for SharedPolyPointer<MoveBase>

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT Move
{

friend QDataStream& ::operator<<(QDataStream&, const Move&);
friend QDataStream& ::operator>>(QDataStream&, Move&);

public:
    Move();

    Move(const MoveBase &move);

    Move(const SireBase::SharedPolyPointer<MoveBase> &ptr);

    Move(const Move &move);

    ~Move();

    Move& operator=(const MoveBase &move);
    Move& operator=(const SireBase::SharedPolyPointer<MoveBase> &ptr);
    Move& operator=(const Move &move);

    void initialise(QuerySystem &system);

    void move(SimSystem &system);

private:
    /** Shared pointer to the actual move object */
    SireBase::SharedPolyPointer<MoveBase> d;
};

/** Initialise the move to work on the Simulation system 'system' */
inline void Move::initialise(QuerySystem &system)
{
    d->initialise(system);
}

/** Perform the move on the simulation system */
inline void Move::move(SimSystem &system)
{
    d->move(system);
}

}

Q_DECLARE_METATYPE(SireSystem::Move);

SIRE_END_HEADER

#endif
