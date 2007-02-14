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

#include "moleculeversion.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MoleculeVersion> r_molversion;

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeVersion &molver)
{
    writeHeader(ds, r_molversion, 1) << molver.majversion << molver.minversion;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeVersion &molver)
{
    VersionID v = readHeader(ds, r_molversion);

    if (v == 1)
    {
        ds >> molver.majversion >> molver.minversion;
    }
    else
        throw version_error(v, "1", r_molversion, CODELOC);

    return ds;
}

/** Empty constructor - version = 0:0 */
MoleculeVersion::MoleculeVersion() : majversion(0), minversion(0)
{}

/** Construct a specific version */
MoleculeVersion::MoleculeVersion(quint32 major, quint32 minor)
                : majversion(major), minversion(minor)
{}

/** Copy constructor */
MoleculeVersion::MoleculeVersion(const MoleculeVersion &other)
                : majversion(other.majversion), minversion(other.minversion)
{}

/** Destructor */
MoleculeVersion::~MoleculeVersion()
{}

/** Assignment operator */
MoleculeVersion& MoleculeVersion::operator=(const MoleculeVersion &other)
{
    majversion = other.majversion;
    minversion = other.minversion;

    return *this;
}

/** Return a string representation of this version */
QString MoleculeVersion::toString() const
{
    return QString("%1.%2").arg(majversion).arg(minversion);
}
