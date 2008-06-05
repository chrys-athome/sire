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

#include "moleculegroups.h"

#include "mover.hpp"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MoleculeGroups> r_molgroups;

/** Serialise a MoleculeGroups to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const MoleculeGroups &molgroups)
{
    writeHeader(ds, r_molgroups, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(molgroups);
    
    return ds;
}

/** Deserialise a MoleculeGroups from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       MoleculeGroups &molgroups)
{
    VersionID v = readHeader(ds, r_molgroups);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(molgroups);
    }
    else
        throw version_error(v, "1", r_molgroups, CODELOC);
        
    return ds;
}

static MoleculeGroups *_pvt_shared_null = 0;

const MoleculeGroups& MoleculeGroups::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new MoleculeGroups( MolGroups() );
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, empty MolGroups() object */
MoleculeGroups::MoleculeGroups()
               : Property(MoleculeGroups::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
MoleculeGroups::MoleculeGroups(const PropertyBase &property)
               : Property(property.asA<MolGroupsBase>())
{}

/** Construct from passed MolGroupsBase */
MoleculeGroups::MoleculeGroups(const MolGroupsBase &molgroups)
               : Property(molgroups)
{} 

/** Copy constructor */
MoleculeGroups::MoleculeGroups(const MoleculeGroups &other)
               : Property(other)
{}

/** Destructor */
MoleculeGroups::~MoleculeGroups()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
MoleculeGroups& MoleculeGroups::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<MolGroupsBase>());
    return *this;
}

/** Copy assignment operator from another MolGroupsBase */
MoleculeGroups& MoleculeGroups::operator=(const MolGroupsBase &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const MolGroupsBase* MoleculeGroups::operator->() const
{
    return static_cast<const MolGroupsBase*>(&(d()));
}

/** Dereference this pointer */
const MolGroupsBase& MoleculeGroups::operator*() const
{
    return static_cast<const MolGroupsBase&>(d());
}

/** Return a read-only reference to the contained object */
const MolGroupsBase& MoleculeGroups::read() const
{
    return static_cast<const MolGroupsBase&>(d());
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
MolGroupsBase& MoleculeGroups::edit()
{
    return static_cast<MolGroupsBase&>(d());
}
    
/** Return a raw pointer to the MolGroupsBase object */
const MolGroupsBase* MoleculeGroups::data() const
{
    return static_cast<const MolGroupsBase*>(&(d()));
}

/** Return a raw pointer to the MolGroupsBase object */
const MolGroupsBase* MoleculeGroups::constData() const
{
    return static_cast<const MolGroupsBase*>(&(d()));
}
    
/** Return a raw pointer to the MolGroupsBase object */
MolGroupsBase* MoleculeGroups::data()
{
    return static_cast<MolGroupsBase*>(&(d()));
}

/** Automatic casting operator */
MoleculeGroups::operator const MolGroupsBase&() const
{
    return static_cast<const MolGroupsBase&>(d());
}
