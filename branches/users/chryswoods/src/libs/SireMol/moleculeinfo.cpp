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

#include <QSharedData>

#include "moleculeinfo.h"
#include "moleculeinfodata.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<SireMol::MoleculeInfo> r_molinfo;

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeInfo &molinfo)
{
    writeHeader(ds, r_molinfo, 1);

    SharedDataStream sds(ds);

    sds << molinfo.d;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeInfo &molinfo)
{
    VersionID v = readHeader(ds, r_molinfo);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> molinfo.d;
    }
    else
        throw version_error(v, "1", r_molinfo, CODELOC);

    return ds;
}

/** Null constructor */
MoleculeInfo::MoleculeInfo() : d( MoleculeInfoData::null() )
{}

/** Copy constructor - fast as this class is implicitly shared */
MoleculeInfo::MoleculeInfo(const MoleculeInfo &other) : d(other.d)
{}

/** Destructor */
MoleculeInfo::~MoleculeInfo()
{}

/** Assignment operator - fast as this class is implicitly shared */
MoleculeInfo& MoleculeInfo::operator=(const MoleculeInfo &other)
{
    d = other.d;
    return *this;
}
