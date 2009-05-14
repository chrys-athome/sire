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

#include "repexmove.h"

#include "replica.h"
#include "replicas.h"

#include "suprasubsim.h"

#include "SireCluster/node.h"
#include "SireCluster/nodes.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireMove;
using namespace SireMaths;
using namespace SireCluster;
using namespace SireBase;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

////////////
//////////// Implementation of RepExSubMove
////////////

static const RegisterMetaType<RepExSubMove> r_repexsubmove;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const RepExSubMove &repexsubmove)
{
    writeHeader(ds, r_repexsubmove, 1);
    
    SharedDataStream sds(ds);
    
    sds << repexsubmove.partner_properties
        << repexsubmove.have_new_vals;
        
    if (repexsubmove.have_new_vals)
    {
        sds << repexsubmove.new_volume_i.to(angstrom3)
            << repexsubmove.new_energy_i.to(kcal_per_mol)
            << repexsubmove.new_volume_j.to(angstrom3) 
            << repexsubmove.new_energy_j.to(kcal_per_mol);
    }
    
    sds << static_cast<const SupraSubMove&>(repexsubmove);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, RepExSubMove &repexsubmove)
{
    VersionID v = readHeader(ds, r_repexsubmove);
    
    if (v == 1)
    {
        RepExSubMove new_submove;
    
        SharedDataStream sds(ds);
        
        sds >> new_submove.partner_properties
            >> new_submove.have_new_vals;
            
        if (new_submove.have_new_vals)
        {
            double new_volume_i, new_energy_i, new_volume_j, new_energy_j;
            
            sds >> new_volume_i >> new_energy_i
                >> new_volume_j >> new_energy_j;

            new_submove.new_volume_i = new_volume_i * angstrom3;
            new_submove.new_energy_i = new_energy_i * kcal_per_mol;
            
            new_submove.new_volume_j = new_volume_j * angstrom3;
            new_submove.new_energy_j = new_energy_j * kcal_per_mol;
        }
        
        sds >> static_cast<SupraSubMove&>(repexsubmove);
        
        //check that all of the partner properties are valid...
        for (QList< QPair<quint32,QVariant> >::const_iterator 
                                it = new_submove.partner_properties.constBegin();
             it != new_submove.partner_properties.constEnd();
             ++it)
        {
            switch (it->first)
            {
                case RepExSubMove::LAMBDA_VALUE:
                case RepExSubMove::NRG_COMPONENT:
                case RepExSubMove::SPACE_PROPERTY:
                    break;
                    
                default:
                    throw version_error( QObject::tr(
                        "Version 1 of SireMove::RepExSubMove does not support "
                        "the partner property with ID %2.")
                            .arg(it->first), CODELOC );
            }
        }
    }
    else
        throw version_error(v, "1", r_repexsubmove, CODELOC);
        
    return ds;
}

/** Constructor */
RepExSubMove::RepExSubMove()
             : ConcreteProperty<RepExSubMove,SupraSubMove>(),
               new_volume_i(0), new_energy_i(0),
               new_volume_j(0), new_energy_j(0),
               have_new_vals(false)
{}

/** Copy constructor */
RepExSubMove::RepExSubMove(const RepExSubMove &other)
             : ConcreteProperty<RepExSubMove,SupraSubMove>(other),
               new_volume_i(other.new_volume_i), new_energy_i(other.new_energy_i),
               new_volume_j(other.new_volume_j), new_energy_j(other.new_energy_j),
               partner_properties(other.partner_properties),
               have_new_vals(other.have_new_vals)
{}

/** Destructor */
RepExSubMove::~RepExSubMove()
{}

