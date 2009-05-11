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
#include "repexreplicas.h"

#include "simulation.h"

#include "SireCluster/node.h"
#include "SireCluster/nodes.h"

#include "SireUnits/units.h"
#include "SireUnits/dimensions.h"

#include "SireError/errors.h"
#include "SireError/printerror.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMove;
using namespace SireMaths;
using namespace SireCluster;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

////////////
//////////// Implementation of RepExSubMove
////////////

static const RegisterMetaType<RepExSubMove> r_repexsubmove;

/** Perform the sub-moves on the passed sub-system */
void RepExSubMove::move(SupraSubSystem &system, int n_supra_moves,
                        bool record_supra_stats)
{
    if (n_supra_moves <= 0)
        return;

    //replica exchange moves work only with Replica objects
    Replica &replica = system.asA<Replica>();
    
    boost::shared_ptr<RepExReplica> old_state( replica.clone() );
    
    try
    {
        //unpack the system, if necessary
        bool system_is_packed = system.isPacked();
        system.unpack();

        //do we need to swap the system (because of a previously passed
        //replica exchange move)?
        if (this->lastMovePassed())
        {
            system.setSubSystem(new_system);
        }
    
        //do we need to clear statistics?
        if (must_clear_substats)
            system.clearSubStats();
    
        //perform the moves
        for (int i=0; i<n_supra_moves; ++i)
        {
            system.subMove();
            
            if (record_supra_stats)
                system.collectStats();
        }
        
        //now collect the information necessary for the next
        //replica exchange move
        this->collectMoveData(system);
        
        //repack the system, if necessary
        if (system_is_packed)
            system.pack();
    }
    catch(...)
    {
        replica.assign(*old_state);
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
    writeHeader(ds, r_repexmove, 1);
    
    SharedDataStream sds(ds);
    
    sds << repexmove.rangenerator 
        << repexmove.nmoves
        << repexmove.naccept
        << repexmove.nreject;
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, RepExMove &repexmove)
{
    VersionID v = readHeader(ds, r_repexmove);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> repexmove.rangenerator 
            >> repexmove.nmoves
            >> repexmove.naccept
            >> repexmove.nreject;
    }
    else
        throw version_error(v, "1", r_repexmove, CODELOC);
        
    return ds;
}

/** Constructor */
RepExMove::RepExMove() : nmoves(0), naccept(0), nreject(0)
{}

/** Copy constructor */
RepExMove::RepExMove(const RepExMove &other)
          : rangenerator(other.rangenerator),
            nmoves(other.nmoves),
            naccept(other.naccept), nreject(other.nreject)
{}

/** Destructor */
RepExMove::~RepExMove()
{}

/** Copy assignment operator */
RepExMove& RepExMove::operator=(const RepExMove &other)
{
    if (this != &other)
    {
        rangenerator = other.rangenerator;
        nmoves = other.nmoves;
        naccept = other.naccept;
        nreject = other.nreject;
    }
    
    return *this;
}

/** Comparison operator */
bool RepExMove::operator==(const RepExMove &other) const
{
    return nmoves == other.nmoves and
           naccept == other.naccept and
           nreject == other.nreject and
           rangenerator == other.rangenerator;
}

/** Comparison operator */
bool RepExMove::operator!=(const RepExMove &other) const
{
    return nmoves != other.nmoves or
           naccept != other.naccept or
           nreject != other.nreject or
           rangenerator != other.rangenerator;
}

/** Return the total number of attempted surpa- replica exchange
    moves (each supra-move consists of perhaps many individual
    pair swap and test moves) */
quint32 RepExMove::nMoves() const
{
    return nmoves;
}

/** Return the number of attempted replica exchange moves */
quint32 RepExMove::nAttempted() const
{
    return naccept + nreject;
}

/** Return the number of accepted replica exchange moves */
quint32 RepExMove::nAccepted() const
{
    return naccept;
}

/** Return the number of rejected replica exchange moves */
quint32 RepExMove::nRejected() const
{
    return nreject;
}

/** Return the ratio of the number of accepted moves to the 
    total number of attempted moves */
double RepExMove::acceptanceRatio() const
{
    int ntotal = this->nAttempted();
    
    if (ntotal == 0)
        return 0;
    else
        return double(this->nAccepted()) / double(ntotal);
}

/** Completely clear the move statistics */
void RepExMove::clearMoveStatistics()
{
    nmoves = 0;
    naccept = 0;
    nreject = 0;
}

/** Set the random number generator used when performing the 
    replica exchange test */
