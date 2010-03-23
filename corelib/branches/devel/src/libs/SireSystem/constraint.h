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

#include <QUuid>

#include "SireBase/property.h"
#include "SireBase/majorminorversion.h"

#include "SireFF/ffidentifier.h"

#include "SireCAS/expression.h"
#include "SireCAS/symbol.h"
#include "SireCAS/values.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class Constraint;
class NullConstraint;
class PropertyConstraint;
class ComponentConstraint;
class WindowedComponent;
}

QDataStream& operator<<(QDataStream&, const SireSystem::Constraint&);
QDataStream& operator>>(QDataStream&, SireSystem::Constraint&);

QDataStream& operator<<(QDataStream&, const SireSystem::NullConstraint&);
QDataStream& operator>>(QDataStream&, SireSystem::NullConstraint&);

QDataStream& operator<<(QDataStream&, const SireSystem::PropertyConstraint&);
QDataStream& operator>>(QDataStream&, SireSystem::PropertyConstraint&);

QDataStream& operator<<(QDataStream&, const SireSystem::ComponentConstraint&);
QDataStream& operator>>(QDataStream&, SireSystem::ComponentConstraint&);

QDataStream& operator<<(QDataStream&, const SireSystem::WindowedComponent&);
QDataStream& operator>>(QDataStream&, SireSystem::WindowedComponent&);

namespace SireMol
{
class MolNum;
class Molecules;
}

namespace SireSystem
{

class System;
class Delta;

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
    
    virtual Delta apply(const System &system)=0;
    virtual Delta apply(const System &system, const Delta &delta)=0;

    virtual bool wouldBeAffectedBy(const System &system,    
                                   const Delta &delta) const=0;

    virtual bool isSatisfied(const System &system) const=0;
    
    void assertSatisfied(const System &system) const;
    
    static const NullConstraint& null();
    
protected:
    friend class System;        // so can call accept()
    friend class Constraints;   // so can call accept()

    Constraint& operator=(const Constraint &other);

    const QUuid& lastUID() const;
    const SireBase::Version& lastVersion() const;

    virtual void accept(const System &system, const Delta &delta)=0;

    void setSatisfied(const System &system);
    void setUnsatisfied(const System &system);
    
    void clearLastSystem();
    
    bool wasLastSystem(const System &system) const;
    bool wasLastSatisfied() const;

private:
    /** The UID of the last system on which this constraint
        was applied */
    QUuid last_sysuid;
    
    /** The version of the system on which this constraint
        was last applied */
    SireBase::Version last_sysversion;
    
    /** Whether or not this constraint was satisfied for the 
        last system on which it was applied */
    bool last_was_satisfied;
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
    
    static const char* typeName();
    
    NullConstraint* clone() const;
    
    QString toString() const;

    Delta apply(const System &system);
    Delta apply(const System &system, const Delta &delta);

    bool wouldBeAffectedBy(const System &system, const Delta &delta) const;
    
    bool isSatisfied(const System &system) const;

protected:
    void accept(const System &system, const Delta &delta);
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
    
    static const char* typeName();
    
    QString toString() const;

    Delta apply(const System &system);
    Delta apply(const System &system, const Delta &delta);

    bool wouldBeAffectedBy(const System &system, const Delta &delta) const;
    
    bool isSatisfied(const System &system) const;

protected:
    void accept(const System &system, const Delta &delta);

private:
    void setSystem(const System &system);

    /** The ID of the forcefields whose properties are being constrained */
    SireFF::FFIdentifier ffid;
    
    /** The indexes of the forcefields that matched this ID the last
        time this constraint was applied to a system */
    QList<SireFF::FFIdx> ffidxs;
    
    /** The name of the property to constrain */
    QString propname;
    
    /** The expression used to calculate the value of the constraint */
    SireCAS::Expression eqn;
    
    /** The symbols representing the constant components needed
        by the expression */
    QSet<SireCAS::Symbol> syms;
    
    /** The values of the constant components the last time
        this constraint was applied */
    SireCAS::Values last_vals;
    
    /** The last value of the property */
    SireBase::PropertyPtr old_property;
    
    /** The last (numerical) value of the property */
    double old_value;
    
    /** The new value of the equation */
    double new_value;
    
    /** Whether or not there is an old value */
    bool has_old_value;
};

