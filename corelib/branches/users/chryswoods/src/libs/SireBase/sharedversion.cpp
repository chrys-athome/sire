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

#include "sharedversion.h"
#include "versionregistry.hpp"

using namespace SireBase;
using namespace SireBase::detail;
using namespace Siren;

static const RegisterHandle<SharedVersion> r_shared_version;

/** Null constructor */
SharedVersion::SharedVersion() 
              : ImplementsHandle< SharedVersion, Handles<VersionData> >(), v(0,0)
{}

/** Construct the object for a specific version */
SharedVersion::SharedVersion(quint64 vmaj, quint64 vmin)
              : ImplementsHandle< SharedVersion, Handles<VersionData> >(
                                                       new VersionData(vmaj,vmin) ),
                v(vmaj, vmin)
{}

/** Copy constructor */
SharedVersion::SharedVersion(const SharedVersion &other)
              : ImplementsHandle< SharedVersion, Handles<VersionData> >(other),
                v(other.v)
{}

/** Destructor */
SharedVersion::~SharedVersion()
{}

SharedVersion& SharedVersion::operator=(const SharedVersion &other)
{
    Handles<VersionData>::operator=(other);
    v = other.v;
    return *this;
}

bool SharedVersion::operator==(const SharedVersion &other) const
{
    return Handles<VersionData>::operator==(other);
}

/** Increment the major version number - this resets the 
    minor version number to 0 */
void SharedVersion::incrementMajor()
{
    HandleLocker lkr(*this);
    
    ++(resource().last_major_version);
    resource().last_minor_version = 0;
    
    v = Version(resource().last_major_version, 0);
}

/** Increment the minor version number */
void SharedVersion::incrementMinor()
{
    HandleLocker lkr(*this);
    
    ++(resource().last_minor_version);
    
    v = Version(resource().last_major_version, resource().last_minor_version);
}
