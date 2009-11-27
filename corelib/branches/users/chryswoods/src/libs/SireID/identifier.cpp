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

#include "identifier.h"

#include "Siren/stream.h"

using namespace SireID;
using namespace Siren;

static const RegisterObject<Identifier> r_id;

/** Null constructor */
Identifier::Identifier() : Implements<Identifier,ID>()
{}

/** Construct from the passed ID */
Identifier::Identifier(const ID &id) : Implements<Identifier,ID>()
{
    if (id.isA<Identifier>())
        d = id.asA<Identifier>().d;
    else if (not id.isNull())
        d = id;
}

/** Copy constructor */
Identifier::Identifier(const Identifier &other)
           : Implements<Identifier,ID>(other), d(other.d)
{}

/** Destructor */
Identifier::~Identifier()
{}

/** Is this ID null? */
bool Identifier::isNull() const
{
    return d.constData() == 0;
}

/** Return a hash of this identifier */
uint Identifier::hashCode() const
{
    if (d.constData() == 0)
        return 0;
    else
        return d->hashCode();
}
            
/** Return a string representatio of this ID */
QString Identifier::toString() const
{
    if (d.constData() == 0)
        return "null";
    else
        return d->toString();
}

/** Return the base type of this ID */
const ID& Identifier::base() const
{
    if (d.constData() == 0)
        return *this;
    else
        return *d;
}

/** Copy assignment operator */
Identifier& Identifier::operator=(const Identifier &other)
{
    d = other.d;
    return *this;
}

/** Copy assignment operator */
Identifier& Identifier::operator=(const ID &other)
{
    if (other.isA<Identifier>())
        d = other.asA<Identifier>().d;
    else if (other.isNull())
        d = 0;
    else
        d = other;
    
    return *this;
}

/** Comparison operator */
bool Identifier::operator==(const ID &other) const
{
    if (this->isNull())
        return false;
    else
        return *d == other;
}

/** Comparison operator */
bool Identifier::operator!=(const ID &other) const
{
    return Identifier::operator!=(other);
}

/** Comparison operator */
bool Identifier::operator==(const Identifier &other) const
{
    return Identifier::operator==(other.base());
}

/** Comparison operator */
bool Identifier::operator!=(const Identifier &other) const
{
    return not Identifier::operator==(other);
}
