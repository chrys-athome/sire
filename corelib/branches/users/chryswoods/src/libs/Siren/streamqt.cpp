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

#include <QDateTime>
#include <QUuid>

#include "streamqt.h"

#include "Siren/errors.h"

using namespace Siren;

namespace Siren
{
    namespace detail
    {
        template<>
        struct StreamHelper<QUuid>
        {
            static QString typeName(){ return "QUuid"; }
            
            static const void* getKey(const QUuid &object)
            {
                return &object;
            }
            
            static QUuid null(){ return QUuid(); }
        };

        template<>
        struct StreamHelper<QDateTime>
        {
            static QString typeName(){ return "QDateTime"; }
            
            static const void* getKey(const QDateTime &object)
            {
                return &object;
            }
            
            static QDateTime null(){ return QDateTime(); }
        };

        template<>
        struct StreamHelper<QDate>
        {
            static QString typeName(){ return "QDate"; }
            
            static const void* getKey(const QDate &object)
            {
                return &object;
            }
            
            static QDate null(){ return QDate(); }
        };

        template<>
        struct StreamHelper<QTime>
        {
            static QString typeName(){ return "QTime"; }
            
            static const void* getKey(const QTime &object)
            {
                return &object;
            }
            
            static QTime null(){ return QTime(); }
        };
    }
}

Siren::Stream SIREN_EXPORT &operator&(Siren::Stream &s, QDate &d)
{
    s.assertVersion<QDate>(1);
    
    Schema schema = s.item<QDate>();
    
    quint32 julian_day;
    
    if (s.isSaving())
    {
        julian_day = d.toJulianDay();
    }
    
    schema.data("julian_day") & julian_day;
    
    if (s.isLoading())
    {
        d = QDate::fromJulianDay(julian_day);
    }

    return s;
}

static const QTime midnight(0,0,0,0);

Siren::Stream SIREN_EXPORT &operator&(Siren::Stream &s, QTime &t)
{
    s.assertVersion<QTime>(1);
    
    Schema schema = s.item<QTime>();
    
    quint32 ms;
    
    if (s.isSaving())
    {
        ms = midnight.msecsTo(t);
    }
    
    schema.data("msecs") & ms;
    
    if (s.isLoading())
    {
        t = midnight.addMSecs(ms);
    }

    return s;
}

Siren::Stream SIREN_EXPORT &operator&(Siren::Stream &s, QDateTime &d)
{
    s.assertVersion<QDateTime>(1);
    
    Schema schema = s.item<QDateTime>();
    
    QDate date;
    QTime time;
    quint8 timespec;
    
    if (s.isSaving())
    {
        date = d.date();
        time = d.time();
        timespec = d.timeSpec();
    }

    schema.data("date") & date;
    schema.data("time") & time;
    schema.data("timespec") & timespec;
    
    if (s.isLoading())
    {
        d = QDateTime(date, time, (Qt::TimeSpec)(timespec));
    }

    return s;
}

Siren::Stream SIREN_EXPORT &operator&(Siren::Stream &s, QUuid &uid)
{
    s.assertVersion<QUuid>(1);
    
    Schema schema = s.item<QUuid>();
    
    QString uid_string;
    
    if (s.isSaving())
    {
        uid_string = uid.toString();
    }
    
    schema.data("uid") & uid_string;
    
    if (s.isLoading())
    {
        uid = QUuid(uid_string);
    }

    return s;
}
