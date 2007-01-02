/**
  * @file
  *
  * C++ Interface: MolCLJInfo
  *
  * Description: Interface for MolCLJInfo and MolCLJInfoPvt
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIREMM_DETAIL_MOLCLJINFO_H
#define SIREMM_DETAIL_MOLCLJINFO_H

#include <QSharedData>
#include <QSharedDataPointer>
#include <QVector>

#include "SireFF/parametermap.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/cutgroupid.h"

#include "SireMM/cljff.h"
#include "SireMM/cljparameter.h"
#include "SireMM/atomiccharges.h"
#include "SireMM/atomicljs.h"

#include "SireVol/coordgroup.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
namespace detail
{
class MolCLJInfo;
class MolCLJInfoData;
class ChangedMolCLJInfo;
}
}

QDataStream& operator<<(QDataStream&, const SireMM::detail::MolCLJInfo&);
QDataStream& operator>>(QDataStream&, SireMM::detail::MolCLJInfo&);

QDataStream& operator<<(QDataStream&, const SireMM::detail::MolCLJInfoData&);
QDataStream& operator>>(QDataStream&, SireMM::detail::MolCLJInfoData&);

QDataStream& operator<<(QDataStream&, const SireMM::detail::ChangedMolCLJInfo&);
QDataStream& operator>>(QDataStream&, SireMM::detail::ChangedMolCLJInfo&);

namespace SireMM
{

namespace detail
{

using SireFF::ParameterMap;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::CutGroupID;

using SireVol::CoordGroup;

class ChangedMolCLJInfo;

/**
This private class contains the data for MolCLJInfo

@author Christopher Woods
*/
class MolCLJInfoData : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MolCLJInfoData&);
friend QDataStream& ::operator>>(QDataStream&, MolCLJInfoData&);

public:
    MolCLJInfoData();

    MolCLJInfoData(const Molecule &mol,
                   const CLJFF::Parameters &parameters,
                   const ParameterMap &map,
                   const QSet<CutGroupID> &cgids);

    /** Copy constructor */
    MolCLJInfoData(const MolCLJInfoData &other)
              : QSharedData(), 
                coulombproperty(other.coulombproperty),
                ljproperty(other.ljproperty),
                molecule(other.molecule), 
                map(other.map),
                cgids(other.cgids),
                coordinates(other.coordinates),
                chgparams(other.chgparams), 
                ljparams(other.ljparams)
    {}

    ~MolCLJInfoData();

    void assertSameMolecule(const Molecule &mol) const;

    void buildWhole();
    void buildParts(const QSet<CutGroupID> &cgids);

    /** The ParameterName used to find the coulomb parameters */
    ParameterName coulombproperty;
    
    /** The ParameterName used to find the LJ parameters */
    ParameterName ljproperty;

    /** The molecule whose parameters are contained in this object */
    Molecule molecule;

    /** The mapping used to find forcefield parameters from the
        molecule's properties */
    ParameterMap map;

    /** The CutGroupIDs of CutGroups whose coordinates and 
        parameters are stored in this object */
    QSet<CutGroupID> cgids;

    /** The coordinates of the molecule, organised into CoordGroups,
        of the CutGroups whose IDs are in cgids (in the iterator order that 
        they appear in cgids) */
    QVector<CoordGroup> coordinates;

    /** The Charge parameters for the atoms in the molecule,
        arranged identically to 'coordinates' */
    AtomicCharges chgparams;

    /** The LJ parameters for the atoms in the molecule,
        arranged identically to 'coordinates' */
    AtomicLJs ljparams;
};

/**
This is a private class used internally by the CLJ forcefields to hold a
molecule together with its CLJ parameters.

@author Christopher Woods
*/
class MolCLJInfo
{

friend QDataStream& ::operator<<(QDataStream&, const MolCLJInfo&);
friend QDataStream& ::operator>>(QDataStream&, MolCLJInfo&);

public:
    MolCLJInfo();

    MolCLJInfo(const Molecule &molecule,
               const CLJFF::Parameters &parameters,
               const ParameterMap &parammap,
               const QSet<CutGroupID> &cgids = QSet<CutGroupID>());

    MolCLJInfo(const MolCLJInfo &other);

    ~MolCLJInfo();

    MolCLJInfo& operator=(const MolCLJInfo &other);
    
    const Molecule& molecule() const;

    const ParameterMap& map() const;

    const QVector<CoordGroup>& coordinates() const;
    
    const AtomicCharges& chargeParameters() const;
    const AtomicLJs& ljParameters() const;
    
    ChangedMolCLJInfo change(const Molecule &molecule, 
                             const CLJFF::Parameters &parameters) const;

