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

#ifndef SIREMOVE_MOVES_H
#define SIREMOVE_MOVES_H

#include <QList>

#include "move.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class MovesBase;
class Moves;
class SameMoves;
}

QDataStream &operator<<(QDataStream&, const SireMove::MovesBase&);
QDataStream &operator>>(QDataStream&, SireMove::MovesBase&);

QDataStream &operator<<(QDataStream&, const SireMove::Moves&);
QDataStream &operator>>(QDataStream&, SireMove::Moves&);

QDataStream &operator<<(QDataStream&, const SireMove::SameMoves&);
QDataStream &operator>>(QDataStream&, SireMove::SameMoves&);

namespace SireMove
{

using SireCAS::Symbol;

/** This is the base class of all Moves objects. These are objects
    that contain a collection of moves that are applied to a system
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT MovesBase : public SireBase::PropertyBase
{
public:
    MovesBase();
    
    MovesBase(const MovesBase &other);
    
    virtual ~MovesBase();
    
    virtual MovesBase* clone() const=0;
    
    static const char* typeName()
    {
        return "SireMove::MovesBase";
    }
    
    virtual void setEnergyComponent(const Symbol &component)=0;
    const Symbol& energyComponent() const;

    virtual void setSpaceProperty(const PropertyName &spaceproperty)=0;
    const PropertyName& spaceProperty() const;
    
    Ensemble ensemble() const;
    
    bool isConstantEnergy() const;
    bool isConstantTemperature() const;
    bool isConstantVolume() const;
    bool isConstantPressure() const;
    bool isConstantChemicalPotential() const;
    bool isConstantFugacity() const;

    bool isConstantLambda(const Symbol &lam) const;
    
    SireUnits::Dimension::Temperature temperature() const;
    SireUnits::Dimension::Pressure pressure() const;
    SireUnits::Dimension::Pressure fugacity() const;
    SireUnits::Dimension::MolarEnergy chemicalPotential() const;

    void setTemperature(const SireUnits::Dimension::Temperature &temperature);
    void setPressure(const SireUnits::Dimension::Pressure &pressure);
    void setChemicalPotential(
                    const SireUnits::Dimension::MolarEnergy &chemical_potential);
    void setFugacity(const SireUnits::Dimension::Pressure &fugacity);
        
    virtual System move(const System &system, int nmoves,
                        bool record_stats)=0;
    
    System move(const System &system, int nmoves);
    System move(const System &system);
    
    virtual QList<Move> moves() const=0;

protected:
    /** Set the temperature for all moves that have a constant temperature
        to 'temperature'. It has already been checked that these moves
        between them sample at constant temperature */
    virtual void _pvt_setTemperature(
                            const SireUnits::Dimension::Temperature &temperature)=0;
                            
    /** Set the pressure for all moves that have a constant pressure
        to 'pressure'. It has already been checked that these moves
        between them sample at constant pressure */
    virtual void _pvt_setPressure(
                            const SireUnits::Dimension::Pressure &pressure)=0;
                            
    /** Set the fugacity for all moves that have a constant fugacity
        to 'fugacity'. It has already been checked that these moves
        between them sample at constant fugacity */
    virtual void _pvt_setFugacity(
                            const SireUnits::Dimension::Pressure &fugacity)=0;
};

/** This is a Moves class that just applies the same move over and
    over again
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT SameMoves 
         : public SireBase::ConcreteProperty<SameMoves,MovesBase>
{

friend QDataStream& ::operator<<(QDataStream&, const SameMoves&);
friend QDataStream& ::operator>>(QDataStream&, SameMoves&);

public:
    SameMoves();
    SameMoves(const MoveBase &move);
    
    SameMoves(const SameMoves &other);
    
    ~SameMoves();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<SameMoves>() );
    }
    
    SameMoves* clone() const
    {
        return new SameMoves(*this);
    }
    
    SameMoves& operator=(const SameMoves &other);
    
    bool operator==(const SameMoves &other) const;
    bool operator!=(const SameMoves &other) const;
    
    using MovesBase::move;
    
    void setEnergyComponent(const Symbol &component);
    void setSpaceProperty(const PropertyName &spaceproperty); 
    
    System move(const System &system, int nmoves, bool record_stats);
    
    QList<Move> moves() const;

private:
    void _pvt_setTemperature(const SireUnits::Dimension::Temperature &temperature);
    void _pvt_setPressure(const SireUnits::Dimension::Pressure &pressure);
    void _pvt_setFugacity(const SireUnits::Dimension::Pressure &fugacity);

    /** The move that will be repeatedly applied */
    Move mv;
};

/** This is the polymorphic pointer holder for the 
    Moves hierarchy of classes (blocks of simulation moves).
    
    Like all Property polymorphic pointer holder classes,
    this class holds the polymorphic Moves object as 
    an implicitly shared pointer. You can access the 
    const functions of this object by dereferencing this
    pointer, or by using the Moves::read() function, e.g.;
    
    return moves->moves();
    return moves.read().moves();
    
    You must use the Moves::edit() function to
    access the non-const member functions, e.g.;
    
    system = moves.edit().move(system);
    
    Because an implicitly shared pointer is held, this
    class can be copied and passed around quickly. A copy
    is only made when the object being pointed to is
    edited via the .edit() function.

    @author Christopher Woods
*/
class SIREMOVE_EXPORT Moves : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Moves&);
friend QDataStream& ::operator>>(QDataStream&, Moves&);

public:
    Moves();
    Moves(const SireBase::PropertyBase &property);
    Moves(const MovesBase &moves);

    Moves(const Moves &other);
    
    ~Moves();
    
    virtual Moves& operator=(const SireBase::PropertyBase &property);
    virtual Moves& operator=(const MovesBase &other);

    const MovesBase* operator->() const;
    const MovesBase& operator*() const;
    
    const MovesBase& read() const;
    MovesBase& edit();
    
    const MovesBase* data() const;
    const MovesBase* constData() const;
    
    MovesBase* data();
    
    operator const MovesBase&() const;

    static const Moves& shared_null();
};

}

Q_DECLARE_METATYPE( SireMove::Moves )
Q_DECLARE_METATYPE( SireMove::SameMoves )

SIRE_EXPOSE_CLASS( SireMove::MovesBase )
SIRE_EXPOSE_CLASS( SireMove::SameMoves )

SIRE_EXPOSE_PROPERTY( SireMove::Moves, SireMove::MovesBase )

SIRE_END_HEADER

#endif
