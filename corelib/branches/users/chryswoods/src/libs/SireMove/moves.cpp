/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "moves.h"
#include "simulation.h"
#include "simcontroller.h"

#include "SireSystem/system.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMove;
using namespace SireSystem;
using namespace SireBase;
using namespace SireStream;

///////
/////// Implementation of MovesBase
///////

static const RegisterMetaType<MovesBase> r_movesbase( MAGIC_ONLY,
                                                      "SireMove::MovesBase" );
                                                      
/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const MovesBase &movesbase)
{
    writeHeader(ds, r_movesbase, 1);
    
    ds << static_cast<const PropertyBase&>(movesbase);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MovesBase &movesbase)
{
    VersionID v = readHeader(ds, r_movesbase);
    
    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(movesbase);
    }
    else
        throw version_error(v, "1", r_movesbase, CODELOC);
        
    return ds;
}

/** Constructor */
MovesBase::MovesBase() : PropertyBase()
{}

/** Copy constructor */
MovesBase::MovesBase(const MovesBase &other)
          : PropertyBase(other)
{}

/** Destructor */
MovesBase::~MovesBase()
{}

/** Apply 'nmoves' moves on the system 'system', returning the system
    after the moves. Move statistics are not recorded */
System MovesBase::move(const System &system, int nblocks)
{
    return this->move(system, nblocks, false);
}

/** Apply a single move to the system 'system' and return the result.
    No statistics are collected */
System MovesBase::move(const System &system)
{
    return this->move(system, 1, false);
}

///////
/////// Implementation of SameMoves
///////

static const RegisterMetaType<SameMoves> r_samemoves;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const SameMoves &samemoves)
{
    writeHeader(ds, r_samemoves, 1);
    
    SharedDataStream sds(ds);
    
    sds << samemoves.mv << static_cast<const MovesBase&>(samemoves);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, SameMoves &samemoves)
{
    VersionID v = readHeader(ds, r_samemoves);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> samemoves.mv >> static_cast<MovesBase&>(samemoves);
    }
    else
        throw version_error(v, "1", r_samemoves, CODELOC);
        
    return ds;
}

/** Constructor */
SameMoves::SameMoves() : ConcreteProperty<SameMoves,MovesBase>()
{}

/** Construct to run the move 'move' multiple times */
SameMoves::SameMoves(const MoveBase &move)
          : ConcreteProperty<SameMoves,MovesBase>(), mv(move)
{}

/** Copy constructor */
SameMoves::SameMoves(const SameMoves &other)
          : ConcreteProperty<SameMoves,MovesBase>(other), mv(other.mv)
{}

/** Destructor */
SameMoves::~SameMoves()
{}

/** Copy assignment operator */
SameMoves& SameMoves::operator=(const SameMoves &other)
{
    mv = other.mv;
    MovesBase::operator=(other);
    return *this;
}

/** Comparison operator */
bool SameMoves::operator==(const SameMoves &other) const
{
    return mv == other.mv;
}

/** Comparison operator */
bool SameMoves::operator!=(const SameMoves &other) const
{
    return mv != other.mv;
}

/** Apply the move 'nmoves' times to the system 'system', returning
    the result */
System SameMoves::move(const System &system, int nmoves, bool record_stats)
{
    SameMoves old_state(*this);
    System new_system(system);
    
    try
    {
        mv.edit().move(new_system, nmoves, record_stats);
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
    
    return new_system;
}

/** Apply the move 'nmoves' times to the system 'system', returning
    the result */
System SameMoves::move(const System &system, int nmoves, bool record_stats,
                       SimController &controller)
{
    qDebug() << CODELOC;

    if (nmoves == 0)
        return system;

    qDebug() << CODELOC;

    SameMoves old_state(*this);

    qDebug() << CODELOC;

    System new_system(system);
    
    try
    {
        qDebug() << CODELOC;

        controller.initialise(new_system, *this, nmoves, record_stats);

        qDebug() << CODELOC;
    
        MoveBase *move = &(mv.edit());

        qDebug() << move;
        qDebug() << move->what();
        qDebug() << CODELOC;
    
        do
        {
            qDebug() << CODELOC;
            move->move(new_system, 1, record_stats);
            qDebug() << CODELOC;
        }
        while (controller.nextMove());

        qDebug() << CODELOC;
    }
    catch(...)
    {
        qDebug() << CODELOC;
        this->operator=(old_state);
        qDebug() << CODELOC;
        throw;
    }

    qDebug() << CODELOC;
        
    if (not controller.aborted())
    {
        qDebug() << CODELOC;
        return new_system;
    }
    else
    {
        qDebug() << CODELOC;
        this->operator=(old_state);
        qDebug() << CODELOC;
        return system;
    }
}

/** Return the moves used by this object */
QList<Move> SameMoves::moves() const
{
    QList<Move> mvs;
    mvs.append(mv);
    
    return mvs;
}

///////
/////// Implementation of Moves
///////

static const RegisterMetaType<Moves> r_moves;

/** Serialise a Moves to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Moves &moves)
{
    writeHeader(ds, r_moves, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(moves);
    
    return ds;
}

/** Deserialise a Moves from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Moves &moves)
{
    VersionID v = readHeader(ds, r_moves);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(moves);
    }
    else
        throw version_error(v, "1", r_moves, CODELOC);
        
    return ds;
}

static Moves *_pvt_shared_null = 0;

const Moves& Moves::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new Moves( SameMoves() );
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, empty MovesBase */
Moves::Moves() : Property(Moves::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
Moves::Moves(const PropertyBase &property) : Property(property.asA<MovesBase>())
{}

/** Construct from passed MovesBase */
Moves::Moves(const MovesBase &moves) : Property(moves)
{}

/** Copy constructor */
Moves::Moves(const Moves &other) : Property(other)
{}

/** Destructor */
Moves::~Moves()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
Moves& Moves::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<MovesBase>());
    return *this;
}

/** Copy assignment operator from another MovesBase */
Moves& Moves::operator=(const MovesBase &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const MovesBase* Moves::operator->() const
{
    return static_cast<const MovesBase*>(&(d()));
}

/** Dereference this pointer */
const MovesBase& Moves::operator*() const
{
    return static_cast<const MovesBase&>(d());
}

/** Return a read-only reference to the contained object */
const MovesBase& Moves::read() const
{
    return static_cast<const MovesBase&>(d());
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
MovesBase& Moves::edit()
{
    return static_cast<MovesBase&>(d());
}
    
/** Return a raw pointer to the MovesBase object */
const MovesBase* Moves::data() const
{
    return static_cast<const MovesBase*>(&(d()));
}

/** Return a raw pointer to the MovesBase object */
const MovesBase* Moves::constData() const
{
    return static_cast<const MovesBase*>(&(d()));
}
    
/** Return a raw pointer to the MovesBase object */
MovesBase* Moves::data()
{
    return static_cast<MovesBase*>(&(d()));
}

/** Automatic casting operator */
Moves::operator const MovesBase&() const
{
    return static_cast<const MovesBase&>(d());
}
