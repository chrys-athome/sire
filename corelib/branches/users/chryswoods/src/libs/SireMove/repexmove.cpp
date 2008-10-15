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

/** Internal */
void RepExMove::testAndSwap(RepExReplicas &replicas, bool record_stats)
{
    int nreplicas = this->count();
    
    if (nreplicas <= 1)
        return;

    //decide whether we will swap even or odd pairs
    int start = 0;
    
    if (nreplicas > 2)
    {
        if (this->generator().randBool())
            start = 1;
    }

    for (int i=start; i<nreplicas; i+=2)
    {
        //get the pair of replicas to be tested
        RepExReplica low_replica = replicas[i];
        RepExReplica high_replica = replicas[i+1];

        bool need_pv_term = false;
        double old_pv_term = 0;
        
        //we need to collect together all of the parts of the
        //test for this bits that change
        if (low_replica.temperature() != high_replica.temperature() or
            low_replica.pressure() != high_replica.pressure())
        {
            //we need to calculate the pV term - we need the 
            //old volumes of the systems
            ...
            
            need_pv_term = true;
        }
        
        //calculate the energies of both replicas at both levels
        //(using the desired energy component)
        double old_low_energy = low_replica.energy();
        double old_high_energy = high_replica.energy();
        
        RepExReplica::swapSystems(low_replica, high_replica);

        double new_low_energy = old_low_energy;
        double new_high_energy = old_high_energy;

        if (low_replica.energyComponent() != high_replica.energyComponent() or
            low_replica.lambdaComponent() != high_replica.lambdaComponent() or
            low_replica.lambdaValue() != high_replica.lambdaValue())
        {
            //the Hamiltonians of the two replicas are different - this
            //means we have a deltaE term
            new_low_energy = low_replica.energy();
            new_high_energy = high_replica.energy();
        }

        double new_pv_term = 0;
        if (need_pv_term)
        {
            //we need to calculate the new pV term....
            ...;
        }

        //now calculate the delta value...
        double delta = ;

        if (this->test(delta))
        {
            //update the replicas
            replicas.setReplica(i, low_replica);
            replicas.setReplica(i+1, high_replica);
            this->moveAccepted();
        }
        else
            this->moveRejected();
    }
}

RepExReplicas RepExMove::run(const RepExReplicas &replicas, int nmoves, 
                             bool record_stats)
{
    if (nmoves <= 0)
        return replicas;

    RepExReplicas new_replicas(replicas);
    
    RepExMove old_state(*this);
    
    try
    {
        for (int i=0; i<nmoves; ++i)
        {
            //perform a block of sampling
            new_replicas.setReplicas( BasicRepRunner().run(new_replicas) );
            
            //now perform the replica exchange test
            this->testAndSwap(new_replicas, record_stats);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
    
    return new_replicas;
}
