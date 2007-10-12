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

#include <QDataStream>
#include <QSharedData>

#include "moleculedata.h"
#include "molviewproperty.h"

#include "SireBase/incremint.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireStream;
using namespace SireMol;
using namespace SireBase;

static const RegisterMetaType<MoleculeData> r_moldata;

/////////
///////// Implementation of MoleculeData::PropVersions
/////////

/** Increment the global version number of the molecule,
    and return it */
quint64 MoleculeData::PropVersions::increment()
{
    QMutexLocker lkr(&mutex);
    
    ++version;
    
    return version;
}

/** Increment the version number of the property with key 'key',
    also updating and returning the global version number of 
    the molecule */
quint64 MoleculeData::PropVersions::increment(const QString &key,
                                              quint64 &molversion)
{
    QMutexLocker lkr(&mutex);
    
    quint64 prop_version;
    
    //increment the property version
    QHash<QString,quint64>::iterator it = property_version.find(key);
    
    if (it == property_version.end())
    {
        prop_version = 1;
        property_version.insert(key, prop_version);
    }
    else
    {
        prop_version = it.value() + 1;
        it.value() = prop_version;
    }

    //now increment the global version
    ++version;
    
    //return the values
    molversion = version;
    
    return prop_version;
}

/** Reset all of the version numbers back to 1 */
quint64 MoleculeData::PropVersions::reset(QHash<QString,quint64> &prop_vrsns)
{
    QMutexLocker lkr(&mutex);

    property_version.clear();
    
    for (QHash<QString,quint64>::iterator it = prop_vrsns.begin();
         it != prop_vrsns.end();
         ++it)
    {
        it.value() = 1;
        property_version.insert( it.key(), it.value() );
    }
    
    version = 1;
    
    return version;
}

/////////
///////// Implementation of MoleculeData
/////////

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeData &moldata)
{
    writeHeader(ds, r_moldata, 1);
    
    SharedDataStream sds(ds);
    
    sds << moldata.molinfo << moldata.props 
        << moldata.vrsn << moldata.molnum;
        
    throw SireError::incomplete_code( QObject::tr(
            "Need to be able to save and restore version numbers!!!"),
                CODELOC );
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeData &moldata)
{
    VersionID v = readHeader(ds, r_moldata);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> moldata.molinfo >> moldata.props 
            >> moldata.vrsn >> moldata.molnum;
        
        throw SireError::incomplete_code( QObject::tr(
            "Need to be able to save and restore version numbers!!!"),
                CODELOC );
    }
    else
        throw version_error(v, "1", r_moldata, CODELOC);

    return ds;
}

/** Shared null MoleculeData */
static SharedDataPointer<MoleculeData> shared_null;

/** Return a QSharedDataPointer to the shared null MoleculeData object */
SharedDataPointer<MoleculeData> MoleculeData::null()
{
    if (shared_null.constData() == 0)
    {
        shared_null = new MoleculeData();
    }

    return shared_null;
}

boost::shared_ptr<MoleculeData::PropVersions> MoleculeData::shared_nullversions;

const boost::shared_ptr<MoleculeData::PropVersions>& MoleculeData::getNullVersions()
{
    if (shared_nullversions == 0)
        shared_nullversions.reset( new MoleculeData::PropVersions() );
        
    return shared_nullversions;
}

/** Null constructor */
MoleculeData::MoleculeData()
                : QSharedData(), vrsn(0), molnum(0),
                  vrsns(getNullVersions())
{}

/** Copy constructor */
MoleculeData::MoleculeData(const MoleculeData &other)
                : QSharedData(),
                  molinfo(other.molinfo),
                  props(other.props),
                  vrsn(other.vrsn),
                  molnum(other.molnum),
                  vrsns(other.vrsns)
{}

/** Destructor */
MoleculeData::~MoleculeData()
{}

/** Assignment operator */
MoleculeData& MoleculeData::operator=(const MoleculeData &other)
{
    if (this != &other)
    {
        molinfo = other.molinfo;
        props = other.props;
        vrsn = other.vrsn;
        molnum = other.molnum;
        vrsns = other.vrsns;
    }
    
    return *this;
}

/** Comparison operator - two molecules are the same if they have
    the same ID and version numbers. */
bool MoleculeData::operator==(const MoleculeData &other) const
{
    return molnum == other.molnum and 
           vrsn == other.vrsn;
}

/** Comparison operator - two molecules are the same if they have
    the same ID and version numbers. */
bool MoleculeData::operator!=(const MoleculeData &other) const
{
    return molnum != other.molnum or vrsn != other.vrsn;
}

static Incremint molid_incremint;

/** Here is the function to increment a Molecule Number */
MolNum MolNum::increment() const
{
    return MolNum( molid_incremint.increment() );
}

/** Give this molecule a brand new ID number! */
void MoleculeData::getNewID()
{
    //get the new ID number...
    molnum = molnum.increment();
    
    //now reset the version numbers so they are all '1'
    vrsns.reset( new PropVersions() );
    vrsn = vrsns->reset(prop_vrsns);
}

/** Return the version number of the property at key 'key'.
    If there is no such key in this molecule, or 
    the value is supplied by the key itself, then
    a version number of 0 is returned */
quint64 MoleculeData::version(const PropertyName &key) const
{
    if (key.hasValue())
        return 0;
    else
        return prop_vrsns.value(key, 0);
}

/** Return whether this molecule contains a property at key 'key' */
bool MoleculeData::hasProperty(const PropertyName &key) const
{
    return props.hasProperty(key);
}

/** Return whether this molecule contains metadata at 
    metakey 'metakey' */
bool MoleculeData::hasMetadata(const PropertyName &metakey) const
{
    return props.hasMetadata(metakey);
}

