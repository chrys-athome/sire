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

#include "hremove.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<HREMove> r_hremove;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const HREMove &hremove)
{
    writeHeader(ds, r_hremove, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const REMove&>(hremove);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, HREMove &hremove)
{
    VersionID v = readHeader(ds, r_hremove);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> static_cast<REMove&>(hremove);
    }
    else
        throw version_error(v, "1", r_hremove, CODELOC);
    
    return ds;
}

/** Constructor */
HREMove::HREMove() : ConcreteProperty<HREMove,REMove>()
{}

/** Copy constructor */
HREMove::HREMove(const HREMove &other)
        : ConcreteProperty<HREMove,REMove>(other)
{}

/** Destructor */
HREMove::~HREMove()
{}

/** Copy assignment operator */
HREMove& HREMove::operator=(const HREMove &other)
{
    REMove::operator=(other);
    return *this;
}

/** Comparison operator */
bool HREMove::operator==(const HREMove &other) const
{
    return REMove::operator==(other);
}

/** Comparison operator */
bool HREMove::operator!=(const HREMove &other) const
{
    return REMove::operator!=(other);
}

/** Internal function used to perform the actual Hamiltonian replica
    exchange test */
void HREMove::performRETest(RESet &replicas, bool record_stats)
{
    if (nrg_symbol.isNull())
        throw SireError::program_bug( QObject::tr(
            "Program bug - the energy symbol for a Hamiltonian replica exchange "
            "move should never be null."), CODELOC );

    int nreplicas = replicas.count();
    
    if (nreplicas <= 1)
        return;
    
    const Symbol &lam = replicas.symbol();
    
    if (lam.isNull())
        throw SireError::incompatible_error( QObject::tr(
            "You cannot perform Hamiltonian replica exchange with the passed "
            "replica set as there is no symbol over which the Hamiltonian is varied."),
                CODELOC );
            
    
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
        Replica low_replica = replicas[i].system();
        Replica high_replica = replicas[i+1].system();
        
        System low_system = low_replica.system();
        System high_system = high_replica.system();
        
        //calculate the energies of both replicas at both levels
        //(using the desired energy component)
        double old_low_energy = low_system.energy(nrg_symbol);
        double old_high_energy = high_system.energy(nrg_symbol);
        
        low_system.setComponent(lam, low_replica.parameter(+1));
        high_system.setComponent(lam, high_replica.parameter(-1));
        
        double new_low_energy = low_system.energy(nrg_symbol);
        double new_high_energy = high_system.energy(nrg_symbol);

        if (this->test(new_low_energy, old_low_energy,
                       new_high_energy, old_high_energy))
        {
            //swap the replicas over
            replicas.swap(i, i+1);
            REMove::moveAccepted();
        }
        else
            REMove::moveRejected();
    }
}
