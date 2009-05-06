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

#ifndef SIREMOVE_INTEGRATOR_H
#define SIREMOVE_INTEGRATOR_H

#include "SireBase/property.h"
#include "SireBase/propertymap.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class Integrator;
class NullIntegrator;
}

QDataStream& operator<<(QDataStream&, const SireMove::Integrator&);
QDataStream& operator>>(QDataStream&, SireMove::Integrator&);

QDataStream& operator<<(QDataStream&, const SireMove::NullIntegrator&);
QDataStream& operator>>(QDataStream&, SireMove::NullIntegrator&);

namespace SireFF
{
class ForceTable;
}

namespace SireMol
{
class MoleculeGroup;
}

namespace SireMaths
{
class RanGenerator;
}

namespace SireMove
{

class Ensemble;

using SireMol::MoleculeGroup;

using SireFF::ForceTable;

using SireBase::PropertyMap;

using SireMaths::RanGenerator;

/** This is the virtual base class of all dynamics integrators. An integrator
    is the kernel used to advance the coordinates of the system from one
    timestep to the next
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT Integrator : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Integrator&);
friend QDataStream& ::operator>>(QDataStream&, Integrator&);

public:
    Integrator();
    
    Integrator(const Integrator &other);
    
    virtual ~Integrator();
    
    static const char* typeName()
    {
        return "SireMove::Integrator";
    }

    virtual Integrator* clone() const=0;
    
    static const NullIntegrator& null();
    
    virtual QString toString() const=0;
    
    virtual void integrate(MoleculeGroup &molgroup, const ForceTable &forcetable,
                           const PropertyMap &map = PropertyMap())=0;

    virtual void setTimeStep(const SireUnits::Dimension::Time &timestep)=0;

    virtual SireUnits::Dimension::Time timeStep() const=0;

    virtual SireUnits::Dimension::MolarEnergy kineticEnergy() const=0;
    
    virtual void clearStatistics()=0;
    
    virtual void setGenerator(const RanGenerator &generator)=0;
};

/** This class holds a null integrator, which doesn't advance anything anywhere

    @author Christopher Woods
*/
class SIREMOVE_EXPORT NullIntegrator 
            : public SireBase::ConcreteProperty<NullIntegrator,Integrator>
{

friend QDataStream& ::operator<<(QDataStream&, const NullIntegrator&);
friend QDataStream& ::operator>>(QDataStream&, NullIntegrator&);

public:
    NullIntegrator();
    
    NullIntegrator(const NullIntegrator &other);
    
    ~NullIntegrator();
    
    NullIntegrator& operator=(const NullIntegrator &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<NullIntegrator>() );
    }
    
    bool operator==(const NullIntegrator &other) const;
    bool operator!=(const NullIntegrator &other) const;
    
    QString toString() const;
    
    void integrate(MoleculeGroup &molgroup, const ForceTable &forcetable,
                   const PropertyMap &map = PropertyMap());

    void setTimeStep(const SireUnits::Dimension::Time &timestep);

    SireUnits::Dimension::Time timeStep() const;

    SireUnits::Dimension::MolarEnergy kineticEnergy() const;
    
    void clearStatistics();
    
    void setGenerator(const RanGenerator &generator);
};

typedef SireBase::PropPtr<Integrator> IntegratorPtr;

}

Q_DECLARE_METATYPE( SireMove::NullIntegrator )

SIRE_EXPOSE_CLASS( SireMove::NullIntegrator )

SIRE_EXPOSE_PROPERTY( SireMove::IntegratorPtr, SireMove::Integrator )

SIRE_END_HEADER

#endif
