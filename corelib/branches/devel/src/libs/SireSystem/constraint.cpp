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

#include "constraint.h"
#include "system.h"
#include "delta.h"

#include "SireMaths/maths.h"

#include "SireSystem/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireSystem;
using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of Constraint
//////////

static const RegisterMetaType<Constraint> r_constraint( MAGIC_ONLY,
                                                        "SireSystem::Constraint" );

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const Constraint &constraint)
{
    writeHeader(ds, r_constraint, 1);
    
    ds << static_cast<const Property&>(constraint);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, Constraint &constraint)
{
    VersionID v = readHeader(ds, r_constraint);
    
    if (v == 1)
    {
        ds >> static_cast<Property&>(constraint);
        
        constraint.last_sysuid = QUuid();
        constraint.last_sysversion = Version();
        constraint.last_was_satisfied = false;
    }
    else
        throw version_error( v, "1", r_constraint, CODELOC );
        
    return ds;
}

/** Constructor */
Constraint::Constraint() : Property(), last_was_satisfied(false)
{}

/** Copy constructor */
Constraint::Constraint(const Constraint &other) 
           : Property(other),
             last_sysuid(other.last_sysuid), 
             last_sysversion(other.last_sysversion),
             last_was_satisfied(other.last_was_satisfied)
{}

/** Destructor */
Constraint::~Constraint()
{}

/** Copy assignment operator */
Constraint& Constraint::operator=(const Constraint &other)
{
    if (this != &other)
    {
        Property::operator=(other);
        last_sysuid = other.last_sysuid;
        last_sysversion = other.last_sysversion;
        last_was_satisfied = other.last_was_satisfied;
    }
    
    return *this;
}

/** Return the UID of the last system applied to the constraint */
const QUuid& Constraint::lastUID() const
{
    return last_sysuid;
}

/** Return the version of the last system applied to the constraint */
const Version& Constraint::lastVersion() const
{
    return last_sysversion;
}

/** Internal function called by the constraint to say 
    that it is satisfied on the passed system */
void Constraint::setSatisfied(const System &system)
{
    last_was_satisfied = true;
    last_sysuid = system.UID();
    last_sysversion = system.version();
}

/** Internal function called by the constraint to say 
    that it is not satisfied on the passed system */
void Constraint::setUnsatisfied(const System &system)
{
    last_was_satisfied = false;
    last_sysuid = system.UID();
    last_sysversion = system.version();
}

/** Clear the cache for the last system */
void Constraint::clearLastSystem()
{
    last_was_satisfied = false;
    last_sysuid = QUuid();
    last_sysversion = Version();
}

/** Return whether or not the passed system was the last one
    on which this constraint was applied */
bool Constraint::wasLastSystem(const System &system) const
{
    return system.UID() == last_sysuid and
           system.version() == last_sysversion;
}

/** Return whether or not the last system satisfied this constraint */
bool Constraint::wasLastSatisfied() const
{
    return last_was_satisfied;
}

/** Assert that the constraint is satisfied in the passed system 

    \throw SireSystem::constraint_error
*/
void Constraint::assertSatisfied(const System &system) const
{
    if (not this->isSatisfied(system))
        throw SireSystem::constraint_error( QObject::tr(
            "The constraint %1 is not valid in the system %2.")
                .arg(this->toString())
                .arg(system.name()), CODELOC );
}

//////////
////////// Implementation of NullConstraint
//////////

static const RegisterMetaType<NullConstraint> r_nullconstraint;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, 
                                          const NullConstraint &nullconstraint)
{
    writeHeader(ds, r_nullconstraint, 1);
    
    ds << static_cast<const Constraint&>(nullconstraint);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, 
                                          NullConstraint &nullconstraint)
{
    VersionID v = readHeader(ds, r_nullconstraint);
    
    if (v == 1)
    {
        ds >> static_cast<Constraint&>(nullconstraint);
    }
    else
        throw version_error(v, "1", r_nullconstraint, CODELOC);
        
    return ds;
}

