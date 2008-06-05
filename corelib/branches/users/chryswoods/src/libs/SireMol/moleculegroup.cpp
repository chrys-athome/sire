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

#include "moleculegroup.h"

#include "mover.hpp"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MoleculeGroup> r_molgroup;

/** Serialise a MoleculeGroup to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const MoleculeGroup &molgroup)
{
    writeHeader(ds, r_molgroup, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(molgroup);
    
    return ds;
}

/** Deserialise a MoleculeGroup from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       MoleculeGroup &molgroup)
{
    VersionID v = readHeader(ds, r_molgroup);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(molgroup);
    }
    else
        throw version_error(v, "1", r_molgroup, CODELOC);
        
    return ds;
}

static MoleculeGroup *_pvt_shared_null = 0;

const MoleculeGroup& MoleculeGroup::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new MoleculeGroup( MolGroup() );
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, empty MolGroup */
MoleculeGroup::MoleculeGroup()
              : Property(MoleculeGroup::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
MoleculeGroup::MoleculeGroup(const PropertyBase &property)
              : Property(property.asA<MolGroup>())
{}

/** Construct from passed MolGroup */
MoleculeGroup::MoleculeGroup(const MolGroup &molgroup)
              : Property(molgroup)
{}

/** Copy constructor */
MoleculeGroup::MoleculeGroup(const MoleculeGroup &other)
              : Property(other)
{}

/** Destructor */
MoleculeGroup::~MoleculeGroup()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
MoleculeGroup& MoleculeGroup::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<MolGroup>());
    return *this;
}

/** Copy assignment operator from another MolGroup */
MoleculeGroup& MoleculeGroup::operator=(const MolGroup &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const MolGroup* MoleculeGroup::operator->() const
{
    return static_cast<const MolGroup*>(&(d()));
}

/** Dereference this pointer */
const MolGroup& MoleculeGroup::operator*() const
{
    return static_cast<const MolGroup&>(d());
}

/** Return a read-only reference to the contained object */
const MolGroup& MoleculeGroup::read() const
{
    return static_cast<const MolGroup&>(d());
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
MolGroup& MoleculeGroup::edit()
{
    return static_cast<MolGroup&>(d());
}
    
/** Return a raw pointer to the MolGroup object */
const MolGroup* MoleculeGroup::data() const
{
    return static_cast<const MolGroup*>(&(d()));
}

/** Return a raw pointer to the MolGroup object */
const MolGroup* MoleculeGroup::constData() const
{
    return static_cast<const MolGroup*>(&(d()));
}
    
/** Return a raw pointer to the MolGroup object */
MolGroup* MoleculeGroup::data()
{
    return static_cast<MolGroup*>(&(d()));
}

/** Automatic casting operator */
MoleculeGroup::operator const MolGroup&() const
{
    return static_cast<const MolGroup&>(d());
}
