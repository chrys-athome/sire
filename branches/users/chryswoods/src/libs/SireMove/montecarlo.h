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

#include "SireSystem/move.h"

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
class Simulation;
}

namespace SireMove
{

using SireSim::Simulation;

using SireSystem::MoveBase;

using SireMaths::RanGenerator;

using SireUnits::Dimension::Temperature;

/** This is the base class of all Monte Carlo moves

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MonteCarlo : public MoveBase
{

friend QDataStream& ::operator<<(QDataStream&, const MonteCarlo&);
friend QDataStream& ::operator>>(QDataStream&, MonteCarlo&);

public:
    MonteCarlo(const RanGenerator &generator = RanGenerator());
    MonteCarlo(const MonteCarlo &other);

    ~MonteCarlo();

    MonteCarlo& operator=(const MonteCarlo &other);

    quint32 nAttempted() const;
    quint32 nAccepted() const;
    quint32 nRejected() const;

    double acceptanceRatio() const;

    virtual void clearMoveStatistics();

    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

    void setTemperature(Temperature temperature);
    Temperature temperature() const;

protected:
    bool test(double new_energy,
              double old_energy,
              double new_bias,
              double old_bias);

    bool test(double new_energy,
              double old_energy);

    RanGenerator& _pvt_generator()
    {
        return _generator;
    }

private:
    /** The random number generator used during the moves
        and in the test */
    RanGenerator _generator;

    /** beta (== 1 / kT) */
    double beta;

    /** The number of times this move has been attempted */
    quint32 naccept;

    /** The number of times this move has been rejected */
    quint32 nreject;
};

}

SIRE_END_HEADER

#endif
