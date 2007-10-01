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

//////////
////////// Implementation of WeightFunction
//////////

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
