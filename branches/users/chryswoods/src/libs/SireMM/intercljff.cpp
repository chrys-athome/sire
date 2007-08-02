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

#include "intercljff.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireStream;

static const RegisterMetaType<InterCLJFF> r_intercljff;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const InterCLJFF &intercljff)
{
    writeHeader(ds, r_intercljff, 1);

    SharedDataStream sds(ds);

    sds << static_cast<const Inter2BodyFF<CLJFF>&>(intercljff);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      InterCLJFF &intercljff)
{
    VersionID v = readHeader(ds, r_intercljff);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> static_cast<Inter2BodyFF<CLJFF>&>(intercljff);
    }
    else
        throw version_error(v, "1", r_intercljff, CODELOC);

    return ds;
}

/** Constructor */
InterCLJFF::InterCLJFF() : Inter2BodyFF<CLJFF>()
{}

/** Construct a CLJ forcefield using the passed Space, combining rules and
    switching function */
InterCLJFF::InterCLJFF(const Space &space, const SwitchingFunction &switchfunc)
           : Inter2BodyFF<CLJFF>(space, switchfunc)
{}

/** Copy constructor */
InterCLJFF::InterCLJFF(const InterCLJFF &other)
           : Inter2BodyFF<CLJFF>(other)
{}

/** Destructor */
InterCLJFF::~InterCLJFF()
{}