    ChangedMolCLJInfo change(const Residue &residue,
                             const CLJFF::Parameters &parameters) const;

    bool isWholeMolecule() const;

    void setWholeMolecule();
    void setPartialMolecule(const QSet<CutGroupID> &cgids);

private:
    bool propertiesChanged(const CLJFF::Parameters &parameters);

    /** Shared pointer to the data of this object */
    QSharedDataPointer<MolCLJInfoData> d;
};

/** Copy constructor */
inline MolCLJInfo::MolCLJInfo(const MolCLJInfo &other) : d(other.d)
{}

inline MolCLJInfo& MolCLJInfo::operator=(const MolCLJInfo &other)
{
    d = other.d;
    return *this;
}

/** Return the molecule whose parameters are contained in this
    group */
inline const Molecule& MolCLJInfo::molecule() const
{
    return d->molecule;
}

/** Return the mapping used to obtain parameters for this molecule
    from its properties */
inline const ParameterMap& MolCLJInfo::map() const
{
    return d->map;
}

/** Return the coordinates of the atoms in the molecule,
    arranged into CoordGroups, indexed by CutGroupID */
inline const QVector<CoordGroup>& MolCLJInfo::coordinates() const
{
    return d->coordinates;
}

/** Return the charge parameters of the atoms in the molecule,
    arranged into groups, indexed by CutGroupID */
inline const AtomicCharges& MolCLJInfo::chargeParameters() const
{
    return d->chgparams;
}

/** Return the LJ parameters of the atoms in the molecule,
    arranged into groups, indexed by CutGroupID */
inline const AtomicLJs& MolCLJInfo::ljParameters() const
{
    return d->ljparams;
}

/** This class is used to store information about changed molecules

    @author Christopher Woods
*/
class ChangedMolCLJInfo
{

friend QDataStream& ::operator<<(QDataStream&, const ChangedMolCLJInfo&);
friend QDataStream& ::operator>>(QDataStream&, ChangedMolCLJInfo&);

public:
    ChangedMolCLJInfo();

    ChangedMolCLJInfo( const MolCLJInfo &oldmol,
                       const MolCLJInfo &newmol );

    ChangedMolCLJInfo( const MolCLJInfo &oldmol,
                       const MolCLJInfo &newmol,
                       const QSet<CutGroupID> &movedparts );

    ~ChangedMolCLJInfo();

    const MolCLJInfo& newMol() const;
    const MolCLJInfo& oldMol() const;

    const MolCLJInfo& newParts() const;
    const MolCLJInfo& oldParts() const;

    bool movedAll() const;

    ChangedMolCLJInfo change(const Molecule &molecule,
                             const CLJFF::Parameters &parameters) const;

    ChangedMolCLJInfo change(const Residue &residue,
                             const CLJFF::Parameters &parameters) const;

private:
    void assertSane() const;

    /** Shared pointer to the data for the whole of the old configuration
        of the molecule */
    MolCLJInfo oldmol;

    /** Shared pointer to the data for the whole of the new configuration
        of the molecule */
    MolCLJInfo newmol;

    /** Shared pointer to the data of the old configuration of the parts of
        the molecule that have moved */
    MolCLJInfo oldparts;

    /** Shared pointer to the data of the new configuration of the parts of
        the molecule that have moved */
    MolCLJInfo newparts;

    /** The set of CutGroupIDs of all of the CutGroups that have changed
        between the old and new configurations. This will be empty
        if all of the CutGroups have changed (and thus oldmol == oldparts
        and newmol == newparts) */
    QSet<CutGroupID> cgids;
};

/** Return the CLJ and coordinate information for the
    whole molecule after the change */
inline const MolCLJInfo& ChangedMolCLJInfo::newMol() const
{
    return newmol;
}

/** Return the CLJ and coordinate information for the
    whole molecule for before the change */
inline const MolCLJInfo& ChangedMolCLJInfo::oldMol() const
{
    return oldmol;
}

/** Return the CLJ and coordinate information for the
    parts of the molecule that changed from after the change. */
inline const MolCLJInfo& ChangedMolCLJInfo::newParts() const
{
    return newparts;
}

/** Return the CLJ and coordinate information for the
     parts of the molecule that changed from before the change. */
inline const MolCLJInfo& ChangedMolCLJInfo::oldParts() const
{
    return oldparts;
}

/** Return whether or not the whole molecule changed, or
    whether only part of the molecule changed */
inline bool ChangedMolCLJInfo::movedAll() const
{
    return cgids.isEmpty();
}

}

}

SIRE_END_HEADER

#endif
