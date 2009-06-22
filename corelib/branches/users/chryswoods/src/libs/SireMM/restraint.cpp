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

#include "restraint.h"

#include "SireMol/moleculedata.h"
#include "SireMol/molecules.h"
#include "SireMol/molid.h"
#include "SireMol/molnum.h"

#include "SireFF/forcetable.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireFF;
using namespace SireMol;
using namespace SireBase;
using namespace SireUnits::Dimension;
using namespace SireStream;

////////////
//////////// Implementation of Restraint
////////////

static const RegisterMetaType<Restraint> r_restraint( MAGIC_ONLY, 
                                                      Restraint::typeName() );

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const Restraint &restraint)
{
    writeHeader(ds, r_restraint, 1);
    
    ds << static_cast<const Property&>(restraint);
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, Restraint &restraint)
{
    VersionID v = readHeader(ds, r_restraint);
    
    if (v == 1)
    {
        ds >> static_cast<Property&>(restraint);
    }
    else
        throw version_error( v, "1", r_restraint, CODELOC );
        
    return ds;
}

/** Constructor */
Restraint::Restraint() : Property()
{}

/** Copy constructor */
Restraint::Restraint(const Restraint &other) : Property(other)
{}

/** Destructor */
Restraint::~Restraint()
{}

/** Copy assignment operator */
Restraint& Restraint::operator=(const Restraint &other)
{
    if (this != &other)
    {
        Property::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Restraint::operator==(const Restraint &other) const
{
    return Property::operator==(other);
}

/** Comparison operator */
bool Restraint::operator!=(const Restraint &other) const
{
    return Property::operator!=(other);
}

Q_GLOBAL_STATIC( NullRestraint, nullRestraint )

/** Return the global null restraint */
const NullRestraint& Restraint::null()
{
    return *(nullRestraint());
}

////////////
//////////// Implementation of Restraint3D
////////////

static const RegisterMetaType<Restraint3D> r_restraint3d( MAGIC_ONLY,
                                                          Restraint3D::typeName() );

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const Restraint3D &restraint3d)
{
    writeHeader(ds, r_restraint3d, 1);

    SharedDataStream sds(ds);

    sds << restraint3d.spce
        << static_cast<const Restraint&>(restraint3d);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, Restraint3D &restraint3d)
{
    VersionID v = readHeader(ds, r_restraint3d);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> restraint3d.spce
            >> static_cast<Restraint&>(restraint3d);
    }
    else
        throw version_error( v, "1", r_restraint3d, CODELOC );
        
    return ds;
}

/** Constructor */
Restraint3D::Restraint3D() : Restraint()
{}

/** Copy constructor */
Restraint3D::Restraint3D(const Restraint3D &other)
            : Restraint(), spce(other.spce)
{}

/** Destructor */
Restraint3D::~Restraint3D()
{}

/** Copy assignment operator */
Restraint3D& Restraint3D::operator=(const Restraint3D &other)
{
    Restraint::operator=(other);
    spce = other.spce;
    
    return *this;
}

/** Comparison operator */
bool Restraint3D::operator==(const Restraint3D &other) const
{
    return spce == other.spce and Restraint::operator==(other);
}

/** Comparison operator */
bool Restraint3D::operator!=(const Restraint3D &other) const
{
    return spce != other.spce or Restraint::operator!=(other);
}

/** Return the 3D space in which this restraint operates */
const Space& Restraint3D::space() const
{
    return spce.read();
}

/** Set the 3D space in which this restraint operates */
void Restraint3D::setSpace(const Space &space)
{
    spce = space;
}

////////////
//////////// Implementation of NullRestraint
////////////

static const RegisterMetaType<NullRestraint> r_nullrestraint;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, 
                                      const NullRestraint &nullrestraint)
{
    writeHeader(ds, r_nullrestraint, 1);
    
    ds << static_cast<const Restraint3D&>(nullrestraint);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, NullRestraint &nullrestraint)
{
    VersionID v = readHeader(ds, r_nullrestraint);
    
    if (v == 1)
    {
        ds >> static_cast<Restraint3D&>(nullrestraint);
    }
    else
        throw version_error( v, "1", r_nullrestraint, CODELOC );
        
    return ds;
}

/** Constructor */
NullRestraint::NullRestraint() : ConcreteProperty<NullRestraint,Restraint3D>()
{}

/** Copy constructor */
NullRestraint::NullRestraint(const NullRestraint &other)
              : ConcreteProperty<NullRestraint,Restraint3D>(other)
{}

/** Destructor */
NullRestraint::~NullRestraint()
{}

/** Copy assignment operator */
NullRestraint& NullRestraint::operator=(const NullRestraint &other)
{
    Restraint3D::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullRestraint::operator==(const NullRestraint &other) const
{
    return Restraint3D::operator==(other);
}

/** Comparison operator */
bool NullRestraint::operator!=(const NullRestraint &other) const
{
    return Restraint3D::operator!=(other);
}

const char* NullRestraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullRestraint>() );
}

/** The null restraint has no energy */
MolarEnergy NullRestraint::energy() const
{
    return MolarEnergy(0);
}

/** The null restraint will not change the force */
void NullRestraint::force(MolForceTable&, double) const
{}

/** The null restraint will not change the force */
void NullRestraint::force(ForceTable&, double) const
{}

/** The null restraint cannot be updated */
void NullRestraint::update(const MoleculeData&, const PropertyMap&)
{}

/** The null restraint cannot be updated */
void NullRestraint::update(const Molecules&, const PropertyMap&)
{}

/** There are no molecules in the NullRestraint */
Molecules NullRestraint::molecules() const
{
    return Molecules();
}

/** There are no molecules in the NullRestraint */
bool NullRestraint::contains(MolNum) const
{
    return false;
}

/** There are no molecules in the NullRestraint */
bool NullRestraint::contains(const MolID&) const
{
    return false;
}
    
/** There are no molecules in the NullRestraint */
bool NullRestraint::usesMoleculesIn(const ForceTable &forcetable) const
{
    return false;
}

/** There are no molecules in the NullRestraint */
bool NullRestraint::usesMoleculesIn(const Molecules &molecules) const
{
    return false;
}
