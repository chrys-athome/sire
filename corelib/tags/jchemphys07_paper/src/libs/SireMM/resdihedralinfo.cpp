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

#include "moldihedralinfo.h"
#include "resdihedralinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;

static const RegisterMetaType<ResDihedralInfo> r_resdihinfo;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const ResDihedralInfo &info)
{
    writeHeader(ds, r_resdihinfo, 1)
          << static_cast<const ResInternalInfo<Dihedral>&>(info);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, ResDihedralInfo &info)
{
    VersionID v = readHeader(ds, r_resdihinfo);

    if (v == 1)
    {
        ds >> static_cast<ResInternalInfo<Dihedral>&>(info);
    }
    else
        throw version_error(v, "1", r_resdihinfo, CODELOC);

    return ds;
}

/** Empty constructor */
ResDihedralInfo::ResDihedralInfo() : ResInternalInfo<Dihedral>()
{}

/** Construct for residue 'resnum' in the passed molecule */
ResDihedralInfo::ResDihedralInfo(const MolDihedralInfo &molinfo, ResNum resnum)
                : ResInternalInfo<Dihedral>( molinfo.residue(resnum) )
{}

/** Copy constructor */
ResDihedralInfo::ResDihedralInfo(const ResInternalInfo<Dihedral> &other)
                : ResInternalInfo<Dihedral>(other)
{}

/** Destructor */
ResDihedralInfo::~ResDihedralInfo()
{}

/** Return the number of dihedrals in the residue */
int ResDihedralInfo::nDihedrals() const
{
    return nInternals();
}

/** Return the number of intra-residue dihedrals in the residue */
int ResDihedralInfo::nIntraDihedrals() const
{
    return nIntraInternals();
}

/** Return the number of inter-residue dihedrals in the residue */
int ResDihedralInfo::nInterDihedrals() const
{
    return nInterInternals();
}

/** Return an iterator over all of the dihedrals in the residue.

    This returns an iterator pointing to the first dihedral, or an
    invalid iterator if there are no dihedrals to iterate over.
*/
ResDihedralInfo::const_iterator ResDihedralInfo::dihedrals() const
{
    return internals();
}

/** Return an iterator over all of the intra-residue dihedrals in the residue.

    This returns an iterator pointing to the first dihedral, or an
    invalid iterator if there are no dihedrals to iterate over.
*/
ResDihedralInfo::const_iterator ResDihedralInfo::intraDihedrals() const
{
    return intraInternals();
}

/** Return an iterator over all of the inter-residue dihedrals in the residue.

    This returns an iterator pointing to the first dihedral, or an
    invalid iterator if there are no dihedrals to iterate over.
*/
ResDihedralInfo::const_iterator ResDihedralInfo::interDihedrals() const
{
    return interInternals();
}
