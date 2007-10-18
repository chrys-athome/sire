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

#include "atomindex.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<AtomIndex> r_atomindex;

/** Function to serialise an AtomIndex to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const SireMol::AtomIndex &atm)
{
    writeHeader(ds, r_atomindex, 1) << atm.nme << atm.resnum;

    return ds;
}

/** Function to deserialise an AtomIndex from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, SireMol::AtomIndex &atm)
{
    VersionID v = readHeader(ds, r_atomindex);

    if (v == 1)
    {
        ds >> atm.nme >> atm.resnum;
    }
    else
        throw version_error(v, "1", r_atomindex, CODELOC);

    return ds;
}

/** Function to construct an AtomIndex from a boost tuple of the name and residue number */
AtomIndex::AtomIndex( const tuple<QString,ResNum> &tuple )
          : nme( tuple.get<0>() ), resnum( tuple.get<1>() )
{}

/** Return a string representation of the AtomIndex */
QString AtomIndex::toString() const
{
    return QString("(\"%1\",%2)").arg(nme,resnum.toString());
}
