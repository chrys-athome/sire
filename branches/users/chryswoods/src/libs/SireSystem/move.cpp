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

#include "move.h"

#include "SireFF/ffbase.h"

#include "SireSystem/querysystem.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireBase;
using namespace SireFF;
using namespace SireStream;

///////////
/////////// Implementation of MoveBase
///////////

static const RegisterMetaType<MoveBase> r_movebase(MAGIC_ONLY,
                                                   "SireSystem::MoveBase");

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const MoveBase&)
{
    writeHeader(ds, r_movebase, 0);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, MoveBase&)
{
    VersionID v = readHeader(ds, r_movebase);

    if (v != 0)
        throw version_error(v, "0", r_movebase, CODELOC);

    return ds;
}

/** Constructor */
MoveBase::MoveBase() : QSharedData(), nrg_component( SireFF::e_total() )
{}

/** Construct a move that follows potential energy surface 
    represented by 'symbol' */
MoveBase::MoveBase(const Symbol &component) 
         : QSharedData(), nrg_component(component)
{}

/** Copy constructor */
MoveBase::MoveBase(const MoveBase &other)
         : QSharedData(), nrg_component(other.nrg_component)
{}

/** Destructor */
MoveBase::~MoveBase()
{}

/** Assignment operator */
MoveBase& MoveBase::operator=(const MoveBase &other)
{
    nrg_component = other.nrg_component;
    return *this;
}

/** Return the energy component that represents the potential
    energy surface that this move will follow. */
const Symbol& MoveBase::energyComponent() const
{
    return nrg_component;
}

/** Set the energy component that will represent the 
    potential energy surface that this move will follow */
void MoveBase::setEnergyComponent(const Symbol &symbol)
{
    nrg_component = symbol;
}

/** Initialise this move for the system - this ensures that 
    the system has the energy component that this move will
    be sampling */
void MoveBase::assertCompatibleWith(QuerySystem &system) const
{
    system.assertContains(nrg_component);
}

/** Return the energy on the potential energy surface that
    this move follows of the passed system */
double MoveBase::energy(QuerySystem &system) const
{
    return system.energy(nrg_component);
}

///////////
/////////// Implementation of detail::NullMove
///////////

namespace SireSystem
{

namespace detail
{

/** This is a null move, used to allow a functional, though
    ineffective null 'Move' object

    @author Christopher Woods
*/
class NullMove : public MoveBase
{
public:
    NullMove() : MoveBase()
    {}

    NullMove(const NullMove &other) : MoveBase(other)
    {}

    ~NullMove()
    {}

    NullMove& operator=(const NullMove &other)
    {
        MoveBase::operator=(other);
        return *this;
    }

    static const char* typeName()
    {
        return "SireSystem::detail::NullMove";
    }

    const char* what() const
    {
        return NullMove::typeName();
    }

    NullMove* clone() const
    {
        return new NullMove(*this);
    }

    void initialise(QuerySystem&)
    {}

    void move(SimSystem&)
    {}
};

} // end of SireSystem::detail

} // end of SireSystem

Q_DECLARE_METATYPE(SireSystem::detail::NullMove);

using namespace SireSystem::detail;

static const RegisterMetaType<NullMove> r_nullmove;

/** Serialise to a binary datastream */
QDataStream &operator<<(QDataStream &ds, const NullMove &nullmove)
{
    writeHeader(ds, r_nullmove, 1)
        << static_cast<const MoveBase&>(nullmove);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream &operator>>(QDataStream &ds, NullMove &nullmove)
{
    VersionID v = readHeader(ds, r_nullmove);

    if (v == 1)
    {
        ds >> static_cast<MoveBase&>(nullmove);
    }
    else
        throw version_error(v, "1", r_nullmove, CODELOC);

    return ds;
}

static SharedPolyPointer<MoveBase> shared_null( new NullMove() );

///////////
/////////// Implementation of Move
///////////

static const RegisterMetaType<Move> r_move;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const Move &move)
{
    writeHeader(ds, r_move, 1);

    SharedDataStream(ds) << move.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, Move &move)
{
    VersionID v = readHeader(ds, r_move);

    if (v == 1)
    {
        SharedDataStream(ds) >> move.d;
    }
    else
        throw version_error(v, "1", r_move, CODELOC);

    return ds;
}

/** Null constructor */
Move::Move() : d(shared_null)
{}

/** Construct from the passed move */
Move::Move(const MoveBase &move) : d(move)
{}

/** Copy constructor */
Move::Move(const Move &other) : d(other.d)
{}

/** Destructor */
Move::~Move()
{}

/** Assign from a move object */
Move& Move::operator=(const MoveBase &move)
{
    d = move;

    return *this;
}

/** Copy assignment */
Move& Move::operator=(const Move &other)
{
    d = other.d;
    return *this;
}

/** Set the energy component that will represent the 
    potential energy surface that this move will follow */
void Move::setEnergyComponent(const Symbol &component)
{
    d->setEnergyComponent(component);
}
    
/** Return the energy component that represents the potential
    energy surface that this move will follow. */
Symbol Move::energyComponent() const
{
    return d->energyComponent();
}
