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

#include "velocitygenerator.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireCAS;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of VelocityGenerator
/////////

static const RegisterMetaType<VelocityGenerator> r_velgen( MAGIC_ONLY,
                                                    VelocityGenerator::typeName() );
                                                    
/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const VelocityGenerator &velgen)
{
    writeHeader(ds, r_velgen, 1);
    
    ds << static_cast<const Property&>(velgen);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, VelocityGenerator &velgen)
{
    VersionID v = readHeader(ds, r_velgen);
    
    if (v == 1)
    {
        ds >> static_cast<Property&>(velgen);
    }
    else
        throw version_error( v, "1", r_velgen, CODELOC );
        
    return ds;
}

/** Constructor */
VelocityGenerator::VelocityGenerator() : Property()
{}

/** Copy constructor */
VelocityGenerator::VelocityGenerator(const VelocityGenerator &other)
                  : Property(other)
{}

/** Destructor */
VelocityGenerator::~VelocityGenerator()
{}

/** Copy assignment operator */
VelocityGenerator& VelocityGenerator::operator=(const VelocityGenerator &other)
{
    Property::operator=(other);
    return *this;
}

/** Comparison operator */
bool VelocityGenerator::operator==(const VelocityGenerator&) const
{
    return true;
}

/** Comparison operator */
bool VelocityGenerator::operator!=(const VelocityGenerator&) const
{
    return false;
}

Q_GLOBAL_STATIC( NullVelocityGenerator, nullVelocityGenerator )

/** Return the global null generator */
const NullVelocityGenerator& VelocityGenerator::null()
{
    return *(nullVelocityGenerator());
}

/////////
///////// Implementation of NullVelocityGenerator
/////////

static const RegisterMetaType<NullVelocityGenerator> r_nullvelgen;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const NullVelocityGenerator &nullvelgen)
{
    writeHeader(ds, r_nullvelgen, 1);
    
    ds << static_cast<const VelocityGenerator&>(nullvelgen);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, 
                                        NullVelocityGenerator &nullvelgen)
{
    VersionID v = readHeader(ds, r_nullvelgen);
    
    if (v == 1)
    {
        ds >> static_cast<VelocityGenerator&>(nullvelgen);
    }
    else
        throw version_error(v, "1", r_nullvelgen, CODELOC);
        
    return ds;
}

/** Constructor */
NullVelocityGenerator::NullVelocityGenerator()
                      : ConcreteProperty<NullVelocityGenerator,VelocityGenerator>()
{}

/** Copy constructor */
NullVelocityGenerator::NullVelocityGenerator(const NullVelocityGenerator &other)
                      : ConcreteProperty<NullVelocityGenerator,VelocityGenerator>(other)
{}

/** Destructor */
NullVelocityGenerator::~NullVelocityGenerator()
{}

/** Copy assignment operator */
NullVelocityGenerator& 
NullVelocityGenerator::operator=(const NullVelocityGenerator &other)
{
    VelocityGenerator::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullVelocityGenerator::operator==(const NullVelocityGenerator &other) const
{
    return VelocityGenerator::operator==(other);
}

/** Comparison operator */
bool NullVelocityGenerator::operator!=(const NullVelocityGenerator &other) const
{
    return VelocityGenerator::operator!=(other);
}

/////////
///////// Implementation of VelocitiesFromProperty
/////////

static const RegisterMetaType<VelocitiesFromProperty> r_velfromprop;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const VelocitiesFromProperty &velfromprop)
{
    writeHeader(ds, r_velfromprop, 1);
    
    SharedDataStream sds(ds);
    
    sds << velfromprop.vel_property
        << static_cast<const VelocityGenerator&>(velfromprop);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        VelocitiesFromProperty &velfromprop)
{
    VersionID v = readHeader(ds, r_velfromprop);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> velfromprop.vel_property
            >> static_cast<VelocityGenerator&>(velfromprop);
    }
    else
        throw version_error(v, "1", r_velfromprop, CODELOC);
        
    return ds;
}

/** Constructor */
VelocitiesFromProperty::VelocitiesFromProperty()
                       : ConcreteProperty<VelocitiesFromProperty,VelocityGenerator>()
{}

/** Construct to get the velocities from the property 'property' */
VelocitiesFromProperty::VelocitiesFromProperty(const PropertyName &property)
                       : ConcreteProperty<VelocitiesFromProperty,VelocityGenerator>(),
                         vel_property(property)
{}

/** Copy constructor */
VelocitiesFromProperty::VelocitiesFromProperty(const VelocitiesFromProperty &other)
                : ConcreteProperty<VelocitiesFromProperty,VelocityGenerator>(other),
                  vel_property(other.vel_property)
{}

/** Destructor */
VelocitiesFromProperty::~VelocitiesFromProperty()
{}

/** Copy assignment operator */
VelocitiesFromProperty& 
VelocitiesFromProperty::operator=(const VelocitiesFromProperty &other)
{
    VelocityGenerator::operator=(other);
    vel_property = other.vel_property;
    
    return *this;
}

/** Comparison operator */
bool VelocitiesFromProperty::operator==(const VelocitiesFromProperty &other) const
{
    return vel_property == other.vel_property and
           VelocityGenerator::operator==(other);
}

/** Comparison operator */
bool VelocitiesFromProperty::operator!=(const VelocitiesFromProperty &other) const
{
    return vel_property != other.vel_property or
           VelocityGenerator::operator!=(other);
}

/////////
///////// Implementation of RandomVelocities
/////////
