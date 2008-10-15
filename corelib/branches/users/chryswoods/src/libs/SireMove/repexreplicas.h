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

#ifndef SIREMOVE_REPEXREPLICAS_H
#define SIREMOVE_REPEXREPLICAS_H

#include "replicas.h"

#include "SireCAS/symbol.h"

#include "SireBase/propertymap.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class RepExReplica;
class RepExReplicas;
}

QDataStream& operator<<(QDataStream&, const SireMove::RepExReplica&);
QDataStream& operator>>(QDataStream&, SireMove::RepExReplica&);

namespace SireMove
{

using SireCAS::Symbol;

using SireBase::PropertyMap;

/** This class represents a single replica in a replica exchange
    simulation
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT RepExReplica 
            : public SireBase::ConcreteProperty<RepExReplica,Replica>
{

friend class RepExReplicas; // so can call editor functions

friend QDataStream& ::operator<<(QDataStream&, const RepExReplica&);
friend QDataStream& ::operator>>(QDataStream&, RepExReplica&);

public:
    RepExReplica();
    RepExReplica(const Replica &replica);
    
    RepExReplica(const RepExReplica &other);
    
    ~RepExReplica();
    
    RepExReplica& operator=(const RepExReplica &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RepExReplica>() );
    }
    
    bool operator==(const RepExReplica &other) const;
    bool operator!=(const RepExReplica &other) const;
    
    double energy();

    const Symbol& energyComponent() const;
    
    const Symbol& lambdaComponent() const;
    double lambdaValue() const;
    
    const SireUnits::Dimension::Temperature& temperature() const;
    const SireUnits::Dimension::Pressure& pressure() const;

    const PropertyMap& propertyNames() const;

    static void swapSystems(RepExReplica &rep0, RepExReplica &rep1);

protected:
    virtual void setEnergyComponent(const Symbol &symbol);
    virtual void setLambdaComponent(const Symbol &symbol);
    virtual void setLambdaValue(double value);
    virtual void setTemperature(const SireUnits::Dimension::Temperature &temperature);
    virtual void setPressure(const SireUnits::Dimension::Pressure &pressure);
    virtual void setPropertyNames(const PropertyMap &map);

private:
    /** The symbol that represents the component of the energy to
        be evaluated for this replica */
    Symbol nrg_component;
    
    /** The symbol representing the lambda component for 
        lambda-based Hamiltonian replica exchange - this is null
        if this type of replica exchange is not being performed */
    Symbol lambda_component;
    
    /** The value of lambda for this replica. This is 0 if this is 
        not a lambda-based HRE replica */
    double lambda_value;
    
    /** The temperature of this replica */
    SireUnits::Dimension::Temperature replica_temperature;
    
    /** The pressure of this replica - this is zero if this is  
        a constant volume replica */
    SireUnits::Dimension::Pressure replica_pressure;
    
    /** The property map used to get the names of properties of this replica
        (e.g. the simulation space, from which the volume can be calculated,
         the molecular momenta property etc.) */
    PropertyMap replica_properties;
};

/** This class holds all of the replicas and associated data
    necessary for a replica exchange simulation 
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT RepExReplicas 
            : public SireBase::ConcreteProperty<RepExReplicas,Replicas>
{

friend QDataStream& operator<<(QDataStream&, const RepExReplicas&);
friend QDataStream& operator>>(QDataStream&, RepExReplicas&);

public:
    RepExReplicas();
    RepExReplicas(int n);
    RepExReplicas(const System &system, int n);
    RepExReplicas(const QVector<System> &systems);
    
    RepExReplicas(const Replicas &other);
    RepExReplicas(const RepExReplicas &other);
    
    ~RepExReplicas();
    
    RepExReplicas& operator=(const Replicas &other);
    RepExReplicas& operator=(const RepExReplicas &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RepExReplicas>() );
    }
    
    const char* what() const
    {
        return RepExReplicas::typeName();
    }
    
    RepExReplicas* clone() const
    {
        return new RepExReplicas(*this);
    }

    bool operator==(const RepExReplicas &other) const;
    bool operator!=(const RepExReplicas &other) const;
    
    const RepExReplica& operator[](int i) const;
    
    const RepExReplica& at(int i) const;
    
    void setReplicas(const Replicas &replicas);
    
    void setReplica(const Replica &replica);
    void setReplica(int i, const Replica &replica);
    
    void setEnergyComponent(const Symbol &symbol);
    void setEnergyComponent(int i, const Symbol &symbol);
    
    void setLambdaComponent(const Symbol &symbol);
    void setLambdaComponent(int i, const Symbol &symbol);
    
    void setLambdaValue(double value);
    void setLambdaValue(int i, double value);
    
    void setTemperature(const SireUnits::Dimension::Temperature &temperature);
    void setTemperature(int i, const SireUnits::Dimension::Temperature &temperature);
    
    void setPressure(const SireUnits::Dimension::Pressure &pressure);
    void setPressure(int i, const SireUnits::Dimension::Pressure &pressure);
    
    void setPropertyNames(const PropertyMap &map);
    void setPropertyNameS(int i, const PropertyMap &map);

protected:
    RepExReplica& _pvt_replica(int i);
};

}

Q_DECLARE_METATYPE( SireMove::RepExReplica )
Q_DECLARE_METATYPE( SireMove::RepExReplicas )

SIRE_EXPOSE_CLASS( SireMove::RepExReplica )
SIRE_EXPOSE_CLASS( SireMove::RepExReplicas )

SIRE_END_HEADER

#endif
