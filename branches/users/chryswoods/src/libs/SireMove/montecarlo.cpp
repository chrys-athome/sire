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

#include "montecarlo.h"

#include "SireStream/datastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<MonteCarlo> r_mc(MAGIC_ONLY, "SireMove::MonteCarlo");

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const MonteCarlo &mc)
{
    writeHeader(ds, r_mc, 1) 
          << mc.rangen << static_cast<const MoveBase&>(mc);
          
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MonteCarlo &mc)
{
    VersionID v = readHeader(ds, r_mc);
    
    if (v == 1)
    {
        ds >> mc.rangen >> static_cast<MoveBase&>(mc);
    }
    else
        throw version_error(v, "1", r_mc, CODELOC);
    
    return ds;
}

/** Construct using the global random number generator */
MonteCarlo::MonteCarlo() : MoveBase()
{}

/** Construct using the supplied random number generator */
MonteCarlo::MonteCarlo(const RanGenerator &generator)
           : MoveBase(), rangen(generator)
{}

/** Copy constructor */
MonteCarlo::MonteCarlo(const MonteCarlo &other)
           : MoveBase(other), rangen(other.rangen)
{}

/** Destructor */
MonteCarlo::~MonteCarlo()
{}

/** Copy assignment */
MonteCarlo& MonteCarlo::operator=(const MonteCarlo &other)
{
    rangen = other.rangen;
    MoveBase::operator=(other);
    
    return *this;
}

/** Set the random number generator to use for these moves */
void MonteCarlo::setGenerator(const RanGenerator &generator)
{
    rangen = generator;
}

/** Return the random number generator used for these moves */
const RanGenerator& MonteCarlo::generator() const
{
    return rangen;
}
