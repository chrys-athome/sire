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

#ifndef SIREMOVE_MOVE_H
#define SIREMOVE_MOVE_H

#include "SireCAS/symbol.h"

#include "SireBase/property.h"
#include "SireBase/propertymap.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class Move;
class MoveBase;
class NullMove;
}

QDataStream& operator<<(QDataStream&, const SireMove::Move&);
QDataStream& operator>>(QDataStream&, SireMove::Move&);

QDataStream& operator<<(QDataStream&, const SireMove::MoveBase&);
QDataStream& operator>>(QDataStream&, SireMove::MoveBase&);

QDataStream& operator<<(QDataStream&, const SireMove::NullMove&);
QDataStream& operator>>(QDataStream&, SireMove::NullMove&);

namespace SireSystem
{
class System;
}

namespace SireMove
{

class Ensemble;

using SireCAS::Symbol;

using SireSystem::System;

using SireBase::PropertyName;

/** This is the base class of all of the move classes

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MoveBase : public SireBase::PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const MoveBase&);
friend QDataStream& ::operator>>(QDataStream&, MoveBase&);

public:
    MoveBase();
    
    MoveBase(const MoveBase &other);
    
    virtual ~MoveBase();
    
    static const char* typeName()
    {
        return "SireMove::MoveBase";
    }
    
    virtual MoveBase* clone() const=0;
    
    virtual void move(System &system, int nmoves, bool record_stats)=0;

    void move(System &system);
    void move(System &system, int nmoves);

    const Symbol& energyComponent() const;
    virtual void setEnergyComponent(const Symbol &component);

    const PropertyName& spaceProperty() const;
    void setSpaceProperty(const PropertyName &spaceproperty);

    virtual Ensemble ensemble() const=0;

    bool isConstantEnergy() const;
    bool isConstantTemperature() const;
    bool isConstantVolume() const;
    bool isConstantPressure() const;
    bool isConstantChemicalPotential() const;
    bool isConstantFugacity() const;
    
    virtual bool isConstantLambda(const Symbol &lam) const;

    SireUnits::Dimension::Temperature temperature() const;
    SireUnits::Dimension::Pressure pressure() const;
    SireUnits::Dimension::Pressure fugacity() const;
    SireUnits::Dimension::MolarEnergy chemicalPotential() const;

    void setTemperature(const SireUnits::Dimension::Temperature &temperature);
    void setPressure(const SireUnits::Dimension::Pressure &pressure);
    void setChemicalPotential(
                    const SireUnits::Dimension::MolarEnergy &chemical_potential);
    void setFugacity(const SireUnits::Dimension::Pressure &fugacity);

protected:
    MoveBase& operator=(const MoveBase &other);

    bool operator==(const MoveBase &other) const;
    bool operator!=(const MoveBase &other) const;

    virtual void _pvt_setTemperature(
                            const SireUnits::Dimension::Temperature &temperature);
                            
    virtual void _pvt_setPressure(
                            const SireUnits::Dimension::Pressure &pressure);
                            
    virtual void _pvt_setFugacity(
                            const SireUnits::Dimension::Pressure &fugacity);

private:
    /** The component of the energy that describes the Hamiltonian
        that this move samples */
    Symbol nrgcomponent;
    
    /** The name of the property that contains the System space property.
        This is necessary as we may have to map the molecules back into
        the space at the end of the move */
    PropertyName spaceproperty;
};

/** This is a null move - it doesn't change the system at all! */
class SIREMOVE_EXPORT NullMove : public SireBase::ConcreteProperty<NullMove,MoveBase>
{
public:
    NullMove();
    NullMove(const NullMove &other);
    
    ~NullMove();
    
    NullMove& operator=(const NullMove &other);
    
    bool operator==(const NullMove &other) const;
    bool operator!=(const NullMove &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<NullMove>() );
    }
    
    NullMove* clone() const
    {
        return new NullMove(*this);
    }
    
    void move(System &system, int nmoves, bool record_stats);
    
    Ensemble ensemble() const;
};

/** This is the polymorphic pointer holder for the 
    Move hierarchy of classes (simulation moves).
    
    Like all Property polymorphic pointer holder classes,
    this class holds the polymorphic Move object as 
    an implicitly shared pointer. You can access the 
    const functions of this object by dereferencing this
    pointer, or by using the Move::read() function, e.g.;
    
    cout << move->what();
    cout << move.read().what();
    
    You must use the Move::edit() function to
    access the non-const member functions, e.g.;
    
    move.edit().move(system);
    
    Because an implicitly shared pointer is held, this
    class can be copied and passed around quickly. A copy
    is only made when the object being pointed to is
    edited via the .edit() function.

    @author Christopher Woods
*/
class SIREMOVE_EXPORT Move : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Move&);
friend QDataStream& ::operator>>(QDataStream&, Move&);

public:
    Move();
    Move(const SireBase::PropertyBase &property);
    Move(const MoveBase &move);

    Move(const Move &other);
    
    ~Move();
    
    virtual Move& operator=(const SireBase::PropertyBase &property);
    virtual Move& operator=(const MoveBase &other);

    const MoveBase* operator->() const;
    const MoveBase& operator*() const;
    
    const MoveBase& read() const;
    MoveBase& edit();
    
    const MoveBase* data() const;
    const MoveBase* constData() const;
    
    MoveBase* data();
    
    operator const MoveBase&() const;

    static const Move& shared_null();
};

}

Q_DECLARE_METATYPE( SireMove::Move )
Q_DECLARE_METATYPE( SireMove::NullMove )

SIRE_EXPOSE_CLASS( SireMove::MoveBase )
SIRE_EXPOSE_CLASS( SireMove::NullMove )

SIRE_EXPOSE_PROPERTY( SireMove::Move, SireMove::MoveBase )

SIRE_END_HEADER

#endif
