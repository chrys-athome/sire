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

#include <QObject>

#include "cljpair.h"

#include "SireMaths/maths.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;

static const RegisterMetaType<CLJPair> r_cljpair;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJPair &cljpair)
{
    writeHeader(ds, r_cljpair, 1) << cljpair.charge2() << cljpair.sigma()
                                  << cljpair.epsilon();

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJPair &cljpair)
{
    VersionID v = readHeader(ds, r_cljpair);

    if (v == 1)
    {
        double chg2,sig,eps;

        ds >> chg2 >> sig >> eps;

        cljpair = CLJPair(chg2, sig, eps);
    }
    else
        throw version_error(v, "1", r_cljpair, CODELOC);

    return ds;
}

/** Construct a null pair */
CLJPair::CLJPair() : chg2(0.0), sig(0.0), eps(0.0)
{}

/** Construct a CLJPair that has charge^2 = chg2, combined sigma = sig and combined
    epsilon = eps */
CLJPair::CLJPair(double c2, double s, double e) : chg2(c2), sig(s), eps(e)
{}

/** Construct a CLJPair from the passed CLJ parameter */
CLJPair::CLJPair(const CLJParameter &cljparam)
        : chg2( SireMaths::pow_2(cljparam.charge()) ),
          sig( cljparam.sigma() ),
          eps( cljparam.epsilon() )
{}

/** Destructor */
CLJPair::~CLJPair()
{}

/** Return the LJ 'A' parameter */
double CLJPair::A() const
{
    return double(4.0) * epsilon() * SireMaths::pow_12( sigma() );
}

/** Return the LJ 'B' parameter */
double CLJPair::B() const
{
    return double(4.0) * epsilon() * SireMaths::pow_6( sigma() );
}

/** Return the LJ 'rmin' parameter - this is the location of the minimum.

    rmin = 2^(1/6) * sigma
*/
double CLJPair::rmin() const
{
    // 2.0 ^ (1/6) = 1.122462048309372981439932526193103967671
    return sigma() * double(1.122462048309372981439932526193103967671);
}

/** Return a string representation of the CLJPair */
QString CLJPair::toString() const
{
    return QObject::tr("CLJPair( Charge^2=%1, Sigma=%2, Epsilon=%3 )")
                        .arg(charge2()).arg(sigma()).arg(epsilon());
}