/** Copy assignment operator */
RepExSubMove& RepExSubMove::operator=(const RepExSubMove &other)
{
    if (this != &other)
    {
        partner_properties = other.partner_properties;

        new_volume_i = other.new_volume_i;
        new_energy_i = other.new_energy_i;
        
        new_volume_j = other.new_volume_j;
        new_energy_j = other.new_energy_j;
        
        have_new_vals = other.have_new_vals;
        
        SupraSubMove::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool RepExSubMove::operator==(const RepExSubMove &other) const
{
    return (this == &other) or
           ( have_new_vals == other.have_new_vals and
             new_volume_i == other.new_volume_i and
             new_energy_i == other.new_energy_i and
             new_volume_j == other.new_volume_j and
             new_energy_j == other.new_energy_j and
             SupraSubMove::operator==(other) );
}

/** Comparison operator */
bool RepExSubMove::operator!=(const RepExSubMove &other) const
{
    return not this->operator==(other);
}

/** Return a string representation of this move */
QString RepExSubMove::toString() const
{
    if (have_new_vals)
        return QObject::tr( "RepExSubMove( E_i = %1 kcal mol-1, V_i = %2 A^3 : "
                            "E_j = %3 kcal mol-1, V_j = %4 A^3 )")
                 .arg( new_energy_i.to(kcal_per_mol) )
                 .arg( new_volume_i.to(angstrom3) )
                 .arg( new_energy_j.to(kcal_per_mol) )
                 .arg( new_volume_j.to(angstrom3) );
                 
    else
        return QObject::tr( "RepExSubMove()" );
}

static void throwNoValues(const char *value, const QString &codeloc)
{
    throw SireError::invalid_state( QObject::tr(
        "Cannot get the value of %1 as new values have not been evaluated!")
            .arg(value), codeloc );
}

/** Return the energy of the replica in its normal state at the 
    end of the block of moves */
MolarEnergy RepExSubMove::energy_i() const
{
    if (not have_new_vals)
        ::throwNoValues( "E_i", CODELOC );
        
    return new_energy_i;
}

/** Return the volume of the replica in its normal state at
    the end of the block of moves */
Volume RepExSubMove::volume_i() const
{
    if (not have_new_vals)
        ::throwNoValues( "V_i", CODELOC );
        
    return new_volume_i;
}

/** Return the energy of the replica in its partner state 
    at the end of the block of moves */
MolarEnergy RepExSubMove::energy_j() const
{
    if (not have_new_vals)
        ::throwNoValues( "E_j", CODELOC );
        
    return new_energy_j;
}

/** Return the volume of the replica in its partner state
    at the end of the block of moves */
Volume RepExSubMove::volume_j() const
{
    if (not have_new_vals)
        ::throwNoValues( "V_j", CODELOC );
        
    return new_volume_j;
}

/** Internal function used to extract a value of type 'T' from
    the passed QVariant, throwing an exception if this is not possible
    
    \throw SireError::invalid_cast
*/
template<class T>
static T convert(const QVariant &value)
{
    if (not value.canConvert<T>())
        throw SireError::invalid_cast( QObject::tr(
            "Cannot apply a deferred command as the argument of type %1 "
            "cannot be cast to a value of type %2.")
                .arg( typeid(T).name() )
                .arg( QVariant::typeToName(value.type()) ), CODELOC );
                
    return value.value<T>();
}

/** Evaluate the energy and volume of this replica after
    it has been swapped into its partner state */
void RepExSubMove::evaluateSwappedState(const Replica &replica)
{
    if (have_new_vals)
        return;

    Replica state_i = replica;

    if (state_i.isPacked())
        state_i.unpack();

    //get the energy and volume at this state
    new_volume_i = state_i.volume();
    new_energy_i = state_i.energy();

    if (partner_properties.isEmpty())
    {
        new_volume_j = new_volume_i;
        new_energy_j = new_energy_j;
    }
    else
    {
        Replica state_j = state_i;
        
        for (QList< QPair<quint32,QVariant> >::const_iterator 
                                                it = partner_properties.constBegin();
             it != partner_properties.constEnd();
             ++it)
        {
            switch (it->first)
            {
                case LAMBDA_VALUE:
                    //set a new lambda value
                    state_j.setLambdaValue( ::convert<double>(it->second) );
                    break;
                    
                case NRG_COMPONENT:
                    //set a new Hamiltonian (represented by the component)
                    state_j.setEnergyComponent( ::convert<Symbol>(it->second) );
                    break;
                    
                case SPACE_PROPERTY:
                    //set a new space property
                    state_j.setSpaceProperty( ::convert<PropertyName>(it->second) );
                    break;
                    
                default:
                    throw SireError::unsupported( QObject::tr(
                        "A request was made of an unsuppoted action in RepExSubMove. "
                        "The action with ID %1 was requested, but this is not "
                        "supported with this version of RepExSubMove.")
                            .arg(it->first), CODELOC );
            }
        }
        
        new_volume_j = state_j.volume();
        new_energy_j = state_j.energy();
    }
}

/** Perform the sub-moves on the passed sub-system */
void RepExSubMove::move(SupraSubSystem &system, int n_supra_moves,
                        int n_supra_moves_per_block, bool record_stats)
{
    //replica exchange moves work only with Replica objects
    Replica &replica = system.asA<Replica>();

    SupraSubSystemPtr old_replica = replica.clone();
    SupraSubMovePtr old_state = this->clone();

    try
    {
        have_new_vals = false;

        if (n_supra_moves <= 0)
        {
            if (n_supra_moves_per_block <= 0)
                this->evaluateSwappedState(replica);
        
            return;
        }

        //unpack the system, if necessary
        bool replica_was_packed = replica.isPacked();
        replica.unpack();
    
        //perform the moves
        for (int i=0; i<n_supra_moves; ++i)
        {
            replica.subMove(record_stats);
        }

        //if we have finished a block of sub-moves, then collect
        //the information necessary to perform the replica exchange test
        if (n_supra_moves >= n_supra_moves_per_block)
            this->evaluateSwappedState(replica);
        
        //repack the system, if necessary
        if (replica_was_packed)
            replica.pack();
    }
    catch(...)
    {
        replica.copy(*old_replica);
        this->copy(*old_state);
        
        throw;
    }
}

////////////
//////////// Implementation of RepExMove
////////////

static const RegisterMetaType<RepExMove> r_repexmove;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const RepExMove &repexmove)
{
    writeHeader(ds, r_repexmove, 2);

    SharedDataStream sds(ds);
    
    sds << repexmove.rangenerator
        << repexmove.nmoves
        << repexmove.naccept
        << repexmove.nreject
        << static_cast<const SupraMove&>(repexmove);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, RepExMove &repexmove)
{
    VersionID v = readHeader(ds, r_repexmove);

    if (v == 2)
    {
        SharedDataStream sds(ds);
        
        sds >> repexmove.rangenerator
            >> repexmove.nmoves
            >> repexmove.naccept
            >> repexmove.nreject
            >> static_cast<SupraMove&>(repexmove);
    }
    else if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> repexmove.rangenerator
            >> repexmove.nmoves
            >> repexmove.naccept
            >> repexmove.nreject;
    }
    else
        throw version_error(v, "1-2", r_repexmove, CODELOC);
        
    return ds;
}

