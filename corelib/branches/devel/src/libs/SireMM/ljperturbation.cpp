/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "ljperturbation.h"
#include "atomljs.h"

#include "SireMol/molecule.h"
#include "SireMol/moleditor.h"
#include "SireMol/mover.hpp"

#include "SireCAS/values.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireCAS;
using namespace SireBase;
using namespace SireUnits::Dimension;
using namespace SireStream;

static const RegisterMetaType<LJPerturbation> r_ljpert;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const LJPerturbation &ljpert)
{
    writeHeader(ds, r_ljpert, 1);
    
    ds << static_cast<const Perturbation&>(ljpert);
    
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       LJPerturbation &ljpert)
{
    VersionID v = readHeader(ds, r_ljpert);
    
    if (v == 1)
    {
        ds >> static_cast<Perturbation&>(ljpert);
    }
    else
        throw version_error(v, "1", r_ljpert, CODELOC);
        
    return ds;
}

/** Constructor - this creates a LJ perturbation that 
    perturbs from LJs in "initial_LJ" to LJs in
    "final_LJ", placing the current LJs in "LJ",
    and using Perturbation::defaultEquation() to map the
    charges */
LJPerturbation::LJPerturbation()
               : ConcreteProperty<LJPerturbation,Perturbation>()
{}

/** Construct, using the passed map to find the properties used
    by this perturbation */
LJPerturbation::LJPerturbation(const PropertyMap &map)
               : ConcreteProperty<LJPerturbation,Perturbation>(map)
{}

/** Construct, using the passed map to find the properties used
    by this perturbation and the passed mapping function to map
    the LJs between the states */
LJPerturbation::LJPerturbation(const Expression &mapping_function,
                               const PropertyMap &map)
               : ConcreteProperty<LJPerturbation,Perturbation>(mapping_function, map)
{}

/** Copy constructor */
LJPerturbation::LJPerturbation(const LJPerturbation &other)
               : ConcreteProperty<LJPerturbation,Perturbation>(other)
{}

/** Destructor */
LJPerturbation::~LJPerturbation()
{}

const char* LJPerturbation::typeName()
{
    return QMetaType::typeName( qMetaTypeId<LJPerturbation>() );
}

/** Copy assignment operator */
LJPerturbation& LJPerturbation::operator=(const LJPerturbation &other)
{
    Perturbation::operator=(other);
    return *this;
}

/** Comparison operator */
bool LJPerturbation::operator==(const LJPerturbation &other) const
{
    return Perturbation::operator==(other);
}

/** Comparison operator */
bool LJPerturbation::operator!=(const LJPerturbation &other) const
{
    return not LJPerturbation::operator==(other);
}

/** Perturb the LJs in the passed molecule using the reaction
    coordinate(s) in 'values' 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void LJPerturbation::perturbMolecule(MolEditor &molecule, const Values &values) const
{
    const AtomLJs &initial_ljs = molecule.property( propertyMap()["initial_LJ"] )
                                         .asA<AtomLJs>();
                                           
    const AtomLJs &final_ljs = molecule.property( propertyMap()["final_LJ"] )
                                       .asA<AtomLJs>();
                                            
    AtomLJs ljs(initial_ljs);
    
    const Expression &f = this->mappingFunction();
    const Symbol &initial = this->symbols().initial();
    const Symbol &final = this->symbols().final();
    
    for (CGIdx i(0); i<initial_ljs.nCutGroups(); ++i)
    {
        for (Index j(0); j<initial_ljs.nAtoms(i); ++j)
        {
            CGAtomIdx atomidx(i,j);

            Values atom_values = values + 
                                    (initial == initial_ljs[atomidx].sigma().value()) +
                                    (final == final_ljs[atomidx].sigma().value());
        
            double new_sigma = f(atom_values);
            
            atom_values = values +
                            (initial == initial_ljs[atomidx].epsilon().value()) +
                            (final == final_ljs[atomidx].epsilon().value());
                            
            double new_epsilon = f(atom_values);
        
            ljs.set( atomidx, LJParameter(Length(new_sigma), MolarEnergy(new_epsilon)) );
        }
    }
    
    molecule.setProperty( propertyMap()["LJ"].source(), ljs );
}
