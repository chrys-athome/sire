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

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "resourcemanager.h"
#include "backend.h"

#include "Siren/forages.h"
#include "Siren/mutex.h"
#include "Siren/errors.h"

#include <QDebug>

using namespace SireCluster;
using namespace SireCluster::resources;
using namespace Siren;

class ResourceManagerData
{
public:
    ResourceManagerData();
    ~ResourceManagerData();
    
    /** Lock to serialise access to the data of this resource */
    Mutex datamutex;

    /** The list of backend resources, indexed by their UID */
    QHash<QUuid,DormantBackend> resources;
    
    /** The list of free backends */
    QList<QUuid> free_resources;
    
    /** The set of pending reservations, with the 
        time and date at which the reservation expires */
    QHash< QUuid,QPair<QDateTime,ActiveBackend> > reservations;

    /** The version number of the resource list - this changes
        when resources are added or removed */
    int resource_version;
};

ResourceManagerData::ResourceManagerData() 
                    : datamutex( QMutex::Recursive ),
                      resource_version(0)
{}

ResourceManagerData::~ResourceManagerData()
{}

static ResourceManagerData *d(0);

void ResourceManager::init()
{
    if (not d)
    {
        d = new ResourceManagerData();
    }
}

void ResourceManager::end()
{
    delete d;
    d = 0;
}

/** Call this function to register a backend with this resource manager */
void ResourceManager::registerResource(const DormantBackend &backend)
{
    MutexLocker lkr( &(d->datamutex) );

    if (not d->resources.contains(backend.UID()))
    {
        if (not backend.UID().isNull())
            throw Siren::program_bug( QObject::tr(
                "How are we registering an unregistered backend "
                "(%1) which already has a UID? (%2)")
                    .arg(backend.description())
                    .arg(backend.UID().toString()), CODELOC );

        backend.setUID( QUuid::createUuid() );

        d->resources.insert(backend.UID(),backend);
        d->free_resources.append(backend.UID());
        d->resource_version += 1;
    }
}

/** Unregister the backend 'backend' - this will prevent this backend
    from being available to anyone else and will deactivate it */
void ResourceManager::unregisterResource(ActiveBackend &backend)
{
    MutexLocker lkr( &(d->datamutex) );

    if (d->resources.contains(backend.UID()))
    {
        d->resources.remove(backend.UID());
        d->free_resources.removeAll(backend.UID());
        
        QMutableHashIterator< QUuid,QPair<QDateTime,ActiveBackend> >
                        it( d->reservations );
                        
        while (it.hasNext())
        {
            it.next();
            
            if (it.value().second.UID() == backend.UID())
                it.remove();
        }
        
        d->resource_version += 1;
    }
    
    lkr.unlock();
    
    //now force the deactivation of this backend
    backend.abortJob();
    backend = ActiveBackend();
}

static ActiveBackend reserveResource(const QList<QUuid> &backends,
                                     int ms)
{
    if (backends.isEmpty())
    {
        return ActiveBackend();
    }
    else if (backends.count() == 1)
    {
        if (ms <= 0)
            return d->resources.value( backends.at(0) ).tryActivate();
        else
            return d->resources.value( backends.at(0) ).tryActivate(ms);
    }
    else
    {
        int wait_ms = ms / backends.count();
        
        if (wait_ms <= 0)
        {
            foreach (QUuid backend, backends)
            {
                ActiveBackend active = d->resources.value(backend).tryActivate();
                
                if (not active.isNull())
                    return active;
            }
        }
        else
        {
            foreach (QUuid backend, backends)
            {
                ActiveBackend active = d->resources.value(backend)
                                                   .tryActivate(wait_ms);
                
                if (not active.isNull())
                    return active;
            }
        }
    }
    
    return ActiveBackend();
}

/** This function clears all expired reservations, freeing the resources
    up to be made available for other uses */
static void clearExpiredReservations()
{
    MutexLocker lkr( &(d->datamutex) );

    QMutableHashIterator< QUuid,QPair<QDateTime,ActiveBackend> > 
            it( d->reservations );
    
    QDateTime now = QDateTime::currentDateTime();
    
    while (it.hasNext())
    {
        it.next();
        
        if (it.value().first > now)
        {
            //the reservation has expired
            d->free_resources.append( it.value().second.UID() );
            it.remove();
        }
    }
    
    //also check for any backends that are now free
    foreach (DormantBackend backend, d->resources)
    {
        if (not d->free_resources.contains(backend.UID()))
        {
            if (not backend.isActivated())
                d->free_resources.append( backend.UID() );
        }
    }
}

