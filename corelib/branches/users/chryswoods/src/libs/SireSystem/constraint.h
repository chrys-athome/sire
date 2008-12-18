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

#ifndef SIRESYSTEM_CONSTRAINT_H
#define SIRESYSTEM_CONSTRAINT_H

#include "SireBase/property.h"

#include "SireFF/ffidentifier.h"
#include "SireCAS/expression.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class Constraint;
class NullConstraint;
class PropertyConstraint;
}

QDataStream& operator<<(QDataStream&, const SireSystem::Constraint&);
QDataStream& operator>>(QDataStream&, SireSystem::Constraint&);

QDataStream& operator<<(QDataStream&, const SireSystem::NullConstraint&);
QDataStream& operator>>(QDataStream&, SireSystem::NullConstraint&);

QDataStream& operator<<(QDataStream&, const SireSystem::PropertyConstraint&);
QDataStream& operator>>(QDataStream&, SireSystem::PropertyConstraint&);

namespace SireSystem
{

class System;

/** This is the base class of all constraints. A constraint is an object
    that is added to a System that tries to ensure that a condition is
    enforced. For example, a constraint could be used to change
    the geometry of molecules with respect to lambda, or to change
    forcefield parameters with respect to alpha
    
    A Constraint class does its best to enforce a constraint - if it
    can't, then an exception is raised when a violation of the 
    constraint is detected (SireSystem::constraint_error)
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT Constraint : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Constraint&);
friend QDataStream& ::operator>>(QDataStream&, Constraint&);

public:
    Constraint();
    Constraint(const Constraint &other);
    
    virtual ~Constraint();

    static const char* typeName()
    {
        return "SireSystem::Constraint";
    }
    
    virtual Constraint* clone() const=0;
    
    virtual QString toString() const=0;
    
    virtual bool apply(System &system) const=0;

    virtual bool isSatisfied(System &system) const=0;
    
    void assertSatisfied(System &system) const;
    
    static const NullConstraint& null();
    
protected:
    Constraint& operator=(const Constraint &other);
};

/** The null constraint */
class SIRESYSTEM_EXPORT NullConstraint
         : public SireBase::ConcreteProperty<NullConstraint,Constraint>
{

friend QDataStream& ::operator<<(QDataStream&, const NullConstraint&);
friend QDataStream& ::operator>>(QDataStream&, NullConstraint&);

public:
    NullConstraint();
    
    NullConstraint(const NullConstraint &other);
    
    ~NullConstraint();
    
    NullConstraint& operator=(const NullConstraint &other);
    
    bool operator==(const NullConstraint &other) const;
    bool operator!=(const NullConstraint &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<NullConstraint>() );
    }
    
    NullConstraint* clone() const
    {
        return new NullConstraint(*this);
    }
    
    QString toString() const;
    
    bool isSatisfied(System &system) const;
    
    bool apply(System &system) const;
};

/** This constraint is used to constrain the value of a
    numerical property of the system (or part of the system)
    to a specific value, or to the result of an expression.
    
    You can use this constraint, for example, to constrain
    the value of alpha for soft-core forcefields to map
    to the value of lambda
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT PropertyConstraint
         : public SireBase::ConcreteProperty<PropertyConstraint,Constraint>
{

friend QDataStream& ::operator<<(QDataStream&, const PropertyConstraint&);
friend QDataStream& ::operator>>(QDataStream&, PropertyConstraint&);

public:
    PropertyConstraint();
    PropertyConstraint(const QString &name, const SireCAS::Expression &expression);
    PropertyConstraint(const QString &name, const SireFF::FFID &ffid,
                       const SireCAS::Expression &expression);
    
    PropertyConstraint(const PropertyConstraint &other);
    
    ~PropertyConstraint();
    
    PropertyConstraint& operator=(const PropertyConstraint &other);
    
    bool operator==(const PropertyConstraint &other) const;
    bool operator!=(const PropertyConstraint &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<PropertyConstraint>() );
    }
    
    PropertyConstraint* clone() const
    {
        return new PropertyConstraint(*this);
    }
    
    QString toString() const;
    
    bool isSatisfied(System &system) const;
    
    bool apply(System &system) const;

private:
    /** The ID of the forcefields whose properties are being constrained */
    SireFF::FFIdentifier ffid;
    
    /** The name of the property to constrain */
    QString propname;
    
    /** The expression used to calculate the value of the constraint */
    SireCAS::Expression eqn;
};

typedef SireBase::PropPtr<Constraint> ConstraintPtr;

}

Q_DECLARE_METATYPE( SireSystem::NullConstraint )
Q_DECLARE_METATYPE( SireSystem::PropertyConstraint )

SIRE_EXPOSE_CLASS( SireSystem::Constraint )
SIRE_EXPOSE_CLASS( SireSystem::NullConstraint )
SIRE_EXPOSE_CLASS( SireSystem::PropertyConstraint )

SIRE_EXPOSE_PROPERTY( SireSystem::ConstraintPtr, SireSystem::Constraint )

SIRE_END_HEADER

#endif
