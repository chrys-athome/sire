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

#ifndef SIRESYSTEM_GEOMETRYCOMPONENT_H
#define SIRESYSTEM_GEOMETRYCOMPONENT_H

#include "constraint.h"

#include "SireCAS/expression.h"
#include "SireCAS/symbol.h"
#include "SireCAS/values.h"

#include "SireVol/space.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class GeometryComponent;
}

QDataStream& operator<<(QDataStream&, const SireSystem::GeometryComponent&);
QDataStream& operator>>(QDataStream&, SireSystem::GeometryComponent&);

namespace SireVol
{
class Space;
}

namespace SireSystem
{

/** This is the base class of constraints that constrains the value 
    of a system component
    to be equal to a geometric expression. For example, you could
    use this to constrain the value of the component "r_diff" to be
    equal to the difference between the distances between two pairs
    of atoms
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT GeometryComponent : public Constraint
{

friend QDataStream& ::operator<<(QDataStream&, const GeometryComponent&);
friend QDataStream& ::operator>>(QDataStream&, GeometryComponent&);

public:
    GeometryComponent();
    GeometryComponent(const GeometryComponent &other);
    
    ~GeometryComponent();
    
    static const char* typeName();
    
    const SireCAS::Symbol& component() const;
    
    const SireCAS::Expression& expression() const;
    
    bool isSatisfied(const System &system) const;
    
    bool dependsOnMolecules() const;
    
    bool apply(System &system) const;
    bool apply(System &system, SireMol::MolNum molnum) const;
    bool apply(System &system, const SireMol::Molecules &molecules) const;
    
    virtual void setSpace(const SireVol::Space &space);

    const SireVol::Space& space() const;
    
protected:
    GeometryComponent(const SireCAS::Symbol &constrained_symbol,
                      const SireCAS::Expression &geometry_expression);

    GeometryComponent& operator=(const GeometryComponent &other);
    bool operator==(const GeometryComponent &other) const;
    bool operator!=(const GeometryComponent &other) const;

    virtual bool wouldChange(const System &system, const SireCAS::Values &values) const=0;
    virtual SireCAS::Values getValues(const System &system)=0;

private:
    /** The symbol that is constrained to match the geometry expression */
    SireCAS::Symbol constrained_symbol;

    /** The expected current value of the symbol */
    double expected_value;
    
    /** The current values of the symbols used by this constraint */
    SireCAS::Values current_values;
    
    /** The geometry expression that is used to constrain the symbol */
    SireCAS::Expression geometry_expression;
    
    /** The space within which the geometry is evaluated */
    SireVol::SpacePtr spce;
};

}

SIRE_EXPOSE_CLASS( SireSystem::GeometryComponent )

SIRE_END_HEADER

#endif