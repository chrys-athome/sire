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

#include "SireSystem/checkpoint.h"
#include "SireSystem/simsystem.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<MTSMC> r_mtsmc;

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const MTSMC &mtsmc)
{
    writeHeader(ds, r_mtsmc, 1);
    
    SharedDataStream sds(ds);
    sds << mtsmc.fast_moves << mtsmc.nfast << mtsmc.fast_component
        << static_cast<const MonteCarlo&>(mtsmc);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MTSMC &mtsmc)
{
    VersionID v = readHeader(ds, r_mtsmc);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> mtsmc.fast_moves >> mtsmc.nfast >> mtsmc.fast_component
            >> static_cast<MonteCarlo&>(mtsmc);
    }
    else
        throw version_error(v, "1", r_mtsmc, CODELOC);
    
    return ds;
}

/** Null constructor */
MTSMC::MTSMC() 
      : MonteCarlo(),
        fast_component(SireFF::e_total()), nfast(0)
{}

/** Create an MTSMC move that uses 'moves' to perform the 'fast' moves,
    and performs 'nfastmoves' using the 'fast' energy represented by
    'fastcomponent' */
MTSMC::MTSMC(const Moves &moves, const Symbol &fastcomponent, quint32 nfastmoves)
      : MonteCarlo(),
        fast_moves(moves), fast_component(fastcomponent), nfast(nfastmoves)
{
    //make sure that all of the moves use the fast component
    //to calculate the energy
    fast_moves.setEnergyComponent(fast_component);
}

/** Copy constructor */
MTSMC::MTSMC(const MTSMC &other)
      : MonteCarlo(other), 
        fast_moves(other.fast_moves), fast_component(other.fast_component),
        nfast(other.nfast)
{}

/** Destructor */
MTSMC::~MTSMC()
{}

/** Copy assignment operator */
MTSMC& MTSMC::operator=(const MTSMC &other)
{
    if (this != &other)
    {
        fast_moves = other.fast_moves;
        fast_component = other.fast_component;
        nfast = other.nfast;
        MonteCarlo::operator=(other);
    }
    
    return *this;
}

/** Set the moves that will be performed using the 'fast' energy */
void MTSMC::setMoves(const Moves &moves)
{
    fast_moves = moves;
    fast_moves.setEnergyComponent(fast_component);
}

/** Set the moves that will be performed using the 'fast' energy
    represented by the component 'fastcomponent' */
void MTSMC::setMoves(const Moves &moves, const Symbol &fastcomponent)
{
    fast_moves = moves;
    fast_component = fastcomponent;
    
    fast_moves.setEnergyComponent(fast_component);
}

/** Set the number of moves that will be performed using the
    fast energy */
void MTSMC::setNumFastMoves(quint32 nmoves)
{
    nfast = nmoves;
}

/** Return the moves that will be performed using the fast
    energy component */
const Moves& MTSMC::moves() const
{
    return fast_moves;
}

/** Return the number of moves that will be performed on the 
    fast forcefield */
quint32 MTSMC::numFastMoves() const
{
    return nfast;
}

/** Return the energy component that represents the fast 
    component of the forcefield. */
const Symbol& MTSMC::fastEnergyComponent() const
{
    return fast_component;
}

/** Set the energy component that will be used for the 
    'fast' moves. */
void MTSMC::setFastEnergyComponent(const Symbol &component)
{
    if (component != fast_component)
    {
        fast_component = component;
        fast_moves.setEnergyComponent(fast_component);
    }
}

/** Assert that this move is compatible with the passed system */
void MTSMC::assertCompatibleWith(QuerySystem &system) const
{
    fast_moves.assertCompatibleWith(system);
    MonteCarlo::assertCompatibleWith(system);
}

/** Perform a single MTSMC move on the passed system */
void MTSMC::move(SimSystem &system)
{
    // MTSMC algorithm
    //
    // Performs boltzmann sampling on potential V == V1 + V2
    //
    // (1) Start at configuration x_old
    // (2) Evaluate V2(x_old)
    // (3) Set y_old = x_old
    // (4) Perform n steps from y_old to y_new using only potential V1 - reach y_new
    // (5) Set x_new = y_new
    // (6) Accept x_old -> x_new using min[ 1, exp(-beta * {V2(x_new)-V2(x_old)}) ]

    //get the old energy on the slow forcefield
    double V_old = energy(system);   // This is the energy V
    
    // V2 = V - V1
    double V2_old = V_old - system.energy(fast_component);
    
    //checkpoint the system
    CheckPoint checkpoint = system.checkPoint();
    Moves old_moves = fast_moves;
    
    try
    {
        //perform the moves using the fast forcefield
        fast_moves.run(system, nfast);
        
        //get the new energy after the moves
        double V_new = energy(system);
        double V2_new = V_new - system.energy(fast_component);
    
        if (not this->test(V2_new, V2_old))
            //the move has been rejected - reset the state,
            //but don't restore the moves, as I don't want to
            //lose their statistics
            system.rollBack(checkpoint);
            
    }
    catch(...)
    {
        //restore the system and the moves
        system.rollBack(checkpoint);
        fast_moves = old_moves;
        throw;
    }
}
