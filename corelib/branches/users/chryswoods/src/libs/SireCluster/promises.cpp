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

#include <QVector>
#include <QTime>

#include "promises.h"
#include "promise.h"
#include "workpacket.h"

#include "SireID/index.h"

#include "Siren/forages.h"
#include "Siren/errors.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::resources;
using namespace SireID;
using namespace Siren;

static const RegisterHandle<Promises> r_promises;

/** Null constructor */
Promises::Promises() : ImplementsHandle< Promises, Handles< QVector<Promise> > >()
{}

/** Internal constructor used to pass in the array of promises */
Promises::Promises(const QVector<Promise> &promises)
         : ImplementsHandle< Promises, Handles< QVector<Promise> > >(
                            new QVector<Promise>(promises) )
{}

/** Copy constructor */
Promises::Promises(const Promises &other)
         : ImplementsHandle< Promises, Handles< QVector<Promise> > >(other)
{}

/** Destructor */
Promises::~Promises()
{}

/** Copy assignment operator */
Promises& Promises::operator=(const Promises &other)
{
    Handles< QVector<Promise> >::operator=(other);
    return *this;
}

/** Comparison operator */
bool Promises::operator==(const Promises &other) const
{
    return Handles< QVector<Promise> >::operator==(other);
}

/** Comparison operator */
bool Promises::operator!=(const Promises &other) const
{
    return Handles< QVector<Promise> >::operator!=(other);
}

/** Return the ith promise

    \throw Siren::invalid_index
*/
Promise Promises::operator[](int i) const
{
    HandleLocker lkr(*this);
    return resource().at( Index(i).map( resource().count() ) );
}

/** Return the ith promise

    \throw Siren::invalid_index
*/
Promise Promises::at(int i) const
{
    return Promises::operator[](i);
}

/** Return whether or not this is empty */
bool Promises::isEmpty() const
{
    if (isNull())
        return true;
    else
    {
        HandleLocker lkr(*this);
        return resource().isEmpty();
    }
}

/** Return the number of promises in this set */
int Promises::nPromises() const
{
    if (isNull())
        return 0;
    else
    {
        HandleLocker lkr(*this);
        return resource().count();
    }
}

/** Return the number of promises in this set */
int Promises::count() const
{
    return nPromises();
}

/** Return a string representation of the promises */
QString Promises::toString() const
{
    if (isNull())
        return QObject::tr("Promises::null");
    else
        return QObject::tr("Promises( nPromises() == %1 )")
                        .arg(this->nPromises());
}

uint Promises::hashCode() const
{
    return qHash( Promises::typeName() ) + nPromises();
}

/** Wait for-ages until all of the promises complete. Because
    this thread will do nothing else, it will donate its
    cycles to complete the job */
void Promises::wait()
{
    if (isNull())
        return;
        
    HandleLocker lkr(*this);
    QVector<Promise> promises = resource();
    lkr.unlock();
    
    for (int i=0; i<promises.count(); ++i)
    {
        check_for_ages();
    
        //this will run the job in the local thread
        //if it is not running, and has not started yet
        promises[i].runLocal();
    }
    
    //all jobs are now either finished or running on remote resources
    for (int i=0; i<promises.count(); ++i)
    {
        check_for_ages();
        promises[i].wait();
    }
}

/** Wait up to 'ms' milliseconds for all of the promises to complete.
    This returns whether or not all of the promises have completed */
bool Promises::wait(int ms)
{
    if (ms < 0)
    {
        this->wait();
        return true;
    }

    QVector<Promise> promises;
    {
        HandleLocker lkr(*this);
        promises = resource();
    }
    
    QTime t;
    t.start();
    
    for (int i=0; i<promises.count(); ++i)
    {
        if (ms <= 0)
            return false;

        if (not promises[i].wait(ms))
            return false;
            
        ms -= t.elapsed();
    }
    
    return true;
}

/** Run all of the passed workpackets in the local thread, returning
    promises for the completed work */
Promises Promises::runLocal(const QVector<WorkPacketPtr> &workpackets)
{
    if (workpackets.isEmpty())
        return Promises();
        
    QVector<Promise> promises(workpackets.count());
    
    for (int i=0; i<promises.count(); ++i)
    {
        promises[i] = Promise(workpackets.at(i));
    }
    
    for (int i=0; i<promises.count(); ++i)
    {
        check_for_ages();
        
        promises[i].runLocal();
    }
    
    return promises;
}

/** Run all of the passed workpackets in the local thread, returning
    promises for the completed work */
Promises Promises::runLocal(const QList<WorkPacketPtr> &workpackets)
{
    return Promises::runLocal(workpackets.toVector());
}