/** Null constructor */
NullConstraint::NullConstraint() : ConcreteProperty<NullConstraint,Constraint>()
{}

/** Copy constructor */
NullConstraint::NullConstraint(const NullConstraint &other)
               : ConcreteProperty<NullConstraint,Constraint>(other)
{}

/** Destructor */
NullConstraint::~NullConstraint()
{}

/** Copy assignment operator */
NullConstraint& NullConstraint::operator=(const NullConstraint &other)
{
    Constraint::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullConstraint::operator==(const NullConstraint&) const
{
    return true;
}

/** Comparison operator */
bool NullConstraint::operator!=(const NullConstraint&) const
{
    return false;
}

/** Return a string representation */
QString NullConstraint::toString() const
{
    return QObject::tr("NullConstraint");
}

/** Return whether this constraint is satisfied */
bool NullConstraint::isSatisfied(const System&) const
{
    return true;
}

/** Apply this constraint on the passed system - this returns
    the Delta that would have to be applied to the system
    to maintain the constraint */
Delta NullConstraint::apply(const System&)
{
    return Delta();
}

/** Apply this constraint on the passed system, given
    that the change contained in 'Delta' will be performed.
    This returns a new delta that is a modification of the
    passed delta, that changes it so that this constraint
    will be maintained */
Delta NullConstraint::apply(const System&, const Delta &delta)
{
    return delta;
}

/** Return whether or not this constraint would be affected
    by the passed delta on the passed system */
bool NullConstraint::wouldBeAffectedBy(const System&, const Delta&) const
{
    return false;
}

/** Accept the new system, which was created from the last system  
    processed by this constraint using the passed delta */
void NullConstraint::accept(const System&, const Delta&)
{}

static SharedPolyPointer<NullConstraint> shared_null;

const NullConstraint& Constraint::null()
{
    if (shared_null.constData() == 0)
    {
        QMutexLocker lkr( SireBase::globalLock() );
        
        if (shared_null.constData() == 0)
            shared_null = new NullConstraint();
    }
    
    return *(shared_null.constData());
}

const char* NullConstraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullConstraint>() );
}

//////////
////////// Implementation of PropertyConstraint
//////////

static const RegisterMetaType<PropertyConstraint> r_propconstraint;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, 
                                          const PropertyConstraint &constraint)
{
    writeHeader(ds, r_propconstraint, 1);
    
    SharedDataStream sds(ds);
    
    sds << constraint.ffid << constraint.propname
        << constraint.eqn
        << static_cast<const Constraint&>(constraint);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          PropertyConstraint &constraint)
{
    VersionID v = readHeader(ds, r_propconstraint);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);

        //completely clear any cached data
        constraint = PropertyConstraint();
        
        sds >> constraint.ffid >> constraint.propname
            >> constraint.eqn
            >> static_cast<Constraint&>(constraint);
            
        constraint.syms = constraint.eqn.symbols();
    }
    else
        throw version_error(v, "1", r_propconstraint, CODELOC);
        
    return ds;
}

/** Null constructor */
PropertyConstraint::PropertyConstraint()
                   : ConcreteProperty<PropertyConstraint,Constraint>(),
                     old_value(0), new_value(0), has_old_value(false)
{}

/** Construct to constrain the property with name 'name' in all forcefields
    to have the value resulting from the expression 'expression' */
PropertyConstraint::PropertyConstraint(const QString &name, 
                                       const SireCAS::Expression &expression)
                   : ConcreteProperty<PropertyConstraint,Constraint>(),
                     propname(name), eqn(expression), syms(expression.symbols()),
                     old_value(0), new_value(0), has_old_value(false)
{}

/** Construct to constrain the property with name 'name' in the forcefield(s)
    that match the ID 'ffid' to have the value resulting the expression
    'expression' */
PropertyConstraint::PropertyConstraint(const QString &name, const SireFF::FFID &id,
                                       const SireCAS::Expression &expression)
                   : ConcreteProperty<PropertyConstraint,Constraint>(),
                     ffid(id), propname(name), eqn(expression),
                     syms(expression.symbols()),
                     old_value(0), new_value(0), has_old_value(false)
{}

