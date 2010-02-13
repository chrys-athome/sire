/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include "publickey.h"
#include "privatekey.h"

#include "Siren/stream.h"
#include "Siren/errors.h"

using namespace SireSec;
using namespace Siren;

static const RegisterObject<PublicKey> r_public_key;

/** Null constructor */
PublicKey::PublicKey() : Implements<PublicKey,Key>()
{}

/** Internal constructor */
PublicKey::PublicKey(const boost::shared_ptr<Crypt::KeyContext> &context)
          : Implements<PublicKey,Key>(), d(context)
{}

/** Copy constructor */
PublicKey::PublicKey(const PublicKey &other)
          : Implements<PublicKey,Key>(other), d(other.d)
{}

/** Destructor */
PublicKey::~PublicKey()
{}

/** Copy assignment operator */
PublicKey& PublicKey::operator=(const PublicKey &other)
{
    d = other.d;
    super::operator=(other);
    return *this;
}

/** Comparison operator */
bool PublicKey::operator==(const PublicKey &other) const
{
    return d.get() == other.d.get() and super::operator==(other);
}

/** Comparison operator */
bool PublicKey::operator!=(const PublicKey &other) const
{
    return not PublicKey::operator==(other);
}

bool PublicKey::isValid() const
{
    return d.get() != 0;
}

QString PublicKey::toString() const
{
    return QObject::tr("PublicKey");
}

uint PublicKey::hashCode() const
{
    return qHash(PublicKey::typeName());
}

void PublicKey::stream(Siren::Stream &s)
{
    throw Siren::incomplete_code("Need to convert to certificate", CODELOC);
}

/** Public keys are always available (that's their purpose!) */
bool PublicKey::availableToThisThread() const
{
    return true;
}
