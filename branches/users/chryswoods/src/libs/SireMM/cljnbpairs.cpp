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

#include "cljnbpairs.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireMol;

static const RegisterMetaType<CLJNBPairs> r_cljnbpairs;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJNBPairs &cljnbpairs)
{
    writeHeader(ds, r_cljnbpairs, 1)
        << static_cast<const AtomPairs<CLJFactor>&>(cljnbpairs);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJNBPairs &cljnbpairs)
{
    VersionID v = readHeader(ds, r_cljnbpairs);

    if (v == 1)
    {
        ds >> static_cast<AtomPairs<CLJFactor>&>(cljnbpairs);
    }
    else
        throw version_error(v, "1", r_cljnbpairs, CODELOC);

    return ds;
}

/** Null constructor */
CLJNBPairs::CLJNBPairs() : AtomPairs<CLJFactor>( CLJFactor(1,1) )
{}

/** Construct, using 'default_scale' for all of the atom-atom
    interactions in the molecule 'molinfo' */
CLJNBPairs::CLJNBPairs(const MoleculeInfo &molinfo, const CLJFactor &default_scale)
           : AtomPairs<CLJFactor>(molinfo, default_scale)
{}

/** Copy constructor */
CLJNBPairs::CLJNBPairs(const CLJNBPairs &other)
           : AtomPairs<CLJFactor>(other)
{}

/** Destructor */
CLJNBPairs::~CLJNBPairs()
{}

/** Copy assignment operator */
CLJNBPairs& CLJNBPairs::operator=(const CLJNBPairs &other)
{
    AtomPairs<CLJFactor>::operator=(other);
    return *this;
}
