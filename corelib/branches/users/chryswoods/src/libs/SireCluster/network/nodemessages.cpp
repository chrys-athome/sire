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

#include "nodemessages.h"

#include "Siren/stream.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::network;
using namespace Siren;

////////
//////// Implementation of ReserveRequest
////////

static const RegisterObject<ReserveRequest> r_reserve_request;

/** Null constructor */
ReserveRequest::ReserveRequest() : Implements<ReserveRequest,Message>()
{}

/** Copy constructor */
ReserveRequest::ReserveRequest(const ReserveRequest &other)
               : Implements<ReserveRequest,Message>(other)
{}

/** Destructor */
ReserveRequest::~ReserveRequest()
{}

/** Copy assignment operator */
ReserveRequest& ReserveRequest::operator=(const ReserveRequest &other)
{
    super::operator=(other);
    return *this;
}

/** Comparison operator */
bool ReserveRequest::operator==(const ReserveRequest &other) const
{
    return super::operator==(other);
}

/** Comparison operator */
bool ReserveRequest::operator!=(const ReserveRequest &other) const
{
    return super::operator!=(other);
}

uint ReserveRequest::hashCode() const
{
    return qHash( ReserveRequest::typeName() );
}

QString ReserveRequest::toString() const
{
    return "ReserveRequest()";
}

void ReserveRequest::stream(Siren::Stream &s)
{
    s.assertVersion<ReserveRequest>(1);
    
    Schema schema = s.item<ReserveRequest>();
    
    super::stream( schema.base() );
}

/** Read and act on this message */
void ReserveRequest::read(const QUuid &sender, quint64 message_id) const
{
    qDebug() << "RECEIVE A RESERVEREQUEST FROM" << sender.toString() << message_id;
}
