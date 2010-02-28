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

#include "hostinfo.h"

#include "Siren/stream.h"
#include "Siren/streamqt.h"

using namespace SireCluster;
using namespace SireCluster::network;
using namespace SireSec;
using namespace Siren;

static const RegisterObject<HostInfo> r_hostinfo;

/** Null constructor */
HostInfo::HostInfo() : Implements<HostInfo,Object>()
{}

/** Construct specifying the host's unique ID, hostname, and
    public encryption and signature keys */
HostInfo::HostInfo(const QUuid &u,
                   const PublicKey &encrypt, const PublicKey &sign)
         : Implements<HostInfo,Object>(),
           uid(u), encrypt_key(encrypt), sign_key(sign)
{
    if (uid.isNull())
        this->operator=( HostInfo() );
}

/** Copy constructor */
HostInfo::HostInfo(const HostInfo &other)
         : Implements<HostInfo,Object>(other),
           uid(other.uid), encrypt_key(other.encrypt_key), 
           sign_key(other.sign_key)
{}

/** Destructor */
HostInfo::~HostInfo()
{}

/** Copy assignment operator */
HostInfo& HostInfo::operator=(const HostInfo &other)
{
    if (this != &other)
    {
        uid = other.uid;
        encrypt_key = other.encrypt_key;
        sign_key = other.sign_key;
        super::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool HostInfo::operator==(const HostInfo &other) const
{
    return uid == other.uid;
}

/** Comparison operator */
bool HostInfo::operator!=(const HostInfo &other) const
{
    return not HostInfo::operator==(other);
}

uint HostInfo::hashCode() const
{
    return qHash(HostInfo::typeName()) + qHash(uid);
}

QString HostInfo::toString() const
{
    if (uid.isNull())
        return QObject::tr("HostInfo::null");
    else
        return QString("HostInfo( UID = %1 )").arg(uid.toString());
}

void HostInfo::stream(Siren::Stream &s)
{
    s.assertVersion<HostInfo>(1);
    
    Schema schema = s.item<HostInfo>();
    
    schema.data("UID") & uid;
    schema.data("encrypt_key") & encrypt_key;
    schema.data("sign_key") & sign_key;
    
    super::stream( schema.base() );
}

/** Return whether or not this is null */
bool HostInfo::isNull() const
{
    return uid.isNull();
}

/** Return the unique ID for the host */
const QUuid& HostInfo::UID() const
{
    return uid;
}

/** Return the public key used to encrypt data to be sent to
    this host */
const PublicKey& HostInfo::encryptKey() const
{
    return encrypt_key;
}

/** Return the public key used to verify the signature on
    messages sent by this host */
const PublicKey& HostInfo::signatureKey() const
{
    return sign_key;
}
