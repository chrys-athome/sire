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
#include "ensemble.h"

#include "SireUnits/units.h"
#include "SireUnits/temperature.h"

#include "SireSystem/system.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireBase;
using namespace SireUnits;
using namespace SireUnits::Dimension;
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
    
    ds << movebase.nrgcomponent
       << static_cast<const PropertyBase&>(movebase);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MoveBase &movebase)
{
    VersionID v = readHeader(ds, r_movebase);
    
    if (v == 1)
    {
        ds >> movebase.nrgcomponent
           >> static_cast<PropertyBase&>(movebase);
    }
    else
        throw version_error(v, "1", r_movebase, CODELOC);
        
    return ds;
}

/** Constructor */
MoveBase::MoveBase() 
         : PropertyBase(), nrgcomponent(ForceFields::totalComponent())
{}

/** Copy constructor */
MoveBase::MoveBase(const MoveBase &other) 
         : PropertyBase(other), nrgcomponent(other.nrgcomponent)
{}

/** Destructor */
MoveBase::~MoveBase()
{}

/** Copy assignment operator */
MoveBase& MoveBase::operator=(const MoveBase &other)
{
    nrgcomponent = other.nrgcomponent;
    PropertyBase::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool MoveBase::operator==(const MoveBase &other) const
{
    return nrgcomponent == other.nrgcomponent;
}

/** Comparison operator */
bool MoveBase::operator!=(const MoveBase &other) const
{
    return nrgcomponent != other.nrgcomponent;
}

/** Perform a single move on the system 'system' without 
    recording any statistics */
void MoveBase::move(System &system)
{
    this->move(system, 1, false);
}

/** Perform 'n' moves on the system without recording any 
    statistics */
void MoveBase::move(System &system, int nmoves)
{
    this->move(system, nmoves, false);
}

/** Set the energy component that describes the Hamiltonian that this move
    will sample */
void MoveBase::setEnergyComponent(const Symbol &component)
{
    nrgcomponent = component;
}

/** Return the symbol that describes the Hamiltonian that this move 
    will sample */
const Symbol& MoveBase::energyComponent() const
{
    return nrgcomponent;
}

/** Return whether or not this move keeps the total energy constant */
bool MoveBase::isConstantEnergy() const
{
    return this->ensemble().isConstantEnergy();
}

/** Return whether or not this move keeps the temperature constant */
bool MoveBase::isConstantTemperature() const
{
    return this->ensemble().isConstantTemperature();
}

/** Return whether or not this move keeps the volume constant */
bool MoveBase::isConstantVolume() const
{
    return this->ensemble().isConstantVolume();
}

/** Return whether or not this move keeps the pressure constant */
bool MoveBase::isConstantPressure() const
{
    return this->ensemble().isConstantPressure();
}

/** Return whether or not this move keeps the chemical potential constant */
bool MoveBase::isConstantChemicalPotential() const
{
    return this->ensemble().isConstantChemicalPotential();
}

/** Return whether or not this move keeps the fugacity (related to the chemical
    potential) constant */
bool MoveBase::isConstantFugacity() const
{
    return this->ensemble().isConstantFugacity();
}

/** Return the constant temperature that this move samples

    \throw SireError::incompatible_error
*/
Temperature MoveBase::temperature() const
{
    if (not this->isConstantTemperature())
        throw SireError::incompatible_error( QObject::tr(
            "The move %1 does not have a constant temperature as it "
            "samples from the %2")
                .arg( this->what() ).arg(this->ensemble().toString()),
                    CODELOC );
                    
    return this->ensemble().temperature();
}

/** Return the constant pressure that this move samples 

    \throw SireError::incompatible_error
*/
Pressure MoveBase::pressure() const
{
    if (not this->isConstantPressure())
        throw SireError::incompatible_error( QObject::tr(
            "The move %1 does not have a constant pressure as it "
            "samples from the %2")
                .arg( this->what() ).arg(this->ensemble().toString()),
                    CODELOC );
                    
    return this->ensemble().pressure();
}

/** Return the constant fugacity that this move samples 

    \throw SireError::incompatible_error
*/
Pressure MoveBase::fugacity() const
{
    if (not this->isConstantFugacity())
        throw SireError::incompatible_error( QObject::tr(
            "The move %1 does not have a constant fugacity as it "
            "samples from the %2")
                .arg( this->what() ).arg(this->ensemble().toString()),
                    CODELOC );
                    
    return this->ensemble().fugacity();
}

/** Return the constant chemical potential that this move samples 

    \throw SireError::incompatible_error
*/
MolarEnergy MoveBase::chemicalPotential() const
{
    if (not this->isConstantChemicalPotential())
        throw SireError::incompatible_error( QObject::tr(
            "The move %1 does not have a constant chemical potential as it "
            "samples from the %2")
                .arg( this->what() ).arg(this->ensemble().toString()),
                    CODELOC );
                    
    return this->ensemble().chemicalPotential();
}

/** Return whether or not this move keeps the symbol 'lam' constant */
bool MoveBase::isConstantLambda(const Symbol&) const
{
    return true;
}

/** Set the temperature - this should never be called! */
void MoveBase::_pvt_setTemperature(const Temperature &temperature)
{
    throw SireError::program_bug( QObject::tr(
        "Cannot set the temperature to %1 C in %2, as, despite the move promising "
        "it samples the %3, it doesn't provide a function to set the temperature!")
            .arg(temperature.to(celsius))
            .arg(this->what())
            .arg(this->ensemble().toString()), CODELOC );
}
                        
/** Set the pressure - this should never be called! */
void MoveBase::_pvt_setPressure(const Pressure &pressure)
{
    throw SireError::program_bug( QObject::tr(
        "Cannot set the pressure to %1 atm in %2, as, despite the move promising "
        "it samples the %3, it doesn't provide a function to set the pressure!")
            .arg(pressure.to(atm))
            .arg(this->what())
            .arg(this->ensemble().toString()), CODELOC );
}
                       
/** Set the pressure - this should never be called! */
void MoveBase::_pvt_setFugacity(const Pressure &fugacity)
{
    throw SireError::program_bug( QObject::tr(
        "Cannot set the fugacity to %1 bar in %2, as, despite the move promising "
        "it samples the %3, it doesn't provide a function to set the fugacity!")
            .arg(fugacity.to(bar))
            .arg(this->what())
            .arg(this->ensemble().toString()), CODELOC );
}

/** Set the temperature that this constant temperature move samples 
    to 'temperature'
    
    \throw SireError::incompatible_error
*/
void MoveBase::setTemperature(const Temperature &temperature)
{
    if (not this->isConstantTemperature())
        throw SireError::incompatible_error( QObject::tr(
            "The move %1 cannot not have a constant temperature (%2 C) as it "
            "samples from the %3")
                .arg( this->what() )
                .arg( temperature.to(celsius) )
                .arg(this->ensemble().toString()),
                    CODELOC );

    this->_pvt_setTemperature(temperature);
}

/** Set the pressure that this constant pressure move samples 
    to 'temperature'
    
    \throw SireError::incompatible_error
*/
void MoveBase::setPressure(const SireUnits::Dimension::Pressure &pressure)
{
    if (not this->isConstantPressure())
        throw SireError::incompatible_error( QObject::tr(
            "The move %1 cannot not have a constant pressure (%2 atm) as it "
            "samples from the %3")
                .arg( this->what() )
                .arg( pressure.to(atm) )
                .arg(this->ensemble().toString()),
                    CODELOC );

    this->_pvt_setPressure(pressure);
}

/** Set the chemical potential that this constant chemical potential move samples 
    to 'temperature'
    
    \throw SireError::incompatible_error
*/
void MoveBase::setChemicalPotential(const MolarEnergy &chemical_potential)
{
    if (not this->isConstantChemicalPotential())
        throw SireError::incompatible_error( QObject::tr(
            "The move %1 cannot not have a constant chemical potential "
            "(%2 kcal mol-1) as it "
            "samples from the %3")
                .arg( this->what() )
                .arg(chemical_potential.to(kcal_per_mol) )
                .arg(this->ensemble().toString()),
                    CODELOC );

    this->_pvt_setFugacity( Ensemble::MuVT(this->temperature(), chemical_potential)
                                        .fugacity() );
}

/** Set the fugacity that this constant fugacity move samples 
    to 'temperature'
    
    \throw SireError::incompatible_error
*/
void MoveBase::setFugacity(const Pressure &fugacity)
{
    if (not this->isConstantFugacity())
        throw SireError::incompatible_error( QObject::tr(
            "The move %1 cannot not have a constant fugacity "
            "(%2 bar) as it "
            "samples from the %3")
                .arg( this->what() )
                .arg(fugacity.to(bar) )
                .arg(this->ensemble().toString()),
                    CODELOC );

    this->_pvt_setFugacity( fugacity );
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
void NullMove::move(System &system, int nmoves, bool record_stats)
{
    return;
}

/** NullMove doesn't change anything (so must be NVE) */
Ensemble NullMove::ensemble() const
{
    return Ensemble::NVE();
}
