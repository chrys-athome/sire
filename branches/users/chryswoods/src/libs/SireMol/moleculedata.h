/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMOL_MOLECULEDATA_H
#define SIREMOL_MOLECULEDATA_H

#include <QSharedData>
#include <QMutex>

#include <boost/shared_ptr.hpp>

#include "SireBase/properties.h"
#include "SireBase/propertymap.h"

#include "SireBase/shareddatapointer.hpp"

#include "moleculeinfodata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeData;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeData&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeData&);

namespace SireMol
{

using SireBase::PropertyName;
using SireBase::Property;
using SireBase::Properties;

/**
This class holds the shared molecule data for the Molecule classes
(which are all just views on this MolculeData class).

This class is not part of the standard API of the program and should not
be used in your own code.

@author Christopher Woods
*/
class MoleculeData : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeData&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeData&);

public:
    MoleculeData();

    MoleculeData(const MoleculeData &other);

    ~MoleculeData();

    MoleculeData& operator=(const MoleculeData &other);

    bool operator==(const MoleculeData &other) const;
    bool operator!=(const MoleculeData &other) const;

    /** The version number of this molecule - two molecules
        with the same version number and ID number are identical */
    quint64 version() const
    {
        return vrsn;
    }

    quint64 version(const PropertyName &key) const;

    /** Return the info object that contains all of the
        metainformation about the atoms, residues, chains,
        cutgroups and segments that make up this molecule,
        and also gives the name and number of this molecule */
    const MoleculeInfoData& info() const
    {
        return *molinfo;
    }

    bool hasProperty(const PropertyName &key) const;
    bool hasMetadata(const PropertyName &metakey) const;
    bool hasMetadata(const PropertyName &key,
                     const PropertyName &metakey) const;
                     
    const char* propertyType(const PropertyName &key) const;
    const char* metadataType(const PropertyName &metakey) const;
    const char* metadataType(const PropertyName &key,
                             const PropertyName &metakey) const;

    /** Return all of the properties of this molecule - this
        includes the coordinates, connectivity and any
        forcefield parameters that have been assigned to
        this molecule */
    const Properties& properties() const
    {
        return props;
    }

    const Property& property(const PropertyName &key) const;
    
    const Property& property(const PropertyName &key,
                             const Property &default_value) const;
    
    const Property& metadata(const PropertyName &metakey) const;

    const Property& metadata(const PropertyName &key,
                             const PropertyName &metakey) const;

    const Property& metadata(const PropertyName &metakey,
                             const Property &default_value) const;
                             
    const Property& metadata(const PropertyName &key,
                             const PropertyName &metakey,
                             const Property &default_value) const;

    void setProperty(const QString &key, 
                     const Property &value, bool clear_metadata=false) const;

    void removeProperty(const QString &key);

    void setMetadata(const QString &metakey, const Property &value);
    void setMetadata(const QString &key, const QString &metakey, 
                     const Property &value);

    void removeMetadata(const QString &metakey);
    void removeMetadata(const QString &key, const QString &metakey);

    /** Return the shared null MoleculeData */
    static SireBase::SharedDataPointer<MoleculeData> null();

private:
    /** The metainfo about the molecule - this contains the names of the molecule,
        residue and all atoms, and additional metainfo about all of the residues
        and atoms. This object may also be used to map from atom or residue IDs
        to CGAtomIDs (which are used to lookup the coordinates) */
    SireBase::SharedDataPointer<MoleculeInfoData> molinfo;

    /** All of the properties of this molecule - this includes
        the coordinates of the atoms, their connectivity and
        all of the molecule's forcefield parameters (for all forcefields that
        it is to be added to) and all additional properties that may need to
        be added to the molecule (together with their versions held
        as a metadata property "version" with each property) */
    Properties props;

    /** The version number of this molecule - this changes 
        whenever the molecule is changed in any way. If two molecules
        have the same ID number and version then they must be the same */
    quint64 vrsn;

    class PropVersions
    {
    public:
        PropVersions() : version(0)
        {}
        
        ~PropVersions()
        {}
        
        quint64 increment();
        quint64 increment(const QString &key, quint64 &mol);
        
    private:
        /** Mutex used to serialise access to the last version
            number */
        QMutex mutex;
        
        /** The last version number assigned to 
            this molecule */ 
        quint64 version;
        
        /** The last version number assigned to
            each property of the molecule */
        QHash<QString,quint64> property_version;
    };

    /** The version number of each of the properties in 
        this molecule */
    QHash<QString,quint64> prop_vrsns;

    /** The incremints that are used to update the version numbers
        of the different parts of this molecule */
    boost::shared_ptr<PropVersions> vrsns;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeData)

SIRE_END_HEADER

#endif