void RepExMove::setGenerator(const RanGenerator &generator)
{
    rangenerator = generator;
}

/** Return the random number generator used when performing the
    replica exchange test */
const RanGenerator& RepExMove::generator() const
{
    return rangenerator;
}

/** Internal function called to record that a move has been accepted */
void RepExMove::acceptedMove()
{
    ++naccept;
}

/** Internal function called to record that a move has been rejected */
void RepExMove::rejectedMove()
{
    ++nreject;
}
          
/** Perform a replica exchange swap and test between the passed
    two replicas */
bool RepExMove::testAndSwap(RepExReplica &replica_a, RepExReplica &replica_b)
{
    //get the ensembles of the two replicas
    const Ensemble &ensemble_a = replica_a.ensemble();
    const Ensemble &ensemble_b = replica_b.ensemble();
    
    if ( (ensemble_a.isNVT() and ensemble_a.isNVT()) or 
         (ensemble_b.isNPT() and ensemble_b.isNPT()) )
    {
        bool need_pv = (ensemble_a.isNPT() and ensemble_b.isNPT());
    
        //get the values of the thermodynamic parameters
        double beta_a = 1.0 / (k_boltz * ensemble_a.temperature()).value();
        double beta_b = 1.0 / (k_boltz * ensemble_b.temperature()).value();
        
        Pressure p_a(0);
        Pressure p_b(0);
        
        if (need_pv)
        {
            p_a = ensemble_a.pressure();
            p_b = ensemble_b.pressure();
        }
        
        bool rep_a_is_packed = replica_a.isPacked();
        bool rep_b_is_packed = replica_b.isPacked();
        
        try
        {
            if (rep_a_is_packed)
                replica_a.unpack();
                
            if (rep_b_is_packed)
                replica_b.unpack();
        
            //now get the values of the system properties at their current replica level
            MolarEnergy H_a_i = replica_a.energy();
            MolarEnergy H_b_j = replica_b.energy();
        
            Volume V_a_i(0);
            Volume V_b_j(0);
        
            if (need_pv)
            {
                V_a_i = replica_a.volume();
                V_b_j = replica_b.volume();
            }
        
            //now swap the replicas
            RepExReplica::swapSystems(replica_a, replica_b);
        
            //get the values of the system properties at the new replica level
            MolarEnergy H_a_j = replica_a.energy();
            MolarEnergy H_b_i = replica_b.energy();
        
            Volume V_a_j(0);
            Volume V_b_i(0);
        
            if (need_pv)
            {
                V_a_j = replica_a.volume();
                V_b_i = replica_b.volume();
            }

            if (rep_a_is_packed)
                replica_a.pack();
                
            if (rep_b_is_packed)
                replica_b.pack();
        
            //now calculate delta needed for the Monte Carlo test
            //
            //  For derivation see Appendix C of Christopher Woods' thesis
            //
            //  delta = beta_b * [ H_b_j - H_b_i + P_b (V_b_j - V_b_i) ] + 
            //          beta_a * [ H_a_i - H_a_j + P_a (V_a_i - V_a_j) ]
        
            double delta = beta_b * ( H_b_j - H_b_i + p_b*(V_b_j - V_b_i) ) +
                           beta_a * ( H_a_i - H_a_j + p_a*(V_a_i - V_a_j) );
                       
            return ( delta > 0 or (std::exp(delta) >= rangenerator.rand()) );
        }
        catch(...)
        {
            if (rep_a_is_packed)
                replica_a.pack();
                
            if (rep_b_is_packed)
                replica_b.pack();
                
            throw;
        }
    }
    else
    {
        throw SireError::incompatible_error( QObject::tr(
            "There is no available replica exchange test that allows tests between "
            "replicas with ensembles %1 and %2.")
                .arg(ensemble_a.toString(), ensemble_b.toString()), CODELOC );
                
    }

    return false;
}
          
/** Perform the replica exchange swaps and accept or reject them
    according to replica exchange tests */
void RepExMove::testAndSwap(RepExReplicas &replicas)
{
    if (replicas.count() <= 1)  
        //no pairs to swap
        return;
        
    //do we swap the even pairs or the odd pairs?
    int start = 0;
    int nreplicas = replicas.count();
    
    if (nreplicas > 2 and rangenerator.randBool())
    {
        start = 1;
    }
    
    //loop over all pairs
    for (int i=start; i<nreplicas-1; i+=2)
    {
        RepExReplica replica0 = replicas[i];
        RepExReplica replica1 = replicas[i+1];

        if (this->testAndSwap(replica0, replica1))
        {
            //save the new replicas
            replicas.setReplica(i, replica0);
            replicas.setReplica(i+1, replica1);
            
            this->acceptedMove();
        }
        else
            this->rejectedMove();
    }
}
          
