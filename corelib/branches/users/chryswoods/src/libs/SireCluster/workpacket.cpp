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

#include <QTextStream>

#include "workpacket.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireStream;

///////////
/////////// Implementation of WorkPacketBase
///////////

static const RegisterMetaType<WorkPacketBase> r_workbase( MAGIC_ONLY,
                                                          WorkPacketBase::typeName() );

/** Serialise to a binary datastream */
QDataStream SIRECLUSTER_EXPORT &operator<<(QDataStream &ds, 
                                           const WorkPacketBase &workbase)
{
    writeHeader(ds, r_workbase, 1);
    
    SharedDataStream sds(ds);
    
    sds << workbase.current_progress << workbase.error_data
        << workbase.was_aborted;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRECLUSTER_EXPORT &operator>>(QDataStream &ds, WorkPacketBase &workbase)
{
    VersionID v = readHeader(ds, r_workbase);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> workbase.current_progress >> workbase.error_data
            >> workbase.was_aborted;
    }
    else
        throw version_error(v, "1", r_workbase, CODELOC);
        
    return ds;
}

/** Constructor */
WorkPacketBase::WorkPacketBase() 
               : QSharedData(), current_progress(0), was_aborted(false)
{}

/** Copy constructor */
WorkPacketBase::WorkPacketBase(const WorkPacketBase &other)
               : QSharedData(),
                 current_progress(other.current_progress),
                 error_data(other.error_data),
                 was_aborted(other.was_aborted)
{}

/** Destructor */
WorkPacketBase::~WorkPacketBase()
{}

/** Copy assignment operator */
WorkPacketBase& WorkPacketBase::operator=(const WorkPacketBase &other)
{
    if (this != &other)
    {
        current_progress = other.current_progress;
        error_data = other.error_data;
        was_aborted = other.was_aborted;
    }
    
    return *this;
}

/** Return the current progress of the work (percentage) */
float WorkPacketBase::progress() const
{
    return current_progress;
}

/** Return whether or not the work is in an error state */
bool WorkPacketBase::isError() const
{
    return not error_data.isEmpty();
}

/** Whether or not the job has been aborted */
bool WorkPacketBase::wasAborted() const
{
    return was_aborted;
}

/** Abort this job */
void WorkPacketBase::abort() throw()
{
    was_aborted = true;
    error_data = QByteArray();
    current_progress = 0;
}

/** Reset the WorkPacket back to its original state */
void WorkPacketBase::reset()
{
    current_progress = 0;
    error_data = QByteArray();
    was_aborted = false;
}

/** Perform one chunk of the calculation - this DOES NOT throw
    an exception */
void WorkPacketBase::runChunk() throw()
{
    if (this->isError() or this->wasAborted() or this->hasFinished())
    {
        return;
    }

    try
    {
        float new_progress = this->chunk();
        current_progress = qMin( float(0), qMax(new_progress,float(100)) );
    }
    catch(const SireError::exception &e)
    {
        this->setError(e);
    }
    catch(const std::exception &e)
    {
        this->setError( SireError::std_exception(e) );
    }
    catch(...)
    {
        this->setError( SireError::unknown_exception( QObject::tr(
                "There was an unknown exception thrown while running a chunk "
                "of the WorkPacket %1 (progress = %2 %%)")
                    .arg(this->what()).arg(current_progress), CODELOC ) );
    }
}

/** Internal function used to set the error state to 'e' */
void WorkPacketBase::setError(const SireError::exception &e) throw()
{
    try
    {
        error_data = e.pack();
    }
    catch(...)
    {
        qDebug() << CODELOC;
        qDebug() << "Error saving a binary representation of an error!!!";

        error_data = QByteArray(10, ' ');
    }
}

/** Internal function that will throw the exception representing
    the error state, if we are in an error state */
void WorkPacketBase::raiseAnyErrors()
{
    if (not error_data.isEmpty())
        SireError::exception::unpackAndThrow(error_data);
}

///////////
/////////// Implementation of WorkPacket
///////////

static const RegisterMetaType<WorkPacket> r_workpacket;

/** Serialise to a binary datastream */
QDataStream SIRECLUSTER_EXPORT &operator<<(QDataStream &ds, 
                                           const WorkPacket &workpacket)
{
    writeHeader(ds, r_workpacket, 1);
    
    SharedDataStream sds(ds);
    
    sds << workpacket.d;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRECLUSTER_EXPORT &operator>>(QDataStream &ds, WorkPacket &workpacket)
{
    VersionID v = readHeader(ds, r_workpacket);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> workpacket.d;
    }
    else
        throw version_error(v, "1", r_workpacket, CODELOC);
        
    return ds;
}

/** Create a null work packet */
WorkPacket::WorkPacket()
{}

/** Construct from the passed binary data */
WorkPacket::WorkPacket(const QByteArray &data)
{
    QDataStream ds(data);
    ds >> *this;
}

/** Construct from the passed work object */
WorkPacket::WorkPacket(const WorkPacketBase &work) : d(work)
{}

/** Copy constructor */
WorkPacket::WorkPacket(const WorkPacket &other) : d(other.d)
{}

/** Destructor */
WorkPacket::~WorkPacket()
{}

/** Copy assignment operator */
WorkPacket& WorkPacket::operator=(const WorkPacket &other)
{
    if (other.isNull())
    {
        d = 0;
    }
    else
        d = other.d;
        
    return *this;
}

