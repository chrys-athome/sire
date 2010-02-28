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

#include "message.h"
#include "SireCluster/cluster.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::network;
using namespace Siren;

//////
////// Implementation of Message
//////

static const RegisterObject<Message> r_message( VIRTUAL_CLASS );

/** Constructor */
Message::Message() : Extends<Message,Object>()
{}

/** Copy constructor */
Message::Message(const Message &other) : Extends<Message,Object>(other)
{}

/** Destructor */
Message::~Message()
{}

/** Copy assignment */
Message& Message::operator=(const Message &other)
{
    super::operator=(other);
    return *this;
}

/** Comparison operator */
bool Message::operator==(const Message &other) const
{
    return super::operator==(other);
}

/** Comparison operator */
bool Message::operator!=(const Message &other) const
{
    return super::operator!=(other);
}

QString Message::typeName()
{
    return "SireCluster::network::Message";
}

//////
////// Implementation of Shutdown
//////

static const RegisterObject<Shutdown> r_shutdown;

/** Constructor */
Shutdown::Shutdown() : Implements<Shutdown,Message>()
{}

/** Copy constructor */
Shutdown::Shutdown(const Shutdown &other) : Implements<Shutdown,Message>(other)
{}

/** Destructor */
Shutdown::~Shutdown()
{}

/** Copy assignment operator */
Shutdown& Shutdown::operator=(const Shutdown &other)
{
    super::operator=(other);
    return *this;
}

/** Comparison operator */
bool Shutdown::operator==(const Shutdown &other) const
{
    return super::operator==(other);
}

/** Comparison operator */
bool Shutdown::operator!=(const Shutdown &other) const
{
    return super::operator!=(other);
}

uint Shutdown::hashCode() const
{
    return qHash(Shutdown::typeName());
}

QString Shutdown::toString() const
{
    return "Shutdown()";
}

void Shutdown::stream(Siren::Stream &s)
{
    s.assertVersion<Shutdown>(1);
    
    Schema schema = s.item<Shutdown>();
    
    super::stream(schema.base());
}

/** This just calls Cluster::shutdown() */
void Shutdown::read(const QUuid &sender, quint64 msg_id) const
{
    Cluster::shutdown();
}