static QUuid pvt_reserveResource(int expires,
                                 boost::function< QList<QUuid>() > getResources )
{
    while ( for_ages() )
    {
        ::clearExpiredReservations();

        QList<QUuid> free_resources = getResources();
     
        if (free_resources.isEmpty())
        {
            Siren::sleep(60);
            continue;
        }

        ActiveBackend active = ::reserveResource( free_resources, 60000 );
        
        if (not active.isNull())
        {
            //ok - we've got a backend - create a reservation
            //slip which will expire 'expires' milliseconds from now
            QUuid reservation_uid = QUuid::createUuid();
            
            MutexLocker lkr( &(d->datamutex) );
            
            d->reservations.insert( reservation_uid,
                                         QPair<QDateTime,ActiveBackend>(
                                            QDateTime::currentDateTime()
                                                    .addMSecs(expires),
                                            active ) );
                               
            d->free_resources.removeAll(active.UID());
                                          
            return reservation_uid;
        }
    }

    return QUuid();
}

static QList<QUuid> getAllResources()
{
    MutexLocker lkr( &(d->datamutex) );
    
    return d->free_resources;
}

/** Reserve a backend. This blocks until a backend is available (although
    may be forever if there are no backends registered on this
    resource manager). This returns the UID of a resource reservation which
    you must use to collect the backend (using the collectReservation function).
    The reservation will expire 'expires' milliseconds after this function
    returns */
QUuid ResourceManager::reserveResource(int expires)
{
    return ::pvt_reserveResource(expires, &getAllResources);
}

static QList<QUuid> getResource(const QString &description)
{
    QList<QUuid> backends;
    
    MutexLocker lkr( &(d->datamutex) );
    
    foreach (QUuid uid, d->free_resources)
    {
        if (d->resources.value(uid).description().contains(description))
            backends.append(uid);
    }
    
    return backends;
}

/** Reserve a backend that matches the description 'description' */
QUuid ResourceManager::reserveResource(const QString &description,
                                       int expires)
{
    QList<QUuid> (*get)(const QString&) = &::getResource;
    return ::pvt_reserveResource( expires, boost::bind(get,description) );
}

static QList<QUuid> pvt_reserveResources(int n, int expires,
                                         boost::function< QList<QUuid>() > getResources )
{
    if (n <= 0)
        return QList<QUuid>();

    else if (n == 1)
    {
        QList<QUuid> uids;
        uids.append( ::pvt_reserveResource(expires, getResources) );
        return uids;
    }

    QList<ActiveBackend> actives;

    while ( for_ages() )
    {
        ::clearExpiredReservations();

        QList<QUuid> free_resources = getResources();
        
        if (free_resources.isEmpty())
        {
            Siren::sleep(60);
            continue;
        }

        ActiveBackend active = ::reserveResource( free_resources, 60000 );
        
        if (not active.isNull())
        {
            actives.append(active);

            {
                MutexLocker lkr( &(d->datamutex) );
                d->free_resources.removeAll(active.UID());
            }
            
            if (actives.count() >= n)
                break;
        }
    }

    QList<QUuid> uids;
    
    if (actives.isEmpty())
        return uids;
        
    MutexLocker lkr( &(d->datamutex) );
    
    QDateTime expire_time = QDateTime::currentDateTime().addMSecs(expires);
    
    foreach (ActiveBackend active, actives)
    {
        //ok - we've got a backend - create a reservation
        //slip which will expire 'expires' milliseconds from now
        QUuid reservation_uid = QUuid::createUuid();
        
        d->reservations.insert( reservation_uid,
                                     QPair<QDateTime,ActiveBackend>(
                                        expire_time,
                                        active ) );
                                                                  
        uids.append(reservation_uid);
    }

    return uids;
}

/** Reserve n backends. This blocks until n backends are available (although
    may be forever if there are no backends registered on this
    resource manager). This returns the UID of a resource reservation which
    you must use to collect the backend (using the collectReservation function).
    The reservation will expire 'expires' milliseconds after this function
    returns */
