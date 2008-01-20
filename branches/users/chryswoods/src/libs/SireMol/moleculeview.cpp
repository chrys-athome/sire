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

#include "moleculeview.h"

#include "SireBase/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireStream;
using namespace SireBase;
using namespace SireMol;

RegisterMetaType<MoleculeView> r_molview( MAGIC_ONLY,
                                          "SireMol::MoleculeView" );

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const MoleculeView &molview)
{
    writeHeader(ds, r_molview, 1);

    SharedDataStream sds(ds);
    sds << molview.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       MoleculeView &molview)
{
    VersionID v = readHeader(ds, r_molview);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> molview.d;
    }
    else
        throw version_error(v, "1", r_molview, CODELOC);

    return ds;
}

/** Null constructor */
MoleculeView::MoleculeView() : d( MoleculeData::null() )
{}

/** Construct a view of the molecule whose data is in 'moldata' */
MoleculeView::MoleculeView(const MoleculeData &moldata)
             : d(moldata)
{}

/** Copy constructor */
MoleculeView::MoleculeView(const MoleculeView &other)
             : d(other.d)
{}

/** Destructor */
MoleculeView::~MoleculeView()
{}

/** Copy assignment operator */
MoleculeView& MoleculeView::operator=(const MoleculeView &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MoleculeView::operator==(const MoleculeView &other) const
{
    return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool MoleculeView::operator!=(const MoleculeView &other) const
{
    return d != other.d and *d != *(other.d);
}

/** Assert that this view is looking at the molecule whose data is 
    in 'other' (albeit perhaps a different version of that molecule)
    
    \throw SireError::incompatible_error
*/
void MoleculeView::assertSameMolecule(const MoleculeData &other) const
{
    if (d->number() != other.number())
        //these are different molecules!
        throw SireError::incompatible_error( QObject::tr(
            "The molecules \"%1\", number %2, and \"%3\", number %3, "
            "are different, and therefore incompatible.")
                .arg(d->info().name()).arg(d->number())
                .arg(other.name()).arg(other.number()),
                    CODELOC );
}

/** Assert that this is a view of the same molecule as 'other'
    (albeit at a different version)
    
    \throw SireError::incompatible_error
*/
void MoleculeView::assertSameMolecule(const MoleculeView &other) const
{
    this->assertSameMolecule(other.data());
}

/** Update this view with a new version of the molecule. You 
    can only update the molecule if it has the same info
    object (so same atoms, residues, cutgroups etc.)
    
    \throw SireError::incompatible_error
*/
void MoleculeView::update(const MoleculeData &moldata)
{
    this->assertSameMolecule(moldata);
    d = moldata;
}

/** Return the type of the property at key 'key'

    \throw SireBase::missing_property 
*/
const char* MoleculeView::propertyType(const PropertyName &key) const
{
    return d->property(key)->what();
}

/** Return the type of the metadata at metakey 'metakey' 

    \throw SireBase::missing_property
*/
const char* MoleculeView::metadataType(const PropertyName &metakey) const
{
    return d->metadata(metakey)->what();
}

/** Return the type of the metadata at metakey 'metakey' 
    for the property at key 'key'
    
    \throw SireBase::missing_property
*/
const char* MoleculeView::metadataType(const PropertyName &key,
                                       const PropertyName &metakey) const
{
    return d->metadata(key, metakey)->what();
}

/** Assert that this contains a property at key 'key' 

    \throw SireBase::missing_property
*/
void MoleculeView::assertHasProperty(const PropertyName &key) const
{
    if (not this->hasProperty(key))
        throw SireBase::missing_property( QObject::tr(
            "This view of the molecule \"%1\" (view type %2) "
            "does not have a valid property at key \"%3\".")
                .arg(d->info().name())
                .arg(this->what())
                .arg(key), CODELOC );
}

/** Assert that this contains some metadata at metakey 'metakey' 

    \throw SireBase::missing_property
*/
void MoleculeView::assertHasMetadata(const PropertyName &metakey) const
{
    if (not this->hasMetadata(metakey))
        throw SireBase::missing_property( QObject::tr(
            "This view of the molecule \"%1\" (view type %2) "
            "does not have some valid metadata at metakey \"%3\".")
                .arg(d->info().name())
                .arg(this->what())
                .arg(metakey), CODELOC );
}

/** Assert that this contains some metadata at metakey 'metakey' 
    for the property at key 'key'

    \throw SireBase::missing_property
*/
void MoleculeView::assertHasMetadata(const PropertyName &key,
                                     const PropertyName &metakey) const
{
    if (not this->hasMetadata(key,metakey))
        throw SireBase::missing_property( QObject::tr(
            "This view of the molecule \"%1\" (view type %2) "
            "does not have some valid metadata at metakey \"%3\" "
            "for the property at key \"%4\".")
                .arg(d->info().name())
                .arg(this->what())
                .arg(key), CODELOC );
}