/** Copy constructor */
PropertyConstraint::PropertyConstraint(const PropertyConstraint &other)
                   : ConcreteProperty<PropertyConstraint,Constraint>(other),
                     ffid(other.ffid), ffidxs(other.ffidxs),
                     propname(other.propname), eqn(other.eqn),
                     syms(other.syms),
                     last_vals(other.last_vals), old_property(other.old_property),
                     old_value(other.old_value), new_value(other.new_value),
                     has_old_value(other.has_old_value)
{}

/** Destructor */
PropertyConstraint::~PropertyConstraint()
{}

/** Copy assignment operator */
PropertyConstraint& PropertyConstraint::operator=(const PropertyConstraint &other)
{
    if (this != &other)
    {
        Constraint::operator=(other);
        ffid = other.ffid;
        ffidxs = other.ffidxs;
        propname = other.propname;
        eqn = other.eqn;
        syms = other.syms;
        last_vals = other.last_vals;
        old_property = other.old_property;
        old_value = other.old_value;
        new_value = other.new_value;
        has_old_value = other.has_old_value;
    }
    
    return *this;
}

/** Comparison operator */
bool PropertyConstraint::operator==(const PropertyConstraint &other) const
{
    return ffid == other.ffid and propname == other.propname and 
           eqn == other.eqn;
}

/** Comparison operator */
bool PropertyConstraint::operator!=(const PropertyConstraint &other) const
{
    return not this->operator==(other);
}

/** Return a string representation of the constraint */
QString PropertyConstraint::toString() const
{
    return QObject::tr("PropertyConstraint( FFID=%1 property=%2 expression=%3 )")
                .arg(ffid.toString(), propname, eqn.toString());
}

/** Internal function used to update this constraint so that it is applied
    to the passed system */
void PropertyConstraint::setSystem(const System &system)
{
    if (Constraint::wasLastSystem(system))
        return;
    
    Constraint::clearLastSystem();
    
    if (not ffid.isNull())
        ffidxs = ffid.map(system.forceFields());
        
    has_old_value = false;
    old_value = 0;
    old_property = PropertyPtr();
    
    if (not ffidxs.isEmpty())
    {
        bool no_prop = false;
    
        foreach (FFIdx ffidx, ffidxs)
        {
            if (system.containsProperty(ffidx, propname))
            {
                const Property &prop = system.property(ffidx, propname);
                
                if (old_property.isNull())
                {
                    old_property = prop;
                }
                else if (not old_property.read().equals(prop))
                {
                    //some of the forcefield properties are different
                    old_property = PropertyPtr();
                    break;
                }
            }
            else 
            {
                //this property doesn't exist in at least one of the forcefields
                old_property = PropertyPtr();
                break;
            }

        }
    }
    else 
    {
        if (system.containsProperty(propname))
            old_property = system.property(propname);
    }

    if (not old_property.isNull())
    {
        if (old_property.read().isA<VariantProperty>())
        {
            const VariantProperty &var = old_property.read().asA<VariantProperty>();
            
            if (var.canConvert<double>())
            {
                has_old_value = true;
                old_value = var.convertTo<double>();
            }
        }
    }
    
    last_vals = system.constants(syms);
    
    new_value = eqn(last_vals);
    
    if (new_value == old_value)
        Constraint::setSatisfied(system);

    else
        Constraint::setUnsatisfied(system);
}

/** Return whether or not this constraint is satisfied in the passed system */
bool PropertyConstraint::isSatisfied(const System &system) const
{
    if (Constraint::wasLastSystem(system))
        return Constraint::wasLastSatisfied();

    else
    {
        std::auto_ptr<PropertyConstraint> copy( this->clone() );
        
        copy->setSystem(system);
        
        return copy->isSatisfied();
    }
}

/** Return whether or not this constraint would be affected by the 
    change 'delta' applied to the passed system 'system' */