QList<QUuid> ResourceManager::reserveResources(int n, int expires)
{
    return ::pvt_reserveResources(n, expires, &getAllResources);
}

/** Reserve 'n' backends that match the passed description. This blocks
    until all of the backends are available */
QList<QUuid> ResourceManager::reserveResources(const QString &description,
                                               int n, int expires)
{
    QList<QUuid> (*get)(const QString&) = &::getResource;
    return ::pvt_reserveResources( n, expires, boost::bind(get,description) );
}


static QUuid pvt_tryReserveResource(int ms, int expires,
                                    boost::function< QList<QUuid>() > getResources )
{
    if (ms <= 0)
    {
        ::clearExpiredReservations();
        
        QList<QUuid> free_resources = getResources();
        
        ActiveBackend active = ::reserveResource( free_resources, 0 );

        if (not active.isNull())
        {
            //ok - we've got a backend - create a reservation
            //slip which will expire 'expires' milliseconds from now
            QUuid reservation_uid = QUuid::createUuid();
            
            MutexLocker lkr( &(d->datamutex) );
            
            d->reservations.insert( reservation_uid,
                                         QPair<QDateTime,ActiveBackend>(
                                            QDateTime::currentDateTime()
                                                    .addMSecs(expires),
                                            active ) );
                               
            d->free_resources.removeAll(active.UID());
                                                                      
            return reservation_uid;
        }
    }
    else
    {
        QTime t;
        t.start();

        while ( for_ages() )
        {
            ::clearExpiredReservations();

            QList<QUuid> free_resources = getResources();
            
            if (free_resources.isEmpty())
            {
                int wait = qMin( 1 + (ms / 2), 60000 );
            
                Siren::msleep(wait);
                ms -= t.restart();
                
                if (ms <= 0)
                    break;
                else
                    continue;
            }

            ms -= t.restart();

            if (ms <= 0)
                break;

            int wait = qMin(ms, 60000);

            ActiveBackend active = ::reserveResource( free_resources, wait );
            
            if (not active.isNull())
            {
                //ok - we've got a backend - create a reservation
                //slip which will expire 'expires' milliseconds from now
                QUuid reservation_uid = QUuid::createUuid();
                
                MutexLocker lkr( &(d->datamutex) );
                
                d->reservations.insert( reservation_uid,
                                             QPair<QDateTime,ActiveBackend>(
                                                QDateTime::currentDateTime()
                                                        .addMSecs(expires),
                                                active ) );
                                   
                d->free_resources.removeAll(active.UID());
                                                                          
                return reservation_uid;
            }
        }
    }
    
    return QUuid();
}

/** Try to reserve a backend, waiting for up to 'ms' milliseconds for
    a backend to become available. The reservation will be held
    for 'expires' milliseconds once the backend is available */
QUuid ResourceManager::tryReserveResource(int ms, int expires)
{
    return ::pvt_tryReserveResource(ms, expires, &getAllResources);
}

/** Try to reserve a backend that matches the description 'description'
    for up to 'ms' milliseconds */
QUuid ResourceManager::tryReserveResource(int ms,
                                          const QString &description,
                                          int expires)
{
    QList<QUuid> (*get)(const QString&) = &::getResource;
    return ::pvt_reserveResource( expires, boost::bind(get,description) );
}

