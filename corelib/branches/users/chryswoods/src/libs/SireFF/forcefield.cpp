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

#include "forcefield.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include <QDebug>

using namespace SireFF;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<ForceField> r_ff;

/** Serialise a ForceField to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds,
                                      const ForceField &ff)
{
    writeHeader(ds, r_ff, 1);
    
    SharedDataStream sds(ds);
    
    qDebug() << CODELOC;
    
    sds << static_cast<const Property&>(ff);
    
    return ds;
}

/** Deserialise a ForceField from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds,
                                      ForceField &ff)
{
    VersionID v = readHeader(ds, r_ff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);

        qDebug() << CODELOC;

        sds >> static_cast<Property&>(ff);
    }
    else
        throw version_error(v, "1", r_ff, CODELOC);
        
    return ds;
}

static ForceField *_pvt_shared_null = 0;

const ForceField& ForceField::shared_null()
{
    if (_pvt_shared_null == 0)
        #warning Probably want to write a NullFF forcefield.
        _pvt_shared_null = new ForceField();
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, NullFF() object */
ForceField::ForceField() : Property(ForceField::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
ForceField::ForceField(const PropertyBase &property)
           : Property(property.asA<FF>())
{}

/** Construct from passed FF */
ForceField::ForceField(const FF &ff) : Property(ff)
{} 

/** Copy constructor */
ForceField::ForceField(const ForceField &other)
           : Property(other)
{}

/** Destructor */
ForceField::~ForceField()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
ForceField& ForceField::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<FF>());
    return *this;
}

/** Copy assignment operator from another FF */
ForceField& ForceField::operator=(const FF &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const FF* ForceField::operator->() const
{
    return static_cast<const FF*>(&(d()));
}

/** Dereference this pointer */
const FF& ForceField::operator*() const
{
    return static_cast<const FF&>(d());
}

/** Return a read-only reference to the contained object */
const FF& ForceField::read() const
{
    return static_cast<const FF&>(d());
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
FF& ForceField::edit()
{
    return static_cast<FF&>(d());
}
    
/** Return a raw pointer to the FF object */
const FF* ForceField::data() const
{
    return static_cast<const FF*>(&(d()));
}

/** Return a raw pointer to the FF object */
const FF* ForceField::constData() const
{
    return static_cast<const FF*>(&(d()));
}
    
/** Return a raw pointer to the FF object */
FF* ForceField::data()
{
    return static_cast<FF*>(&(d()));
}

/** Automatic casting operator */
ForceField::operator const FF&() const
{
    return static_cast<const FF&>(d());
}
