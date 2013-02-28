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

#include "titrator.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<Titrator> r_titrator;

QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Titrator &titrator)
{
    writeHeader(ds, r_titrator, 1);
    
    ds << static_cast<const Property&>(titrator);
    
    return ds;
}

QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Titrator &titrator)
{
    VersionID v = readHeader(ds, r_titrator);
    
    if (v == 1)
    {
        ds >> static_cast<Property&>(titrator);
    }
    else
        throw version_error(v, "1", r_titrator, CODELOC);
    
    return ds;
}

/** Constructor */
Titrator::Titrator() : ConcreteProperty<Titrator,Property>()
{}

/** Copy constructor */
Titrator::Titrator(const Titrator &other)
         : ConcreteProperty<Titrator,Property>(other)
{}

/** Destructor */
Titrator::~Titrator()
{}

const char* Titrator::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Titrator>() );
}

const char* Titrator::what() const
{
    return Titrator::typeName();
}

/** Copy assignment operator */
Titrator& Titrator::operator=(const Titrator &other)
{
    if (this != &other)
    {
        Property::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Titrator::operator==(const Titrator &other) const
{
    return Property::operator==(other);
}

/** Comparison operator */
bool Titrator::operator!=(const Titrator &other) const
{
    return not Titrator::operator==(other);
}
