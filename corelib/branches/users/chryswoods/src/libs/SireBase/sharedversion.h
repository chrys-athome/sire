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

#ifndef SIREBASE_SHAREDVERSION_H
#define SIREBASE_SHAREDVERSION_H

#include "Siren/handle.h"

#include "version.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

namespace detail
{

/** Private class used by SharedVersion to hold the version data
    of the last incremented version */
class SIREBASE_EXPORT VersionData
{
public:
    VersionData() : last_major_version(0), last_minor_version(0)
    {}
    
    VersionData(quint64 majv, quint64 minv)
           : last_major_version(majv), last_minor_version(minv)
    {}
    
    ~VersionData()
    {}
    
    quint64 last_major_version;
    quint64 last_minor_version;
};

} // end of namespace detail

/** This is a class that provides a version numbering scheme that
    is guaranteed to provide unique version numbers, even for 
    different copies of this object 
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT SharedVersion 
        : public Siren::ImplementsHandle< SharedVersion,
                                          Siren::Handles<detail::VersionData> >
{
public:
    SharedVersion();
    SharedVersion(quint64 vmaj, quint64 vmin);
    
    SharedVersion(const SharedVersion &other);
    
    ~SharedVersion();
    
    SharedVersion& operator=(const SharedVersion &other);
    
    bool operator==(const SharedVersion &other) const;
    bool operator!=(const SharedVersion &other) const;

    QString toString() const;
    uint hashCode() const;
    
    void incrementMajor();
    void incrementMinor();
    
    const Version& version() const;
    
    quint64 majorVersion() const;
    quint64 minorVersion() const;
    
private:
    /** The major and minor version of this object */
    Version v;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Return the actual version */
inline const Version& SharedVersion::version() const
{
    return v;
}

/** Return the major version number */
inline quint64 SharedVersion::majorVersion() const
{
    return v.majorVersion();
}

/** Return the minor version number */
inline quint64 SharedVersion::minorVersion() const
{
    return v.minorVersion();
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

} // end of namespace SireBase

Q_DECLARE_METATYPE( SireBase::SharedVersion )

SIRE_EXPOSE_CLASS( SireBase::SharedVersion )

SIRE_END_HEADER

#endif
