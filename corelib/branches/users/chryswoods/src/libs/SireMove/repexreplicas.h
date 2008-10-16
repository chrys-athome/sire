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
    RepExReplica& operator=(const Replica &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RepExReplica>() );
    }
    
    bool operator==(const RepExReplica &other) const;
    bool operator!=(const RepExReplica &other) const;
    
    SireUnits::Dimension::Energy energy();

    const Symbol& energyComponent() const;
    
    const Symbol& lambdaComponent() const;
    double lambdaValue() const;
    
    SireUnits::Dimension::Temperature temperature() const;
    SireUnits::Dimension::Pressure pressure() const;
    SireUnits::Dimension::Volume volume() const;

    bool isConstantTemperature() const;
    bool isConstantPressure() const;
    bool isConstantLambda(const Symbol &lam) const;

    static bool differentTemperatures(const RepExReplica &rep0,
                                      const RepExReplica &rep1);
                                      
    static bool differentPressures(const RepExReplica &rep0,
                                   const RepExReplica &rep1);
                                   
    static bool differentHamiltonians(const RepExReplica &rep0,
                                      const RepExReplica &rep1);

    static void swapSystems(RepExReplica &rep0, RepExReplica &rep1);

protected:
    virtual void setEnergyComponent(const Symbol &symbol);
    virtual void setLambdaComponent(const Symbol &symbol);
    virtual void setLambdaValue(double value);
    virtual void setEnergy(const SireUnits::Dimension::Energy &energy);
    virtual void setTemperature(const SireUnits::Dimension::Temperature &temperature);
    virtual void setVolume(const SireUnits::Dimension::Volume &volume);
    virtual void setPressure(const SireUnits::Dimension::Pressure &pressure);

private:
    void updateEnsembleParameters();

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
    
    /** Whether or not this is a constant potential energy replica */
    bool is_constant_energy;
    
    /** Whether or not this is a constant temperature replica */
    bool is_constant_temperature;
    
    /** Whether or not this is a constant volume replica */
    bool is_constant_volume;
    
    /** Whether or not this is a constant pressure replica */
    bool is_constant_pressure;
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
    
    RepExReplicas& operator=(const RepExReplicas &other);
    RepExReplicas& operator=(const Replicas &other);
    
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
