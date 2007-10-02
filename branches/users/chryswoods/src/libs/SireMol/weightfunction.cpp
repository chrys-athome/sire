/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "weightfunction.h"

using namespace SireMol;
using namespace SireBase;

//////////
////////// Implementation of WeightFuncBase
//////////

/** Constructor */
WeightFuncBase::WeightFuncBase() : PropertyBase()
{}
  
/** Copy constructor */  
WeightFuncBase::WeightFuncBase(const WeightFuncBase &other)
               : PropertyBase(other)
{}
  
/** Destructror */  
WeightFuncBase::~WeightFuncBase()
{}

//////////
////////// Implementation of WeightFunction
//////////

static QSharedPolyPointer<WeightFuncBase>& shared_null;

const QSharedPolyPointer<WeightFuncBase>& getSharedNull()
{
    if (shared_null.constData() == 0)
        shared_null = new RelFromNumber();
        
    return shared_null;
}

/** Default constructor - this is a RelByNumber weight function */
WeightFunction::WeightFunction() : d( getSharedNull() )
{}

/** Create from the passed WeightFuncBase */
WeightFunction::WeightFunction(const WeightFuncBase &weightfunc)
               : d(weightfunc)
{}

/** Create from the passed property

    \throw SireError::invalid_cast
*/
WeightFunction::WeightFunction(const SireBase::Property &property)
               : d(property.base())
{}

/** Copy constructor */
WeightFunction::WeightFunction(const WeightFunction &other)
               d(other.d)
{}

/** Destructor */
WeightFunction::~WeightFunction()
{}

/** Copy assignment from a WeightFuncBase */
WeightFunction& WeightFunction::operator=(const WeightFuncBase &weightfunc)
{
    if (&weightfunc != d.constData())
    {
        d = weightfunc;
    }
    
    return *this;
}

/** Copy assignment operator */
WeightFunction& WeightFunction::operator=(const WeightFunction &weightfunc)
{
    if (&weightfunc != this)
    {
        d = other.d;
    }
    
    return *this;
}

/** Copy assignment from a Property

    \throw SireError::invalid_cast
*/
WeightFunction& WeightFunction::operator=(const SireBase::Property &other)
{
    d = other.base();
    return *this;
}

