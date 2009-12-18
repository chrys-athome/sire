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

#include "perturbation.h"
#include "molecule.h"
#include "moleditor.h"
#include "mover.hpp"

#include "SireCAS/values.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireCAS;
using namespace SireStream;

//////////
////////// Implementation of PerturbationSymbols
//////////

PerturbationSymbols::PerturbationSymbols()
                    : lam("lambda"), init("initial"), finl("final")
{}

PerturbationSymbols::~PerturbationSymbols()
{}

/** Return the symbol used to represent the driving (reaction)
    coordinate */
const Symbol& PerturbationSymbols::lambda() const
{
    return lam;
}

/** Return the symbol used to represent the initial state */
const Symbol& PerturbationSymbols::initial() const
{
    return init;
}

/** Return the symbol used to represent the final state */
const Symbol& PerturbationSymbols::final() const
{
    return finl;
}

//////////
////////// Implementation of Perturbation
//////////

static const RegisterMetaType<Perturbation> r_pert( MAGIC_ONLY, 
                                                    Perturbation::typeName() );
                                                    
/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const Perturbation &pert)
{
    writeHeader(ds, r_pert, 1);
    
    SharedDataStream sds(ds);
    
    sds << pert.mapping_eqn << pert.map << static_cast<const Property&>(pert);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Perturbation &pert)
{
    VersionID v = readHeader(ds, r_pert);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> pert.mapping_eqn >> pert.map >> static_cast<Property&>(pert);
    }
    else
        throw version_error(v, "1", r_pert, CODELOC);
        
    return ds;
}

static Expression *default_equation(0);

Q_GLOBAL_STATIC( QMutex, globalMutex )
Q_GLOBAL_STATIC( PerturbationSymbols, perturbationSymbols )

/** Return the symbols object that contains the symbols used
    by the mapping equation */
const PerturbationSymbols& Perturbation::symbols()
{
    return *(perturbationSymbols());
}

/** Return the default mapping equation for the perturbations - this
    linearly maps from the initial values at lambda=0 to the
    final value at lambda=1 */
const Expression& Perturbation::defaultFunction()
{
    if (not default_equation)
    {
        QMutexLocker lkr( globalMutex() );
        
        if (not default_equation)
        {
            default_equation = new Expression();
            
            *default_equation = ((1 - Perturbation::symbols().lambda()) * 
                                            Perturbation::symbols().initial() ) + 
                                ( Perturbation::symbols().lambda() * 
                                            Perturbation::symbols().final() );
        }
    }

    return *default_equation;
}

/** Constructor */
Perturbation::Perturbation() : Property(), mapping_eqn( defaultFunction() )
{}

/** Copy constructor */
Perturbation::Perturbation(const Perturbation &other) 
             : Property(other), mapping_eqn(other.mapping_eqn), map(other.map)
{}

/** Destructor */
Perturbation::~Perturbation()
{}

/** Copy assignment operator */
Perturbation& Perturbation::operator=(const Perturbation &other)
{
    if (this != &other)
    {
        mapping_eqn = other.mapping_eqn;
        map = other.map;
        
        Property::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Perturbation::operator==(const Perturbation &other) const
{
    return mapping_eqn == other.mapping_eqn and map == other.map and
           Property::operator==(other);
}

/** Comparison operator */
bool Perturbation::operator!=(const Perturbation &other) const
{
    return not Perturbation::operator==(other);
}

/** Return the equation used to control the mapping from the
    the initial value (represented using symbols().initial()) to
    the final value (represented using symbols().final()) as a 
    function of the reaction coordinate (which is normally
    represented using symbols().lambda()) */
const Expression& Perturbation::mappingFunction() const
{
    return mapping_eqn;
}
    
/** Return the property map used to find the properties used,
    and affected by this perturbation */
const PropertyMap& Perturbation::propertyMap() const
{
    return map;
}

/** Perturb the passed molecule, returning the result

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError:incompatible_error
*/
Molecule Perturbation::perturb(const Molecule &molecule, const Values &values) const
{
    MolEditor editor = molecule.edit();
    this->perturbMolecule(editor, values);
    return editor.commit();
}

const char* Perturbation::typeName()
{
    return "SireMol::Perturbation";
}

Q_GLOBAL_STATIC( PerturbationPtr, perturbationPtr );

const NullPerturbation& Perturbation::null()
{
    PerturbationPtr *ptr = perturbationPtr();
    
    if (ptr->constData() == 0)
    {
        QMutexLocker lkr( globalMutex() );
        
        if (ptr->constData() == 0)
            *ptr = new NullPerturbation();
    }
    
    return ptr->constData()->asA<NullPerturbation>();
}

//////////
////////// Implementation of NullPerturbation
//////////

static const RegisterMetaType<NullPerturbation> r_nullpert;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const NullPerturbation &nullpert)
{
    writeHeader(ds, r_nullpert, 1);
    
    ds << static_cast<const Perturbation&>(nullpert);
    
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, NullPerturbation &nullpert)
{
    VersionID v = readHeader(ds, r_nullpert);
    
    if (v == 1)
    {
        ds >> static_cast<Perturbation&>(nullpert);
    }
    else
        throw version_error(v, "1", r_nullpert, CODELOC);
        
    return ds;
}

NullPerturbation::NullPerturbation() : ConcreteProperty<NullPerturbation,Perturbation>()
{}

NullPerturbation::NullPerturbation(const NullPerturbation &other)
                 : ConcreteProperty<NullPerturbation,Perturbation>(other)
{}

NullPerturbation::~NullPerturbation()
{}

const char* NullPerturbation::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullPerturbation>() );
}

