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

#include "qhash_siremm.h"
#include "SireMol/qhash_siremol.h"

#include "resangleinfo.h"
#include "molangleinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;

static const RegisterMetaType<ResAngleInfo> r_resanginfo;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const ResAngleInfo &info)
{
    writeHeader(ds, r_resanginfo, 1)
        << static_cast<const ResInternalInfo<Angle>&>(info);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, ResAngleInfo &info)
{
    VersionID v = readHeader(ds, r_resanginfo);

    if (v == 1)
    {
        ds >> static_cast<ResInternalInfo<Angle>&>(info);
    }
    else
        throw version_error(v, "1", r_resanginfo, CODELOC);

    return ds;
}

/** Empty constructor */
ResAngleInfo::ResAngleInfo() : ResInternalInfo<Angle>()
{}

/** Construct for residue 'resnum' in the passed molecule */
ResAngleInfo::ResAngleInfo(const MolAngleInfo &molinfo, ResNum resnum)
             : ResInternalInfo<Angle>( molinfo.residue(resnum) )
{}

/** Copy constructor */
ResAngleInfo::ResAngleInfo(const ResInternalInfo<Angle> &other)
             : ResInternalInfo<Angle>(other)
{}

/** Destructor */
ResAngleInfo::~ResAngleInfo()
{}

/** Return the number of angles in the residue */
int ResAngleInfo::nAngles() const
{
    return nInternals();
}

/** Return the number of intra-residue angles in the residue */
int ResAngleInfo::nIntraAngles() const
{
    return nIntraInternals();
}

/** Return the number of inter-residue angles in the residue */
int ResAngleInfo::nInterAngles() const
{
    return nInterInternals();
}

/** Return an iterator over all of the angles in the residue.

    This returns an iterator pointing to the first angle, or an
    invalid iterator if there are no angles to iterate over.
*/
ResAngleInfo::const_iterator ResAngleInfo::angles() const
{
    return internals();
}

/** Return an iterator over all of the intra-residue angles in the residue.

    This returns an iterator pointing to the first angle, or an
    invalid iterator if there are no angles to iterate over.
*/
ResAngleInfo::const_iterator ResAngleInfo::intraAngles() const
{
    return intraInternals();
}

/** Return an iterator over all of the inter-residue angles in the residue.

    This returns an iterator pointing to the first angle, or an
    invalid iterator if there are no angles to iterate over.
*/
ResAngleInfo::const_iterator ResAngleInfo::interAngles() const
{
    return interInternals();
}