/** This constraint is used to constrain the value of a
    component of the system to a specific value, or the result
    of an expression based on other components in the system.
    
    You can use this constraint, for example, to constrain
    the value of lambda_forwards to equal Min( 1, lambda+delta_lambda )
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT ComponentConstraint
         : public SireBase::ConcreteProperty<ComponentConstraint,Constraint>
{

friend QDataStream& ::operator<<(QDataStream&, const ComponentConstraint&);
friend QDataStream& ::operator>>(QDataStream&, ComponentConstraint&);

public:
    ComponentConstraint();
    ComponentConstraint(const SireCAS::Symbol &component, 
                        const SireCAS::Expression &expression);
    
    ComponentConstraint(const ComponentConstraint &other);
    
    ~ComponentConstraint();
    
    ComponentConstraint& operator=(const ComponentConstraint &other);
    
    bool operator==(const ComponentConstraint &other) const;
    bool operator!=(const ComponentConstraint &other) const;
    
    static const char* typeName();
    
    QString toString() const;
    
    const SireCAS::Symbol& component() const;
    
    const SireCAS::Expression& expression() const;

    Delta apply(const System &system);
    Delta apply(const System &system, const Delta &delta);

    bool wouldBeAffectedBy(const System &system, const Delta &delta) const;
    
    bool isSatisfied(const System &system) const;

private:
    /** The component whose value is constrained */
    SireCAS::Symbol constrained_component;
    
    /** The expression used to calculate the value of the constraint */
    SireCAS::Expression eqn;
    
    /** The symbols representing the constant components that
        are used in the constraint expression */
    QSet<SireCAS::Symbol> syms;
};

/** This constraint is used to constrain a component to adopt one of the values
    from a set - this is used to implement FEP windows, where lambda_forwards
    can be constrained to be the next lambda value along
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT WindowedComponent
         : public SireBase::ConcreteProperty<WindowedComponent,Constraint>
{

friend QDataStream& ::operator<<(QDataStream&, const WindowedComponent&);
friend QDataStream& ::operator>>(QDataStream&, WindowedComponent&);

public:
    WindowedComponent();
    WindowedComponent(const SireCAS::Symbol &component,
                      const SireCAS::Symbol &reference,
                      const QVector<double> &values,
                      int step_size = 1);
    
    WindowedComponent(const WindowedComponent &other);
    
    ~WindowedComponent();
    
    WindowedComponent& operator=(const WindowedComponent &other);
    
    bool operator==(const WindowedComponent &other) const;
    bool operator!=(const WindowedComponent &other) const;
    
    static const char* typeName();
    
    QString toString() const;
    
    const SireCAS::Symbol& component() const;
    const SireCAS::Symbol& referenceComponent() const;
    
    const QVector<double>& windowValues() const;
    
    int stepSize() const;

    Delta apply(const System &system);
    Delta apply(const System &system, const Delta &delta);

    bool wouldBeAffectedBy(const System &system, const Delta &delta) const;
    
    bool isSatisfied(const System &system) const;

private:
    /** The component whose value is being constrained */
    SireCAS::Symbol constrained_component; 
    
    /** The component whose value provides the value of the reference window */
    SireCAS::Symbol reference_component;
    
    /** The list of values of all of the windows */
    QVector<double> window_values;
    
    /** The step size - this allows us to be set to a window that
        is 'step_size' windows away from the reference */
    qint32 step_size;
};

typedef SireBase::PropPtr<Constraint> ConstraintPtr;

}

Q_DECLARE_METATYPE( SireSystem::NullConstraint )
Q_DECLARE_METATYPE( SireSystem::PropertyConstraint )
Q_DECLARE_METATYPE( SireSystem::ComponentConstraint )
Q_DECLARE_METATYPE( SireSystem::WindowedComponent )

SIRE_EXPOSE_CLASS( SireSystem::Constraint )
SIRE_EXPOSE_CLASS( SireSystem::NullConstraint )
SIRE_EXPOSE_CLASS( SireSystem::PropertyConstraint )
SIRE_EXPOSE_CLASS( SireSystem::ComponentConstraint )
SIRE_EXPOSE_CLASS( SireSystem::WindowedComponent )

SIRE_EXPOSE_PROPERTY( SireSystem::ConstraintPtr, SireSystem::Constraint )

SIRE_END_HEADER

#endif