bool PropertyConstraint::wouldBeAffectedBy(const System &system, 
                                           const Delta &delta) const
{
    if ( not Constraint::wasLastSystem(system) )
    {
        std::auto_ptr<PropertyConstraint> copy(this->clone());
        copy->setSystem(system);
        return copy->wouldBeAffectedBy(system, delta);
    }
    
    double delta_new_value = new_value;
    double delta_old_value = old_value;
    bool delta_has_old_value = has_old_value;
    
    if (ffidxs.isEmpty())
    {
        if (delta.involves(propname))
        {
            const Property &prop = delta.newProperty(propname);
        
            delta_has_old_value = false;
            
            if (prop.isA<VariantProperty>())
            {
                const VariantProperty &var = prop.asA<VariantProperty>();
            
                if (var.canConvert<double>())
                {
                    delta_old_value = var.convertTo<double>();
                    delta_has_old_value = true;
                }
            }
        }
    }
    else
    {
        if (delta.involves(propname, ffidxs))
        {
            delta_has_old_value = false;
            
            const Property *delta_prop = 0;

            bool first_prop = true;

            foreach (FFIdx ffidx, ffidxs)
            {
                const Property &property = delta.newProperty(propname, ffidx);
                
                if (first_prop)
                {
                    delta_prop = &property;
                }
                else if (not delta_prop->equals(property))
                {
                    //there are some changes in property
                    delta_prop = 0;
                    break;
                }
            }
            
            if (delta_prop)
            {
                if (delta_prop->isA<VariantProperty>())
                {
                    const VariantProperty &var = delta_prop->asA<VariantProperty>();
            
                    if (var.canConvert<double>())
                    {
                        delta_old_value = var.convertTo<double>();
                        delta_has_old_value = true;
                    }
                }
            }
        }
    }

    if (not delta_has_old_value)
        return true;
    
    if (delta.involves(syms))
    {
        delta_new_value = eqn( delta.update(last_vals) );
    }
    
    return delta_old_value != delta_new_value;
}

/** Apply this constraint on the passed system - this returns
    the Delta that would have to be applied to the system
    to maintain the constraint */
Delta PropertyConstraint::apply(const System &system)
{
    this->setSystem(system);
    
    if ( Constrant::wasLastSatisfied() )
        return Delta();
        
    else
        return Delta( propname, old_property.read(), VariantProperty(new_value) );
}

/** Apply this constraint on the passed system, given
    that the change contained in 'Delta' will be performed.
    This returns a new delta that is a modification of the
    passed delta, that changes it so that this constraint
    will be maintained. */
Delta PropertyConstraint::apply(const System &system, const Delta &delta)
{
    this->setSystem(system);
    
    double delta_new_value = new_value;
    double delta_old_value = old_value;
    bool delta_has_old_value = has_old_value;
    
    if (ffidxs.isEmpty())
    {
        if (delta.involves(propname))
        {
            const Property &prop = delta.newProperty(propname);
        
            delta_has_old_value = false;
            
            if (prop.isA<VariantProperty>())
            {
                const VariantProperty &var = prop.asA<VariantProperty>();
            
                if (var.canConvert<double>())
                {
                    delta_old_value = var.convertTo<double>();
                    delta_has_old_value = true;
                }
            }
        }
    }
    else
    {
        if (delta.involves(propname, ffidxs))
        {
            NEED TO WRITE FUNCTION TO PROCESS FFIDXS INDEPENDENTLY
        
            const Property &prop = delta.newProperty(propname, ffidxs);
        
            delta_has_old_value = false;
            
            if (prop.isA<VariantProperty>())
            {
                const VariantProperty &var = prop.asA<VariantProperty>();
            
                if (var.canConvert<double>())
                {
                    delta_old_value = var.convertTo<double>();
                    delta_has_old_value = true;
                }
            }
        }
    }

    if (delta.involves(syms))
    {
        delta_new_value = eqn( delta.update(last_vals) );
    }

    if (delta_has_old_value and (delta_old_value == delta_new_value))
    {
        return delta;
    }
    else if (ffidxs.isEmpty())
        return delta + Delta(propname, old_property,
                             VariantProperty(delta_new_value));
    else
        return delta + Delta(propname, ffidxs, 
                             old_property, VariantProperty(delta_new_value));
}

