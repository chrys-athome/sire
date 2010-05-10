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

#include "polarisecharges.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<PolariseCharges> r_polarise_charges;

QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const PolariseCharges &polchgs)
{
    writeHeader(ds, r_polarise_charges, 1);
    
    SharedDataStream sds(ds);
    
    sds << polchgs.field_component << polchgs.field_probe
        << static_cast<const ChargeConstraint&>(polchgs);

    return ds;
}

QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, 
                                          PolariseCharges &polchgs)
{
    VersionID v = readHeader(ds, r_polarise_charges);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        ProbePtr probe;
    
        polchgs = PolariseCharges();
        
        sds >> polchgs.field_component >> probe 
            >> static_cast<ChargeConstraint&>(polchgs);
            
        polchgs.setProbe(probe);
    }
    else
        throw version_error( v, "1", r_polarise_charges, CODELOC );

    return ds;
}

/** Null constructor */
PolariseCharges::PolariseCharges() 
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(),
                  probe_charge(0)
{}

/** Construct a constraint that uses the total energy field and a 
    single unit charge probe to polarise the molecules in 'molgroup' */
PolariseCharges::PolariseCharges(const MoleculeGroup &molgroup,
                                 const PropertyMap &map)
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(molgroup, map),
                  field_component(System::totalComponent())
{
    this->setProbe( CoulombProbe( 1*mod_electron ) );
}

/** Construct a constraint that uses the total energy field and 
    the passed probe to polarise the molecules in 'molgroup' */
PolariseCharges::PolariseCharges(const MoleculeGroup &molgroup,
                                 const Probe &probe, const PropertyMap &map)
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(molgroup, map),
                  field_component(System::totalComponent())
{
    this->setProbe(probe);
}

/** Construct a constraint that uses the field represented by 'field_component'
    and a single unit charge to polarise the molecules in 'molgroup' */
PolariseCharges::PolariseCharges(const MoleculeGroup &molgroup,
                                 const Symbol &fieldcomp, const PropertyMap &map)
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(molgroup, map),
                  field_component(fieldcomp)
{
    this->setProbe( CoulombProbe( 1*mod_electron ) );
}

/** Construct a constraint that uses the field represented by 'field_component'
    and the passed probe to polarise the molecules in 'molgroup' */
PolariseCharges::PolariseCharges(const MoleculeGroup &molgroup,
                                 const Symbol &fieldcomp, const Probe &probe,
                                 const PropertyMap &map)
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(molgroup, map),
                  field_component(fieldcomp)
{
    this->setProbe(probe);
}

/** Copy constructor */
PolariseCharges::PolariseCharges(const PolariseCharges &other)
                : ConcreteProperty<PolariseCharges,ChargeConstraint>(other),
                  field_component(other.field_component),
                  field_probe(other.field_probe), probe_charge(other.probe_charge)
{}

/** Destructor */
PolariseCharges::~PolariseCharges()
{}

/** Copy assignment operator */
PolariseCharges& PolariseCharges::operator=(const PolariseCharges &other)
{
    if (this != &other)
    {
        field_component = other.field_component;
        field_probe = other.field_probe;
        probe_charge = other.field_charge;
    
        ChargeConstraint::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool PolariseCharges::operator==(const PolariseCharges &other) const
{
    return (this == &other) or
           (field_component == other.field_component and
            field_probe == other.field_probe and ChargeConstraint::operator==(other));
}

/** Comparison operator */
bool PolariseCharges::operator!=(const PolariseCharges &other) const
{
    return not PolariseCharges::operator==(other);
}

const char* PolariseCharges::typeName()
{
    return QMetaType::typeName( qMetaTypeId<PolariseCharges>() );
}

PolariseCharges* PolariseCharges::clone() const
{
    return new PolariseCharges(*this);
}

/** Return a string representation of this constraint */
QString PolariseCharges::toString() const
{
    return "SireSystem::PolariseCharges";
}

/** Set the baseline system for the constraint - this is 
    used to pre-calculate everything for the system
    and to check if the constraint is satisfied */
void PolariseCharges::setSystem(const System &system)
{
    //loop over each molecule
    //{
        //if this molecule has moves
        //{
            //loop over each selected atom
    
            //if the connectivity has changed
                //find the atoms bonded to this atom
    
            //calculate the distance vector matrix to each atom
    
            //invert this matrix
        //}

        //calculate the potential on the atoms
        
        //calculate the potential difference
        
        //calculate the polarised charges
    //}
}

/** Return whether or not the changes in the passed
    delta *may* have changed the system since the last
    subversion 'subversion' */
bool PolariseCharges::mayChange(const Delta &delta, quint32 last_subversion) const
{
    return false;
}

/** Fully apply this constraint on the passed delta - this returns
    whether or not this constraint affects the delta */
bool PolariseCharges::fullApply(Delta &delta)
{
    return false;
}

/** Apply this constraint based on the delta, knowing that the 
    last application of this constraint was on this system, 
    at subversion number last_subversion */
bool PolariseCharges::deltaApply(Delta &delta, quint32 last_subversion)
{
    return false;
}
