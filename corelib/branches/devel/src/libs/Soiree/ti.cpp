/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2013  Christopher Woods
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

#ifdef SIRE_BROKEN_CODE

#include "ti.h"

#include "SireMaths/maths.h"
#include "SireMaths/freeenergyaverage.h"

#include "SireUnits/temperature.h"
#include "SireUnits/units.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

using namespace Soiree;
using namespace SireBase;
using namespace SireMaths;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

////////// Implementation of class Gradient

static const RegisterMetaType<Gradient> r_grad;

QDataStream SOIREE_EXPORT &operator<<(QDataStream &ds, const Gradient &grad)
{
    writeHeader(ds, r_grad, 1);
    
    SharedDataStream sds(ds);

    sds << grad.mean_fwds << grad.mean_bwds << grad.exp_fwds
        << grad.exp_bwds << grad.stdev_fwds << grad.stdev_bwds
        << grad.skew_fwds << grad.skew_bwds << grad.kt
        << grad.nsamples;
    
    return ds;
}

QDataStream SOIREE_EXPORT &operator>>(QDataStream &ds, Gradient &grad)
{
    VersionID v = readHeader(ds, r_grad);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> grad.mean_fwds >> grad.mean_bwds >> grad.exp_fwds
            >> grad.exp_bwds >> grad.stdev_fwds >> grad.stdev_bwds
            >> grad.skew_fwds >> grad.skew_bwds >> grad.kt
            >> grad.nsamples;
    }
    else
        throw version_error(v, "1", r_grad, CODELOC);
    
    return ds;
}

/** kT at room temperature (298.15 K) in internal units */
static double kt_room_temp = k_boltz * 298.15;

/** Constructor */
Gradient::Gradient()
         : mean_fwds(0), mean_bwds(0), exp_fwds(0), exp_bwds(0),
           stdev_fwds(0), stdev_bwds(0), skew_fwds(0), skew_bwds(0),
           kt(kt_room_temp), nsamples(0)
{}

/** Construct the gradient with the passed value */
Gradient::Gradient(MolarEnergy value)
         : mean_fwds(value), mean_bwds(value), exp_fwds(value), exp_bwds(value),
           stdev_fwds(0), stdev_bwds(0), skew_fwds(0), skew_bwds(0),
           kt(kt_room_temp), nsamples(1)
{}

/** Construct the gradient with the passed values representing the forwards
    and backwards gradients. Note that both the forwards and backwards
    gradient must have the same sign (e.g. the average gradient is assumed
    to be 0.5*(forwards_value + backwards_value) */
Gradient::Gradient(MolarEnergy fwds, MolarEnergy bwds)
         : mean_fwds(fwds), mean_bwds(bwds), exp_fwds(fwds), exp_bwds(bwds),
           stdev_fwds(0), stdev_bwds(0), skew_fwds(0), skew_bwds(0),
           kt(kt_room_temp), nsamples(1)
{}

/** Construct the gradient with the passed value, with the value collected
    at the specified temperature */
Gradient::Gradient(MolarEnergy value, const Temperature &temperature)
         : mean_fwds(value), mean_bwds(value), exp_fwds(value), exp_bwds(value),
           stdev_fwds(0), stdev_bwds(0), skew_fwds(0), skew_bwds(0),
           kt(k_boltz * temperature), nsamples(1)
{}

/** Construct the gradient with the passed values representing the forwards
    and backwards gradients. Note that both the forwards and backwards
    gradient must have the same sign (e.g. the average gradient is assumed
    to be 0.5*(forwards_value + backwards_value). The values were collected
    at the specified temperature */
Gradient::Gradient(MolarEnergy fwds, MolarEnergy bwds, const Temperature &temperature)
         : mean_fwds(fwds), mean_bwds(bwds), exp_fwds(fwds), exp_bwds(bwds),
           stdev_fwds(0), stdev_bwds(0), skew_fwds(0), skew_bwds(0),
           kt(k_boltz * temperature), nsamples(1)
{}

/** Return (approximately) the Student's T-value for n samples
    to the 95% confidence limit */
/*static double tValue(int n)
{
    if (n <= 0)
        return 0;
    else if (n == 1)
        return 6.31375;
    else if (n == 2)
        return 2.91999;
    else if (n == 3)
        return 2.35336;
    else if (n == 4)
        return 2.13185;
    else if (n == 5)
        return 2.01505;
    else if (n <= 10)
        return 1.81246;
    else if (n <= 30)
        return 1.69726;
    else if (n <= 100)
        return 1.66023;
    else
        return 1.64487;
}*/

/** Combine the two passed gradient values and return the result */
Gradient Gradient::combine(const Gradient &grad0, const Gradient &grad1)
{
    if (grad0.nSamples() == 0)
        return grad1;
    else if (grad1.nSamples() == 0)
        return grad0;
    else
    {
        throw SireError::incomplete_code("TODO", CODELOC);
        return Gradient();
    }
}

/** Construct the gradient as the average of the passed list of gradients */
Gradient Gradient::combine(const QList<Gradient> &gradients)
{
    if (gradients.isEmpty())
        return Gradient();
    else if (gradients.count() == 1)
        return gradients.at(0);
    else if (gradients.count() == 2)
        return combine(gradients.at(0), gradients.at(1));
    else
    {
        Gradient result = gradients.at(0);
        
        for (int i=1; i<gradients.count(); ++i)
        {
            result = Gradient::combine(result, gradients.at(i));
        }
        
        return result;
    }
}

/** Copy constructor */
Gradient::Gradient(const Gradient &other)
         : mean_fwds(other.mean_fwds), mean_bwds(other.mean_bwds),
           exp_fwds(other.exp_fwds), exp_bwds(other.exp_bwds),
           stdev_fwds(other.stdev_fwds), stdev_bwds(other.stdev_bwds),
           skew_fwds(other.skew_fwds), skew_bwds(other.skew_bwds),
           kt(other.kt), nsamples(other.nsamples)
{}

/** Destructor */
Gradient::~Gradient()
{}

/** Copy assignment operator */
Gradient& Gradient::operator=(const Gradient &other)
{
    if (this != &other)
    {
        mean_fwds = other.mean_fwds;
        mean_bwds = other.mean_bwds;
        exp_fwds = other.exp_fwds;
        exp_bwds = other.exp_bwds;
        stdev_fwds = other.stdev_
    }
}

bool Gradient::operator==(const Gradient &other) const;
bool Gradient::operator!=(const Gradient &other) const;

const char* Gradient::what() const;
const char* Gradient::typeName();

QString Gradient::toString() const;

MolarEnergy Gradient::value() const;

MolarEnergy Gradient::forwardsValue() const;
MolarEnergy Gradient::backwardsValue() const;

MolarEnergy Gradient::error() const;

MolarEnergy Gradient::forwardsError() const;
MolarEnergy Gradient::backwardsError() const;

#endif // ifndef SIRE_BROKEN_CODE
