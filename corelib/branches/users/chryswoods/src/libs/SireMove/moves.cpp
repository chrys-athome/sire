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
#include "ensemble.h"

#include "SireUnits/units.h"
#include "SireUnits/temperature.h"
#include "SireUnits/dimensions.h"

#include "SireSystem/system.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMove;
using namespace SireSystem;
using namespace SireCAS;
using namespace SireUnits;
using namespace SireUnits::Dimension;
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

/** Return the energy component that these moves will sample. This
    raises an error if not all moves are sampling the same energy
    component 
    
    \throw SireError::incompatible_error
*/
const Symbol& MovesBase::energyComponent() const
{
    QList<Move> mvs = this->moves();
    
    const Symbol &component = mvs.takeFirst()->energyComponent();
    
    for (QList<Move>::const_iterator it = mvs.constBegin();
         it != mvs.constEnd();
         ++it)
    {
        if ((*it)->energyComponent() != component)
        {
            QSet<Symbol> symbols;
            symbols.insert(component);
        
            for (QList<Move>::const_iterator it2 = mvs.constBegin();
                 it2 != mvs.constEnd();
                 ++it2)
            {
                symbols.insert( (*it2)->energyComponent() );
            }
        
            throw SireError::incompatible_error( QObject::tr(
                "Not all moves sample the same energy component. "
                "The energy components sampled are %1.")
                    .arg( Sire::toString(symbols.toList()) ), CODELOC );
        }
    }
    
    return component;
}

/** Return the name of the property of the system space that
    these moves use when they manipulate the simulation box. This
    raises an error if not all moves are using the same property
    
    \throw SireError::incompatible_error
*/
const PropertyName& MovesBase::spaceProperty() const
{
    QList<Move> mvs = this->moves();
    
    const PropertyName &property = mvs.takeFirst()->spaceProperty();
    
    for (QList<Move>::const_iterator it = mvs.constBegin();
         it != mvs.constEnd();
         ++it)
    {
        if ((*it)->spaceProperty() != property)
        {
            QSet<QString> properties;
            properties.insert( property.toString() );
        
            for (QList<Move>::const_iterator it2 = mvs.constBegin();
                 it2 != mvs.constEnd();
                 ++it2)
            {
                properties.insert( (*it2)->spaceProperty().toString() );
            }
        
            throw SireError::incompatible_error( QObject::tr(
                "Not all moves use the same System space property. "
                "The properties used are are %1.")
                    .arg( Sire::toString(properties.toList()) ), CODELOC );
        }
    }
    
    return property;
}

/** Return the ensemble that will be sampled by these moves */
Ensemble MovesBase::ensemble() const
{
    QList<Move> mvs = this->moves();
    
    Ensemble merged = mvs.takeFirst()->ensemble();
    
    for (QList<Move>::const_iterator it = mvs.constBegin();
         it != mvs.constEnd();
         ++it)
    {
        merged = Ensemble::merge(merged, (*it)->ensemble());
    }
    
    return merged;
}

/** Return whether or not these moves keeps the total energy constant */
bool MovesBase::isConstantEnergy() const
{
    return this->ensemble().isConstantEnergy();
}

/** Return whether or not these moves keep the temperature constant */
bool MovesBase::isConstantTemperature() const
{
    return this->ensemble().isConstantTemperature();
}

/** Return whether or not these moves keep the volume constant */
bool MovesBase::isConstantVolume() const
{
    return this->ensemble().isConstantVolume();
}

/** Return whether or not these moves keep the pressure constant */
bool MovesBase::isConstantPressure() const
{
    return this->ensemble().isConstantPressure();
}

/** Return whether or not these moves keep the chemical potential constant */
bool MovesBase::isConstantChemicalPotential() const
{
    return this->ensemble().isConstantChemicalPotential();
}

/** Return whether or not these moves keep the fugacity (related to the chemical
    potential) constant */
bool MovesBase::isConstantFugacity() const
{
    return this->ensemble().isConstantFugacity();
}

/** Return the constant temperature that these moves sample

    \throw SireError::incompatible_error
*/
Temperature MovesBase::temperature() const
{
    if (not this->isConstantTemperature())
        throw SireError::incompatible_error( QObject::tr(
            "These moves do not have a constant temperature as they "
            "sample from the %1")
                .arg(this->ensemble().toString()),
                    CODELOC );
                    
    return this->ensemble().temperature();
}

/** Return the constant pressure that these moves sample 

    \throw SireError::incompatible_error
*/
Pressure MovesBase::pressure() const
{
    if (not this->isConstantPressure())
        throw SireError::incompatible_error( QObject::tr(
            "Theses moves do not have a constant pressure as they "
            "sample from the %1")
                .arg( this->ensemble().toString() ),
                    CODELOC );
                    
    return this->ensemble().pressure();
}