/** Return whether or not this is the null (empty) work packet */
bool WorkPacket::isNull() const
{
    return d.constData() == 0;
}

/** Create and load a work packet from the passed data */
WorkPacket WorkPacket::load(const QByteArray &data)
{
    return WorkPacket(data);
}

/** Save this workpacket to a binary array */
QByteArray WorkPacket::save() const
{
    QByteArray data;
    
    //reserve 8MB of space - this prevents too many reallocations
    data.reserve( 8 * 1024 * 1024 );
    
    QDataStream ds(&data, QIODevice::WriteOnly);
    
    ds << *this;
    
    return data;
}

/** Reset the WorkPacket back to its initial state */
void WorkPacket::reset()
{
    if (not this->isNull())
    {
        d->reset();
    }
}

/** Return whether or not this work is in an error state */
bool WorkPacket::isError() const
{
    if (not this->isNull())
    {
        return d->isError();
    }
    else
        return false;
}

/** Return whether or not the work was aborted */
bool WorkPacket::wasAborted() const
{
    if (not this->isNull())
    {
        return d->wasAborted();
    }
    else
        return false;
}

/** Return whether or not the work has finished (or is in an 
    error state) */
bool WorkPacket::hasFinished() const throw()
{
    if (not this->isNull())
    {
        return d->isError() or d->wasAborted() or d->hasFinished();
    }
    else
        return true;
}

/** Abort the work */
void WorkPacket::abort()
{
    if (not this->isNull())
    {
        d->abort();
    }
}

/** Run a chunk of work */
void WorkPacket::runChunk() throw()
{
    if (not this->hasFinished())
    {
        d->runChunk();
    }
}

/** Return the current progress of the calculation (percentage) */
float WorkPacket::progress() const
{
    if (not this->isNull())
    {
        return d->progress();
    }
    else
        return 100;
}

/** Return a reference to the underlying Worker object */
const WorkPacketBase& WorkPacket::base() const
{
    if (this->isNull())
        throw SireError::nullptr_error( QObject::tr(
            "The null WorkPacket has no base!"), CODELOC );
        
    return *d;
}

///////////
/////////// Implementation of WorkTest
///////////

static const RegisterMetaType<WorkTest> r_worktest;

/** Serialise to a binary datastream */
QDataStream SIRECLUSTER_EXPORT &operator<<(QDataStream &ds, const WorkTest &worktest)
{
    writeHeader(ds, r_worktest, 1);
    
    SharedDataStream sds(ds);

    sds << worktest.current << worktest.start << worktest.end << worktest.step
        << static_cast<const WorkPacketBase&>(worktest);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRECLUSTER_EXPORT &operator>>(QDataStream &ds, WorkTest &worktest)
{
    VersionID v = readHeader(ds, r_worktest);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> worktest.current >> worktest.start >> worktest.end >> worktest.step
            >> static_cast<WorkPacketBase&>(worktest);
    }
    else
        throw version_error(v, "1", r_worktest, CODELOC);
        
    return ds;
}

/** Constructor */
WorkTest::WorkTest()
         : WorkPacketBase(), current(0), start(0), end(0), step(0)
{}

/** Construct a work test that counts from start to end in steps of 'step' */
WorkTest::WorkTest(int _start, int _end, int _step)
         : WorkPacketBase(), 
           current(_start), start(_start), end(_end), step(_step)
{}

/** Copy constructor */
WorkTest::WorkTest(const WorkTest &other)
         : WorkPacketBase(other), current(other.current),
           start(other.start), end(other.end), step(other.step)
{}

/** Destructor */
WorkTest::~WorkTest()
{}

/** Copy assignment operator */
WorkTest& WorkTest::operator=(const WorkTest &other)
{
    if (this != &other)
    {
        current = other.current;
        start = other.start;
        end = other.end;
        step = other.step;
        WorkPacketBase::operator=(other);
    }
    
    return *this;
}

/** Return whether or not the work has finished */
bool WorkTest::hasFinished() const throw()
{
    return current == end;
}

/** Reset this WorkPacket back to its original state */
void WorkTest::reset()
{
    current = start;
    WorkPacketBase::reset();
}

/** Perform one chunk of the work, returning the progress
    after the chunk */
float WorkTest::chunk()
{
    if (step == 0)
        throw SireError::invalid_arg( QObject::tr(
                "You cannot use a step size of zero!"), CODELOC );

    if (start < end)
    {
        if (step < 0)
            throw SireError::invalid_arg( QObject::tr(
                "You cannot use a negative step size if start is less than end!"),
                    CODELOC );
    
        current = qMin( current+step, end );
        
        QTextStream ts(stdout);
        ts << "I've counted to " << current << "\n";
        sleep(1);
        
        return 100.0 - ( 100.0 * double(end - current) / double(end - start) );
    }
    else
    {
        if (step > 0)
            throw SireError::invalid_arg( QObject::tr(
                "You cannot use a positive step size if start is greater than end!"),
                    CODELOC );
    
        current = qMax( current+step, end );

        QTextStream ts(stdout);
        ts << "I've counted to " << current << "\n";
        sleep(1);
        
        return 100.0 - ( 100.0 * double(end - current) / double(end - start) );
    }
}