RepExMove::RepExMove();

RepExMove::RepExMove(const RepExMove &other);

RepExMove::~RepExMove();

RepExMove& RepExMove::operator=(const RepExMove &other);

bool RepExMove::operator==(const RepExMove &other) const;
bool RepExMove::operator!=(const RepExMove &other) const;

int RepExMove::nAttempted() const;
int RepExMove::nAccepted() const;
int RepExMove::nRejected() const;

int RepExMove::nMoves() const;

double RepExMove::acceptanceRatio() const;

void RepExMove::clearStatistics();

void RepExMove::setGenerator(const RanGenerator &generator);
const RanGenerator& RepExMove::generator() const;

QVector<SupraSubSim> RepExMove::submitSimulations(Nodes &nodes, Replicas &replicas,
                                                  bool even_pairs, bool record_stats)
{
    int nreplicas = replicas.nReplicas();

    QVector<SupraSubSim> subsims(nreplicas);
    
    //pair the replicas up...
    //do we swap the even pairs or the odd pairs?
    int start = 1;

    if (even_pairs)
        start = 0;

    if (start == 1)
        //the first replica hasn't got a partner - start it on its own
        subsims[0] = this->submitSimulation(nodes, replicas[0], record_stats);

    if ( (nreplicas-start) % 2 == 1 )
        //the last replica hasn't got a partner - start it on its own too
        subsims[nreplicas-1] = this->submitSimulation(nodes, replicas[nreplicas-1]),
                                                      record_stats );

    for (int i=start; i<nreplicas-1; i+=2)
    {
        //submit the simulations for replica i and replica i+1
        QPair<SupraSubSim,SupraSubSim> sims = this->submitSimulation(
                                                        replicas[i], replicas[i+1] );
        
        subsims[i] = sims.first;
        subsims[i+1] = sims.second;
    }

    return subsims;
}

