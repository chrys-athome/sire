/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "mtsmc.h"

#include "SireSystem/system.h"

#include "SireMol/moleculegroup.h"

#include "SireCAS/symbol.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMol;
using namespace SireCAS;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<MTSMC> r_mtsmc;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const MTSMC &mtsmc)
{
    writeHeader(ds, r_mtsmc, 1);
    
    SharedDataStream sds(ds);
    
    sds << mtsmc.fastmoves << mtsmc.fastcomponent << mtsmc.nfastmoves
        << static_cast<const MonteCarlo&>(mtsmc);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MTSMC &mtsmc)
{
    VersionID v = readHeader(ds, r_mtsmc);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> mtsmc.fastmoves >> mtsmc.fastcomponent >> mtsmc.nfastmoves
            >> static_cast<MonteCarlo&>(mtsmc);
    }
    else
        throw version_error(v, "1", r_mtsmc, CODELOC);
        
    return ds;
}

/** Null constructor */
MTSMC::MTSMC() : ConcreteProperty<MTSMC,MonteCarlo>(), nfastmoves(0)
{}

/** Construct a multiple time step Monte Carlo move that performs
    'nfastmoves' moves using 'fastmoves' for every slow move */
MTSMC::MTSMC(const Moves &fast_moves, int nfast_moves)
      : ConcreteProperty<MTSMC,MonteCarlo>(),
        fastmoves(fast_moves), nfastmoves(0)
{
    if (nfast_moves > 0)
    {
        nfastmoves = quint32(nfast_moves);
    }
    
    fastmoves.edit().setGenerator( MonteCarlo::generator() );
    MonteCarlo::setEnsemble( fast_moves.ensemble() );
}

/** Construct a multiple time step Monte Carlo move that performs
    'nfastmoves' moves using 'fastmoves' operating on 
    the energy component 'fastcomponent' for every slow move */
MTSMC::MTSMC(const Moves &fast_moves, const Symbol &fast_component, 
             int nfast_moves)
      : ConcreteProperty<MTSMC,MonteCarlo>(),
        fastmoves(fast_moves), fastcomponent(fast_component),
        nfastmoves(nfast_moves)
{
    MonteCarlo::setEnsemble( fast_moves.ensemble() );
    fastmoves.edit().setGenerator( MonteCarlo::generator() );
}

/** Copy constructor */
MTSMC::MTSMC(const MTSMC &other)
      : ConcreteProperty<MTSMC,MonteCarlo>(other),
        fastmoves(other.fastmoves), fastcomponent(other.fastcomponent),
        nfastmoves(other.nfastmoves)
{}

/** Destructor */
MTSMC::~MTSMC()
{}

/** Copy assignment operator */
MTSMC& MTSMC::operator=(const MTSMC &other)
{
    if (this != &other)
    {
        fastmoves = other.fastmoves;
        fastcomponent = other.fastcomponent;
        nfastmoves = other.nfastmoves;
        MonteCarlo::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool MTSMC::operator==(const MTSMC &other) const
{
    return MonteCarlo::operator==(other) and
           nfastmoves == other.nfastmoves and
           fastcomponent == other.fastcomponent and
           fastmoves == other.fastmoves;
}

/** Comparison operator */
bool MTSMC::operator!=(const MTSMC &other) const
{
    return MonteCarlo::operator!=(other) or
           nfastmoves != other.nfastmoves or
           fastcomponent != other.fastcomponent or
           fastmoves != other.fastmoves;
}

/** Completely clear all of the move statistics */
void MTSMC::clearStatistics()
{
    fastmoves.edit().clearStatistics();    
    MonteCarlo::clearStatistics();
}

/** Return a string representation of this move */
QString MTSMC::toString() const
{
    return QObject::tr("MTSMC( slow = %1, fast = %2, nFast() = %3 "
                       "nAccepted() = %4 nRejected() = %5 )")
               .arg(this->slowEnergyComponent().toString(),
                    this->fastEnergyComponent().toString())
               .arg(this->nFastMoves())
               .arg(this->nAccepted())
               .arg(this->nRejected());
}

/** Return the energy component on which the fast moves will operate */
const Symbol& MTSMC::fastEnergyComponent() const
{
    return fastcomponent;
}

/** Return the energy component that will ultimately be used
    to generate the ensemble */
const Symbol& MTSMC::slowEnergyComponent() const
{
    return MonteCarlo::energyComponent();
}

/** Set the moves to be performed using the fast energy component.
    Note that these moves will be performd using the current
    fast energy component, which will override any energy
    component currently set for these moves */
void MTSMC::setFastMoves(const Moves &fast_moves)
{
    Symbol fastcomponent = this->fastEnergyComponent();
    
    MovesPtr new_fastmoves = fast_moves;
    new_fastmoves.edit().setEnergyComponent(fastcomponent);
    new_fastmoves.edit().setGenerator( MonteCarlo::generator() );

    MonteCarlo::setEnsemble(new_fastmoves.read().ensemble());
    
    fastmoves = new_fastmoves;
}

/** Return the number of fast moves to perform per slow move */
void MTSMC::setNFastMoves(int nfast)
{
    nfastmoves = nfast;
}

/** Set the energy component to be used for the fast moves */
void MTSMC::setFastEnergyComponent(const Symbol &component)
{
    if (component != fastcomponent)
    {
        fastmoves.edit().setEnergyComponent(component);
        fastcomponent = component;
    }
}

/** Set the energy component that will be used for the slow moves */
void MTSMC::setSlowEnergyComponent(const Symbol &component)
{
    MonteCarlo::setEnergyComponent(component);
}

/** Return the fast moves that will be performed for every
    slow move */
const Moves& MTSMC::fastMoves() const
{
    return fastmoves.read();
}

/** Return the number of fast moves to perform per slow move */
int MTSMC::nFastMoves() const
{
    return nfastmoves;
}

/** Set the random number generator used by this and all of the 
    contained moves */
void MTSMC::setGenerator(const RanGenerator &rangenerator)
{
    MonteCarlo::setGenerator( rangenerator );
    fastmoves.edit().setGenerator( MonteCarlo::generator() );
} 

/** Perform the move - this will perform nfastmoves using fastmoves,
    and will then accept or reject the result based on the difference
    in the difference in energy between the fast and slow energies
    before and after the moves */
void MTSMC::move(System &system, int nmoves, bool record_stats)
{
    if (nmoves == 0 or nfastmoves == 0)
        //nothing to do
        return;

    System old_system_state(system);
    MTSMC old_state(*this);
    
    try
    {
        for (int i=0; i<nmoves; ++i)
        {
            //get the old energies
            double old_slow_nrg = system.energy(this->slowEnergyComponent());
            double old_fast_nrg = system.energy(this->fastEnergyComponent());
    
            //save the old system
            System old_system = system;
            
            //now perform the moves (without recording statistics)
            system = fastmoves.edit().move(system, nfastmoves, false);
            
            //get the new energies
            double new_fast_nrg = system.energy(this->fastEnergyComponent());
            double new_slow_nrg = system.energy(this->slowEnergyComponent());
            
            //work out the delta
            #warning Need to check MTSMC acceptance test
            double new_nrg = new_slow_nrg - new_fast_nrg;
            double old_nrg = old_slow_nrg - old_fast_nrg;
            
            if (not MonteCarlo::test(new_nrg, old_nrg))
            {
                //restore the old configuration
                system = old_system;
            }

            if (record_stats)
            {
                system.collectStats();
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        system = old_system_state;
        
        throw;
    }
}
