/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2013  Christopher Woods
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

#include "cpuid.h"

#ifdef SIRE_FOUND_CPUID
    #include <libcpuid/libcpuid.h> // CONDITIONAL_INCLUDE
#endif

#include "SireStream/shareddatastream.h"
#include "SireError/errors.h"

#include "tostring.h"

using namespace SireBase;
using namespace SireStream;
using namespace SireError;

static const RegisterMetaType<CPUID> r_cpuid;

QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds, const CPUID &cpuid)
{
    writeHeader(ds, r_cpuid, 1);
    
    SharedDataStream sds(ds);
    sds << cpuid.props;
    return ds;
}

QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, CPUID &cpuid)
{
    VersionID v = readHeader(ds, r_cpuid);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> cpuid.props;
    }
    else
        throw SireStream::version_error(v, "1", r_cpuid, CODELOC);
    
    return ds;
}

/** Constructor */
CPUID::CPUID() : ConcreteProperty<CPUID,Property>()
{}

/** Copy constructor */
CPUID::CPUID(const CPUID &other) : ConcreteProperty<CPUID,Property>(), props(other.props)
{}

/** Destructor */
CPUID::~CPUID()
{}

/** Copy assignment operator */
CPUID& CPUID::operator=(const CPUID &other)
{
    props = other.props;
    return *this;
}

/** Comparison operator */
bool CPUID::operator==(const CPUID &other) const
{
    return props == other.props;
}

/** Comparison operator */
bool CPUID::operator!=(const CPUID &other) const
{
    return not CPUID::operator==(other);
}

CPUID* CPUID::clone() const
{
    return new CPUID(*this);
}

const char* CPUID::what() const
{
    return CPUID::typeName();
}

const char* CPUID::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CPUID>() );
}

QString CPUID::toString() const
{
    return QObject::tr( "CPUID( %1 )" ).arg( Sire::toString(props) );
}
