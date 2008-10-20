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
#include "replicarunner.h"

#include "SireMPI/mpinodes.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMaths;
using namespace SireStream;

static const RegisterMetaType<RepExMove> r_repexmove;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const RepExMove &repexmove)
{
    writeHeader(ds, r_repexmove, 1);
    
    SharedDataStream sds(ds);
    
    sds << repexmove.rangenerator << repexmove.naccept
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
        sds >> repexmove.rangenerator >> repexmove.naccept
            >> repexmove.nreject;
    }
    else
        throw version_error(v, "1", r_repexmove, CODELOC);
        
    return ds;
}

/** Constructor */
RepExMove::RepExMove() : naccept(0), nreject(0)
{}

/** Copy constructor */
RepExMove::RepExMove(const RepExMove &other)
          : rangenerator(other.rangenerator),
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
        naccept = other.naccept;
        nreject = other.nreject;
    }
    
    return *this;
}

/** Comparison operator */
bool RepExMove::operator==(const RepExMove &other) const
{
    return naccept == other.naccept and
           nreject == other.nreject and
           rangenerator == other.rangenerator;
}

/** Comparison operator */
bool RepExMove::operator!=(const RepExMove &other) const
{
    return naccept != other.naccept or
           nreject != other.nreject or
           rangenerator != other.rangenerator;
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
        
        //now get the values of the system properties at their current replica level
        Energy H_a_i = replica_a.energy();
        Energy H_b_i = replica_b.energy();
        
        Volume V_a_i(0);
        Volume V_b_i(0);
        
        if (need_pv)
        {
           V_a_i = replica_a_i.volume();
           V_b_i = replica_b_i.volume();
        }
        
        //now swap the replicas
        RepExReplica::swap(replica_a, replica_b);
        
        //get the values of the system properties at the new replica level
        Energy H_a_j = replica_a.energy();
        Energy H_b_j = replica_b.energy();
        
        Volume V_a_j(0);
        Volume V_b_j(0);
        
        if (need_pv)
        {
            V_a_j = replica_a.volume();
            V_b_j = replica_b.volume();
        }
        
        //now calculate delta needed for the Monte Carlo test
        //
        //   For derivation see Appendix C of Christopher Woods' thesis
        //
        //  delta = beta_b * [ H_b_j - H_b_i + P_b (V_b_j - V_b_i) ] + 
        //          beta_a * [ H_a_i - H_a_j + P_a (V_a_i - V_a_j) ]
        
        double delta = beta_b * ( H_b_j - H_b_i + P_b*(V_b_j - V_b_i) ) +
                       beta_a * ( H_a_i - H_a_j + P_a*(V_a_i - V_a_j) );
                       
        return ( delta > 0 or (std::exp(delta) >= rangenerator.rand()) );
    }
    else
    {
        throw SireError::incompatible_error( QObject::tr(
            "There is no available replica exchange test that allows tests between "
            "replicas with ensembles %1 and %2.")
                .arg(ensemble0.toString(), ensemble1.toString()), CODELOC );
                
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
    for (int i=start; i<nreplicas; ++i)
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
    running all jobs using the passed replica runner, and recording statistics if
    'record_stats' is true. */
void RepExMove::move(RepExReplicas &replicas, int nmoves,
                     const ReplicaRunner &reprunner, bool record_stats)
{
    if (nmoves <= 0 or replicas.isEmpty())
        return;

    RepExMove old_state(*this);
    boost::shared_ptr<RepExReplicas> old_replicas( replicas.clone() );
    
    try
    {
        for (int i=0; i<nmoves; ++i)
        {
            //run a block of sampling on each replica
            reprunner.run(replicas, record_stats);
            
            //now perform the replica exchange test on the replicas
            this->testAndSwap(replicas);
        }
    }
    catch(...)
    {
        //restore the old state
        this->operator=(old_state);
        replicas.assign( *old_replicas );
        throw;
    }
}

/** Perform 'nmoves' replica exchange moves on the replicas in 'replicas',
    running all jobs in the local thread, and recording statistics if
    'record_stats' is true. */
void RepExMove::move(RepExReplicas &replicas, int nmoves, bool record_stats)
{
    this->move(replicas, nmoves, BasicRepRunner(), record_stats);
}

/** Perform 'nmoves' replica exchange moves on the replicas in 'replicas',
    running all jobs on as many of the nodes in 'nodes' as possible, 
    and recording statistics if 'record_stats' is true. */
void RepExMove::move(RepExReplicas &replicas, int nmoves,
                     const MPINodes &nodes, bool record_stats)
{
    this->move(replicas, nmoves, MPIRepRunner(nodes), record_stats);
}
