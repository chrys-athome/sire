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

#ifndef SIREMOVE_VELOCITYGENERATOR_H
#define SIREMOVE_VELOCITYGENERATOR_H

#include "SireBase/property.h"
#include "SireBase/propertymap.h"

#include "SireCAS/expression.h"
#include "SireCAS/symbol.h"

#include "SireMaths/rangenerator.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class VelocityGenerator;
class VelocitiesFromProperty;
class RandomVelocities;

class NullVelocityGenerator;
}

QDataStream& operator<<(QDataStream&, const SireMove::VelocityGenerator&);
QDataStream& operator>>(QDataStream&, SireMove::VelocityGenerator&);

QDataStream& operator<<(QDataStream&, const SireMove::NullVelocityGenerator&);
QDataStream& operator>>(QDataStream&, SireMove::NullVelocityGenerator&);

QDataStream& operator<<(QDataStream&, const SireMove::VelocitiesFromProperty&);
QDataStream& operator>>(QDataStream&, SireMove::VelocitiesFromProperty&);

QDataStream& operator<<(QDataStream&, const SireMove::RandomVelocities&);
QDataStream& operator>>(QDataStream&, SireMove::RandomVelocities&);

namespace SireCAS
{
class Symbol;
}

namespace SireMove
{

using SireMaths::RanGenerator;

using SireCAS::Symbol;

/** This is the base class of generators that are used
    to get velocities for molecules
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT VelocityGenerator : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const VelocityGenerator&);
friend QDataStream& ::operator>>(QDataStream&, VelocityGenerator&);

public:
    VelocityGenerator();
    
    VelocityGenerator(const VelocityGenerator &other);
    
    virtual ~VelocityGenerator();
    
    static const char* typeName()
    {
        return "SireMove::VelocityGenerator";
    }
    
    virtual VelocityGenerator* clone() const=0;
    
    virtual void setGenerator(const RanGenerator &generator);
    
    static const NullVelocityGenerator& null();
    
protected:
    VelocityGenerator& operator=(const VelocityGenerator &other);
    
    bool operator==(const VelocityGenerator &other) const;
    bool operator!=(const VelocityGenerator &other) const;

};

/** This is the null velocity generator

    @author Christopher Woods
*/
class SIREMOVE_EXPORT NullVelocityGenerator
           : public SireBase::ConcreteProperty<NullVelocityGenerator,VelocityGenerator>
{

friend QDataStream& ::operator<<(QDataStream&, const NullVelocityGenerator&);
friend QDataStream& ::operator>>(QDataStream&, NullVelocityGenerator&);

public:
    NullVelocityGenerator();
    
    NullVelocityGenerator(const NullVelocityGenerator &other);
    
    ~NullVelocityGenerator();
    
    NullVelocityGenerator& operator=(const NullVelocityGenerator &other);
    
    bool operator==(const NullVelocityGenerator &other) const;
    bool operator!=(const NullVelocityGenerator &other) const;
    
    static const char* typeName();
};

/** This is a velocity generator that extracts velocities from a 
    specified molecular property
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT VelocitiesFromProperty
        : public SireBase::ConcreteProperty<VelocitiesFromProperty,VelocityGenerator>
{

friend QDataStream& ::operator<<(QDataStream&, const VelocitiesFromProperty&);
friend QDataStream& ::operator>>(QDataStream&, VelocitiesFromProperty&);

public:
    VelocitiesFromProperty();
    
    VelocitiesFromProperty(const SireBase::PropertyName &property);
    
    VelocitiesFromProperty(const VelocitiesFromProperty &other);
    
    ~VelocitiesFromProperty();

    VelocitiesFromProperty& operator=(const VelocitiesFromProperty &other);
    
    bool operator==(const VelocitiesFromProperty &other) const;
    bool operator!=(const VelocitiesFromProperty &other) const;
    
    static const char* typeName();

private:
    /** The name of the property from which the velocities will be obtained */
    SireBase::PropertyName vel_property;
};

/** This is a velocity generator that generates random velocities 
    according to a specified equation (where the symbol 'x' represents
    a random number on [0,1], 
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT RandomVelocities
          : public SireBase::ConcreteProperty<RandomVelocities,VelocityGenerator>
{

friend QDataStream& ::operator<<(QDataStream&, const RandomVelocities&);
friend QDataStream& ::operator>>(QDataStream&, RandomVelocities&);

public:
    RandomVelocities();
    
    RandomVelocities(const SireCAS::Expression &ranfunction);
    
    RandomVelocities(const RandomVelocities &other);
    
    ~RandomVelocities();
    
    RandomVelocities& operator=(const RandomVelocities &other);
    
    bool operator==(const RandomVelocities &other) const;
    bool operator!=(const RandomVelocities &other) const;
    
    static const char* typeName();

    static Symbol x();

    void setGenerator(const RanGenerator &rangenerator);
    
    const RanGenerator& generator() const;

private:
    /** The expression used to generate the random velocity */
    SireCAS::Expression rand_func;
    
    /** The random number generator */
    RanGenerator ran_generator;
};


typedef SireBase::PropPtr<VelocityGenerator> VelGenPtr;

}

Q_DECLARE_METATYPE( SireMove::NullVelocityGenerator )
Q_DECLARE_METATYPE( SireMove::VelocitiesFromProperty )
Q_DECLARE_METATYPE( SireMove::RandomVelocities )

SIRE_EXPOSE_CLASS( SireMove::VelocityGenerator )
SIRE_EXPOSE_CLASS( SireMove::NullVelocityGenerator )
SIRE_EXPOSE_CLASS( SireMove::VelocitiesFromProperty )
SIRE_EXPOSE_CLASS( SireMove::RandomVelocities )

SIRE_EXPOSE_PROPERTY( SireMove::VelGenPtr, SireMove::VelocityGenerator )

SIRE_END_HEADER

#endif
