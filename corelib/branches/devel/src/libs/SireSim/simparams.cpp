/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "simparams.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSim;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<SimParams> r_simparams( MAGIC_ONLY, SimParams::typeName() );

QDataStream SIRESIM_EXPORT &operator<<(QDataStream &ds, const SimParams &params)
{
    writeHeader(ds, r_simparams, 0);

    ds << static_cast<const Property&>(params);
    
    return ds;
}

QDataStream SIRESIM_EXPORT &operator>>(QDataStream &ds, SimParams &params)
{
    VersionID v = readHeader(ds, r_simparams);
    
    if (v == 0)
    {
        ds >> static_cast<Property&>(params);
    }
    else
        throw version_error(v, "1", r_simparams, CODELOC);
        
    return ds;
}

/** Constructor */
SimParams::SimParams() : Property()
{}

/** Copy constructor */
SimParams::SimParams(const SimParams &other) : Property(other)
{}

/** Destructor */
SimParams::~SimParams()
{}

const char* SimParams::typeName()
{
    return "SireSim::SimParams";
}
