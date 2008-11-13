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

#include "sireglobal.h"

#include <QSharedData>

#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireCluster
{
class WorkPacket;
class WorkPacketBase;
class WorkTest;
}

QDataStream& operator<<(QDataStream&, const SireCluster::WorkPacketBase&);
QDataStream& operator>>(QDataStream&, SireCluster::WorkPacketBase&);

QDataStream& operator<<(QDataStream&, const SireCluster::WorkPacket&);
QDataStream& operator>>(QDataStream&, SireCluster::WorkPacket&);

QDataStream& operator<<(QDataStream&, const SireCluster::WorkTest&);
QDataStream& operator>>(QDataStream&, SireCluster::WorkTest&);

namespace SireCluster
{

/** This is the base class of all WorkPackets. A WorkPacket
    contains all of the code and input data for a piece of work,
    and also contains space to return the output and current
    progress 
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT WorkPacketBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const WorkPacketBase&);
friend QDataStream& ::operator>>(QDataStream&, WorkPacketBase&);

public:
    WorkPacketBase();
    WorkPacketBase(const WorkPacketBase &other);
    
    virtual ~WorkPacketBase();
    
    void abort() throw();
    
    virtual void reset();
    
    void runChunk() throw();
    
    float progress() const;
    
    virtual bool hasFinished() const throw()=0;
    
    bool isError() const;
    
    bool wasAborted() const;
    
    virtual WorkPacketBase* clone() const=0;
    
    static const char* typeName()
    {
        return "SireCluster::WorkPacketBase";
    }
    
    virtual const char* what() const=0;

    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }
    
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }

protected:
    virtual float chunk()=0;
    
    void raiseAnyErrors();

    WorkPacketBase& operator=(const WorkPacketBase&);
    
private:
    void setError(const SireError::exception &e) throw();

    /** The current progress of the work */
    float current_progress;
    
    /** A binary representation of any errors */
    QByteArray error_data;
    
    /** Whether or not the work was aborted */
    bool was_aborted;
};

/** This class is the generic holder for all work packets.
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT WorkPacket
{

friend QDataStream& ::operator<<(QDataStream&, const WorkPacket&);
friend QDataStream& ::operator>>(QDataStream&, WorkPacket&);

public:
    WorkPacket();
    WorkPacket(const WorkPacketBase &work);
    WorkPacket(const WorkPacket &other);
    
    ~WorkPacket();
    
    WorkPacket& operator=(const WorkPacket &other);

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<WorkPacket>() );
    }
    
    const char* what() const
    {
        return WorkPacket::typeName();
    }

    bool isNull() const;

    void abort();

    void runChunk() throw();
    
    void reset();
    
    float progress() const;
    
    bool wasAborted() const;
    
    bool hasFinished() const throw();

    bool isError() const;

    const WorkPacketBase& base() const;

    template<class T>
    bool isA() const
    {
        return d->isA<T>();
    }
    
    template<class T>
    const T& asA() const
    {
        return d->asA<T>();
    }

private:
    /** Implicitly shared pointer to the work */
    SireBase::SharedPolyPointer<WorkPacketBase> d;
};

/** This is a small test packet that can be used to test
    that everything is working */
class SIRECLUSTER_EXPORT WorkTest : public WorkPacketBase
{

friend QDataStream& ::operator<<(QDataStream&, const WorkTest&);
friend QDataStream& ::operator>>(QDataStream&, WorkTest&);

public:
    WorkTest();
    WorkTest(int start, int end, int step=1);
    WorkTest(const WorkTest &other);
    
    ~WorkTest();
    
    WorkTest& operator=(const WorkTest &other);
    
    WorkTest* clone() const
    {
        return new WorkTest(*this);
    }
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<WorkTest>() );
    }
    
    const char* what() const
    {
        return WorkTest::typeName();
    }
    
    void reset();
    
    bool hasFinished() const throw();

protected:
    float chunk();
    
private:
    /** The range to loop over */
    qint32 current, start, end, step;
};

}

Q_DECLARE_METATYPE( SireCluster::WorkPacket )
Q_DECLARE_METATYPE( SireCluster::WorkTest )

SIRE_EXPOSE_CLASS( SireCluster::WorkPacketBase )
SIRE_EXPOSE_CLASS( SireCluster::WorkPacket )
SIRE_EXPOSE_CLASS( SireCluster::WorkTest )

SIRE_END_HEADER

#endif
