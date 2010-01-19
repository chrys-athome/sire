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

#include "resourcemanager.h"

using namespace SireCluster;

ResourceManager::Data::Data()
{}

ResourceManager::Data::~Data()
{}

Q_GLOBAL_STATIC( ResourceManager::Data, data );

/** Call this function to register a backend with this resource manager */
void ResourceManager::registerBackend(const DormantBackend &backend)
{
    QMutexLocker lkr( &(data().datamutex) );

    if (not data().resources.contains(backend))
    {
        if (not backend.UID().isNull())
            throw Siren::program_bug( QObject::tr(
                "How are we registering an unregistered backend "
                "(%1) which already has a UID? (%2)")
                    .arg(backend.description())
                    .arg(backend.UID().toString()), CODELOC );

        backend.setUID( QUuid::createUuid() );

        data().resources.append(backend);
    }
}

/** Unregister the backend 'backend' - this will prevent this backend
    from being available to anyone else and will deactivate it */
void ResourceManager::unregisterBackend(ActiveBackend &backend)
{
    QMutexLocker lkr( &(data().datamutex) );
    
    QListIterator<DormantBackend> it( data().backends );
    
    while (it.hasNext())
    {
        const DormantBackend &b = it.next();
        
        if (b.UID() == backend.UID())
        {
            b.setUID( QUuid() );
            it.remove();
        }
    }
    
    lkr.unlock();
    
    //now force the deactivation of this backend
    backend.abortJob();
    backend = ActiveBackend();
}

/** Return the list of available backends, together with their
    descriptions (note this returns the list of all backends that
    are available now - even if they are busy) */
QHash<QUuid,QString> ResourceManager::availableBackends()
{
    QMutexLocker lkr( &(data()->datamutex) );
    
    QHash<QUuid,QString> resources;
    
    foreach (DormantBackend backend, data()->backends)
    {
        resources.insert( backend.UID(), backend.description() );
    }
    
    return resources;
}

static ActiveBackend reserveBackend(const QList<DormantBackend> &backends,
                                    int ms)
{
    if (backends.isEmpty())
    {
        return ActiveBackend();
    }
    else if (backends.count() == 1)
    {
        if (ms <= 0)
            return backends.at(0).tryAcquire();
        else
            return backends.at(0).tryAcquire(ms);
    }
    else
    {
        int wait_ms = ms / backends.count();
        
        if (wait_ms <= 0)
        {
            foreach (DormantBackend backend, backends)
            {
                ActiveBackend active = backend.tryActivate();
                
                if (not active.isNull())
                    return active;
            }
        }
        else
        {
            foreach (DormantBackend backend, backends)
            {
                ActiveBackend active = backend.tryActivate(wait_ms);
                
                if (not active.isNull())
                    return active;
            }
        }
    }
}

void ResourceManager::clearExpiredReservations()
{
    QMutexLocker lkr( &(data()->datamutex) );

    //how long has it been since the last time this was called?
    int elapsed = data()->timer.restart();
    
    QMutableHashIterator< QUuid,QPair<int,ActiveBackend> > it( data()->reservations );
    
    while (it.hasNext())
    {
        it.next();
        
        it.value().first -= elapsed;
        
        if (it.value().first < 0)
            //the reservation has expired
            it.remove();
    }
}

/** Reserve a backend. This blocks until a backend is available (although
    may be forever if there are no backends registered on this
    resource manager). This returns the UID of a resource reservation which
    you must use to collect the backend (using the collectReservation function).
    The reservation will expire 'expires' milliseconds after this function
    returns */
QUuid ResourceManager::reserveBackend(int expires)
{
    int loop_wait = 0;

    try
    {
        while ( for_ages() )
        {
            ResourceManager::clearExpiredReservations();

            QList<DormantBackend> backends;
            {
                QMutexLocker lkr( &(data()->datamutex) );
                backends = data()->backends;
            }

            ActiveBackend active = ::reserveBackend(backends);
            
            if (not active.isNull())
            {
                
                return active;
                
            //increase the time we are prepared to wait
            if (loop_wait = 0)
                loop_wait = 5;
            else
                loop_wait *= 2;
                
            //lets not wait forever...
            loop_wait = qMin(loop_wait, 60000);
        }
    }
    catch(const Siren::for_ages_over&)
    {
        return QUuid();
    }
}

/** Reserve the backend with specific UID 'uid' */
QUuid ResourceManager::reserveBackend(const QUuid &uid, int expires)
{
    int loop_wait = 0;

    while (true)
    {
        ResourceManager::clearExpiredReservations();

        QList<DormantBackend> backends;
        {
            QMutexLocker lkr( &(data()->datamutex) );
            backends = data()->backends;
        }

        ActiveBackend active = ::reserveBackend(backends);
        
        if (not active.isNull())
        {
            
            return active;
            
        //increase the time we are prepared to wait
        if (loop_wait = 0)
            loop_wait = 5;
        else
            loop_wait *= 2;
            
        //lets not wait forever...
        loop_wait = qMin(loop_wait, 60000);
    }
}

QUuid ResourceManager::reserveBackend(const QString &description,
                            int expires);

QList<QUuid> ResourceManager::reserveBackends(int n, int expires);
QList<QUuid> ResourceManager::reserveBackends(const QList<QUuid> &uids,
                                    int expires);
QList<QUuid> ResourceManager::reserveBackends(const QString &description,
                                    int n, int expires);

QUuid ResourceManager::tryReserveBackend(int ms, int expires);
QUuid ResourceManager::tryReserveBackend(int ms, const QUuid &uid,
                               int expires);
QUuid ResourceManager::tryReserveBackend(int ms,
                               const QString &description,
                               int expires);

QList<QUuid> ResourceManager::tryReserveBackends(int ms, int n, int expires);
QList<QUuid> ResourceManager::tryReserveBackends(int ms, const QList<QUuid> &uids,
                                       int expires);
QList<QUuid> ResourceManager::tryReserveBackends(int ms,
                                       const QString &description,
                                       int n, int expires);

ActiveBackend ResourceManager::collectReservation(const QUuid &uids);
QList<ActiveBackend> ResourceManager::collectReservation(const QList<QUuid> &uids);

void ResourceManager::releaseReservation(const QUuid &uid);
void ResourceManager::releaseReservation(const QList<QUuid> &uids);
