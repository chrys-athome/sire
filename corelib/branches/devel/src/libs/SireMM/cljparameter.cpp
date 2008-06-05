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

#include "cljparameter.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;

static const RegisterMetaType<CLJParameter> r_cljparam;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJParameter &cljparam)
{
    writeHeader(ds, r_cljparam, 1)
          << static_cast<const ChargeParameter&>(cljparam)
          << static_cast<const LJParameter&>(cljparam);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJParameter &cljparam)
{
    VersionID v = readHeader(ds, r_cljparam);

    if (v == 1)
    {
        ds >> static_cast<ChargeParameter&>(cljparam)
           >> static_cast<LJParameter&>(cljparam);
    }
    else
        throw version_error(v, "1", r_cljparam, CODELOC);

    return ds;
}

/** Create a a dummy parameter. */
CLJParameter::CLJParameter() : ChargeParameter(), LJParameter()
{}

/** Copy constructor */
CLJParameter::CLJParameter(const CLJParameter &other)
                             : ChargeParameter(other), LJParameter(other)
{}

/** Construct a CLJParameter with the supplied values of the charge,
    sigma and epsilon */
CLJParameter::CLJParameter(const ChargeParameter &charge, double sigma, double epsilon)
                             : ChargeParameter(charge), LJParameter(sigma, epsilon)
{}

/** Construct a CLJParameter from the supplied charge and LJ parameter */
CLJParameter::CLJParameter(const ChargeParameter &charge, const LJParameter &lj)
                             : ChargeParameter(charge), LJParameter(lj)
{}

/** Destructor */
CLJParameter::~CLJParameter()
{}

/** Comparison operator */
bool CLJParameter::operator==(const CLJParameter &other) const
{
    return ChargeParameter::operator==(other) and LJParameter::operator==(other);
}

/** Comparison operator */
bool CLJParameter::operator!=(const CLJParameter &other) const
{
    return ChargeParameter::operator!=(other) or LJParameter::operator!=(other);
}

/** Return a dummy CLJParameter */
CLJParameter CLJParameter::dummy()
{
    return CLJParameter(0.0,0.0,0.0);
}

/** Return a string representation of the CLJ parameter */
QString CLJParameter::toString() const
{
    return QString("CLJ[ %1 %2 ]")
            .arg(ChargeParameter::toString(), LJParameter::toString());
}
