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

#include "privatekey.h"
#include "publickey.h"
#include "pubprilock.h"

#include "SireSec/errors.h"

#include "Siren/errors.h"
#include "Siren/stream.h"

using namespace SireSec;
using namespace Siren;

namespace SireSec
{
    namespace detail
    {
        class PrivateKeyData
        {
        public:
            PrivateKeyData()
            {}
            
            ~PrivateKeyData()
            {}
        };
    }
}

static const RegisterObject<PrivateKey> r_private_key;

/** Null Constructor */
PrivateKey::PrivateKey() : Implements<PrivateKey,Key>( Key::NonStreamable )
{}

/** Copy constructor */
PrivateKey::PrivateKey(const PrivateKey &other) : Implements<PrivateKey,Key>(other)
{}

/** Destructor */
PrivateKey::~PrivateKey()
{}

/** Copy assignment operator */
PrivateKey& PrivateKey::operator=(const PrivateKey &other)
{
    d = other.d;
    super::operator=(other);
    return *this;
}

/** Comparison operator */
bool PrivateKey::operator==(const PrivateKey &other) const
{
    return d.get() == other.d.get() and super::operator==(other);
}

/** Comparison operator */
bool PrivateKey::operator!=(const PrivateKey &other) const
{
    return not PrivateKey::operator==(other);
}

//QPair<PublicKey,PrivateKey> PrivateKey::generate()
//QPair<PublicKey,PrivateKey> PrivateKey::generate(const QDateTime &expiry)

QString PrivateKey::toString() const
{
    if (d.get() == 0)
        return QObject::tr("PrivateKey::null");

    else if (availableToThisThread())
    {
        if (isTemporal())
            return QObject::tr("PrivateKey( available - expires %1 )")
                            .arg(this->bestBefore().toString());
        else
            return QObject::tr("PrivateKey( available )");
    }
    else
        return QObject::tr("PrivateKey( unavailable to this thread )");
}

uint PrivateKey::hashCode() const
{
    return qHash( PrivateKey::typeName() );
}

void PrivateKey::stream(Siren::Stream &s)
{
    this->assertIsStreamable();
}
