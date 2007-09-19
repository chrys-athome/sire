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

#ifndef SIREID_Identifier_H
#define SIREID_Identifier_H

#include "id.h"

#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireID
{
template<class S, class T>
class Identifier_T_;
}

template<class S, class T>
QDataStream& operator<<(QDataStream&, const SireID::Identifier_T_<S,T>&);
template<class S, class T>
QDataStream& operator>>(QDataStream&, SireID::Identifier_T_<S,T>&);

template<class S, class T>
XMLStream& operator<<(XMLStream&, const SireID::Identifier_T_<S,T>&);
template<class S, class T>
XMLStream& operator>>(XMLStream&, SireID::Identifier_T_<S,T>&);

namespace SireID
{

namespace detail
{

/** Private base class of Identifier_T_<T> */
class Identifier_T_Base
{
protected:
    Identifier_T_Base()
    {}
    
    ~Identifier_T_Base()
    {}
    
    void throwNullIDError() const;
    void throwVersionError(SireStream::VersionID v,
                           const QString &supported_versions,
                           const RegisterMetaTypeBase &r_type) const;
};

}

/** This is a generic holder that is capable of holding any
    class that is derived from 'ID'. This is useful if a generic
    function wants to return a generic ID object.
    
    @author Christopher Woods
*/
template<class S, class T>
class SIREID_EXPORT Identifier_T_ : public T, private detail::Identifier_T_Base
{

friend QDataStream& operator<<<>(QDataStream&, const Identifier_T_<S,T>&);
friend QDataStream& operator>><>(QDataStream&, Identifier_T_<S,T>&);

friend XMLStream& operator<<<>(XMLStream&, const Identifier_T_<S,T>&);
friend XMLStream& operator>><>(XMLStream&, Identifier_T_<S,T>&);

static QString typenam;

public:
    Identifier_T_() : idptr(0)
    {}
    
    Identifier_T_(const T &idobj) : idptr( idobj.clone() )
    {}
    
    Identifier_T_(const Identifier_T_<S,T> &other) : idptr( other.idptr->clone() )
    {}
    
    ~Identifier_T_()
    {
        delete idptr;
    }
    
    S& operator=(const Identifier_T_<S,T> &other)
    {
        if (&other != this)
        {
            delete idptr;
            idptr = other.idptr->clone();
        }
        
        return static_cast<S&>(*this);
    }
    
    S& operator=(const ID &idobj)
    {
        if (&idobj != idptr)
        {
            delete idptr;
            idptr = idobj.clone();
        }
        
        return static_cast<S&>(*this);
    }
    
    bool operator==(const Identifier_T_<S,T> &other) const
    {
        if (idptr == 0 or other.idptr == 0)
            return idptr == other.idptr;
        else
            return *idptr == *(other.idptr);
    }
    
    bool operator==(const ID &idobj) const
    {
        return idptr != 0 and *idptr == idobj;
    }
    
    bool operator!=(const Identifier_T_<S,T> &other) const
    {
        if (idptr == 0 or other.idptr == 0)
            return idptr != other.idptr;
        else
            return *idptr != *(other.idptr);
    }
    
    bool operator!=(const ID &idobj) const
    {
        return idptr == 0 or *idptr != idobj;
    }
    
    bool isNull() const
    {
        return idptr == 0;
    }
    
    void assertNotNull() const
    {
        if (idptr == 0)
            this->throwNullIDError();
    }
    
    T* clone() const
    {
        if (idptr == 0)
            return new S();
        else
            return idptr->clone();
    }
    
    uint hash() const
    {
        if (idptr == 0)
            return 0;
        else
            return idptr->hash();
    }

    QString toString() const
    {
        if (idptr == 0)
            return QString::null;
        else
            return idptr->toString();
    }
    
    static const char* typeName()
    {
        return qPrintable( QString(Identifier_T_<S,T>::typenam) );
    }
    
    const char* what() const
    {
        if (idptr == 0)
            return Identifier_T_<S,T>::typeName();
        else
            return idptr->what();
    }
    
    template<class X>
    bool isA() const
    {
        if (idptr != 0)
            return idptr->isA<X>();
        else
            return 0;
    }
    
    template<class X>
    const X& asA() const
    {
        this->assertNotNull();
    
        return idptr->asA<X>();
    }
    
    template<class X>
    X& asA()
    {
        this->assertNotNull();
    
        return idptr->asA<X>();
    }
    
    QDataStream& save(QDataStream &ds, 
                      const RegisterMetaTypeBase &r_type) const
    {
        SireStream::writeHeader(ds, r_type, 1);
        
        return ds;
    }
    
    QDataStream& load(QDataStream &ds, 
                      const RegisterMetaTypeBase &r_type)
    {
        SireStream::VersionID v = SireStream::readHeader(ds, r_type);
        
        if (v == 1)
        {
        }
        else
            Identifier_T_Base::throwVersionError(v, "1", r_type);
            
        return ds;
    }
    
private:
    /** Pointer to the actual ID object */
    T *idptr;
}; 

template<class S, class T>
QString Identifier_T_<S,T>::typenam = QString("Identifier_T_<%1>").arg(T::typeName());

class Identifier : public Identifier_T_<Identifier,ID>
{
public:
    Identifier() : Identifier_T_<Identifier,ID>()
    {}
    
    Identifier(const ID &id) : Identifier_T_<Identifier,ID>(id)
    {}
    
    Identifier(const Identifier &other)
           : Identifier_T_<Identifier,ID>(other)
    {}
    
    ~Identifier()
    {}
};

}

QDataStream& operator<<(QDataStream&, const SireID::Identifier&);
QDataStream& operator>>(QDataStream&, SireID::Identifier&);

XMLStream& operator<<(XMLStream&, const SireID::Identifier&);
XMLStream& operator>>(XMLStream&, SireID::Identifier&);

uint qHash(const SireID::Identifier &id);

Q_DECLARE_METATYPE(SireID::Identifier);

SIRE_END_HEADER

#endif
