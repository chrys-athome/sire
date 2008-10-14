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

#include "remove.h"

#include "SireMPI/mpinode.h"
#include "SireMPI/mpinodes.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMPI;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<REMove> r_remove( MAGIC_ONLY,
                                                "SireMove::REMove" );

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const REMove &remove)
{
    writeHeader(ds, r_remove, 1);
    
    SharedDataStream sds(ds);
    
    sds << remove.rangenerator
        << remove.naccepted << remove.nrejected
        << static_cast<const PropertyBase&>(remove);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, REMove &remove)
{
    VersionID v = readHeader(ds, r_remove);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> remove.rangenerator
            >> remove.naccepted >> remove.nrejected
            >> static_cast<PropertyBase&>(remove);
    }
    else
        throw version_error(v, "1", r_remove, CODELOC);
        
    return ds;
}

/** Constructor */
REMove::REMove() : PropertyBase(), naccepted(0), nrejected(0)
{}

/** Copy constructor */
REMove::REMove(const REMove &other)
       : PropertyBase(other),
         rangenerator(other.rangenerator),
         naccepted(other.naccepted), nrejected(other.nrejected)
{}

/** Destructor */
REMove::~REMove()
{}

/** Copy assignment operator */
REMove& REMove::operator=(const REMove &other)
{
    if (this != &other)
    {
        PropertyBase::operator=(other);
        rangenerator = other.rangenerator;
        naccepted = other.naccepted;
        nrejected = other.nrejected;
    }
    
    return *this;
}

/** Comparison operator */
bool REMove::operator==(const REMove &other) const
{
    return naccepted == other.naccepted and 
           nrejected == other.nrejected and
           rangenerator == other.rangenerator;
}

/** Comparison operator */
bool REMove::operator!=(const REMove &other) const
{
    return not this->operator==(other);
}

/** Protected function called by a derived class to increment
    the number of accepted replica exchange moves */
void REMove::moveAccepted()
{
    ++naccepted;
}

/** Protected function called by a derived class to increment
    the number of rejected replica exchange moves */
void REMove::moveRejected()
{
    ++nrejected;
}

/** Return the total number of replica exchange moves attempted */
int REMove::nAttempted() const
{
    return naccepted + nrejected;
}

/** Return the total number of accepted replica exchange moves */
int REMove::nAccepted() const
{
    return naccepted;
}

/** Return the total number of rejected replica exchange moves */
int REMove::nRejected() const
{
    return nrejected;
}

/** Return the ratio of the number of accepted moves to the number
    of attempted moves */
double REMove::acceptanceRatio() const
{
    int ntotal = this->nAttempted();
    
    if (ntotal == 0)
        return 0;
    else
        return double(naccepted) / double(ntotal);
}

/** Clear statistics about the number of attempted and accepted moves */
void REMove::clearMoveStatistics()
{
    naccepted = 0;
    nrejected = 0;
}

/** Set the random number generator that will be used in the Monte Carlo
    test used to accept or reject a replica exchange move */
void REMove::setGenerator(const RanGenerator &generator)
{
    rangenerator = generator;
}

/** Return the random number generator used in the Monte Carlo acceptance
    test used to accept or reject a replica exchange move */
const RanGenerator& REMove::generator() const
{
    return rangenerator;
}

/** Internal function called by derived classes to perform a block of
    sampling on each of the replicas in the passed RESet */
void REMove::performSampling(RESet &replicas, bool record_stats) const
{
    int nreplicas = replicas.count();
    
    if (nreplicas == 0)
        return;
    
    RESet new_replicas = replicas;
    
    //loop over all replicas in the set
    for (int i=0; i<nreplicas; ++i)
    {
        //get a copy of this replica
        Replica replica = replicas[i];
        
        //run the simulation
        ReplicaSim new_replica = replica.run(record_stats);
        
        //update the new set of replicas (so that if something goes wrong
        //then we don't have the old set in a half-way state)
        new_replicas.update(new_replica);
    }
    
    //everything finished ok - copy the new replicas to the old
    replicas = new_replicas;
}

/** Internal function called by derived classes to perform a block of
    sampling on each of the replicas in the passed RESet on the 
    remote MPI node 'node' */
