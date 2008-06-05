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
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<CLJNBPairs> r_cljnbpairs;

////////
//////// Fully instantiate the template class
////////

template class AtomPairs<CLJScaleFactor>;
template class CGAtomPairs<CLJScaleFactor>;

////////
//////// Implementation of CLJScaleFactor
////////

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJScaleFactor &sclfac)
{
    ds << sclfac.cscl << sclfac.ljscl;
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJScaleFactor &sclfac)
{
    ds >> sclfac.cscl >> sclfac.ljscl;
    return ds;
}

////////
//////// Implementation of CLJNBPairs
////////

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJNBPairs &cljnbpairs)
{
    writeHeader(ds, r_cljnbpairs, 1)
        << static_cast<const AtomPairs<CLJScaleFactor>&>(cljnbpairs);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJNBPairs &cljnbpairs)
{
    VersionID v = readHeader(ds, r_cljnbpairs);

    if (v == 1)
    {
        ds >> static_cast<AtomPairs<CLJScaleFactor>&>(cljnbpairs);
    }
    else
        throw version_error(v, "1", r_cljnbpairs, CODELOC);

    return ds;
}

/** Null constructor */
CLJNBPairs::CLJNBPairs() : ConcreteProperty<CLJNBPairs,
                               AtomPairs<CLJScaleFactor> >( CLJScaleFactor(1,1) )
{}

/** Construct, using 'default_scale' for all of the atom-atom
    interactions in the molecule 'molinfo' */
CLJNBPairs::CLJNBPairs(const MoleculeInfoData &molinfo, 
                       const CLJScaleFactor &default_scale)
           : ConcreteProperty<CLJNBPairs,
                   AtomPairs<CLJScaleFactor> >(molinfo, default_scale)
{}

/** Copy constructor */
CLJNBPairs::CLJNBPairs(const CLJNBPairs &other)
           : ConcreteProperty< CLJNBPairs, AtomPairs<CLJScaleFactor> >(other)
{}

/** Destructor */
CLJNBPairs::~CLJNBPairs()
{}

/** Copy assignment operator */
CLJNBPairs& CLJNBPairs::operator=(const CLJNBPairs &other)
{
    AtomPairs<CLJScaleFactor>::operator=(other);
    return *this;
}

/** Comparison operator */
bool CLJNBPairs::operator==(const CLJNBPairs &other) const
{
    return AtomPairs<CLJScaleFactor>::operator==(other);
}

/** Comparison operator */
bool CLJNBPairs::operator!=(const CLJNBPairs &other) const
{
    return AtomPairs<CLJScaleFactor>::operator!=(other);
}
