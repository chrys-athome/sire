/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "version.h"

using namespace SireBase;
using namespace Siren;

static const RegisterPrimitive<Version> r_version;

/** Constructor */
Version::Version() : Primitive<Version>(), maj(0), min(0)
{}

/** Constructor */
Version::Version(quint64 major, quint64 minor)
        : Primitive<Version>(), maj(major), min(minor)
{}

/** Copy constructor */
Version::Version(const Version &other)
        : Primitive<Version>(), maj(other.maj), min(other.min)
{}

/** Destructor */
Version::~Version()
{}

/** Return a string representation of this version number */
QString Version::toString() const
{
    return QString("%1.%2").arg(maj).arg(min);
}

uint Version::hashCode() const
{
    return qHash( Version::typeName() ) 
              + qHash(maj) + qHash(min);
}

void Version::stream(Stream &s)
{
    s.assertVersion<Version>(1);
    
    Schema schema = s.item<Version>();
    
    schema.data("major") & maj;
    schema.data("minor") & min;
}
