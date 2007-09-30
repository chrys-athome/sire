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
#include "molecularproperty.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<MoleculeData> r_moldata;

/////////
///////// Implementation of MoleculeData::PropVersions
/////////

/** Increment the global version number of the molecule,
    and return it */
Version MoleculeData::PropVersions::increment()
{
    QMutexLocker lkr(&mutex);
    
    ++version;
    
    return version;
}

/** Increment the version number of the property with key 'key',
    also updating and returning the global version number of 
    the molecule */
Version MoleculeData::PropVersions::increment(const QString &key,
                                              Version &molversion)
{
    QMutexLocker lkr(&mutex);
    
    //increment the property version
    Version prop_version = property_version.value(key, 0) + 1;

    //now increment the global version
    ++version;
    
    //return the values
    molversion = version;
    
    return prop_version;
}

/////////
///////// Implementation of MoleculeData
/////////

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeData &moldata)
{
    writeHeader(ds, r_moldata, 1);
    
    SharedDataStream sds(ds);
    
    sds << moldata.molinfo << moldata.props << moldata.vrsn;
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeData &moldata)
{
    VersionID v = readHeader(ds, r_moldata);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> moldata.molinfo >> moldata.props >> moldata.vrsn;
        
        throw SireError::incomplete_code( QObject::tr(
            "Need to be able to save and restore version numbers!!!"),
                CODELOC );
    }
    else
        throw version_error(v, "1", r_moldata, CODELOC);

    return ds;
}

/** Shared null MoleculeData */
static QSharedDataPointer<MoleculeData> shared_null;

/** Return a QSharedDataPointer to the shared null MoleculeData object */
QSharedDataPointer<MoleculeData> MoleculeData::null()
{
    if (shared_null.constData() == 0)
    {
        shared_null = new MoleculeData();
    }

    return shared_null;
}

static shared_ptr<MoleculeData::PropVersions> shared_nullversions;

static const shared_ptr<MoleculeData::PropVersions>& getNullVersions()
{
    if (shared_nullversions.constData() == 0)
        shared_nullversions.reset( new MoleculeData::PropVersions() );
        
    return shared_nullversions;
}

/** Null constructor */
MoleculeData::MoleculeData()
                : QSharedData(), vrsn(0), vrsns(getNullVersions())
{}

/** Copy constructor */
MoleculeData::MoleculeData(const MoleculeData &other)
                : QSharedData(),
                  molinfo(other.molinfo),
                  props(other.props),
                  vrsn(other.vrsn),
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
        vrsns = other.vrsns;
    }
    
    return *this;
}

/** Comparison operator - two molecules are the same if they have
    the same ID and version numbers. */
bool MoleculeData::operator==(const MoleculeData &other) const
{
    return molinfo.number() == other.molinfo.number() and 
           vrsn == other.vrsn;
}

/** Comparison operator - two molecules are the same if they have
    the same ID and version numbers. */
bool MoleculeData::operator!=(const MoleculeData &other) const
{
    return molinfo.number() != other.molinfo.number() or vrsn != other.vrsn;
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
    if (key.isNull())
        throw SireError::invalid_arg( QObject::tr(
            "You cannot set a property with a null key!"), CODELOC );

    if (value.isA<MolecularProperty>())
        value.asA<MolecularProperty>().assertCompatibleWith(molinfo);
        
    //now the property version number
    prop_vrsns.insert( key, vrsns->increment(key, vrsn) );
}

/** Completely remove the property at key 'key', together
    with all of its metadata. This does nothing if there is
    no property with this key */
void MoleculeData::removeProperty(const QString &key)
{
    if (props.contains(key))
    {
        props.remove(key);
        prop_vrsns.remove(key);
    
        //do not remove from the shared version numbers, in
        //case the user re-adds a property with this key - 
        //we have to still ensure that the version number is
        //unique :-)
        
        //increment the global version number
        vrsn = vrsns->increment();
    }
}

/** Return the version number of the property at key 'key'.
    If there is no such key in this molecule, or 
    the value is supplied by the key itself, then
    a version number of 0 is returned */
Version MoleculeData::version(const PropertyName &key) const
{
    if (key.hasValue())
        return 0;
    else
        return prop_vrsns.value(key, 0);
}
