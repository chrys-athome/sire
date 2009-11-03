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

#ifndef SIRE_RESTARTPACKET_H
#define SIRE_RESTARTPACKET_H

#include <QString>
#include <QByteArray>

#include "SireCluster/workpacket.h"

SIRE_BEGIN_HEADER

namespace Sire
{
class RestartPacket;
}

QDataStream& operator<<(QDataStream&, const Sire::RestartPacket&);
QDataStream& operator>>(QDataStream&, Sire::RestartPacket&);

namespace Sire
{

/** This is a workpacket that contains a simulation to be 
    performed. This workpacket performs that simulation
    
    @author Christopher Woods
*/
class SIRE_EXPORT RestartPacket : public SireCluster::WorkPacketBase
{

friend QDataStream& ::operator<<(QDataStream&, const RestartPacket&);
friend QDataStream& ::operator>>(QDataStream&, RestartPacket&);

public:
    RestartPacket();
    
    RestartPacket(const QString &restart_file);
    RestartPacket(const QByteArray &restart_data);
    
    RestartPacket(const RestartPacket &other);
    
    ~RestartPacket();
    
    static const char* typeName();
    
    const char* what() const;
    
    RestartPacket& operator=(const RestartPacket &other);
    
    bool operator==(const RestartPacket &other) const;
    bool operator!=(const RestartPacket &other) const;

    RestartPacket* clone() const;

    bool hasFinished() const;

protected:
    float chunk();
    
private:
    /** The name of the file containing the restart data */
    QString restart_file;
    
    /** The actual binary restart data */
    QByteArray restart_data;
};

}

Q_DECLARE_METATYPE( Sire::RestartPacket )

SIRE_END_HEADER

#endif
