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

using namespace SireCluster;
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
};

ResourceManagerData::ResourceManagerData() : datamutex( QMutex::Recursive )
{}

ResourceManagerData::~ResourceManagerData()
{}

Q_GLOBAL_STATIC( ResourceManagerData, data );

/** Call this function to register a backend with this resource manager */
void ResourceManager::registerBackend(const DormantBackend &backend)
{
    MutexLocker lkr( &(data()->datamutex) );

    if (not data()->resources.contains(backend.UID()))
    {
        if (not backend.UID().isNull())
            throw Siren::program_bug( QObject::tr(
                "How are we registering an unregistered backend "
                "(%1) which already has a UID? (%2)")
                    .arg(backend.description())
                    .arg(backend.UID().toString()), CODELOC );

        backend.setUID( QUuid::createUuid() );

        data()->resources.insert(backend.UID(),backend);
        data()->free_resources.append(backend.UID());
    }
}

/** Unregister the backend 'backend' - this will prevent this backend
    from being available to anyone else and will deactivate it */
void ResourceManager::unregisterBackend(ActiveBackend &backend)
{
    MutexLocker lkr( &(data()->datamutex) );

    if (data()->resources.contains(backend.UID()))
    {
        data()->resources.remove(backend.UID());
        data()->free_resources.removeAll(backend.UID());
        
        QMutableHashIterator< QUuid,QPair<QDateTime,ActiveBackend> >
                        it( data()->reservations );
                        
        while (it.hasNext())
        {
            it.next();
            
            if (it.value().second.UID() == backend.UID())
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
    MutexLocker lkr( &(data()->datamutex) );
    
    QHash<QUuid,QString> resources;
    
    foreach (DormantBackend backend, data()->resources)
    {
        resources.insert( backend.UID(), backend.description() );
    }
    
    return resources;
}

static ActiveBackend reserveBackend(const QList<QUuid> &backends,
                                    int ms)
{
    if (backends.isEmpty())
    {
        return ActiveBackend();
    }
    else if (backends.count() == 1)
    {
        if (ms <= 0)
            return data()->resources.value( backends.at(0) ).tryActivate();
        else
            return data()->resources.value( backends.at(0) ).tryActivate(ms);
    }
    else
    {
        int wait_ms = ms / backends.count();
        
        if (wait_ms <= 0)
        {
            foreach (QUuid backend, backends)
            {
                ActiveBackend active = data()->resources.value(backend).tryActivate();
                
                if (not active.isNull())
                    return active;
            }
        }
        else
        {
            foreach (QUuid backend, backends)
            {
                ActiveBackend active = data()->resources.value(backend)
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
    MutexLocker lkr( &(data()->datamutex) );

    QMutableHashIterator< QUuid,QPair<QDateTime,ActiveBackend> > 
            it( data()->reservations );
    
    QDateTime now = QDateTime::currentDateTime();
    
    while (it.hasNext())
    {
        it.next();
        
        if (it.value().first < now)
        {
            //the reservation has expired
            data()->free_resources.append( it.value().second.UID() );
            it.remove();
        }
    }
    
    //also check for any backends that are now free
    foreach (DormantBackend backend, data()->resources)
    {
        if (not data()->free_resources.contains(backend.UID()))
        {
            if (not backend.isActivated())
                data()->free_resources.append( backend.UID() );
        }
    }
}

static QUuid pvt_reserveBackend(int expires,
                                boost::function< QList<QUuid>() > getBackends )
{
    while ( for_ages() )
    {
        ::clearExpiredReservations();

        QList<QUuid> free_resources = getBackends();
        
        if (free_resources.isEmpty())
        {
            Siren::sleep(60);
            continue;
        }

        ActiveBackend active = ::reserveBackend( getBackends(), 60000 );
        
        if (not active.isNull())
        {
            //ok - we've got a backend - create a reservation
            //slip which will expire 'expires' milliseconds from now
            QUuid reservation_uid = QUuid::createUuid();
            
            MutexLocker lkr( &(data()->datamutex) );
            
            data()->reservations.insert( reservation_uid,
                                         QPair<QDateTime,ActiveBackend>(
                                            QDateTime::currentDateTime()
                                                    .addMSecs(expires),
                                            active ) );
                               
            data()->free_resources.removeAll(active.UID());
                                                                      
            return reservation_uid;
        }
    }

    return QUuid();
}

static QList<QUuid> getAllBackends()
{
    MutexLocker lkr( &(data()->datamutex) );
    
    return data()->free_resources;
}

/** Reserve a backend. This blocks until a backend is available (although
    may be forever if there are no backends registered on this
    resource manager). This returns the UID of a resource reservation which
    you must use to collect the backend (using the collectReservation function).
    The reservation will expire 'expires' milliseconds after this function
    returns */
QUuid ResourceManager::reserveBackend(int expires)
{
    return ::pvt_reserveBackend(expires, &getAllBackends);
}

static QList<QUuid> getBackend(const QUuid &uid)
{
    QList<QUuid> backends;
    
    MutexLocker lkr( &(data()->datamutex) );
    
    if (data()->free_resources.contains(uid))
    {
        backends.append(uid);
    }
    
    return backends;
}

/** Reserve the backend with specific UID 'uid' */
QUuid ResourceManager::reserveBackend(const QUuid &uid, int expires)
{
    QList<QUuid> (*get)(const QUuid&) = &::getBackend;
    return ::pvt_reserveBackend( expires, boost::bind(get,uid) );
}

static QList<QUuid> getBackend(const QString &description)
{
    QList<QUuid> backends;
    
    MutexLocker lkr( &(data()->datamutex) );
    
    foreach (QUuid uid, data()->free_resources)
    {
        if (data()->resources.value(uid).description().contains(description))
            backends.append(uid);
    }
    
    return backends;
}

/** Reserve a backend that matches the description 'description' */
QUuid ResourceManager::reserveBackend(const QString &description,
                                      int expires)
{
    QList<QUuid> (*get)(const QString&) = &::getBackend;
    return ::pvt_reserveBackend( expires, boost::bind(get,description) );
}

static QList<QUuid> pvt_reserveBackends(int n, int expires,
                                        boost::function< QList<QUuid>() > getBackends )
{
    if (n <= 0)
        return QList<QUuid>();

    else if (n == 1)
    {
        QList<QUuid> uids;
        uids.append( ::pvt_reserveBackend(expires, getBackends) );
        return uids;
    }

    QList<ActiveBackend> actives;

    while ( for_ages() )
    {
        ::clearExpiredReservations();

        QList<QUuid> free_resources = getBackends();
        
        if (free_resources.isEmpty())
        {
            Siren::sleep(60);
            continue;
        }

        ActiveBackend active = ::reserveBackend( getBackends(), 60000 );
        
        if (not active.isNull())
        {
            actives.append(active);

            {
                MutexLocker lkr( &(data()->datamutex) );
                data()->free_resources.removeAll(active.UID());
            }
            
            if (actives.count() >= n)
                break;
        }
    }

    QList<QUuid> uids;
    
    if (actives.isEmpty())
        return uids;
        
    MutexLocker lkr( &(data()->datamutex) );
    
    QDateTime expire_time = QDateTime::currentDateTime().addMSecs(expires);
    
    foreach (ActiveBackend active, actives)
    {
        //ok - we've got a backend - create a reservation
        //slip which will expire 'expires' milliseconds from now
        QUuid reservation_uid = QUuid::createUuid();
        
        data()->reservations.insert( reservation_uid,
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
QList<QUuid> ResourceManager::reserveBackends(int n, int expires)
{
    return ::pvt_reserveBackends(n, expires, &getAllBackends);
}

static QList<QUuid> getBackends(const QList<QUuid> &uids)
{
    QList<QUuid> backends;
    
    MutexLocker lkr( &(data()->datamutex) );

    foreach (QUuid uid, uids)
    {
        if (data()->free_resources.contains(uid))
        {
            backends.append(uid);
        }
    }
    
    return backends;
}

/** Reserve the backends whose UID's are listed in 'uids'. This blocks
    until all of these backends are available */
QList<QUuid> ResourceManager::reserveBackends(const QList<QUuid> &uids,
                                              int expires)
{
    QList<QUuid> (*get)(const QList<QUuid>&) = &::getBackends;
    return ::pvt_reserveBackends( uids.count(), expires, 
                                  boost::bind(get,uids) );
}

/** Reserve 'n' backends that match the passed description. This blocks
    until all of the backends are available */
QList<QUuid> ResourceManager::reserveBackends(const QString &description,
                                              int n, int expires)
{
    QList<QUuid> (*get)(const QString&) = &::getBackend;
    return ::pvt_reserveBackends( n, expires, boost::bind(get,description) );
}


static QUuid pvt_tryReserveBackend(int ms, int expires,
                                   boost::function< QList<QUuid>() > getBackends )
{
    QTime t;
    t.start();

    while ( for_ages() )
    {
        ::clearExpiredReservations();

        QList<QUuid> free_resources = getBackends();
        
        if (free_resources.isEmpty())
        {
            Siren::sleep(60);
            continue;
        }

        ms -= t.restart();

        if (ms <= 0)
            break;

        int wait = qMin(ms, 60000);

        ActiveBackend active = ::reserveBackend( getBackends(), wait );
        
        if (not active.isNull())
        {
            //ok - we've got a backend - create a reservation
            //slip which will expire 'expires' milliseconds from now
            QUuid reservation_uid = QUuid::createUuid();
            
            MutexLocker lkr( &(data()->datamutex) );
            
            data()->reservations.insert( reservation_uid,
                                         QPair<QDateTime,ActiveBackend>(
                                            QDateTime::currentDateTime()
                                                    .addMSecs(expires),
                                            active ) );
                               
            data()->free_resources.removeAll(active.UID());
                                                                      
            return reservation_uid;
        }
    }

    return QUuid();
}

/** Try to reserve a backend, waiting for up to 'ms' milliseconds for
    a backend to become available. The reservation will be held
    for 'expires' milliseconds once the backend is available */
QUuid ResourceManager::tryReserveBackend(int ms, int expires)
{
    return ::pvt_tryReserveBackend(ms, expires, &getAllBackends);
}

/** Try to reserve the backend with UID 'uid' for up to 'ms' milliseconds */
QUuid ResourceManager::tryReserveBackend(int ms, const QUuid &uid,
                                         int expires)
{
    QList<QUuid> (*get)(const QUuid&) = &::getBackend;
    return ::pvt_tryReserveBackend( ms, expires, boost::bind(get,uid) );
}

/** Try to reserve a backend that matches the description 'description'
    for up to 'ms' milliseconds */
QUuid ResourceManager::tryReserveBackend(int ms,
                                         const QString &description,
                                         int expires)
{
    QList<QUuid> (*get)(const QString&) = &::getBackend;
    return ::pvt_reserveBackend( expires, boost::bind(get,description) );
}

static QList<QUuid> pvt_tryReserveBackends(int n, int ms, int expires,
                                           boost::function< QList<QUuid>() > getBackends )
{
    if (n <= 0)
        return QList<QUuid>();

    else if (n == 1)
    {
        QList<QUuid> uids;
        uids.append( ::pvt_tryReserveBackend(ms, expires, getBackends) );
        return uids;
    }

    QList<ActiveBackend> actives;

    QTime t;
    t.start();

    while ( for_ages() )
    {
        ::clearExpiredReservations();

        QList<QUuid> free_resources = getBackends();
        
        ms -= t.restart();
        
        if (ms <= 0)
            break;
        
        int wait = qMin(ms, 60000);
        
        if (free_resources.isEmpty())
        {
            Siren::msleep( wait / 2 );
            continue;
        }

        ActiveBackend active = ::reserveBackend( getBackends(), wait );
        
        if (not active.isNull())
        {
            actives.append(active);

            {
                MutexLocker lkr( &(data()->datamutex) );
                data()->free_resources.removeAll(active.UID());
            }
            
            if (actives.count() >= n)
                break;
        }
    }

    QList<QUuid> uids;
    
    if (actives.isEmpty())
        return uids;
        
    MutexLocker lkr( &(data()->datamutex) );
    
    QDateTime expire_time = QDateTime::currentDateTime().addMSecs(expires);
    
    foreach (ActiveBackend active, actives)
    {
        //ok - we've got a backend - create a reservation
        //slip which will expire 'expires' milliseconds from now
        QUuid reservation_uid = QUuid::createUuid();
        
        data()->reservations.insert( reservation_uid,
                                     QPair<QDateTime,ActiveBackend>(
                                        expire_time,
                                        active ) );
                                                                  
        uids.append(reservation_uid);
    }

    return uids;
}

/** Try to reserve up to 'n' backends for 'ms' milliseconds. This will
    return as many backends as could be reserved in this time, with
    their reservations set to expire 'expires' milliseconds after 
    all of the backends were acquired */
QList<QUuid> ResourceManager::tryReserveBackends(int n, int ms, int expires)
{
    return ::pvt_tryReserveBackends(n, ms, expires, &getAllBackends);
}

/** Try to reserve the backends whose UIDs are in 'uids', for up to 'ms'
    milliseconds. This will return reservations for all of the backends
    that were successfully reserved */
QList<QUuid> ResourceManager::tryReserveBackends(const QList<QUuid> &uids, int ms,
                                                 int expires)
{
    QList<QUuid> (*get)(const QList<QUuid>&) = &::getBackends;
    return ::pvt_tryReserveBackends( uids.count(), ms, expires, boost::bind(get,uids) );
}

/** Try to reserve up to 'n' backends that match the description 'description',
    returning all matching backends that are obtained within 'ms' milliseconds */
QList<QUuid> ResourceManager::tryReserveBackends(const QString &description,
                                                 int n, int ms, int expires)
{
    QList<QUuid> (*get)(const QString&) = &::getBackend;
    return ::pvt_tryReserveBackends( n, ms, expires, boost::bind(get,description) );
}

/** Collect the reservation with UID 'uid'. This returns a null ActiveBackend
    if this reservation doesn't exist or has expired */
ActiveBackend ResourceManager::collectReservation(const QUuid &uid)
{
    MutexLocker lkr( &(data()->datamutex) );
    
    QPair<QDateTime,ActiveBackend> reservation = data()->reservations.take(uid);
    
    if (reservation.first < QDateTime::currentDateTime())
        return reservation.second;
    
    else
        return ActiveBackend();
}

/** Collect the reservations with UIDs in 'uids'. This returns null ActiveBackends
    for reservations that have either expired or are not valid */
QHash<QUuid,ActiveBackend> ResourceManager::collectReservation(const QList<QUuid> &uids)
{
    QHash<QUuid,ActiveBackend> backends;
    
    MutexLocker lkr( &(data()->datamutex) );
    
    QDateTime now = QDateTime::currentDateTime();
    
    foreach (QUuid uid, uids)
    {
        QPair<QDateTime,ActiveBackend> reservation = data()->reservations.take(uid);
        
        if (reservation.first < now)
            backends.insert( uid, reservation.second );
        else
            backends.insert( uid, ActiveBackend() );
    }
    
    return backends;
}

/** Release the reservation with UID 'uid' */
void ResourceManager::releaseReservation(const QUuid &uid)
{
    MutexLocker lkr( &(data()->datamutex) );
    data()->reservations.remove(uid);
}

/** Release the reservations with UIDs in 'uids' */
void ResourceManager::releaseReservation(const QList<QUuid> &uids)
{
    MutexLocker lkr( &(data()->datamutex) );
    
    foreach (QUuid uid, uids)
    {
        data()->reservations.remove(uid);
    }
}
