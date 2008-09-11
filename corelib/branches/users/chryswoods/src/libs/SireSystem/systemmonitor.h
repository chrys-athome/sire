/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIRESYSTEM_SYSTEMMONITOR_H
#define SIRESYSTEM_SYSTEMMONITOR_H

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class SystemMonitor;
class SysMonBase;
class NullMonitor;
}

QDataStream& operator<<(QDataStream&, const SireSystem::SystemMonitor&);
QDataStream& operator>>(QDataStream&, SireSystem::SystemMonitor&);

QDataStream& operator<<(QDataStream&, const SireSystem::SysMonBase&);
QDataStream& operator>>(QDataStream&, SireSystem::SysMonBase&);

QDataStream& operator<<(QDataStream&, const SireSystem::NullMonitor&);
QDataStream& operator>>(QDataStream&, SireSystem::NullMonitor&);

namespace SireSystem
{

class System;

/** This is the virtual base class of all system monitors. A system
    monitor is an object that monitors a system during a simulation,
    e.g. collecting the average energy, saving a radial distribution
    function etc.
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SysMonBase : public SireBase::PropertyBase
{
public:
    SysMonBase();
    
    SysMonBase(const SysMonBase &other);
    
    virtual ~SysMonBase();
    
    static const char* typeName()
    {
        return "SireSystem::SysMonBase";
    }
    
    virtual SysMonBase* clone() const=0;
    
    virtual void monitor(System &system)=0;
};

/** This is a null monitor that doesn't monitor anything */
class SIRESYSTEM_EXPORT NullMonitor
           : public SireBase::ConcreteProperty<NullMonitor,SysMonBase>
{
public:
    NullMonitor();
    
    NullMonitor(const NullMonitor &other);
    
    ~NullMonitor();
    
    NullMonitor& operator=(const NullMonitor &other);
    
    bool operator==(const NullMonitor &other) const;
    bool operator!=(const NullMonitor &other) const;
    
    static const char *typeName()
    {
        return QMetaType::typeName( qMetaTypeId<NullMonitor>() );
    }
    
    NullMonitor* clone() const
    {
        return new NullMonitor(*this);
    }
    
    void monitor(System &system);
};

/** This is the polymorphic pointer holder for the 
    SystemMonitor hierarchy of classes (system monitors).
    
    Like all Property polymorphic pointer holder classes,
    this class holds the polymorphic SystemMonitor object as 
    an implicitly shared pointer. You can access the 
    const functions of this object by dereferencing this
    pointer, or by using the SystemMonitor::read() function, e.g.;
    
    cout << sysmon->what();
    cout << sysmon.read().what();
    
    You must use the SystemMonitor::edit() function to
    access the non-const member functions, e.g.;
    
    sysmon.edit().monitor(system);
    
    Because an implicitly shared pointer is held, this
    class can be copied and passed around quickly. A copy
    is only made when the object being pointed to is
    edited via the .edit() function.

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SystemMonitor : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const SystemMonitor&);
friend QDataStream& ::operator>>(QDataStream&, SystemMonitor&);

public:
    SystemMonitor();
    SystemMonitor(const SireBase::PropertyBase &property);
    SystemMonitor(const SysMonBase &monitor);

    SystemMonitor(const SystemMonitor &other);
    
    ~SystemMonitor();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<SystemMonitor>() );
    }
    
    const char* what() const
    {
        return SystemMonitor::typeName();
    }
    
    virtual SystemMonitor& operator=(const SireBase::PropertyBase &property);
    virtual SystemMonitor& operator=(const SysMonBase &other);

    const SysMonBase* operator->() const;
    const SysMonBase& operator*() const;
    
    const SysMonBase& read() const;
    SysMonBase& edit();
    
    const SysMonBase* data() const;
    const SysMonBase* constData() const;
    
    SysMonBase* data();
    
    operator const SysMonBase&() const;

    static const SystemMonitor& shared_null();
};

}

Q_DECLARE_METATYPE( SireSystem::SystemMonitor )
Q_DECLARE_METATYPE( SireSystem::NullMonitor )

SIRE_EXPOSE_CLASS( SireSystem::SysMonBase )
SIRE_EXPOSE_CLASS( SireSystem::NullMonitor )

SIRE_EXPOSE_PROPERTY( SireSystem::SystemMonitor, SireSystem::SysMonBase )

SIRE_END_HEADER

#endif
