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

#include <QFile>
#include <QList>

#include "restartpacket.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/streamdata.hpp"

#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>

#include <QDebug>

using namespace Sire;
using namespace SireCluster;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<RestartPacket> r_restartpacket;

static QByteArray readFile(const QString &file)
{
    //read the contents of the restart file into memory
    QFile f(file);

    if (not f.open( QIODevice::ReadOnly) )
        throw SireError::file_error(f, CODELOC);
    
    QByteArray restart_data = f.readAll();

    if (restart_data.isEmpty())
        throw SireError::file_error( QObject::tr(
            "There was an error reading data from the file %1. Either "
            "the file is empty, or some read error has occured.")
                .arg(file), CODELOC );

    return restart_data;
}

/** Serialise to a binary datastream */
QDataStream SIRE_EXPORT &operator<<(QDataStream &ds, const RestartPacket &packet)
{
    writeHeader(ds, r_restartpacket, 1);
    
    //we need to serialise the binary restart file itself, so that
    //we can save it for later or beam it across the network
    if (packet.restart_data.isEmpty())
    {
        if (packet.restart_file.isEmpty())
            ds << QByteArray();
        else
        {
            QByteArray data = ::readFile(packet.restart_file);
            
            ds << data;
        }
    }
    else
        ds << packet.restart_data;

    ds << static_cast<const WorkPacketBase&>(packet);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds, RestartPacket &packet)
{
    VersionID v = readHeader(ds, r_restartpacket);
    
    if (v == 1)
    {
        ds >> packet.restart_data >> static_cast<WorkPacketBase&>(packet);
        
        packet.restart_file = QString::null;
    }
    else
        throw version_error( v, "1", r_restartpacket, CODELOC );
        
    return ds;
}

/** Constructor */
RestartPacket::RestartPacket() : WorkPacketBase()
{}

/** Construct to run the simulation in the restart file 'restart_file' */
RestartPacket::RestartPacket(const QString &f)
              : WorkPacketBase(), restart_file(f)
{}

/** Construct to run the simulation in the restart data 'restart_data' */
RestartPacket::RestartPacket(const QByteArray &data)
              : WorkPacketBase(), restart_data(data)
{}

/** Copy constructor */
RestartPacket::RestartPacket(const RestartPacket &other)
              : WorkPacketBase(other), restart_file(other.restart_file),
                                       restart_data(other.restart_data)
{}

/** Destructor */
RestartPacket::~RestartPacket()
{}

const char* RestartPacket::typeName()
{
    return QMetaType::typeName( qMetaTypeId<RestartPacket>() );
}

const char* RestartPacket::what() const
{
    return RestartPacket::typeName();
}

RestartPacket* RestartPacket::clone() const
{
    return new RestartPacket(*this);
}

/** Copy assignment operator */
RestartPacket& RestartPacket::operator=(const RestartPacket &other)
{
    if (this != &other)
    {
        restart_file = other.restart_file;
        restart_data = other.restart_data;
        WorkPacketBase::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool RestartPacket::operator==(const RestartPacket &other) const
{
    return restart_file == other.restart_file and
           restart_data == other.restart_data;
}

/** Comparison operator */
bool RestartPacket::operator!=(const RestartPacket &other) const
{
    return not RestartPacket::operator==(other);
}

/** Return whether or not this workpacket has finished */
bool RestartPacket::hasFinished() const
{
    return false;
}

/** Run a chunk of the restart simulation */
float RestartPacket::chunk()
{
    if (restart_data.isEmpty())
    {
        if (restart_file.isEmpty())
            return 1.0;

        restart_data = ::readFile(restart_file);
    }
    
    //unpack binary data
    QList< boost::tuple<boost::shared_ptr<void>,QString> > objects 
                         = SireStream::load(restart_data);
    
    //what do we have?
    qDebug() << "We have loaded " << objects.count() << " objects";

    for (int i=0; i<objects.count(); ++i)
    {
        qDebug() << "Object" << i << "is a" << objects[i].get<1>();
    }

    return 1.0;
}