void RepExMove::waitUntilFinished(Nodes &nodes, QVector<SupraSubSim> &subsims, 
                                  int max_tries)
{
    bool all_finished = false;
    int ntries = 0;
    
    int nreplicas = subsims.count();
    
    while (not all_finished)
    {
        ++ntries;
        
        if (ntries > max_tries)
            return;
            
        all_finished = true;
        
        for (int i=0; i<nreplicas; ++i)
        {
            subsims[i].wait();
            
            if (subsims.at(i).isError() or subsims.at(i).wasAborted())
            {
                qDebug() << "Replica" << i << "had an error or was aborted."
                         << "Resubmitting - attempt number" << ntries;
            
                //resubmit this calculation
                Node node = nodes.getNode();
                subsims[i] = SupraSubSim::run(node, subsims[i].input());
            
                all_finished = false;
            }
            else if (not subsims.at(i).hasFinished())
            {
                //continue the calculation from where it finished
                SupraSubSimPacket subsim = subsims[i].result();
                
                Node node = nodes.getNode();
                subsims[i] = SupraSubSim::run( node, subsim );
                
                all_finished = false;
            }
        }
        
        if (not all_finished)
        {
            //wait for the resubmitted calculations to finish
            for (int i=0; i<nreplicas; ++i)
            {
                subsims[i].wait();
            }
        }
    }
}

void RepExMove::performMove(Nodes &nodes, Replicas &replicas, bool record_stats)
{
    //will we swap even pairs or odd pairs?
    bool even_pairs = rangenerator.randBool();

    //submit all of the simulations
    QVector<SupraSubSim> subsims = this->submitSimulations(nodes, replicas,
                                                           even_pairs, record_stats);
        
    //wait for all of the simulations to finish (retrying broken simulations
    //just five times)
    this->waitUntilFinished(nodes, subsims, 5);
    
    //copy the results back into the replicas
    int nreplicas = replicas.count();
    
    QVector<RepExSubMove> submoves(nreplicas);
    
    for (int i=0; i<nreplicas; ++i)
    {
        submoves[i] = subsims[i].result().subMoves().asA<RepExSubMove>();
        replicas.setReplica( i, subsims[i].result().subSystem() );
    }
    
    //get rid of the simulation handles, as they are no longer needed
    //(and we need to save memory if we can)
    subsims = QVector<SupraSubSim>();
    
    //now perform all of the replica exchange tests
    this->testAndSwap(replicas, submoves, even_pairs, record_stats);
}

/** Perform 'nmoves' replica exchange moves (block of sampling for all
    replicas, then replica exchange test between all pairs),
    of the system 'system' (which must be a Replicas object), optionally
    recording statistics if 'record_stats' is true 
    
    \throw SireError::invalid_cast
*/
void RepExMove::move(SupraSystem &system, int nmoves, bool record_stats)
{
    Replicas &replicas = system.asA<Replicas>();
    
    if (replicas.nReplicas() == 0 or nmoves <= 0)
        return;
    
    SupraSystemPtr old_replicas = replicas.clone();
    SupraMovePtr old_state = this->clone();
    
    try
    {
        //try to get as many nodes as possible to run the moves
        Nodes nodes = Cluster::getNodes( replicas.nReplicas() - 1, 500 );
        
        ///hold this_thread in a local scope to ensure it is deleted before 'nodes'
        {
            ThisThread this_thread = nodes.borrowThisThread();
        
            for (int i=0; i<nmoves; ++i)
            {
                this->performMove(nodes, replicas, record_stats);
            }
        }
    }
    catch(...)
    {
        replicas.copy(*old_replicas);
        this->copy(*old_state);
        throw;
    }
}
