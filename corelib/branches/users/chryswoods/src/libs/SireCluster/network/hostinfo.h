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

#ifndef SIRECLUSTER_NETWORK_HOSTINFO_H
#define SIRECLUSTER_NETWORK_HOSTINFO_H

#include <QUuid>

#include "Siren/object.h"

#include "SireSec/publickey.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{
namespace network
{

using SireSec::PublicKey;

/** A HostInfo object provides information about a host.
    This includes its unique ID (UID), hostname,
    public encryption key (to encrypt messages sent
    to the host) and public signature verification
    key (to verify that a message truly came from
    that host)
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT HostInfo : public Siren::Implements<HostInfo,Siren::Object>
{
public:
    HostInfo();

    HostInfo(const QUuid &uid, const QString &hostname,
             const PublicKey &encrypt_key,
             const PublicKey &sign_key);

    HostInfo(const HostInfo &other);
    
    ~HostInfo();

    HostInfo& operator=(const HostInfo &other);
    
    bool operator==(const HostInfo &other) const;
    bool operator!=(const HostInfo &other) const;

    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);

    bool isNull() const;
    
    const QUuid& UID() const;
    const QString& hostName() const;
    
    const PublicKey& encryptKey() const;
    const PublicKey& signatureKey() const;

private:
    /** Unique ID used to identify the process in the network */
    QUuid uid;
    
    /** Friendly human name of the host (not necessarily unique) */
    QString hostname;
    
    /** Encryption keys used to encrypt messages sent to the host,
        and to verify the signature of message received from the host */
    PublicKey encrypt_key, sign_key;
};

} // end of namespace network
} // end of namespace SireCluster

Q_DECLARE_METATYPE( SireCluster::network::HostInfo )

SIRE_END_HEADER

#endif
