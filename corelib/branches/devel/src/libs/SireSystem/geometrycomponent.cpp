/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include "geometrycomponent.h"

#include "SireSystem/system.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireCAS;
using namespace SireStream;

static const RegisterMetaType<GeometryComponent> r_geomcomp( MAGIC_ONLY,
                                                      GeometryComponent::typeName() );
                                                      
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const GeometryComponent &geomcomp)
{
    writeHeader(ds, r_geomcomp, 1);
    
    SharedDataStream sds(ds);
    
    sds << geomcomp.constrained_symbol << geomcomp.geometry_expression
        << geomcomp.spce
        << static_cast<const Constraint&>(geomcomp);
        
    return ds;
}

QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, GeometryComponent &geomcomp)
{
    VersionID v = readHeader(ds, r_geomcomp);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> geomcomp.constrained_symbol >> geomcomp.geometry_expression
            >> geomcomp.spce
            >> static_cast<Constraint&>(geomcomp);
            
        geomcomp.expected_value = 0;
        geomcomp.current_values = Values();
    }
    else
        throw version_error(v, "1", r_geomcomp, CODELOC);
        
    return ds;
}

/** Null constructor */
GeometryComponent::GeometryComponent() : Constraint(), expected_value(0)
{}

/** Internal constructor */
GeometryComponent::GeometryComponent(const Symbol &symbol, const Expression &expression)
                  : Constraint(), constrained_symbol(symbol),
                    expected_value(0), geometry_expression(expression)
{}

/** Copy constructor */
GeometryComponent::GeometryComponent(const GeometryComponent &other)
                  : Constraint(other),
                    constrained_symbol(other.constrained_symbol),
                    expected_value(other.expected_value),
                    current_values(other.current_values),
                    geometry_expression(other.geometry_expression),
                    spce(other.spce)
{}

/** Destructor */
GeometryComponent::~GeometryComponent()
{}

/** Copy assignment operator */
GeometryComponent& GeometryComponent::operator=(const GeometryComponent &other)
{
    if (this != &other)
    {
        Constraint::operator=(other);
        constrained_symbol = other.constrained_symbol;
        expected_value = other.expected_value;
        current_values = other.current_values;
        geometry_expression = other.geometry_expression;
        spce = other.spce;
    }
    
    return *this;
}

/** Comparison operator */
bool GeometryComponent::operator==(const GeometryComponent &other) const
{
    return this == &other or
           (Constraint::operator==(other) and
            constrained_symbol == other.constrained_symbol and 
            geometry_expression == other.geometry_expression and
            spce == other.spce);
}

/** Comparison operator */
bool GeometryComponent::operator!=(const GeometryComponent &other) const
{
    return not GeometryComponent::operator==(other);
}

const char* GeometryComponent::typeName()
{
    return "SireSystem::GeometryComponent";
}

/** Return the symbol representing the constrained component */
const Symbol& GeometryComponent::component() const
{
    return constrained_symbol;
}

/** Return the expression used to calculate the constrained value
    from the geometry */
const Expression& GeometryComponent::expression() const
{
    return geometry_expression;
}

/** Return the space used to evaluate the geometry */
const Space& GeometryComponent::space() const
{
    return spce.read();
}

/** Set the space used to evaluate the geometry */
void GeometryComponent::setSpace(const Space &space)
{
    spce = space;
}

/** Return whether or not this constraint is satisfied for
    the passed system */
bool GeometryComponent::isSatisfied(const System &system) const
{
    if (not system.hasConstant(constrained_symbol))
        return false;

    else if (this->wouldChange(system, current_values))
    {
        std::auto_ptr<GeometryComponent> new_geomcomp( 
                                            (GeometryComponent*)(this->clone()) );
        
        Values vals = new_geomcomp->getValues(system);
        
        return geometry_expression(vals) == system.getConstant(constrained_symbol);
    }
    else
    {
        return system.getConstant(constrained_symbol) == expected_value;
    }
}

Q_GLOBAL_STATIC( QMutex, applyMutex )
    
bool GeometryComponent::dependsOnMolecules() const
{
    return true;
}

/** Apply this constraint to the passed system, returning whether
    or not this changes the system */
bool GeometryComponent::apply(System &system) const
{
    if (this->wouldChange(system, current_values))
    {
        std::auto_ptr<GeometryComponent> d( (GeometryComponent*)(this->clone()) );
    
        Values vals = d->getValues(system);
        
        double new_value = geometry_expression(vals);
        
        bool changed = false;

        if (system.hasConstant(constrained_symbol))
        {
            if (system.getConstant(constrained_symbol) != new_value)
            {
                system.setConstant(constrained_symbol, new_value);
                changed = true;
            }
        }
        else
        {
            system.setConstant(constrained_symbol, new_value);
            changed = true;
        }
            
        d->current_values = vals;
        d->expected_value = new_value;

        QMutexLocker lkr( applyMutex() );
        const_cast<GeometryComponent*>(this)->copy(*d);

        return changed;
    }
    else if (system.hasConstant(constrained_symbol))
    {
        if (system.getConstant(constrained_symbol) != expected_value)
        {
            system.setConstant(constrained_symbol, expected_value);
            return true;
        }
        else
            return false;
    }
    else
    {
        system.setConstant(constrained_symbol, expected_value);
        return true;
    }
}
    
bool GeometryComponent::apply(System &system, MolNum) const
{
    return this->apply(system);
}

bool GeometryComponent::apply(System &system, const Molecules&) const
{
    return this->apply(system);
}