void PropertyConstraint::accept(const System &system, const Delta &delta)
{
    //copy the new system into this one - absolutely assumes that
    //'delta' represents the only change since the last setSystem() call
    if ( lastUID() != system.UID() )
    {
        this->setSystem(system);
        return;
    }
    
    if (delta.involves(syms))
    {
        last_vals = delta.update(last_vals);
        new_value = eqn(last_vals);
    }

    bool must_update = false;
    
    if (ffidxs.isEmpty())
    {
        if (delta.involves(propname))
        {
            old_property = delta.newProperty(propname);
            must_update = true;
        }
    }
    else
    {
        if (delta.involves(propname, ffidxs))
        {
            old_property = delta.newProperty(propname, ffidxs);
            must_update = true;
        }
    }
    
    if (must_update)
    {
        has_old_value = false;
        old_value = 0;
    
        if (old_property.read().isA<VariantProperty>())
        {
            const VariantProperty &var = old_property.read().asA<VariantProperty>();
            
            if (var.canConvert<double>())
            {
                has_old_value = true;
                old_value = var.convertTo<double>();
            }
        }
    }
    
    if (has_old_value and (new_value == old_value))
        Constraint::setSatisfied(system);

    else
        Constraint::setUnsatisfied(system);
}

const char* PropertyConstraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<PropertyConstraint>() );
}

//////////
////////// Implementation of ComponentConstraint
//////////

static const RegisterMetaType<ComponentConstraint> r_compconstraint;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, 
                                          const ComponentConstraint &constraint)
{
    writeHeader(ds, r_compconstraint, 1);
    
    SharedDataStream sds(ds);
    
    sds << constraint.constrained_component
        << constraint.eqn
        << static_cast<const Constraint&>(constraint);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          ComponentConstraint &constraint)
{
    VersionID v = readHeader(ds, r_compconstraint);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> constraint.constrained_component
            >> constraint.eqn
            >> static_cast<Constraint&>(constraint);
            
        constraint.syms = constraint.eqn.symbols();
    }
    else
        throw version_error(v, "1", r_compconstraint, CODELOC);
        
    return ds;
}

/** Null constructor */
ComponentConstraint::ComponentConstraint()
                    : ConcreteProperty<ComponentConstraint,Constraint>()
{}

/** Construct to constrain the component with symbol 'component'
    to have the value resulting from the expression 'expression' */
ComponentConstraint::ComponentConstraint(const Symbol &component,
                                         const SireCAS::Expression &expression)
                   : ConcreteProperty<ComponentConstraint,Constraint>(),
                     constrained_component(component), eqn(expression),
                     syms(expression.symbols())
{}

/** Copy constructor */
ComponentConstraint::ComponentConstraint(const ComponentConstraint &other)
                   : ConcreteProperty<ComponentConstraint,Constraint>(other),
                     constrained_component(other.constrained_component), eqn(other.eqn),
                     syms(other.syms)
{}

/** Destructor */
ComponentConstraint::~ComponentConstraint()
{}

/** Copy assignment operator */
ComponentConstraint& ComponentConstraint::operator=(const ComponentConstraint &other)
{
    if (this != &other)
    {
        Constraint::operator=(other);
        constrained_component = other.constrained_component;
        eqn = other.eqn;
        syms = other.syms;
    }
    
    return *this;
}

/** Comparison operator */
bool ComponentConstraint::operator==(const ComponentConstraint &other) const
{
    return constrained_component == other.constrained_component and 
           eqn == other.eqn;
}

/** Comparison operator */
bool ComponentConstraint::operator!=(const ComponentConstraint &other) const
{
    return not this->operator==(other);
}

/** Return a string representation of the constraint */
QString ComponentConstraint::toString() const
{
    return QObject::tr("ComponentConstraint( component=%1 expression=%2 )")
                .arg(constrained_component.toString(), eqn.toString());
}