/** Perform 'nmoves' replica exchange moves on the replicas in 'replicas',
    running all jobs using the nodes 'nodes', and recording statistics if
    'record_stats' is true. */
void RepExMove::move(Nodes &nodes, RepExReplicas &replicas, int nmoves_to_run,
                     int nmoves_per_chunk, bool record_stats)
{
    if (nmoves_to_run <= 0 or replicas.isEmpty())
        return;

    //always try and add this thread to the cluster
    //(as we are sitting around waiting while the job is running)
    ThisThread this_thread = nodes.borrowThisThread();
    
    if (nodes.isEmpty())
        throw SireError::unavailable_resource( QObject::tr(
                "There are no processor resources available to run a replica "
                "exchange simulation!"), CODELOC );

    RepExMove old_state(*this);
    boost::shared_ptr<RepExReplicas> old_replicas( replicas.clone() );
    
    try
    {
        for (int i=0; i<nmoves_to_run; ++i)
        {
            QList<Simulation> running_sims;
            
            for (int j=0; j<replicas.nReplicas(); ++j)
            {
                RepExReplica replica = replicas[j];
            
                Node node = nodes.getNode();
                
                running_sims.append( Simulation::run(node, replica.systemAndMoves(),
                                                     replica.nMoves(), 
                                                     nmoves_per_chunk,
                                       (replica.recordStatistics() and record_stats) ) );
            }

            //wait for all of the simulations to finish
            bool all_finished = false;
            int ntries = 0;
            
            while (not all_finished)
            {
                ++ntries;

                if (ntries > 4)
                    break;

                all_finished = true;
            
                for (int j=0; j<replicas.nReplicas(); ++j)
                {
                    running_sims[j].wait();
                    
                    if (running_sims[j].isError() or 
                        running_sims[j].wasAborted())
                    {
                        qDebug() << "Replica" << j << "had an error or was aborted."
                                 << "Resubmitting - attempt number" << ntries;
                    
                        if (running_sims[j].isError())
                            running_sims[j].throwError();
                    
                        //resubmit this calculation
                        RepExReplica replica = replicas[j];
                        
                        Node node = nodes.getNode();
                        
                        running_sims[j] = Simulation::run(node, replica.systemAndMoves(),
                                                          replica.nMoves(), 
                                                          nmoves_per_chunk,
                                         (replica.recordStatistics() and record_stats) );
                    
                        all_finished = false;
                    }
                    else if (not running_sims[j].hasFinished())
                    {
                        //continue the calculation from where it finished
                        SimPacket sim = running_sims[j].result();
                        
                        Node node = nodes.getNode();
                        
                        running_sims[j] = Simulation::run( node, sim );
                        
                        all_finished = false;
                    }
                }
                
                if (not all_finished)
                {
                    //wait for the resubmitted calculations to finish
                    for (int j=0; j<replicas.nReplicas(); ++j)
                    {
                        running_sims[j].wait();
                    }
                }
            }

            // all the simulations have now stopped - get the results
            for (int j=0; j<replicas.nReplicas(); ++j)
            {
                SimPacket sim = running_sims[j].result();
                
                //release the memory associated with the Simulation for this
                //replica
                running_sims[j] = Simulation();
                
                replicas.setSystemAndMoves(j, sim.systemAndMoves());
            }
            
            //now perform the replica exchange test on the replicas
            this->testAndSwap(replicas);
            
            //increment the number of supra-ensemble moves run
            ++nmoves;
        }
    }
    catch(...)
    {
        //restore the old state
        this->operator=(old_state);
        replicas.copy( *old_replicas );
        throw;
    }
}

void RepExMove::move(Nodes &nodes, RepExReplicas &replicas, int nmoves,
                     bool record_stats)
{
    this->move(nodes, replicas, nmoves, 100, record_stats);
}

void RepExMove::move(RepExReplicas &replicas, int nmoves,
                     int nmoves_per_chunk, bool record_stats)
{
    Nodes nodes;
    this->move(nodes, replicas, nmoves, nmoves_per_chunk, record_stats);
}

void RepExMove::move(RepExReplicas &replicas, int nmoves, bool record_stats)
{
    Nodes nodes;
    this->move(nodes, replicas, nmoves, record_stats);
}
