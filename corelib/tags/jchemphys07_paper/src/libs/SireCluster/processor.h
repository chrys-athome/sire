/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIRECLUSTER_PROCESSOR_H
#define SIRECLUSTER_PROCESSOR_H

#include "sireglobal.h"

#include <QObject>
#include <QMutex>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/utility.hpp>

SIRE_BEGIN_HEADER

namespace SireCluster
{

/** This is the pure virtual base class of all Workers - these are
    the actual, single-instance classes that represent individual processors,
    and perform the actual work on those processors. These classes are hidden
    behind the Processor class, so that we can ensure that there is only a single
    copy of the worker. The Processor classes can be copied and edited at will,
    but then return a pointer to a single WorkerBase class when they are
    activated. Once activated, the Processor class can no longer be edited, and
    the code can be sure that it has the only copy of the WorkerBase.

    This class forms the base of other workers, e.g. ThreadWorker, which
    has a background thread which can perform calculations separately to the
    main thread.

    This class is not thread-safe!

    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT WorkerBase : public boost::noncopyable
{
public:
    WorkerBase();

    virtual ~WorkerBase();
};

namespace detail
{

class SIRECLUSTER_EXPORT ProcessorPvt : boost::noncopyable
{
public:
    ProcessorPvt(const QString &name = QObject::tr("Unnamed"));

    virtual ~ProcessorPvt();

    boost::shared_ptr<WorkerBase> activate();

    bool isActive() const;

    QString name() const;

    void setName(const QString &name);

protected:
    virtual boost::shared_ptr<WorkerBase> _pvt_activate()=0;

    QMutex* mutex() const;

private:

    /** A mutex used to serialise access to this processor */
    QMutex datamutex;

    /** The name of this processor */
    QString nme;

    /** A weak pointer to the activated processor */
    boost::weak_ptr<WorkerBase> active_processor;
};

/** Return a pointer to the mutex that is used to serialise access to
    this object */
inline QMutex* ProcessorPvt::mutex() const
{
    return const_cast<QMutex*>(&datamutex);
}

}

/** This is the base class of all processors. A processor is an object
    that allows computation to be conducted in parallel to the main
    thread of the program (e.g. a new thread, a remote processor or
    an external program).

    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Processor
{
public:
    virtual ~Processor();

    /** Return the name of the type of processor */
    virtual const char* what() const=0;

    QString name() const;

    void setName(const QString &newname);

    boost::shared_ptr<WorkerBase> activate();

    bool isActive() const;

protected:
    Processor(const boost::shared_ptr<detail::ProcessorPvt> &data);
    Processor(const Processor &other);

    detail::ProcessorPvt& data();
    const detail::ProcessorPvt& data() const;

private:
    /** Shared pointer to the data of this processor */
    boost::shared_ptr<detail::ProcessorPvt> d;
};

}

SIRE_END_HEADER

#endif