/** Return the symbol representing the component being constrained */
const Symbol& ComponentConstraint::component() const
{
    return constrained_component;
}

/** Return the expression used to evaluate the constraint */
const Expression& ComponentConstraint::expression() const
{
    return eqn;
}

/** Return whether this constraint is satisfied */
bool ComponentConstraint::isSatisfied(const System &system) const
{
    //get the values of all of the contains components
    Values constants = system.constants(syms);
    
    double old_value = system.constant(constrained_component);
    
    double new_value = eqn(constants);
    
    return (old_value == new_value);
}

/** Apply this constraint on the passed system - this returns
    the Delta that would have to be applied to the system
    to maintain the constraint */
Delta ComponentConstraint::apply(const System &system)
{
    //get the values of all of the contains components
    Values constants = system.constants(syms);
    
    double old_value = system.constant(constrained_component);
    
    double new_value = eqn(constants);
    
    if (old_value == new_value)
        return Delta();
    else
        return Delta(constrained_component, old_value, new_value);
}

/** Apply this constraint on the passed system, given
    that the change contained in 'Delta' will be performed.
    This returns a new delta that is a modification of the
    passed delta, that changes it so that this constraint
    will be maintained. Note that this assumes that the system
    satisfies the constraints before the delta is applied */
Delta ComponentConstraint::apply(const System &system, const Delta &delta)
{
    
}

/** Return whether or not this constraint would be affected
    by the passed delta on the passed system */
bool ComponentConstraint::wouldBeAffectedBy(const System &system, 
                                            const Delta &delta) const
{
    return delta.involves(constrained_component) or
           delta.involves(syms);
}

const char* ComponentConstraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<ComponentConstraint>() );
}

//////////
////////// Implementation of WindowedComponent
//////////

static const RegisterMetaType<WindowedComponent> r_windowedcomp;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, 
                                          const WindowedComponent &windowedcomp)
{
    writeHeader(ds, r_windowedcomp, 1);
    
    SharedDataStream sds(ds);
    
    sds << windowedcomp.constrained_component
        << windowedcomp.reference_component
        << windowedcomp.window_values
        << windowedcomp.step_size
        << static_cast<const Constraint&>(windowedcomp);
        
    return ds;
}
              
/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, 
                                          WindowedComponent &windowedcomp)
{
    VersionID v = readHeader(ds, r_windowedcomp);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> windowedcomp.constrained_component
            >> windowedcomp.reference_component
            >> windowedcomp.window_values
            >> windowedcomp.step_size
            >> static_cast<Constraint&>(windowedcomp);
    }
    else
        throw version_error(v, "1", r_windowedcomp, CODELOC);
        
    return ds;
}

/** Null constructor */
WindowedComponent::WindowedComponent()
                  : ConcreteProperty<WindowedComponent,Constraint>(),
                    step_size(0)
{}

/** Construct a WindowedConstraint that constrains the component represented
    by the symbol 'component' to lie 'step_size' windows above (or below if
    'step_size' is negative) the window in which the component represented
    by the symbol 'reference' resides - where 'values' contains all of
    the values of the windows, in the order that you have arranged
    them. Whilst this will not sort 'window_values', it will remove
    all duplicate values */
WindowedComponent::WindowedComponent(const SireCAS::Symbol &component,
                                     const SireCAS::Symbol &reference,
                                     const QVector<double> &values,
                                     int step)
                  : ConcreteProperty<WindowedComponent,Constraint>(),
                    constrained_component(component),
                    reference_component(reference),
                    window_values(values),
                    step_size(step)
{
    if (component == reference)
        throw SireError::invalid_arg( QObject::tr(
                "You cannot use the component %1 as both the constrained "
                "and reference components in a WindowedComponent.")
                    .arg(component.toString()), CODELOC );

    int i = 0;

    while(true)
    {
        if (i >= window_values.count())
            break;
    
        while (window_values.count( window_values[i] ) > 1)
        {
            window_values.remove( window_values.lastIndexOf(window_values[i]) );
        }
        
        ++i;
    }
} 

