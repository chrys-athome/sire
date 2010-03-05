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

#ifndef SIRECLUSTER_NETWORK_NODEMESSAGES_H
#define SIRECLUSTER_NETWORK_NODEMESSAGES_H

#include "message.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{
namespace network
{

class ReserveRequest : public Siren::Implements<ReserveRequest,Message>
{
public:
    ReserveRequest();
    
    ReserveRequest(const ReserveRequest &other);
    
    ~ReserveRequest();
    
    ReserveRequest& operator=(const ReserveRequest &other);
    
    bool operator==(const ReserveRequest &other) const;
    bool operator!=(const ReserveRequest &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    void read(const QUuid &sender, quint64 message_id) const;
};


} // end of namespace network
} // end of namespace SireCluster

Q_DECLARE_METATYPE( SireCluster::network::ReserveRequest )

SIRE_END_HEADER

#endif
