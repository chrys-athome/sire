/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIRECLUSTER_WORKPACKET_H
#define SIRECLUSTER_WORKPACKET_H

#include "Siren/object.h"
#include "Siren/objptr.hpp"

#include "sireglobal.h"

#include <QSharedData>

SIRE_BEGIN_HEADER

namespace Siren
{
class exception;
}

namespace SireCluster
{

class WorkPacket;

typedef Siren::ObjPtr<WorkPacket> WorkPacketPtr;

/** This is the base class of all WorkPackets. A WorkPacket
    contains all of the code and input data for a piece of work,
    and also contains space to return the output and current
    progress 
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT WorkPacket : public Siren::Extends<WorkPacket,Siren::Object>
{
public:
    WorkPacket();
    WorkPacket(const WorkPacket &other);
    
    virtual ~WorkPacket();
    
    static QString typeName();
    
    void stream(Siren::Stream &s);
    
    virtual bool shouldPack() const;
    virtual int approximatePacketSize() const;

    QByteArray pack() const;
    static WorkPacketPtr unpack(const QByteArray &data);
    
    WorkPacketPtr runChunk() const;
    
    float progress() const;
    
    virtual bool hasFinished() const=0;
    
    virtual bool isError() const;
    virtual void throwError() const;
    
    virtual bool wasAborted() const;

protected:
    virtual WorkPacketPtr chunk() const=0;

    WorkPacket& operator=(const WorkPacket&);
    
    bool operator==(const WorkPacket &other) const;
    bool operator!=(const WorkPacket &other) const;
    
    void setProgress(double new_progress);
    
private:
    /** The current progress of the work */
    float current_progress;
};

/** This is a packet that is sent if the job was aborted.

    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT AbortPacket : public Siren::Implements<AbortPacket,WorkPacket>
{
public:
    AbortPacket();

    AbortPacket(const AbortPacket &other);
    
    ~AbortPacket();
    
    AbortPacket& operator=(const AbortPacket &other);

    bool operator==(const AbortPacket &other) const;
    bool operator!=(const AbortPacket &other) const;

    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &stream);
    
    bool hasFinished() const;
    bool wasAborted() const;

protected:
    WorkPacketPtr chunk() const;
};

/** This is a packet that contains an error. This is returned
    if something went wrong while running a WorkPacket
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT ErrorPacket 
        : public Siren::Implements<ErrorPacket,WorkPacket>
{
public:
    ErrorPacket();
    ErrorPacket(const Siren::exception &e);

    ErrorPacket(const ErrorPacket &other);
    
    ~ErrorPacket();
    
    ErrorPacket& operator=(const ErrorPacket &other);
    
    bool operator==(const ErrorPacket &other) const;
    bool operator!=(const ErrorPacket &other) const;

    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    int approximatePacketSize() const;
    
    bool isError() const;
    void throwError() const;
    
    bool hasFinished() const;

protected:
    WorkPacketPtr chunk() const;
    
private:
    /** A binary representation of the error */
    QByteArray error_data;
};

/** This is a small test packet that can be used to test
    that everything is working */
class SIRECLUSTER_EXPORT WorkTest 
        : public Siren::Implements<WorkTest,WorkPacket>
{
public:
    WorkTest();
    WorkTest(int start, int end, int step=1);
    WorkTest(const WorkTest &other);
    
    ~WorkTest();
    
    WorkTest& operator=(const WorkTest &other);

    bool operator==(const WorkTest &other) const;
    bool operator!=(const WorkTest &other) const;

    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);
    
    int approximatePacketSize() const;
    
    bool hasFinished() const;

protected:
    WorkPacketPtr chunk() const;
    
private:
    /** The range to loop over */
    qint32 current, start, end, step;
};

}

Q_DECLARE_METATYPE( SireCluster::ErrorPacket )
Q_DECLARE_METATYPE( SireCluster::AbortPacket )
Q_DECLARE_METATYPE( SireCluster::WorkTest )

SIRE_EXPOSE_CLASS( SireCluster::WorkPacket )
SIRE_EXPOSE_CLASS( SireCluster::ErrorPacket )
SIRE_EXPOSE_CLASS( SireCluster::AbortPacket )
SIRE_EXPOSE_CLASS( SireCluster::WorkTest )

SIRE_EXPOSE_OBJECT_PTR( SireCluster::WorkPacketPtr, SireCluster::WorkPacket )

SIRE_END_HEADER

#endif
