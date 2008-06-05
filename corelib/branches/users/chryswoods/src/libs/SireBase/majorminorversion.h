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

#ifndef SIREBASE_MAJORMINORERROR_H
#define SIREBASE_MAJORMINORERROR_H

#include <QMutex>
#include <QMutexLocker>
#include <QHash>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

namespace detail
{

/** Private class used by MajorMinorVersion to hold the version data
    of the last incremented version */
class SIREBASE_EXPORT MajorMinorVersionData
{
public:
    MajorMinorVersionData() : last_major_version(0), last_minor_version(0)
    {}
    
    MajorMinorVersionData(quint64 majv, quint64 minv)
           : last_major_version(majv), last_minor_version(minv)
    {}
    
    ~MajorMinorVersionData()
    {}
    
    QMutex version_mutex;
    quint64 last_major_version;
    quint64 last_minor_version;
};

} // end of namespace detail

/** This is a class that provides a version numbering scheme that
    is guaranteed to provide unique version numbers, even for 
    different copies of this object 
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT MajorMinorVersion
{
public:
    MajorMinorVersion();
    MajorMinorVersion(const boost::shared_ptr<detail::MajorMinorVersionData> &other);
    
    MajorMinorVersion(quint64 vmaj, quint64 vmin);
    
    MajorMinorVersion(const MajorMinorVersion &other);
    
    ~MajorMinorVersion();
    
    MajorMinorVersion& operator=(const MajorMinorVersion &other);
    
    bool operator==(const MajorMinorVersion &other) const;
    bool operator!=(const MajorMinorVersion &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MajorMinorVersion>() );
    }
    
    const char* what() const
    {
        return MajorMinorVersion::typeName();
    }
    
    void incrementMajor();
    void incrementMinor();
    
    quint64 majorVersion() const;
    quint64 minorVersion() const;
    
    operator boost::weak_ptr<detail::MajorMinorVersionData>() const
    {
        return d;
    }
    
private:
    static boost::shared_ptr<detail::MajorMinorVersionData> shared_null;
    
    /** Shared pointer to the version data of the last incremented
        version object */
    boost::shared_ptr<detail::MajorMinorVersionData> d;
    
    /** The major version number of this object */
    quint64 major_version;
    /** The minor version number of this object */
    quint64 minor_version;
};

/** This class provides a registry of that last version number
    assigned to the objects identified by keys of type 'T' 
    
    @author Christopher Woods
*/
template<class T>
class VersionRegistry
{
public:
    VersionRegistry();
    ~VersionRegistry();

    MajorMinorVersion registerObject(const T &key);

    bool registered(const T &key);

private:
    /** Mutex used to protect access to the registry */
    QMutex registry_mutex;
    
    /** The data for the registry */
    QHash< T, boost::weak_ptr<detail::MajorMinorVersionData> > registry;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Copy assingment operator */
inline MajorMinorVersion& MajorMinorVersion::operator=(const MajorMinorVersion &other)
{
    if (this != &other)
    {
        d = other.d;
        major_version = other.major_version;
        minor_version = other.minor_version;
    }
    
    return *this;
}

/** Comparision operator */
inline bool MajorMinorVersion::operator==(const MajorMinorVersion &other) const
{
    return major_version == other.major_version and
           minor_version == other.minor_version;
}

/** Comparison operator */
inline bool MajorMinorVersion::operator!=(const MajorMinorVersion &other) const
{
    return major_version != other.major_version or
           minor_version != other.minor_version;
}

/** Return the major version number */
inline quint64 MajorMinorVersion::majorVersion() const
{
    return major_version;
}

/** Return the minor version number */
inline quint64 MajorMinorVersion::minorVersion() const
{
    return minor_version;
}

/** Constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
VersionRegistry<T>::VersionRegistry()
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
VersionRegistry<T>::~VersionRegistry()
{}

/** Return whether or not an object with key 'key' has been  
    registered */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool VersionRegistry<T>::registered(const T &key)
{
    QMutexLocker lkr(&registry_mutex);
    
    return not registry.value(key).expired();
}

/** Register the object identified by the key 'key' and return
    the version number of this object. This will be 1:0 if this
    is a new object, or one major version higher than the last
    assigned number of any other object identified by this key */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MajorMinorVersion VersionRegistry<T>::registerObject(const T &key)
{
    QMutexLocker lkr(&registry_mutex);

    if (registry.contains(key))
    {
        boost::shared_ptr<detail::MajorMinorVersionData> 
                                        d = registry.value(key).lock();
                                        
        if (d.get() != 0)
        {
            MajorMinorVersion v(d);
            v.incrementMajor();
            return v;
        }
    }
    
    //either we don't know about this object, or we used to, but
    //not any more!
    MajorMinorVersion v(1,0);
    
    registry.insert( key, v );

    if (registry.count() % 100 == 0)
    {
        //there are exactly a multiple of 100 objects in the registry
        // - so lets try and clean it up a little
        QMutableHashIterator< T,boost::weak_ptr<detail::MajorMinorVersionData> >
                            it(registry);
        
        while (it.hasNext())
        {
            it.next();
            
            if (it.value().expired())
                //this object has since been removed
                it.remove();
        }

    }
    
    return v;
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

} // end of namespace SireBase

Q_DECLARE_METATYPE( SireBase::MajorMinorVersion )

SIRE_END_HEADER

#endif
