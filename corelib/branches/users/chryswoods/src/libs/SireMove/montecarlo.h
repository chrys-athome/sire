/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMOVE_MONTECARLO_H
#define SIREMOVE_MONTECARLO_H

#include "move.h"

#include "SireCAS/symbol.h"

#include "SireUnits/dimensions.h"

#include "SireMaths/rangenerator.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class MonteCarlo;
}

QDataStream& operator<<(QDataStream&, const SireMove::MonteCarlo&);
QDataStream& operator>>(QDataStream&, SireMove::MonteCarlo&);

namespace SireSim
{
class SimSystem;
}

namespace SireMove
{

using SireMaths::RanGenerator;

using SireUnits::Dimension::Temperature;

using SireCAS::Symbol;

/** This is the base class of all Monte Carlo moves

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MonteCarlo : public MoveBase
{

friend QDataStream& ::operator<<(QDataStream&, const MonteCarlo&);
friend QDataStream& ::operator>>(QDataStream&, MonteCarlo&);

public:
    MonteCarlo();
    
    MonteCarlo(const MonteCarlo &other);

    ~MonteCarlo();

    static const char* typeName()
    {
        return "SireMove::MonteCarlo";
    }

    quint32 nAttempted() const;
    quint32 nAccepted() const;
    quint32 nRejected() const;

    double acceptanceRatio() const;

    virtual void clearMoveStatistics();

    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

    void setTemperature(Temperature temperature);
    Temperature temperature() const;

    void setEnergyComponent(const Symbol &component);
    const Symbol& energyComponent() const;

protected:
    MonteCarlo& operator=(const MonteCarlo &other);

    bool test(double new_energy,
              double old_energy,
              double new_bias,
              double old_bias);

    bool test(double new_energy,
              double old_energy);

private:
    /** The random number generator used during the moves
        and in the test */
    RanGenerator rangenerator;

    /** The symbol representing the energy component that
        this Monte Carlo move is sampling */
    Symbol nrg_component;

    /** beta (== 1 / kT) */
    double beta;

    /** The number of times this move has been attempted */
    quint32 naccept;

    /** The number of times this move has been rejected */
    quint32 nreject;
};

}

SIRE_EXPOSE_CLASS( SireMove::MonteCarlo )

SIRE_END_HEADER

#endif