/** Comparison operator */
bool WeightFunction::operator==(const WeightFunction &other) const
{
    return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool WeightFunction::operator!=(const WeightFunction &other) const
{
    return d != other.d and *d != *(other.d);
}

/** Comparison operator to a Property */
bool WeightFunction::operator==(const Property &other) const
{
    return Property(*d) == other;
}

/** Comparison operator to a Property */
bool WeightFunction::operator!=(const Property &other) const
{
    return Property(*d) != other;
}

/** Return the relative weight of group0 and group1 in the molecule
    whose data is in 'moldata' using the passed PropertyMap to 
    find any required properties
    
    \throw SireError::incompatible_error
    \throw SireBase::missing_property
*/
double WeightFunction::operator()(const MoleculeData &moldata,
                                  const AtomSelection &group0,
                                  const AtomSelection &group1,
                                  const PropertyMap &map) const
{
    return d->operator()(moldata, group0, group1, map);
}

/** Return the relative weight of view0 and view1 using 
    map0 to find any required properties from view0, and 
    map1 to find any required properties from view1 
    
    \throw SireBase::missing_property
*/
double WeightFunction::operator()(const MoleculeView &view0,
                                  const PropertyMap &map0,
                                  const MoleculeView &view1,
                                  const PropertyMap &map1) const
{
    return d->operator()(view0, map0, view1, map1);
}

/** Return the relative weight of view0 and view1 using
    the supplied PropertyMap to find any required properties
    from both views
    
    \throw SireBase::missing_property
*/
double WeightFunction::operator()(const MoleculeView &view0,
                                  const MoleculeView &view1,
                                  const PropertyMap &map) const
{
    return d->operator()(view0, view1, map);
}

//////////
////////// Implementation of AbsFromNumber
//////////

AbsFromNumber::AbsFromNumber()
              : ConcreteProperty<AbsFromNumber,WeightFuncBase>()
{}

AbsFromNumber::AbsFromNumber(const AbsFromNumber &other)
              : ConcreteProperty<AbsFromNumber,WeightFuncBase>(other)
{}

~AbsFromNumber()
{}

inline double AbsFromNumber::weight(int nats0, int nats1)
{
    if (nats0 > nats1)
        return 1;
    else if (nats0 < nats1)
        return 0;
    else
        return 0.5;
}

double AbsFromNumber::operator()(const MoleculeData &moldata,
                                 const AtomSelection &group0,
                                 const AtomSelection &group1,
                                 const PropertyMap&) const
{
    moldata.assertCompatibleWith(group0);
    moldata.assertCompatibleWith(group1);

    return weight(group0.nAtoms(), group1.nAtoms());
}

double AbsFromNumber::operator()(const MoleculeView &view0,
                                 const MoleculeView &view1,
                                 const PropertyMap&) const
{
    return weight(view0.nAtoms(), view1.nAtoms());
}

//////////
////////// Implementation of RelFromNumber
//////////

RelFromNumber::RelFromNumber()
              : ConcreteProperty<RelFromNumber,WeightFuncBase>()
{}

RelFromNumber::AbsFromNumber(const RelFromNumber &other)
              : ConcreteProperty<RelFromNumber,WeightFuncBase>(other)
{}

~RelFromNumber()
{}

inline double RelFromNumber::weight(int nats0, int nats1)
{
    double total = nats0 + nats1;
    
    if (total > 0)
        return nats1 / total;
    else
        return 0.5;
}

double RelFromNumber::operator()(const MoleculeData &moldata,
                                 const AtomSelection &group0,
                                 const AtomSelection &group1,
                                 const PropertyMap&) const
{
    moldata.assertCompatibleWith(group0);
    moldata.assertCompatibleWith(group1);

    return weight(group0.nAtoms(), group1.nAtoms());
}

double RelFromNumber::operator()(const MoleculeView &view0,
                                 const MoleculeView &view1,
                                 const PropertyMap&) const
{
    return weight(view0.nAtoms(), view1.nAtoms());
}

//////////
////////// Implementation of AbsFromMass
//////////

AbsFromMass::AbsFromMass()
              : ConcreteProperty<AbsFromMass,WeightFuncBase>()
{}

AbsFromMass::AbsFromMass(const AbsFromMass &other)
              : ConcreteProperty<AbsFromMass,WeightFuncBase>(other)
{}

~AbsFromMass()
{}

inline double AbsFromMass::weight(double mass0, double mass1)
{
    if (mass0 > mass1)
        return 1;
    else if (mass0 < mass1)
        return 0;
    else
        return 0.5;
}

double AbsFromMass::operator()(const MoleculeData &moldata,
                               const AtomSelection &group0,
                               const AtomSelection &group1,
                               const PropertyMap &map) const
{
    return weight( PartialMolecule(moldata,group0).evaluate().mass(map),
                   PartialMolecule(moldata,group1).evaluate().mass(map) );
}

double AbsFromMass::operator()(const MoleculeView &view0,
                               const MoleculeView &view1,
                               const PropertyMap&) const
{
    return weight( view0.evaluate().mass(map),
                   view1.evaluate().mass(map) );
}

//////////
////////// Implementation of RelFromMass
//////////

RelFromMass::RelFromMass()
              : ConcreteProperty<RelFromMass,WeightFuncBase>()
{}

RelFromMass::RelFromMass(const RelFromMass &other)
              : ConcreteProperty<RelFromMass,WeightFuncBase>(other)
{}

~RelFromMass()
{}

inline double RelFromMass::weight(double mass0, double mass1)
{
    double total = mass0 + mass1;
    
    if (total > 0)
        return mass1 / total;
    else
        return 0.5;
}

double RelFromMass::operator()(const MoleculeData &moldata,
                               const AtomSelection &group0,
                               const AtomSelection &group1,
                               const PropertyMap &map) const
{
    return weight( PartialMolecule(moldata,group0).evaluate().mass(map),
                   PartialMolecule(moldata,group1).evaluate().mass(map) );
}

double RelFromMass::operator()(const MoleculeView &view0,
                               const MoleculeView &view1,
                               const PropertyMap&) const
{
    return weight( view0.evaluate().mass(map),
                   view1.evaluate().mass(map) );
}