void REMove::performSampling(const MPINode &node, 
                             RESet &replicas, bool record_stats) const
{
    int nreplicas = replicas.count();
    
    if (nreplicas == 0)
        return;
    
    RESet new_replicas = replicas;
    
    //loop over all replicas in the set
    for (int i=0; i<nreplicas; ++i)
    {
        //get a copy of this replica
        Replica replica = replicas[i];
        
        //run the simulation
        ReplicaSim new_replica = replica.run(node, record_stats);
        
        //update the new set of replicas (so that if something goes wrong
        //then we don't have the old set in a half-way state)
        new_replicas.update(new_replica);
    }
    
    //everything finished ok - copy the new replicas to the old
    replicas = new_replicas;
}

/** Internal function called by derived classes to perform a block of
    sampling on each of the replicas in the passed RESet using as
    many of the MPI nodes in the cluster 'nodes' as possible */
void REMove::performSampling(MPINodes &nodes, 
                             RESet &replicas, bool record_stats) const
{
    int nreplicas = replicas.count();
    
    if (nreplicas == 0)
        return;
    
    QList<ReplicaSim> running_sims;
    
    //loop over all replicas in the set and set them all running
    for (int i=0; i<nreplicas; ++i)
    {
        //get a copy of this replica
        Replica replica = replicas[i];
        
        //get a free node on which to run the simulation
        MPINode node = nodes.getFreeNode();
        
        //run the simulation - save the handle
        running_sims.append( replica.run(node, record_stats) );
    }

    //now loop over the running simulations and wait for 
    //them all to finish - then update the new replicas with
    //the new results
    RESet new_replicas = replicas;

    for (int i=0; i<nreplicas; ++i)
    {
        ReplicaSim running_sim = running_sims.takeFirst();
        
        //update the new replicas with this simulation - this blocks
        //until the simulation has finished
        new_replicas.update( running_sim );
    }
    
    //everything finished ok - copy the new replicas to the old
    replicas = new_replicas;
}

/** Perform 'nmoves' Hamiltonian replica exchange moves, that
    each involve a block of sampling on the passed set of replicas,
    and then a Hamiltonian replica exchange test that
    attempts to swap pairs within this set */
RESet REMove::move(const RESet &replicas, int nmoves, bool record_stats)
{
    if (nmoves <= 0)
        return replicas;

    RESet new_replicas = replicas;

    Property old_state(*this);
    old_state.detach();
    
    try
    {
        for (int i=0; i<nmoves; ++i)
        {
            //perform a block of sampling
            this->performSampling(new_replicas, record_stats);
            
            //now perform the replica exchange tests
            this->performRETest(new_replicas, record_stats);
        }
    }
    catch(...)
    {
        this->copy(old_state);
        throw;
    }

    return new_replicas;
}

/** Perform 'nmoves' Hamiltonian replica exchange moves, that
    each involve a block of sampling on the passed set of replicas,
    and then a Hamiltonian replica exchange test that
    attempts to swap pairs within this set. All of the sampling will
    be performed on the remove node 'node' */
RESet REMove::move(const MPINode &node,
                   const RESet &replicas, int nmoves, bool record_stats)
{
    if (nmoves <= 0)
        return replicas;

    RESet new_replicas = replicas;

    Property old_state(*this);
    old_state.detach();
    
    try
    {
        for (int i=0; i<nmoves; ++i)
        {
            //perform a block of sampling
            this->performSampling(node, new_replicas, record_stats);
            
            //now perform the replica exchange tests
            this->performRETest(new_replicas, record_stats);
        }
    }
    catch(...)
    {
        this->copy(old_state);
        throw;
    }

    return new_replicas;
}

/** Perform 'nmoves' Hamiltonian replica exchange moves, that
    each involve a block of sampling on the passed set of replicas,
    and then a Hamiltonian replica exchange test that
    attempts to swap pairs within this set. All of the sampling will
    be performed using as many of the nodes in 'nodes' as possible */
RESet REMove::move(MPINodes &nodes,
                   const RESet &replicas, int nmoves, bool record_stats)
{
    if (nmoves <= 0)
        return replicas;

    RESet new_replicas = replicas;

    Property old_state(*this);
    old_state.detach();
    
    try
    {
        for (int i=0; i<nmoves; ++i)
        {
            //perform a block of sampling
            this->performSampling(nodes, new_replicas, record_stats);
            
            //now perform the replica exchange tests
            this->performRETest(new_replicas, record_stats);
        }
    }
    catch(...)
    {
        this->copy(old_state);
        throw;
    }

    return new_replicas;
}