/** Return whether this molecule has metadata at metakey 'metakey'
    for the property at key 'key' 
    
    \throw SireBase::missing_property
*/
bool MoleculeData::hasMetadata(const PropertyName &key,
                               const PropertyName &metakey) const
{
    return props.hasMetadata(key, metakey);
}
                 
/** Return the type name of the property at key 'key'. 

    \throw SireBase::missing_property
*/
const char* MoleculeData::propertyType(const PropertyName &key) const
{
    return props.propertyType(key);
}

/** Return the type name of the metadata at metakey 'metakey'

    \throw SireBase::missing_property
*/
const char* MoleculeData::metadataType(const PropertyName &metakey) const
{
    return props.metadataType(metakey);
}

/** Return the type name of the metadata at metakey 'metakey'
    for the property at key 'key'
    
    \throw SireBase::missing_property
*/
const char* MoleculeData::metadataType(const PropertyName &key,
                                       const PropertyName &metakey) const
{
    return props.metadataType(key, metakey);
}

/** Return the property at key 'key' 

    \throw SireBase::missing_property
*/
const Property& MoleculeData::property(const PropertyName &key) const
{
    return props.property(key);
}

/** Return the property at key 'key', or 'default_value' if there
    is no such property */ 
const Property& MoleculeData::property(const PropertyName &key,
                                       const Property &default_value) const
{
    return props.property(key, default_value);
}

/** Return the metadata at metakey 'metakey'

    \throw SireBase::missing_property
*/
const Property& MoleculeData::metadata(const PropertyName &metakey) const
{
    return props.metadata(metakey);
}

/** Return the metadata at metakey 'metakey' for the property
    at key 'key'
    
    \throw SireBase::missing_property
*/
const Property& MoleculeData::metadata(const PropertyName &key,
                                       const PropertyName &metakey) const
{
    return props.metadata(key, metakey);
}

/** Return the metadata at metakey 'metakey', or 'default_value'
    if there is no such value */
const Property& MoleculeData::metadata(const PropertyName &metakey,
                                       const Property &default_value) const
{
    return props.metadata(metakey, default_value);
}
                         
/** Return the metadata at metakey 'metakey' for the property
    at key 'key', or 'default_value' if there is no such value
    
    \throw SireBase::missing_property
*/
const Property& MoleculeData::metadata(const PropertyName &key,
                                       const PropertyName &metakey,
                                       const Property &default_value) const
{
    return props.metadata(key, metakey, default_value);
}

/** Set the property with the key 'key' to the value 'value'.
    This replaces any current property with that key. This
    also checks to ensure that this property is compatible
    with this molecule (e.g. it ensures that if the property
    is an atomic property, then it has the right number of
    values for the atoms)
    
    \throw SireError::incompatible_error
*/
void MoleculeData::setProperty(const QString &key, 
                               const Property &value,
                               bool clear_metadata)
{
    if (key.isEmpty())
        throw SireError::invalid_arg( QObject::tr(
            "You cannot set a property with an empty key!"), CODELOC );

    if (value.isA<MolViewProperty>())
        value.asA<MolViewProperty>().assertCompatibleWith(*molinfo);
        
    //now the property version number
    prop_vrsns.insert( key, vrsns->increment(key, vrsn) );
    
    //now save the property itself
    props.setProperty(key, value, clear_metadata);
}

/** Completely remove the property at key 'key', together
    with all of its metadata. This does nothing if there is
    no property with this key */
void MoleculeData::removeProperty(const QString &key)
{
    if (props.hasProperty(key))
    {
        props.removeProperty(key);
        prop_vrsns.remove(key);
    
        //do not remove from the shared version numbers, in
        //case the user re-adds a property with this key - 
        //we have to still ensure that the version number is
        //unique :-)
        
        //increment the global version number
        vrsn = vrsns->increment();
    }
}

/** Set the value of the metadata at metakey 'metakey' to 
    the value 'value' */
void MoleculeData::setMetadata(const QString &metakey, const Property &value)
{
    if (metakey.isEmpty())
        throw SireError::invalid_arg( QObject::tr(
            "You cannot set some metadata with an empty metakey!"), CODELOC );

    if (value.isA<MolViewProperty>())
        value.asA<MolViewProperty>().assertCompatibleWith(*molinfo);

    props.setMetadata(metakey, value);

    //increment the global version number
    vrsn = vrsns->increment();
}

/** Set the value of the metadata at metakey 'metakey' of the 
    property at key 'key' to the value 'value'
    
    \throw SireBase::missing_property
*/ 
void MoleculeData::setMetadata(const QString &key, const QString &metakey, 
                               const Property &value)
{
    if (key.isNull())
        throw SireError::invalid_arg( QObject::tr(
            "You cannot set some metadata with an empty key!"), CODELOC );
    else if (metakey.isEmpty())
        throw SireError::invalid_arg( QObject::tr(
            "You cannot set some metadata with an empty metakey!"), CODELOC );

    if (value.isA<MolViewProperty>())
        value.asA<MolViewProperty>().assertCompatibleWith(*molinfo);

    props.setMetadata(key, metakey, value);
    
    //increment the global version number
    vrsn = vrsns->increment();
}

/** Remove the metadata at metakey 'metakey' */
void MoleculeData::removeMetadata(const QString &metakey)
{
    if (props.hasMetadata(metakey))
    {
        props.removeMetadata(metakey);
        vrsn = vrsns->increment();
    }
}

/** Remove the metadata at metakey 'metakey' from the 
    property at key 'key'
    
    \throw SireBase::missing_property
*/
void MoleculeData::removeMetadata(const QString &key, const QString &metakey)
{
    if (props.hasMetadata(key, metakey))
    {
        props.removeMetadata(key, metakey);
        vrsn = vrsns->increment();
    }
}