/** Copy constructor */
WindowedComponent::WindowedComponent(const WindowedComponent &other)
                  : ConcreteProperty<WindowedComponent,Constraint>(other),
                    constrained_component(other.constrained_component),
                    reference_component(other.reference_component),
                    window_values(other.window_values),
                    step_size(other.step_size)
{}

/** Destructor */
WindowedComponent::~WindowedComponent()
{}

/** Copy assignment operator */
WindowedComponent& WindowedComponent::operator=(const WindowedComponent &other)
{
    if (this != &other)
    {
        constrained_component = other.constrained_component;
        reference_component = other.reference_component;
        window_values = other.window_values;
        step_size = other.step_size;
        Constraint::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool WindowedComponent::operator==(const WindowedComponent &other) const
{
    return constrained_component == other.constrained_component and
           reference_component == other.reference_component and
           window_values == other.window_values and
           step_size == other.step_size;
}

/** Comparison operator */
bool WindowedComponent::operator!=(const WindowedComponent &other) const
{
    return not this->operator==(other);
}

/** Return a string representation of this constraint */
QString WindowedComponent::toString() const
{
    return QObject::tr("WindowedComponent( component=%1 reference=%2 step_size=%3 )\n"
                       "   windowed_values = %4")
                .arg(constrained_component.toString(), reference_component.toString())
                .arg(step_size).arg(Sire::toString(window_values));
}

/** Return the symbol representing the component being constrained */
const SireCAS::Symbol& WindowedComponent::component() const
{
    return constrained_component;
}

/** Return the symbol representing the reference component */
const SireCAS::Symbol& WindowedComponent::referenceComponent() const
{
    return reference_component;
}

/** Return the values of all of the windows */
const QVector<double>& WindowedComponent::windowValues() const
{
    return window_values;
}

/** Return the step size for this windows - this is the number of 
    windows above (or below if step_size is negative) for this 
    window compared to the window containing the reference component */
int WindowedComponent::stepSize() const
{
    return step_size;
}

static double getNextWindow(double reference_value,
                            const QVector<double> &window_values,
                            int step_size)
{
    int idx = window_values.indexOf(reference_value);
    
    if (idx == -1)
    {
        //find the closest value
        double del2 = std::numeric_limits<double>::max();
        
        for (int i=0; i<window_values.count(); ++i)
        {
            double my_del2 = SireMaths::pow_2( reference_value - window_values.at(i) );
            
            if (my_del2 < del2)
            {
                del2 = my_del2;
                idx = i;
            }
        }
    }
    
    BOOST_ASSERT( idx != -1 );
    
    idx += step_size;
    
    if (idx < 0)
        idx = 0;
    else if (idx >= window_values.count())
        idx = window_values.count() - 1;

    return window_values.at(idx);
}

/** Return whether or not this constraint is satisfied */
bool WindowedComponent::isSatisfied(const System &system) const
{
    if (window_values.isEmpty())
        return true;

    System copy_system(system);

    double current_val = copy_system.componentValue(constrained_component);
    double ref_val = copy_system.componentValue(reference_component);
    
    if (window_values.count() == 1)
        return window_values.at(0) == current_val;
    else
        return getNextWindow(ref_val, window_values, step_size) == current_val;
}

/** Apply this constraint to the system */
bool WindowedComponent::apply(System &system) const
{
    if (window_values.isEmpty())
        return false;

    Version old_version = system.version();
    
    double ref_val = system.componentValue(reference_component);

    if (window_values.count() == 1)
        system.setComponent(constrained_component, window_values.at(0));
    else
        system.setComponent(constrained_component, 
                            getNextWindow(ref_val, window_values, step_size) );
    
    return old_version != system.version();
}

const char* WindowedComponent::typeName()
{
    return QMetaType::typeName( qMetaTypeId<WindowedComponent>() );
}

NullConstraint* NullConstraint::clone() const
{
    return new NullConstraint(*this);
}

