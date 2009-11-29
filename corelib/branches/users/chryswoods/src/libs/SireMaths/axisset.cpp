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

#include "axisset.h"

#include "Siren/stream.h"

using namespace SireMaths;
using namespace Siren;

static const RegisterPrimitive<AxisSet> r_axisset;

namespace Siren{ template class PrimitiveObject<AxisSet>; }

/** Construct an empty AxisSet. This represents the standard cartesian axes, centered
    on the origin */
AxisSet::AxisSet() : Primitive<AxisSet>(), mat(), invmat(), orgn()
{}

/** Construct an AxisSet using matrix 'matrx', and origin 'orign' */
AxisSet::AxisSet(const Matrix &matrx, Vector vec)
        : Primitive<AxisSet>(), mat(matrx), invmat(mat.inverse()), orgn(vec)
{}

/** Copy constructor */
AxisSet::AxisSet(const AxisSet &other)
        : Primitive<AxisSet>(), mat(other.mat), invmat(other.invmat), orgn(other.orgn)
{}

/** Destructor */
AxisSet::~AxisSet()
{}

/** Copy assignment operator */
AxisSet& AxisSet::operator=(const AxisSet &other)
{
    if (this != &other)
    {
        mat = other.mat;
        invmat = other.invmat;
        orgn = other.orgn;
    }
    
    return *this;
}

/** Comparison operator */
bool AxisSet::operator==(const AxisSet &other) const
{
    return orgn == other.orgn and mat == other.mat;
}

/** Convert a vector from the cartesian frame to this coordinate frame */
Vector AxisSet::fromIdentity(const Vector &vec) const
{
    #warning Is this right? What about origins?

    return (mat * vec) + orgn;
}

/** Convert a vector to the cartesian frame from this coordinate frame */
Vector AxisSet::toIdentity(const Vector &vec) const
{
    return invmat * (vec - orgn);
}

/** Convert a vector from the frame 'frame' to this coordinate frame */
Vector AxisSet::fromFrame(const AxisSet &frame, const Vector &vec) const
{
    return ( mat *  (frame.invmat * (vec - frame.orgn)) ) + orgn;
}

/** Convert a vector to the frame 'frame' from this coordinate frame */
Vector AxisSet::toFrame(const AxisSet &frame, const Vector &vec) const
{
    return ( frame.mat * (invmat * (vec - orgn)) ) + frame.orgn;
}

/** Return a string representation of the AxisSet */
QString AxisSet::toString() const
{
    return QObject::tr("AxisSet, %1, %2").arg(orgn.toString(),mat.toString());
}

uint AxisSet::hashCode() const
{
    return qHash(mat) + qHash(orgn);
}

void AxisSet::stream(Stream &s)
{
    s.assertVersion<AxisSet>(1);
    
    Schema schema = s.item<AxisSet>();
    
    schema.data("origin") & orgn;
    schema.data("axes") & mat;
    
    if (s.isLoading())
        invmat = mat.inverse();
}
