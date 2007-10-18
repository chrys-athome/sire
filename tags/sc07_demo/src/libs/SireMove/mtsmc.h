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

#ifndef SIREMOVE_MTSMC_H
#define SIREMOVE_MTSMC_H

#include "montecarlo.h"

#include "SireSystem/moves.h"

#include "SireCAS/symbol.h"

#include "SireSystem/checkpoint.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class MTSMC;
}

QDataStream& operator<<(QDataStream&, const SireMove::MTSMC&);
QDataStream& operator>>(QDataStream&, SireMove::MTSMC&);

namespace SireMove
{

using SireSystem::Moves;
using SireSystem::QuerySystem;
using SireSystem::SimSystem;

using SireCAS::Symbol;

/** This is a multiple-time-step Monte Carlo move
    (see Hetenyi et. al., J. Chem. Phys., 117, 8203-8207, 2002)
    
    This move performs n Monte Carlo moves using the 'fast'
    forcefield, and then accepts or rejects all of these moves
    together according to the 'slow' forcefield
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT MTSMC : public MonteCarlo
{

friend QDataStream& ::operator<<(QDataStream&, const MTSMC&);
friend QDataStream& ::operator>>(QDataStream&, MTSMC&);

public:
    MTSMC();
    
    MTSMC(const Moves &moves, const Symbol &fastcomponent, 
          quint32 nfastmoves=0);
          
    MTSMC(const MTSMC &other);
    
    ~MTSMC();
    
    MTSMC& operator=(const MTSMC &other);
    
    static const char* typeName()
    {
        return "SireMove::MTSMC";
    }

    const char* what() const
    {
        return MTSMC::typeName();
    }

    MTSMC* clone() const
    {
        return new MTSMC(*this);
    }
    
    void setMoves(const Moves &moves);
    void setMoves(const Moves &moves, const Symbol &fastcomponent);
    
    void setNumFastMoves(quint32 nmoves);
    
    const Moves& moves() const;
    
    quint32 numFastMoves() const;
    
    const Symbol& fastEnergyComponent() const;
    void setFastEnergyComponent(const Symbol &symbol);

    void move(SimSystem &system);

    void assertCompatibleWith(QuerySystem &system) const;

private:
    /** The moves performed on the fast forcefield */
    Moves fast_moves;
    
    /** The fast energy component */
    Symbol fast_component;
    
    /** The number of moves on the fast forcefield 
        per slow move */
    quint32 nfast;
};

}

Q_DECLARE_METATYPE(SireMove::MTSMC);

SIRE_END_HEADER

#endif