NullPerturbation& NullPerturbation::operator=(const NullPerturbation &other)
{
    Perturbation::operator=(other);
    return *this;
}

bool NullPerturbation::operator==(const NullPerturbation &other) const
{
    return Perturbation::operator==(other);
}

bool NullPerturbation::operator!=(const NullPerturbation &other) const
{
    return Perturbation::operator!=(other);
}

void NullPerturbation::perturbMolecule(MolEditor&, const Values&) const
{}

//////////
////////// Implementation of Perturbations
//////////

static const RegisterMetaType<Perturbations> r_perts;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const Perturbations &perts)
{
    writeHeader(ds, r_perts, 1);
    
    SharedDataStream sds(ds);
    
    sds << perts.perts << static_cast<const Perturbation&>(perts);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Perturbations &perts)
{
    VersionID v = readHeader(ds, r_perts);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> perts.perts >> static_cast<Perturbation&>(perts);
    }
    else
        throw version_error(v, "1", r_perts, CODELOC);
        
    return ds;
}

/** Constructor */
Perturbations::Perturbations() : ConcreteProperty<Perturbations,Perturbation>()
{}

/** Construct just to perform the passed perturbation */
Perturbations::Perturbations(const Perturbation &perturbation)
              : ConcreteProperty<Perturbations,Perturbation>()
{
    perts.append(perturbation);
}

/** Construct to perform the passed perturbations */
Perturbations::Perturbations(const QList<PerturbationPtr> &perturbations)
              : ConcreteProperty<Perturbations,Perturbation>(),
                perts(perturbations)
{}

/** Copy constructor */
Perturbations::Perturbations(const Perturbations &other)
              : ConcreteProperty<Perturbations,Perturbation>(other), 
                perts(other.perts)
{}

/** Destructor */
Perturbations::~Perturbations()
{}

/** Copy assignment operator */
Perturbations& Perturbations::operator=(const Perturbations &other)
{
    perts = other.perts;
    Perturbation::operator=(other);
    return *this;
}

/** Comparison operator */
bool Perturbations::operator==(const Perturbations &other) const
{
    return perts == other.perts and Perturbation::operator==(other);
}

/** Comparison operator */
bool Perturbations::operator!=(const Perturbations &other) const
{
    return perts != other.perts or Perturbation::operator!=(other);
}

const char* Perturbations::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Perturbations>() );
}

/** Return a list of all perturbations performed by this object */
QList<PerturbationPtr> Perturbations::perturbations() const
{
    return perts;
}

/** Apply this perturbation to the passed molecule for the 
    specified lambda value */
void Perturbations::perturbMolecule(MolEditor &molecule, const Values &values) const
{
    //if a new mapping equation is specified then change lambda to
    //the result of the equation - this allows a non-linear change
    //with respect to lambda to be applied to all of the perturbations
    //by specifying the new equation once here
    if ( this->mappingFunction() != Perturbation::defaultFunction() )
    {
        double new_lambda = this->mappingFunction().evaluate(values);
        
        Values new_values = values + (symbols().lambda() == new_lambda);
        
        for (QList<PerturbationPtr>::const_iterator it = perts.constBegin();
             it != perts.constEnd();
             ++it)
        {
            it->read().perturbMolecule(molecule, new_values);
        }
    }
    else
    {
        for (QList<PerturbationPtr>::const_iterator it = perts.constBegin();
             it != perts.constEnd();
             ++it)
        {
            it->read().perturbMolecule(molecule, values);
        }
    }
}
