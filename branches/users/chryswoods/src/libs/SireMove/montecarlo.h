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

/** This is the base class of all Monte Carlo moves

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MonteCarlo : public MoveBase
{

friend QDataStream& ::operator<<(QDataStream&, const MonteCarlo&);
friend QDataStream& ::operator>>(QDataStream&, MonteCarlo&);

public:
    MonteCarlo();
    MonteCarlo(const RanGenerator &generator);

    MonteCarlo(const MonteCarlo &other);

    ~MonteCarlo();

    MonteCarlo& operator=(const MonteCarlo &other);

    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

    class CheckPoint
    {
    public:
        CheckPoint();

        CheckPoint(const Simulation &simulation);

        CheckPoint(const CheckPoint &other);

        ~CheckPoint();
    };

protected:
    RanGenerator& generator();

private:
    /** The random number generator used to generate
        and test this Monte Carlo moves */
    RanGenerator rangen;
};

/** Internal function used to return the random number 
    generator used by this move */
inline RanGenerator& MonteCarlo::generator()
{
    return rangen;
}

}

SIRE_END_HEADER

#endif
