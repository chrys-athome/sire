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

#include "Siren/errors.h"
#include "Siren/stream.h"
#include "Siren/datastream.h"

#include <QDebug>

using namespace SireCluster;
using namespace Siren;

///////////
/////////// Implementation of WorkPacket
///////////

static const RegisterObject<WorkPacket> r_workpacket( VIRTUAL_CLASS );

/** Constructor */
WorkPacket::WorkPacket()
           : Extends<WorkPacket,Object>(),
             current_progress(0)
{}

/** Copy constructor */
WorkPacket::WorkPacket(const WorkPacket &other)
           : Extends<WorkPacket,Object>(other),
             current_progress(other.current_progress)
{}

/** Destructor */
WorkPacket::~WorkPacket()
{}

/** Copy assignment operator */
WorkPacket& WorkPacket::operator=(const WorkPacket &other)
{
    if (this != &other)
    {
        current_progress = other.current_progress;
        Object::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool WorkPacket::operator==(const WorkPacket &other) const
{
    return current_progress == other.current_progress;
}

/** Comparison operator */
bool WorkPacket::operator!=(const WorkPacket &other) const
{
    return not WorkPacket::operator==(other);
}

/** Return whether or not this work packet should be stored 
    as a binary array - this is used by Promise to work out
    how to store the initial WorkPacket state. Only large
    packets should be binary packed (as they are then 
    compressed) */
bool WorkPacket::shouldPack() const
{
    return false;
}

/** Return the approximate maximum size (in bytes) of the WorkPacket. This
    doesn't have to exact (or indeed accurate) - it is used
    to help the WorkPacket::pack() function reserve enough
    space when serialising this packet to a binary array. 
    The only penalty of getting this wrong is that you'll
    either allocate too much space, or be reallocating while
    the packet is being written */
int WorkPacket::approximatePacketSize() const
{
    // adds 8 to give a little lee-way
    return sizeof(float) + sizeof(bool) + 8;
}

/** Return the current progress of the work (percentage) */
float WorkPacket::progress() const
{
    return current_progress;
}

/** Return whether or not this is an Error WorkPacket */
bool WorkPacket::isError() const
{
    return false;
}

/** Throw the error, if this is in an error state */
void WorkPacket::throwError() const
{}

/** Whether or not the job has been aborted */
bool WorkPacket::wasAborted() const
{
    return false;
}

void WorkPacket::stream(Stream &s)
{
    s.assertVersion<WorkPacket>(1);
    
    Schema schema = s.item<WorkPacket>();
    
    schema.data("current_progress") & current_progress;
    
    Object::stream( schema.base() );
}

/** Perform one chunk of the calculation and return a workpacket
    that contains the work after this chunk has been run */
WorkPacketPtr WorkPacket::runChunk() const
{
    if (this->wasAborted() or this->hasFinished())
    {
        return *this;
    }
    else
    {
        try
        {
            WorkPacketPtr next_chunk = this->chunk();
            
            return next_chunk;
        }
        catch(const Siren::exception &e)
        {
            return ErrorPacket(e);
        }
        catch(const std::exception &e)
        {
            return ErrorPacket(Siren::std_exception(e));
        }
        catch(...)
        {
            return ErrorPacket(Siren::unknown_error( QObject::tr(
                    "There was an unknown exception thrown while running a chunk "
                    "of the WorkPacket %1 (progress = %2 %%)")
                        .arg(this->what()).arg(this->progress()), CODELOC ) );
        }
    }
}

/** Pack this WorkPacket into a binary array */
QByteArray WorkPacket::pack() const
{
    QByteArray data;
    data.reserve( this->approximatePacketSize() );
    
    DataStream ds(&data, QIODevice::WriteOnly);
    
    this->save(ds);
    
    data = qCompress(data);
    
    return data;
}

/** Unpack the WorkPacket contained in the passed binary array.
    'data' *must* have been created using WorkPacket::pack() */
WorkPacketPtr WorkPacket::unpack(const QByteArray &data)
{
    QByteArray uncompressed_data = qUncompress(data);

    DataStream ds(uncompressed_data);
    
    ObjRef obj = ds.loadNextObject();
    
    return obj.asA<WorkPacket>();
}

///////////
/////////// Implementation of ErrorPacket
///////////

static const RegisterObject<ErrorPacket> r_errorpacket;

/** Constructor */
ErrorPacket::ErrorPacket() : Implements<ErrorPacket,WorkPacket>()
{}

/** Construct an ErrorPacket for the error 'e' */
ErrorPacket::ErrorPacket(const Siren::exception &e)
            : Implements<ErrorPacket,WorkPacket>()
{
    DataStream ds(error_data);
    e.save(ds);
    qCompress(error_data);
}

/** Copy constructor */
ErrorPacket::ErrorPacket(const ErrorPacket &other)
            : Implements<ErrorPacket,WorkPacket>(other), 
              error_data(other.error_data)
{}

/** Destructor */
ErrorPacket::~ErrorPacket()
{}

/** Copy assignment operator */
ErrorPacket& ErrorPacket::operator=(const ErrorPacket &other)
{
    if (this != &other)
    {
        error_data = other.error_data;
        WorkPacket::operator=(other);
    }
    
    return *this;
}

bool ErrorPacket::operator==(const ErrorPacket &other) const
{
    return error_data == other.error_data and
           WorkPacket::operator==(other);
}

bool ErrorPacket::operator!=(const ErrorPacket &other) const
{
    return not ErrorPacket::operator==(other);
}

/** Return the approximate maximum size (in bytes) of the WorkPacket. This
    doesn't have to exact (or indeed accurate) - it is used
    to help the WorkPacket::pack() function reserve enough
    space when serialising this packet to a binary array. 
    The only penalty of getting this wrong is that you'll
    either allocate too much space, or be reallocating while
    the packet is being written */
int ErrorPacket::approximatePacketSize() const
{
    return error_data.count() + WorkPacket::approximatePacketSize();
}

/** Return whether or not the work has finished */
bool ErrorPacket::hasFinished() const
{
    return true;
}

/** Return whether or not this is an error */
bool ErrorPacket::isError() const
{
    return not error_data.isEmpty();
}

/** Throw the error associated with this packet */
void ErrorPacket::throwError() const
{
    if (not error_data.isEmpty())
    {
        QByteArray uncompressed_data = qUncompress(error_data);
        
        DataStream ds(uncompressed_data);
        ObjRef error = ds.loadNextObject();
        error.asA<Siren::exception>().throwSelf();
    }
}

WorkPacketPtr ErrorPacket::chunk() const
{
    return *this;
}

uint ErrorPacket::hashCode() const
{
    return qHash(ErrorPacket::typeName()) + qHash(error_data);
}

QString ErrorPacket::toString() const
{
    return QObject::tr("ErrorPacket()");
}

void ErrorPacket::stream(Stream &s)
{
    s.assertVersion<ErrorPacket>(1);
    
    Schema schema = s.item<ErrorPacket>();
    
    schema.data("error_data") & error_data;
    
    WorkPacket::stream( schema.base() );
}

///////////
/////////// Implementation of AbortPacket
///////////

static const RegisterObject<AbortPacket> r_abortpacket;

/** Constructor */
AbortPacket::AbortPacket() : Implements<AbortPacket,WorkPacket>()
{}

/** Copy constructor */
AbortPacket::AbortPacket(const AbortPacket &other) 
            : Implements<AbortPacket,WorkPacket>(other)
{}

/** Destructor */
AbortPacket::~AbortPacket()
{}

/** Copy assignment operator */
AbortPacket& AbortPacket::operator=(const AbortPacket &other)
{
    WorkPacket::operator=(other);
    return *this;
}

bool AbortPacket::operator==(const AbortPacket &other) const
{
    return WorkPacket::operator==(other);
}

bool AbortPacket::operator!=(const AbortPacket &other) const
{
    return not AbortPacket::operator==(other);
}

/** Return whether or not the work has finished */
bool AbortPacket::hasFinished() const
{
    return true;
}

/** Return whether or not this was aborted (it obviously was!) */
bool AbortPacket::wasAborted() const
{
    return true;
}

WorkPacketPtr AbortPacket::chunk() const
{
    return *this;
}

uint AbortPacket::hashCode() const
{
    return qHash(AbortPacket::typeName());
}

QString AbortPacket::toString() const
{
    return QObject::tr("AbortPacket()");
}

void AbortPacket::stream(Stream &s)
{
    s.assertVersion<AbortPacket>(1);
    
    Schema schema = s.item<AbortPacket>();
    
    WorkPacket::stream( schema.base() );
}

///////////
/////////// Implementation of WorkTest
///////////

static const RegisterObject<WorkTest> r_worktest;

/** Constructor */
WorkTest::WorkTest() : Implements<WorkTest,WorkPacket>(),
                       current(0), start(0), end(0), step(0)
{}

/** Construct a work test that counts from start to end in steps of 'step' */
WorkTest::WorkTest(int _start, int _end, int _step)
         : Implements<WorkTest,WorkPacket>(), 
           current(_start), start(_start), end(_end), step(_step)
{}

/** Copy constructor */
WorkTest::WorkTest(const WorkTest &other)
         : Implements<WorkTest,WorkPacket>(other), current(other.current),
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
        WorkPacket::operator=(other);
    }
    
    return *this;
}