/** Return the constant fugacity that these moves sample 

    \throw SireError::incompatible_error
*/
Pressure MovesBase::fugacity() const
{
    if (not this->isConstantFugacity())
        throw SireError::incompatible_error( QObject::tr(
            "These moves do not have a constant fugacity as they "
            "sample from the %1")
                .arg( this->ensemble().toString() ),
                    CODELOC );
                    
    return this->ensemble().fugacity();
}

/** Return the constant chemical potential that these moves sample 

    \throw SireError::incompatible_error
*/
MolarEnergy MovesBase::chemicalPotential() const
{
    if (not this->isConstantChemicalPotential())
        throw SireError::incompatible_error( QObject::tr(
            "These moves do not have a constant chemical potential as they "
            "samples from the %1")
                .arg( this->ensemble().toString() ),
                    CODELOC );
                    
    return this->ensemble().chemicalPotential();
}

/** Return whether or not these moves keep the symbol 'lam' constant */
bool MovesBase::isConstantLambda(const Symbol &lam) const
{
    QList<Move> mvs = this->moves();
    
    for (QList<Move>::const_iterator it = mvs.constBegin();
         it != mvs.constEnd();
         ++it)
    {
        if (not (*it)->isConstantLambda(lam))
            return false;
    }
    
    return true;
}

/** Set the temperature that these constant temperature moves sample 
    to 'temperature'
    
    \throw SireError::incompatible_error
*/
void MovesBase::setTemperature(const Temperature &temperature)
{
    if (not this->isConstantTemperature())
        throw SireError::incompatible_error( QObject::tr(
            "These moves cannot not have a constant temperature (%1 C) as they "
            "sample from the %2")
                .arg( temperature.to(celsius) )
                .arg(this->ensemble().toString()),
                    CODELOC );

    this->_pvt_setTemperature(temperature);
}

/** Set the pressure that these constant pressure moves sample 
    to 'pressure'
    
    \throw SireError::incompatible_error
*/
void MovesBase::setPressure(const SireUnits::Dimension::Pressure &pressure)
{
    if (not this->isConstantPressure())
        throw SireError::incompatible_error( QObject::tr(
            "These moves cannot not have a constant pressure (%1 atm) as they "
            "sample from the %2")
                .arg( pressure.to(atm) )
                .arg(this->ensemble().toString()),
                    CODELOC );

    this->_pvt_setPressure(pressure);
}

/** Set the chemical potential that these constant chemical potential moves sample 
    to 'chemical_potential'
    
    \throw SireError::incompatible_error
*/
void MovesBase::setChemicalPotential(const MolarEnergy &chemical_potential)
{
    if (not this->isConstantChemicalPotential())
        throw SireError::incompatible_error( QObject::tr(
            "These moves cannot not have a constant chemical potential "
            "(%1 kcal mol-1) as they "
            "sample from the %2")
                .arg(chemical_potential.to(kcal_per_mol) )
                .arg(this->ensemble().toString()),
                    CODELOC );

    this->_pvt_setFugacity( Ensemble::MuVT(this->temperature(), chemical_potential)
                                        .fugacity() );
}

/** Set the fugacity that these constant fugacity moves sample 
    to 'fugacity'
    
    \throw SireError::incompatible_error
*/
void MoveBase::setFugacity(const Pressure &fugacity)
{
    if (not this->isConstantFugacity())
        throw SireError::incompatible_error( QObject::tr(
            "These moves cannot not have a constant fugacity "
            "(%1 bar) as they "
            "samples from the %2")
                .arg(fugacity.to(bar) )
                .arg(this->ensemble().toString()),
                    CODELOC );

    this->_pvt_setFugacity( fugacity );
}

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

/** Set the energy component of all of the moves to 'component' */
void SameMoves::setEnergyComponent(const Symbol &component)
{
    if (mv.read().energyComponent() != component)
    {
        mv.edit().setEnergyComponent(component);
    }
}

/** Set the name of the property that all of the moves will use to 
    find the simulation space (simulation box) to 'spaceproperty' */
void SameMoves::setSpaceProperty(const PropertyName &spaceproperty)
{
    if (mv.read().spaceProperty() != spaceproperty)
    {
        mv.edit().setSpaceProperty(spaceproperty);
    }
}

/** Set the temperature for all moves that have a constant temperature
    to 'temperature'. It has already been checked that these moves
    between them sample at constant temperature */
void SameMoves::_pvt_setTemperature(const Temperature &temperature)
{
    if (mv.read().temperature() != temperature)
    {
        mv.edit().setTemperature(temperature);
    }
}

/** Set the pressure for all moves that have a constant pressure
    to 'pressure'. It has already been checked that these moves
    between them sample at constant pressure */
void SameMoves::_pvt_setPressure(const Pressure &pressure)
{
    if (mv.read().pressure() != pressure)
    {
        mv.edit().setPressure(pressure);
    }
}

/** Set the fugacity for all moves that have a constant fugacity
    to 'fugacity'. It has already been checked that these moves
    between them sample at constant fugacity */
void SameMoves::_pvt_setFugacity(const Pressure &fugacity)
{
    if (mv.read().fugacity() != fugacity)
    {
        mv.edit().setFugacity(fugacity);
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
