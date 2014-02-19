/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#include "interff.h"
#include "cljshiftfunction.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireFF;
using namespace SireBase;
using namespace SireStream;

static RegisterMetaType<InterFF> r_interff;

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const InterFF &interff)
{
    writeHeader(ds, r_interff, 1);
    
    SharedDataStream sds(ds);
    sds << static_cast<const G1FF&>(interff);
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, InterFF &interff)
{
    VersionID v = readHeader(ds, r_interff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> static_cast<G1FF&>(interff);
        
        interff._pvt_updateName();
    }
    else
        throw version_error(v, "1", r_interff, CODELOC);
    
    return ds;
}

/** Constructor */
InterFF::InterFF() : ConcreteProperty<InterFF,G1FF>()
{
    this->_pvt_updateName();
}

/** Construct, specifying the name of the forcefield */
InterFF::InterFF(const QString &name) : ConcreteProperty<InterFF, G1FF>()
{
    G1FF::setName(name);
}

/** Copy constructor */
InterFF::InterFF(const InterFF &other)
        : ConcreteProperty<InterFF,G1FF>(other),
          cljboxes(other.cljboxes), fixed_atoms(other.fixed_atoms),
          cljcomps(other.cljcomps)
{}

/** Destructor */
InterFF::~InterFF()
{}

/** Internal function called when the name of the forcefield changes */
void InterFF::_pvt_updateName()
{
    cljcomps = CLJComponent( this->name() );
    G1FF::_pvt_updateName();
}

const char* InterFF::typeName()
{
    return QMetaType::typeName( qMetaTypeId<InterFF>() );
}

const char* InterFF::what() const
{
    return InterFF::typeName();
}

/** Copy assignment operator */
InterFF& InterFF::operator=(const InterFF &other)
{
    if (this != &other)
    {
        cljboxes = other.cljboxes;
        fixed_atoms = other.fixed_atoms;
        cljcomps = other.cljcomps;
        G1FF::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool InterFF::operator==(const InterFF &other) const
{
    return (this == &other) or
           (G1FF::operator==(other) and fixed_atoms == other.fixed_atoms);
}

/** Comparison operator */
bool InterFF::operator!=(const InterFF &other) const
{
    return not operator==(other);
}

InterFF* InterFF::clone() const
{
    return new InterFF(*this);
}

/** Return the energy components of this forcefield */
const CLJComponent& InterFF::components() const
{
    return cljcomps;
}

/** Set the forcefield property called 'name' to the value 'property' */
bool InterFF::setProperty(const QString &name, const Property &property)
{
    PropertyPtr old_prop = cljfunc.read().property(name);
    
    if (not property.equals(old_prop.read()))
    {
        //need to set the property
        cljfunc = cljfunc.read().setProperty(name, property);
        props = cljfunc.read().properties();
        return true;
    }
    else
        return false;
}

/** Return the value of the forcefield property with name 'name' */
const Property& InterFF::property(const QString &name) const
{
    PropertyPtr prop = cljfunc.read().property(name);
    
    const Property &p = props.property(name);
    
    if (not p.equals(prop.read()))
    {
        const_cast<InterFF*>(this)->props = cljfunc.read().properties();
        return props.property(name);
    }
    
    return p;
}

/** Return whether or not this forcefield contains the property 'property' */
bool InterFF::containsProperty(const QString &name) const
{
    return cljfunc.read().containsProperty(name);
}

/** Return all of the properties of this function */
const Properties& InterFF::properties() const
{
    return props;
}

/** Signal that this forcefield must now be recalculated from scratch */
void InterFF::mustNowRecalculateFromScratch()
{}

/** Recalculate the energy of this forcefield */
void InterFF::recalculateEnergy()
{
    cljcomps.setEnergy(*this, CLJEnergy(0.0,0.0));
}

/** Function called to add a molecule to this forcefield */
void InterFF::_pvt_added(const SireMol::PartialMolecule &mol, const SireBase::PropertyMap &map)
{}

/** Function called to remove a molecule from this forcefield */
void InterFF::_pvt_removed(const SireMol::PartialMolecule &mol)
{}

/** Function called to indicate that a molecule in this forcefield has changed */
void InterFF::_pvt_changed(const SireMol::Molecule &molecule)
{}

/** Function called to indicate that a list of molecules in this forcefield have changed */
void InterFF::_pvt_changed(const QList<SireMol::Molecule> &molecules)
{}

/** Function called to indicate that all molecules in this forcefield have been removed */
void InterFF::_pvt_removedAll()
{}

/** Function called to query whether or not a change in source properties would
    change the properties needed by this forcefield for the molecule with number 'molnum' */
bool InterFF::_pvt_wouldChangeProperties(SireMol::MolNum molnum,
                                         const SireBase::PropertyMap &map) const
{
    return false;
}
