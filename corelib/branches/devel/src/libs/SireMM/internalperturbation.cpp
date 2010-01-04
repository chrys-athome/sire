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

#include "internalperturbation.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireCAS;
using namespace SireBase;
using namespace SireStream;

////////////
//////////// Implementation of InternalPerturbation
////////////

static const RegisterMetaType<InternalPerturbation> r_intpert( MAGIC_ONLY,
                                                    InternalPerturbation::typeName() );

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const InternalPerturbation &intpert)
{
    writeHeader(ds, r_intpert, 1);
    
    SharedDataStream sds(ds);
    
    sds << intpert.base_expression << intpert.initial_forms
        << intpert.final_forms << static_cast<const Perturbation&>(intpert);
        
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      InternalPerturbation &intpert)
{
    VersionID v = readHeader(ds, r_intpert);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> intpert.base_expression >> intpert.initial_forms
            >> intpert.final_forms >> static_cast<Perturbation&>(intpert);
            
        intpert.buildPerturbExpression();
    }
    else
        throw version_error(v, "1", r_intpert, CODELOC);
        
    return ds;
}

/** Internal function used to rebuild the perturb expression from 
    the mapping function, base expression and initial and final forms */
void InternalPerturbation::buildPerturbExpression()
{
    //build all of the perturbed identities
    Identities perturbed_idents;
    
    const Expression &mapfunc = mappingFunction();
    
    foreach (Symbol symbol, initial_forms.symbols())
    {
        if (final_forms.contains(symbol))
        {
            Identities ident;
            ident.set( symbols().initial(), initial_forms[symbol] );
            ident.set( symbols().final(), final_forms[symbol] );
    
            perturbed_idents.set(symbol, mapfunc.substitute(ident));
        }
        else
        {
            Identities ident;
            ident.set( symbols().initial(), initial_forms[symbol] );
            ident.set( symbols().final(), Expression(0) );
    
            perturbed_idents.set(symbol, mapfunc.substitute(ident));
        }
    }
    
    foreach (Symbol symbol, final_forms.symbols())
    {
        if (not initial_forms.contains(symbol))
        {
            Identities ident;
            ident.set( symbols().initial(), Expression(0) );
            ident.set( symbols().final(), final_forms[symbol] );
                               
            perturbed_idents.set(symbol, mapfunc.substitute(ident));
        }
    }
    
    perturb_expression = base_expression.substitute(perturbed_idents);
}

/** Null constructor */
InternalPerturbation::InternalPerturbation() : Perturbation()
{}
                     
/** Construct to map from one function to another (from initial_function to 
    final_function) using the passed mapping function */
InternalPerturbation::InternalPerturbation(const Expression &initial_function,
                                           const Expression &final_function,
                                           const Expression &mapping_function,
                                           const PropertyMap &map)
                     : Perturbation(mapping_function, map)
{
    Symbol f("f");

    initial_forms = (f == initial_function);
    final_forms = (f == final_function);
    base_expression = f;
    
    this->buildPerturbExpression();
}

/** Construct to map from 'base_expression' populated with the identities
    in 'initial_forms' to 'base_expression' populated with the identities
    in 'final_forms', using the passed mapping function to map the identites
    from initial to final */
InternalPerturbation::InternalPerturbation(const Expression &base,
                                           const Identities &initial,
                                           const Identities &final,
                                           const Expression &mapping_function,
                                           const PropertyMap &map)
                     : Perturbation(mapping_function, map),
                       base_expression(base),
                       initial_forms(initial), final_forms(final)
{
    this->buildPerturbExpression();
}

/** Copy constructor */
InternalPerturbation::InternalPerturbation(const InternalPerturbation &other)
                     : Perturbation(other),
                       base_expression(other.base_expression),
                       perturb_expression(other.perturb_expression),
                       initial_forms(other.initial_forms),
                       final_forms(other.final_forms)
{}
 
/** Destructor */
InternalPerturbation::~InternalPerturbation()
{}

/** Copy assignment operator */
InternalPerturbation& InternalPerturbation::operator=(const InternalPerturbation &other)
{
    if (this != &other)
    {
        Perturbation::operator=(other);
        base_expression = other.base_expression;
        perturb_expression = other.perturb_expression;
        initial_forms = other.initial_forms;
        final_forms = other.final_forms;
    }
    
    return *this;
}

/** Comparison operator */
bool InternalPerturbation::operator==(const InternalPerturbation &other) const
{
    return base_expression == other.base_expression and 
           initial_forms == other.initial_forms and
           final_forms == other.final_forms and
           Perturbation::operator==(other);
}

/** Comparison operator */
bool InternalPerturbation::operator!=(const InternalPerturbation &other) const
{
    return not InternalPerturbation::operator==(other);
}

const char* InternalPerturbation::typeName()
{
    return "SireMM::InternalPerturbation";
}

/** Return the base expression - this is the expression into which 
    the mapped identites are substituted */
const Expression& InternalPerturbation::baseExpression() const
{
    return base_expression;
}

/** Return the perturbed expression - this is the expression that
    is used to calculate the energy */
const Expression& InternalPerturbation::perturbExpression() const
{
    return perturb_expression;
}

/** Return the initial forms - these are the identities that 
    are substituted into the base expression at the initial state */
const Identities& InternalPerturbation::initialForms() const
{
    return initial_forms;
}

/** Return the final forms - these are the identities that 
    are substituted into the base expression at the final state */
const Identities& InternalPerturbation::finalForms() const
{
    return final_forms;
}

PerturbationPtr InternalPerturbation::recreate(const Expression &expression) const
{
    PerturbationPtr ret = Perturbation::recreate(expression);
    
    ret.edit().asA<InternalPerturbation>().buildPerturbExpression();
    
    return ret;
}

PerturbationPtr InternalPerturbation::recreate(const Expression &expression,
                                               const PropertyMap &map) const
{
    PerturbationPtr ret = Perturbation::recreate(expression, map);
    
    ret.edit().asA<InternalPerturbation>().buildPerturbExpression();
    
    return ret;
}

PerturbationPtr InternalPerturbation::substitute(const Identities &identities) const
{
    PerturbationPtr ret = Perturbation::substitute(identities);
    
    ret.edit().asA<InternalPerturbation>().buildPerturbExpression();
    
    return ret;
}

////////////
//////////// Implementation of TwoAtomPerturbation
////////////


////////////
//////////// Implementation of ThreeAtomPerturbation
////////////


////////////
//////////// Implementation of FourAtomPerturbation
////////////

