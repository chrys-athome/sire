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

#include "move.h"

#include "SireSystem/system.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireBase;
using namespace SireSystem;
using namespace SireStream;

///////
/////// Implementation of MoveBase
///////

static const RegisterMetaType<MoveBase> r_movebase( MAGIC_ONLY,
                                                    "SireMove::MoveBase" );

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const MoveBase &movebase)
{
    writeHeader(ds, r_movebase, 1);
    
    ds << static_cast<const PropertyBase&>(movebase);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MoveBase &movebase)
{
    VersionID v = readHeader(ds, r_movebase);
    
    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(movebase);
    }
    else
        throw version_error(v, "1", r_movebase, CODELOC);
        
    return ds;
}

/** Constructor */
MoveBase::MoveBase() : PropertyBase()
{}

/** Copy constructor */
MoveBase::MoveBase(const MoveBase &other) : PropertyBase(other)
{}

/** Destructor */
MoveBase::~MoveBase()
{}

/** Perform a single move on the system 'system' */
void MoveBase::move(System &system)
{
    this->move(system, 1);
}

///////
/////// Implementation of Move
///////

static const RegisterMetaType<Move> r_move;

/** Serialise a Move to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Move &move)
{
    writeHeader(ds, r_move, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(move);
    
    return ds;
}

/** Deserialise a Move from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Move &move)
{
    VersionID v = readHeader(ds, r_move);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(move);
    }
    else
        throw version_error(v, "1", r_move, CODELOC);
        
    return ds;
}

static Move *_pvt_shared_null = 0;

const Move& Move::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new Move( NullMove() );
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, empty MoveBase */
Move::Move() : Property(Move::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
Move::Move(const PropertyBase &property) : Property(property.asA<MoveBase>())
{}

/** Construct from passed MoveBase */
Move::Move(const MoveBase &move) : Property(move)
{}

/** Copy constructor */
Move::Move(const Move &other) : Property(other)
{}

/** Destructor */
Move::~Move()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
Move& Move::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<MoveBase>());
    return *this;
}

/** Copy assignment operator from another MoveBase */
Move& Move::operator=(const MoveBase &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const MoveBase* Move::operator->() const
{
    return static_cast<const MoveBase*>(&(d()));
}

/** Dereference this pointer */
const MoveBase& Move::operator*() const
{
    return static_cast<const MoveBase&>(d());
}

/** Return a read-only reference to the contained object */
const MoveBase& Move::read() const
{
    return static_cast<const MoveBase&>(d());
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
MoveBase& Move::edit()
{
    return static_cast<MoveBase&>(d());
}
    
/** Return a raw pointer to the MoveBase object */
const MoveBase* Move::data() const
{
    return static_cast<const MoveBase*>(&(d()));
}

/** Return a raw pointer to the MoveBase object */
const MoveBase* Move::constData() const
{
    return static_cast<const MoveBase*>(&(d()));
}
    
/** Return a raw pointer to the MoveBase object */
MoveBase* Move::data()
{
    return static_cast<MoveBase*>(&(d()));
}

/** Automatic casting operator */
Move::operator const MoveBase&() const
{
    return static_cast<const MoveBase&>(d());
}

///////
/////// Implementation of NullMove
///////

static const RegisterMetaType<NullMove> r_nullmove;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const NullMove &nullmove)
{
    writeHeader(ds, r_nullmove, 1);
    ds << static_cast<const MoveBase&>(nullmove);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, NullMove &nullmove)
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

/** Constructor */
NullMove::NullMove() : ConcreteProperty<NullMove,MoveBase>()
{}

/** Copy constructor */
NullMove::NullMove(const NullMove &other)
         : ConcreteProperty<NullMove,MoveBase>(other)
{}

/** Destructor */
NullMove::~NullMove()
{}

/** Copy assignment operator */
NullMove& NullMove::operator=(const NullMove &other)
{
    return *this;
}

/** Comparison operator */
bool NullMove::operator==(const NullMove &other) const
{
    return true;
}

/** Comparison operator */
bool NullMove::operator!=(const NullMove &other) const
{
    return false;
}

/** NullMove doesn't perform any moves - no matter how hard you try! */
void NullMove::move(System &system, int nmoves)
{
    return;
}
