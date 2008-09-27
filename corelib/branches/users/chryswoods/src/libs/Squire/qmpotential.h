/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SQUIRE_QMPOTENTIAL_H
#define SQUIRE_QMPOTENTIAL_H

#include "SireMol/element.h"

#include "SireBase/property.h"
#include "SireBase/properties.h"

#include "SireVol/space.h"

#include "SireFF/ffcomponent.h"

#include "SireFF/detail/ffmolecules3d.h"
#include "SireFF/detail/atomicparameters3d.hpp"

SIRE_BEGIN_HEADER

namespace Squire
{
class QMPotential;
class QMProgram;
}

QDataStream& operator<<(QDataStream&, const Squire::QMPotential&);
QDataStream& operator>>(QDataStream&, Squire::QMPotential&);

QDataStream& operator<<(QDataStream&, const Squire::QMProgram&);
QDataStream& operator>>(QDataStream&, Squire::QMProgram&);

namespace SireFF
{
class ForceTable;
}

namespace SireCAS
{
class Symbol;
}

namespace Squire
{

using SireBase::PropertyMap;
using SireBase::Property;
using SireBase::PropertyBase;
using SireBase::Properties;

using SireMol::PartialMolecule;
using SireMol::MolGroup;

using SireCAS::Symbol;

using SireVol::SpaceBase;
using SireVol::Space;

using SireFF::ForceTable;

class QMComponent;
class QMProg;

typedef SireFF::ComponentEnergy<QMComponent> QMEnergy;

/** This class represents a QM energy */
class SQUIRE_EXPORT QMComponent : public SireFF::FFComponent
{
public:
    QMComponent(const SireFF::FFName &ffname = SireFF::FFName());
    QMComponent(const SireCAS::Symbol &symbol);
    
    QMComponent(const QMComponent &other);
    
    ~QMComponent();
    
    static const char* typeName()
    {
        return "Squire::QMComponent";
    }
    
    const char* what() const
    {
        return QMComponent::typeName();
    }
    
    QMComponent* clone() const
    {
        return new QMComponent(*this);
    }
    
    const QMComponent& total() const
    {
        return *this;
    }

    void setEnergy(SireFF::FF &ff, const QMEnergy &qmnrg) const;
    void changeEnergy(SireFF::FF &ff, const QMEnergy &qmnrg) const;
    
    SireCAS::Symbols symbols() const
    {
        return *this;
    }
};

/** This class provides the default name of the 
    property that contains the element parameters */
class SQUIRE_EXPORT ElementParameterName
{
public:
    ElementParameterName()
    {}
    
    ~ElementParameterName()
    {}
    
    const QString& element() const
    {
        return element_param;
    }
    
private:
    static QString element_param;
};

/** This class provides the default names of the element
    and coordinates properties that contain these parameters */
class SQUIRE_EXPORT ElementParameterName3D
          : public ElementParameterName, public SireFF::detail::Coords3DParameterName
{
public:
    ElementParameterName3D() : ElementParameterName(),
                               SireFF::detail::Coords3DParameterName()
    {}
    
    ~ElementParameterName3D()
    {}
};

/** Property holder for the QMProgram hierarchy of classes

    @author Christopher Woods
*/
class SQUIRE_EXPORT QMProgram : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const QMProgram&);
friend QDataStream& ::operator>>(QDataStream&, QMProgram&);

public:
    QMProgram();
    QMProgram(const SireBase::PropertyBase &property);
    QMProgram(const QMProg &qmprog);
    
    QMProgram(const QMProgram &other);
    
    ~QMProgram();
    
    virtual QMProgram& operator=(const SireBase::PropertyBase &property);
    virtual QMProgram& operator=(const QMProg &qmprog);

    const QMProg* operator->() const;
    const QMProg& operator*() const;
    
    const QMProg& read() const;
    QMProg& edit();
    
    const QMProg* data() const;
    const QMProg* constData() const;
    
    QMProg* data();
    
    operator const QMProg&() const;

    static const QMProgram& shared_null();
};

/** This is a QM potential. This provides the classes and code necessary
    to calculate QM energies and forces
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT QMPotential
{

friend QDataStream& ::operator<<(QDataStream&, const QMPotential&);
friend QDataStream& ::operator>>(QDataStream&, QMPotential&);

public:
    
    typedef QMEnergy Energy;
    typedef Energy::Components Components;
    typedef ElementParameterName3D ParameterNames;
    
    typedef SireMol::Element Parameter;
    typedef SireFF::detail::AtomicParameters3D<Parameter> Parameters;
    
    typedef SireFF::detail::FFMolecule3D<QMPotential> Molecule;
    typedef SireFF::detail::FFMolecules3D<QMPotential> Molecules;
    typedef SireFF::detail::ChangedMolecule<Molecule> ChangedMolecule;

    QMPotential();
    
    QMPotential(const QMPotential &other);
    
    virtual ~QMPotential();

    static ParameterNames parameters()
    {
        return ParameterNames();
    }

    static const char* typeName()
    {
        return "Squire::QMPotential";
    }
    
    virtual const char* what() const
    {
        return QMPotential::typeName();
    }

    QMPotential& operator=(const QMPotential &other);

    bool setProperty(const QString &name, const PropertyBase &value);
    const Property& property(const QString &name) const;
    bool containsProperty(const QString &name) const;
    const Properties& properties() const;

    bool setSpace(const SpaceBase &space);
    bool setQuantumProgram(const QMProg &program);
    
    const SpaceBase& space() const;
    const QMProg& quantumProgram() const;

    QMPotential::Parameters 
    getParameters(const PartialMolecule &mol,
                  const PropertyMap &map = PropertyMap());
                  
    QMPotential::Parameters
    updateParameters(const QMPotential::Parameters &old_params,
                     const PartialMolecule &old_mol,
                     const PartialMolecule &new_mol,
                     const PropertyMap &map = PropertyMap());
                     
    QMPotential::Parameters
    updateParameters(const QMPotential::Parameters &old_params,
                     const PartialMolecule &old_molecule,
                     const PartialMolecule &new_molecule,
                     const PropertyMap &old_map, const PropertyMap &new_map);
                     
    QMPotential::Molecule
    parameterise(const PartialMolecule &molecule,
                 const PropertyMap &map = PropertyMap());
    
    QMPotential::Molecules 
    parameterise(const MolGroup &molecules,
                 const PropertyMap &map = PropertyMap());

    void calculateForce(const Molecules &molecules, 
                        ForceTable &forcetable, 
                        double scale_force=1) const;
                        
    void calculateForce(const Molecules &molecules, 
                        ForceTable &forcetable,
                        const Symbol &symbol, 
                        const Components &components,
                        double scale_force=1);
    
    void calculateEnergy(const Molecules &molecules, Energy &nrg,
                         double scale_energy=1) const;

    QString forceCommandFile(const Molecules &molecules) const;
    QString energyCommandFile(const Molecules &molecules) const;

protected:
    virtual void changedPotential()=0;
    
private:
    Molecules mapIntoSpace(const Molecules &molecules) const;

    /** The properties that define this potential */
    Properties props;
    
    /** The space in which the molecules exist */
    Space spce;
    
    /** The QM program that is used to calculate the energy */
    QMProgram qmprog;
};

} // end of namespace Squire

Q_DECLARE_METATYPE( Squire::QMComponent )

SIRE_EXPOSE_CLASS( Squire::QMComponent )

SIRE_END_HEADER

#endif
