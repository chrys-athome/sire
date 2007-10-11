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

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeData;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeData&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeData&);

namespace SireMol
{

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
    Version version() const
    {
        return vrsn;
    }

    Version version(const PropertyName &key) const;

    /** Return the info object that contains all of the
        metainformation about the atoms, residues, chains,
        cutgroups and segments that make up this molecule,
        and also gives the name and number of this molecule */
    const MoleculeInfoData& info() const
    {
        return *molinfo;
    }

    /** Return all of the properties of this molecule - this
        includes the coordinates, connectivity and any
        forcefield parameters that have been assigned to
        this molecule */
    const Properties& properties() const
    {
        return props;
    }

    /** Return the property with key 'key'
    
        \throw SireBase::missing_property
    */
    const Property& property(const SireBase::PropertyName &key) const
    {
        return props.value(key);
    }
    
    /** Return the property with key 'key', returning
        'default_value' if there is no such key in this molecule */
    const Property& property(const SireBase::PropertyName &key,
                             const Property &default_value) const;
    
    /** Return the metadata associated with the property
        with key 'key'
        
        \throw SireBase::missing_property
    */
    const Properties& metadata(const QString &key) const
    {
        return props.metadata(key);
    }

    void setProperty(const QString &key, 
                     const Property &value, bool clear_metadata=false) const;

    void removeProperty(const QString &key);

    /** Return the shared null MoleculeData */
    static QSharedDataPointer<MoleculeData> null();

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
    Version vrsn;

    class PropVersions
    {
    public:
        PropVersions() : version(0)
        {}
        
        ~PropVersions()
        {}
        
        Version increment();
        Version increment(const QString &key, Version &mol);
        
    private:
        /** Mutex used to serialise access to the last version
            number */
        QMutex mutex;
        
        /** The last version number assigned to 
            this molecule */ 
        Version version;
        
        /** The last version number assigned to
            each property of the molecule */
        QHash<QString,Version> property_version;
    };

    /** The version number of each of the properties in 
        this molecule */
    QHash<QString,Version> prop_vrsns;

    /** The incremints that are used to update the version numbers
        of the different parts of this molecule */
    boost::shared_ptr<PropVersions> vrsns;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeData)

SIRE_END_HEADER

#endif