static QList<QUuid> pvt_tryReserveResources(int n, int ms, int expires,
                                         boost::function< QList<QUuid>() > getResources )
{
    if (n <= 0)
        return QList<QUuid>();

    else if (n == 1)
    {
        QList<QUuid> uids;
        uids.append( ::pvt_tryReserveResource(ms, expires, getResources) );
        return uids;
    }
    else if (ms <= 0)
    {
        QList<ActiveBackend> actives;
        
        ::clearExpiredReservations();
        
        for (int i=0; i<n; ++i)
        {
            QList<QUuid> free_resources = getResources();

            ActiveBackend active = ::reserveResource(free_resources, 0);
            
            if (not active.isNull())
                actives.append(active);
        }
        
        QList<QUuid> uids;
        
        if (actives.isEmpty())
            return uids;
            
        MutexLocker lkr( &(d->datamutex) );
        
        QDateTime expire_time = QDateTime::currentDateTime().addMSecs(expires);
        
        foreach (ActiveBackend active, actives)
        {
            //ok - we've got a backend - create a reservation
            //slip which will expire 'expires' milliseconds from now
            QUuid reservation_uid = QUuid::createUuid();
            
            d->reservations.insert( reservation_uid,
                                         QPair<QDateTime,ActiveBackend>(
                                            expire_time,
                                            active ) );
                                                                      
            uids.append(reservation_uid);
        }

        return uids;
    }
    else
    {
        QList<ActiveBackend> actives;

        QTime t;
        t.start();

        while ( for_ages() )
        {
            ::clearExpiredReservations();

            QList<QUuid> free_resources = getResources();
            
            ms -= t.restart();
            
            if (ms <= 0)
                break;
            
            int wait = qMin(ms, 60000);
            
            if (free_resources.isEmpty())
            {
                Siren::msleep( wait / 2 );
                continue;
            }

            ActiveBackend active = ::reserveResource( free_resources, wait );
            
            if (not active.isNull())
            {
                actives.append(active);

                {
                    MutexLocker lkr( &(d->datamutex) );
                    d->free_resources.removeAll(active.UID());
                }
                
                if (actives.count() >= n)
                    break;
            }
        }

        QList<QUuid> uids;
        
        if (actives.isEmpty())
            return uids;
            
        MutexLocker lkr( &(d->datamutex) );
        
        QDateTime expire_time = QDateTime::currentDateTime().addMSecs(expires);
        
        foreach (ActiveBackend active, actives)
        {
            //ok - we've got a backend - create a reservation
            //slip which will expire 'expires' milliseconds from now
            QUuid reservation_uid = QUuid::createUuid();
            
            d->reservations.insert( reservation_uid,
                                         QPair<QDateTime,ActiveBackend>(
                                            expire_time,
                                            active ) );
                                                                      
            uids.append(reservation_uid);
        }

        return uids;
    }
}

/** Try to reserve up to 'n' backends for 'ms' milliseconds. This will
    return as many backends as could be reserved in this time, with
    their reservations set to expire 'expires' milliseconds after 
    all of the backends were acquired */
QList<QUuid> ResourceManager::tryReserveResources(int n, int ms, int expires)
{
    return ::pvt_tryReserveResources(n, ms, expires, &getAllResources);
}

/** Try to reserve up to 'n' backends that match the description 'description',
    returning all matching backends that are obtained within 'ms' milliseconds */
QList<QUuid> ResourceManager::tryReserveResources(const QString &description,
                                                  int n, int ms, int expires)
{
    QList<QUuid> (*get)(const QString&) = &::getResource;
    return ::pvt_tryReserveResources( n, ms, expires, boost::bind(get,description) );
}

/** Collect the reservation with UID 'uid'. This returns a null ActiveBackend
    if this reservation doesn't exist or has expired */
ActiveBackend ResourceManager::collectReservation(const QUuid &uid)
{
    MutexLocker lkr( &(d->datamutex) );
    
    QPair<QDateTime,ActiveBackend> reservation = d->reservations.take(uid);
    
    if (reservation.first > QDateTime::currentDateTime())
        return reservation.second;
    
    else
    {
        return ActiveBackend();
    }
}

/** Collect the reservations with UIDs in 'uids'. This returns null ActiveBackends
    for reservations that have either expired or are not valid */
QHash<QUuid,ActiveBackend> ResourceManager::collectReservation(const QList<QUuid> &uids)
{
    QHash<QUuid,ActiveBackend> backends;
    
    MutexLocker lkr( &(d->datamutex) );
    
    QDateTime now = QDateTime::currentDateTime();
    
    foreach (QUuid uid, uids)
    {
        QPair<QDateTime,ActiveBackend> reservation = d->reservations.take(uid);
        
        if (reservation.first > now)
            backends.insert( uid, reservation.second );
        else
            backends.insert( uid, ActiveBackend() );
    }
    
    return backends;
}

/** Release the reservation with UID 'uid' */
void ResourceManager::releaseReservation(const QUuid &uid)
{
    MutexLocker lkr( &(d->datamutex) );
    d->reservations.remove(uid);
}

/** Release the reservations with UIDs in 'uids' */
void ResourceManager::releaseReservation(const QList<QUuid> &uids)
{
    MutexLocker lkr( &(d->datamutex) );
    
    foreach (QUuid uid, uids)
    {
        d->reservations.remove(uid);
    }
}
