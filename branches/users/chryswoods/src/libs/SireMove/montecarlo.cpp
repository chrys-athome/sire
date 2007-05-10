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

#include "montecarlo.h"

#include <cmath>

#include "SireUnits/units.h"
#include "SireUnits/temperature.h"

#include "SireStream/datastream.h"

using namespace SireMove;
using namespace SireUnits;
using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<MonteCarlo> r_mc(MAGIC_ONLY, "SireMove::MonteCarlo");

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const MonteCarlo &mc)
{
    writeHeader(ds, r_mc, 1);

    SharedDataStream sds(ds);

    sds << mc._generator << mc.beta
        << mc.naccept << mc.nreject
        << static_cast<const MoveBase&>(mc);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MonteCarlo &mc)
{
    VersionID v = readHeader(ds, r_mc);

    if (v == 1)
    {
        ds >> mc._generator >> mc.beta
           >> mc.naccept >> mc.nreject
           >> static_cast<MoveBase&>(mc);
    }
    else
        throw version_error(v, "1", r_mc, CODELOC);

    return ds;
}

/** Construct using the supplied random number generator */
MonteCarlo::MonteCarlo(const RanGenerator &generator)
           : MoveBase(), _generator(generator),
             naccept(0), nreject(0)
{
    setTemperature( 25 * celsius );
}

/** Copy constructor */
MonteCarlo::MonteCarlo(const MonteCarlo &other)
           : MoveBase(other), _generator(other._generator),
             beta(other.beta),
             naccept(other.naccept), nreject(other.nreject)
{}

/** Destructor */
MonteCarlo::~MonteCarlo()
{}

/** Copy assignment */
MonteCarlo& MonteCarlo::operator=(const MonteCarlo &other)
{
    _generator = other._generator;
    beta = other.beta;
    naccept = other.naccept;
    nreject = other.nreject;

    MoveBase::operator=(other);

    return *this;
}

/** Set the temperature of the MC simulation */
void MonteCarlo::setTemperature(double temperature)
{
    beta = 1.0 / (k_boltz * temperature);
}

/** Return the temperature of the MC simulation */
double MonteCarlo::temperature() const
{
    return 1.0 / (k_boltz * beta);
}

/** Set the random number generator to use for these moves */
void MonteCarlo::setGenerator(const RanGenerator &generator)
{
    _generator = generator;
}

/** Return the random number generator used for these moves */
const RanGenerator& MonteCarlo::generator() const
{
    return _generator;
}

/** Return the number of attempted moves */
quint32 MonteCarlo::nAttempted() const
{
    return naccept + nreject;
}

/** Return the number of accepted moves */
quint32 MonteCarlo::nAccepted() const
{
    return naccept;
}

/** Return the number of rejected moves */
quint32 MonteCarlo::nRejected() const
{
    return nreject;
}

/** Return the acceptance ratio (ratio of
    accepted moves to attempted moves) */
double MonteCarlo::acceptanceRatio() const
{
    return double(nAccepted()) / double(nAttempted());
}

/** Zero the move statistics */
void MonteCarlo::clearMoveStatistics()
{
    naccept = 0;
    nreject = 0;
}

/** Perform the monte carlo test, using the supplied change in energy
    and the supplied change in biasing probabilities */
bool MonteCarlo::test(double new_energy, double old_energy,
                      double new_bias, double old_bias)
{
    double x = (new_bias / old_bias) * std::exp( -beta*(new_energy - old_energy) );

    if (x > 1 or x > _generator.rand())
    {
        ++naccept;
        return true;
    }
    else
    {
        ++nreject;
        return false;
    }
}
