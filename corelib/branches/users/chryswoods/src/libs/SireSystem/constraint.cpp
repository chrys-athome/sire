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

#include "SireSystem/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

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
    }
    else
        throw version_error( v, "1", r_constraint, CODELOC );
        
    return ds;
}

/** Constructor */
Constraint::Constraint() : Property()
{}

/** Copy constructor */
Constraint::Constraint(const Constraint &other) : Property(other)
{}

/** Destructor */
Constraint::~Constraint()
{}

/** Copy assignment operator */
Constraint& Constraint::operator=(const Constraint &other)
{
    Property::operator=(other);
    return *this;
}

/** Assert that the constraint is satisfied in the passed system 

    \throw SireSystem::constraint_error
*/
void Constraint::assertSatisfied(System &system) const
{
    if (not this->isSatisfied(system))
        throw SireSystem::constraint_error( QObject::tr(
            "The constraint %1 is not valid in the system %2.")
                .arg(this->toString())
                .arg(system.name()), CODELOC );
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
        
        sds >> constraint.ffid >> constraint.propname
            >> constraint.eqn
            >> static_cast<Constraint&>(constraint);
    }
    else
        throw version_error(v, "1", r_propconstraint, CODELOC);
        
    return ds;
}

/** Null constructor */
PropertyConstraint::PropertyConstraint()
                   : ConcreteProperty<PropertyConstraint,Constraint>()
{}

/** Construct to constrain the property with name 'name' in all forcefields
    to have the value resulting from the expression 'expression' */
PropertyConstraint::PropertyConstraint(const QString &name, 
                                       const SireCAS::Expression &expression)
                   : ConcreteProperty<PropertyConstraint,Constraint>(),
                     propname(name), eqn(expression)
{}

/** Construct to constrain the property with name 'name' in the forcefield(s)
    that match the ID 'ffid' to have the value resulting the expression
    'expression' */
PropertyConstraint::PropertyConstraint(const QString &name, const SireFF::FFID &id,
                                       const SireCAS::Expression &expression)
                   : ConcreteProperty<PropertyConstraint,Constraint>(),
                     ffid(id), propname(name), eqn(expression)
{}

/** Copy constructor */
PropertyConstraint::PropertyConstraint(const PropertyConstraint &other)
                   : ConcreteProperty<PropertyConstraint,Constraint>(other),
                     ffid(other.ffid), propname(other.propname), eqn(other.eqn)
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
        propname = other.propname;
        eqn = other.eqn;
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

/** Return whether or not this constraint is satisfied in the passed system */
bool PropertyConstraint::isSatisfied(System &system) const
{
    //evaluate the equation
    Values vals = system.componentValues( eqn.symbols() );
    double val = eqn.evaluate(vals);

    //does the system have a property with the right value?
    double sysval = system.property(ffid, propname).asA<VariantProperty>()
                                                   .convertTo<double>();
                                                   
    return val == sysval;
}

/** Apply this constraint to the system */
void PropertyConstraint::apply(System &system) const
{
    //evaluate the equation
    Values vals = system.componentValues( eqn.symbols() );
    double val = eqn.evaluate(vals);
    
    system.setProperty(ffid, propname, VariantProperty(val));
}
