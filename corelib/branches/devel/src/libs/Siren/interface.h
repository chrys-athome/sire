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

#ifndef SIREN_INTERFACE_H
#define SIREN_INTERFACE_H

#include <QStringList>

#include "sirenglobal.h"

SIREN_BEGIN_HEADER

namespace Siren
{

/** This is the base class of all interfaces. Interfaces
    are pure virtual base classes that provide the only
    means by which multiple inheritance may be used
    with Siren::Objects. An Interface is very similar
    to an Interface in Java
    
    @author Christopher Woods
*/
class SIREN_EXPORT Interface
{
public:
    Interface();
    
    virtual ~Interface();

protected:
    static bool IS_INTERFACE()
    {
        return true;
    }
};

namespace detail
{

class NONE_BASE
{
protected:
    static bool IS_INTERFACE()
    {
        return false;
    }
    
    static QString typeName()
    {
        return QString::null;
    }
};

class NONE_1 : public NONE_BASE {};
class NONE_2 : public NONE_BASE {};
class NONE_3 : public NONE_BASE {};
class NONE_4 : public NONE_BASE {};
class NONE_5 : public NONE_BASE {};
class NONE_6 : public NONE_BASE {};
class NONE_7 : public NONE_BASE {};

}

/** Use this class to provide the list of interfaces that an
    object or handle inherits, e.g.
    
    class MyObject : public Implements<MyObject,Object>,
                     public Interfaces<MyInterface,MyOtherInterface>

    class MyHandle : public Handles<MyHandle,HandleData>,
                     public Interfaces<MyInterface,MyOtherInterface>

    @author Christopher Woods
*/
template<class Derived, 
         class I0,                class I1=detail::NONE_1, 
         class I2=detail::NONE_2, class I3=detail::NONE_3, 
         class I4=detail::NONE_4, class I5=detail::NONE_5,
         class I6=detail::NONE_6, class I7=detail::NONE_7>
class SIREN_EXPORT Interfaces
        : public virtual I0, public virtual I1,
          public virtual I2, public virtual I3,
          public virtual I4, public virtual I5,
          public virtual I6, public virtual I7 
{
public:
    Interfaces() : I0(), I1(), I2(), I3(),
                   I4(), I5(), I6(), I7()
    {}
    
    ~Interfaces()
    {}

    static QString typeName()
    {
        return Derived::typeName();
    }

protected:
    static bool IS_INTERFACE()
    {
        return false;
    }

    static QStringList listInterfaces()
    {
        QStringList ifaces;
    
        if (I0::IS_INTERFACE())
            ifaces.append( I0::typeName() );
        
        if (I1::IS_INTERFACE())
            ifaces.append( I0::typeName() );

        if (I2::IS_INTERFACE())
            ifaces.append( I0::typeName() );

        if (I3::IS_INTERFACE())
            ifaces.append( I0::typeName() );

        if (I4::IS_INTERFACE())
            ifaces.append( I0::typeName() );

        if (I5::IS_INTERFACE())
            ifaces.append( I0::typeName() );

        if (I6::IS_INTERFACE())
            ifaces.append( I0::typeName() );

        if (I7::IS_INTERFACE())
            ifaces.append( I0::typeName() );

        return ifaces;
    }
};

}

SIREN_END_HEADER

#endif