bool WorkTest::operator==(const WorkTest &other) const
{
    return current == other.current and start == other.start and
           end == other.end and step == other.step and
           WorkPacket::operator==(other);
}

bool WorkTest::operator!=(const WorkTest &other) const
{
    return not WorkTest::operator==(other);
}

/** Return the approximate maximum size (in bytes) of the WorkPacket. This
    doesn't have to exact (or indeed accurate) - it is used
    to help the WorkPacket::pack() function reserve enough
    space when serialising this packet to a binary array. 
    The only penalty of getting this wrong is that you'll
    either allocate too much space, or be reallocating while
    the packet is being written */
int WorkTest::approximatePacketSize() const
{
    return 4 * sizeof(qint32) + WorkPacket::approximatePacketSize();
}

/** Return whether or not the work has finished */
bool WorkTest::hasFinished() const
{
    return current == end;
}

WorkPacketPtr WorkTest::chunk() const
{
    if (step == 0)
        throw Siren::invalid_arg( QObject::tr(
                "You cannot use a step size of zero!"), CODELOC );

    if (start < end)
    {
        if (step < 0)
            throw Siren::invalid_arg( QObject::tr(
                "You cannot use a negative step size if start is less than end!"),
                    CODELOC );

        WorkTest next_state(*this);
    
        next_state.current = qMin( current+step, end );
        
        next_state.setProgress( 100 * double(next_state.current - start) / 
                                      double(end - start) );
        
        QTextStream ts(stdout);
        ts << "I've counted to " << current << "\n";
        sleep(1);
        
        return next_state;
    }
    else
    {
        if (step > 0)
            throw Siren::invalid_arg( QObject::tr(
                "You cannot use a positive step size if start is greater than end!"),
                    CODELOC );
    
        WorkTest next_state(*this);
    
        next_state.current = qMax( current+step, end );

        next_state.setProgress( 100 * double(end - next_state.current) /
                                      double(end - start) );

        QTextStream ts(stdout);
        ts << "I've counted to " << current << "\n";
        sleep(1);
        
        return next_state;
    }
}

uint WorkTest::hashCode() const
{
    return qHash(WorkTest::typeName()) + start + end + current;
}

void WorkTest::stream(Stream &s)
{
    s.assertVersion<WorkTest>(1);
    
    Schema schema = s.item<WorkTest>();
    
    schema.data("start") & start;
    schema.data("end") & end;
    schema.data("step") & step;
    schema.data("current") & current;
    
    WorkPacket::stream( schema.base() );
}

QString WorkTest::toString() const
{
    return QObject::tr("WorkTest( from %1 to %2 with step %3, currently %4 )")
            .arg(start).arg(end).arg(step).arg(current);
}
