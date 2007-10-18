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

#include "resbondinfo.h"
#include "molbondinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireMol;

static const RegisterMetaType<ResBondInfo> r_resbondinfo;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const ResBondInfo &info)
{
    writeHeader(ds, r_resbondinfo, 1)
          << static_cast<const ResInternalInfo<Bond>&>(info);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, ResBondInfo &info)
{
    VersionID v = readHeader(ds, r_resbondinfo);

    if (v == 1)
    {
        ds >> static_cast<ResInternalInfo<Bond>&>(info);
    }
    else
        throw version_error(v, "1", r_resbondinfo, CODELOC);

    return ds;
}

/** Empty constructor */
ResBondInfo::ResBondInfo() : ResInternalInfo<Bond>()
{}

/** Construct from residue 'resnum' from 'molinfo' - this creates
    an empty ResBondInfo if there are no bonds for this residue
    in 'molinfo' */
ResBondInfo::ResBondInfo(const MolBondInfo &molinfo, ResNum resnum)
            : ResInternalInfo<Bond>( molinfo.residue(resnum) )
{}

/** Copy constructor */
ResBondInfo::ResBondInfo(const ResInternalInfo<Bond> &other)
            : ResInternalInfo<Bond>(other)
{}

/** Destructor */
ResBondInfo::~ResBondInfo()
{}

/** Return the number of bonds involving this residue */
int ResBondInfo::nBonds() const
{
    return nInternals();
}

/** Return the number of intra-residue bonds in this residue */
int ResBondInfo::nIntraBonds() const
{
    return nIntraInternals();
}

/** Return the number of inter-residue bonds to this residue */
int ResBondInfo::nInterBonds() const
{
    return nInterInternals();
}

/** Return an iterator over all of the bonds in this residue */
ResBondInfo::const_iterator ResBondInfo::bonds() const
{
    return internals();
}

/** Return an iterator over all of the intra-residue bond in
    this residue */
ResBondInfo::const_iterator ResBondInfo::intraBonds() const
{
    return intraInternals();
}

/** Return an iterator over all of the inter-residue bonds
    to this residue */
ResBondInfo::const_iterator ResBondInfo::interBonds() const
{
    return interInternals();
}
