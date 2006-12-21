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
                   const CLJFF::Parameters& parameters,
                   const ParameterMap &map);

    /** Copy constructor */
    MolCLJInfoData(const MolCLJInfoData &other)
              : QSharedData(), molecule(other.molecule), coordinates(other.coordinates),
                chgparams(other.chgparams), ljparams(other.ljparams)
    {}

    ~MolCLJInfoData();

    void assertSameMolecule(const Molecule &mol) const;
    void assertSameMajorVersion(const Molecule &mol) const;

    void move(const Molecule &movedmol);

    void change(const Molecule &changedmol, const CLJFF::Parameters &parameters,
                const ParameterMap &map);

    /** The molecule whose parameters are contained in this object */
    Molecule molecule;

    /** The coordinates of the molecule, organised into CoordGroups
        an indexed by CutGroupID */
    QVector<CoordGroup> coordinates;

    /** The Charge parameters for all of the atoms in the molecule */
    AtomicCharges chgparams;

    /** The LJ parameters for all of the atoms in the molecule */
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
               const ParameterMap &parammap);

    /** Copy constructor */
    MolCLJInfo(const MolCLJInfo &other) : d(other.d)
    {}

    ~MolCLJInfo();

    MolCLJInfo& operator=(const MolCLJInfo &other)
    {
        d = other.d;
        return *this;
    }

    /** Return the molecule whose parameters are contained in this
        group */
    const Molecule& molecule() const
    {
        return d->molecule;
    }

    /** Return the coordinates of the atoms in the molecule,
        arranged into CoordGroups, indexed by CutGroupID */
    const QVector<CoordGroup>& coordinates() const
    {
        return d->coordinates;
    }

    /** Return the charge parameters of the atoms in the molecule,
        arranged into groups, indexed by CutGroupID */
    const QVector< QVector<ChargeParameter> >& chargeParameters() const
    {
        return d->chgparams;
    }

    /** Return the LJ parameters of the atoms in the molecule,
        arranged into groups, indexed by CutGroupID */
    const QVector< QVector<LJParameter> >& ljParameters() const
    {
        return d->ljparams;
    }

    /** Change the record to record that the molecule
        'molecule' has moved.

        \throw SireError::incompatible_error
        \throw SireError::version_error
    */
    void move(const Molecule &molecule)
    {
        d->move(molecule);
    }

    /** Change the record to record that the molecule
        'molecule' has changed, using the passed parameter
        types and map to extract the required parameters
        from the properties of the molecule.

        \throw SireError::incompatible_error
        \throw SireError::invalid_cast
    */
    void change(const Molecule &molecule, const CLJFF::Parameters &parameters,
                const ParameterMap &map)
    {
        d->change(molecule, parameters, map);
    }

private:
    /** Shared pointer to the data of this object */
    QSharedDataPointer<MolCLJInfoData> d;
};

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

    void change(const MolCLJInfo &newinfo,
                const QSet<CutGroupID> &changedparts);

    void change(const MolCLJInfo &newinfo);

    void move(const Molecule &molecule);
    void move(const Residue &residue);

    void assertCompatibleWith(const MolCLJInfo &molinfo);

private:
    void buildParts();

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
