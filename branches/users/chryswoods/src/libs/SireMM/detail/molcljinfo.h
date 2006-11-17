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

#include "SireMol/molecule.h"

#include "SireMM/cljparameter.h"

#include "SireVol/coordgroup.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
namespace detail
{
class MolCLJInfo;
class MolCLJInfoData;
}
}

QDataStream& operator<<(QDataStream&, const SireMM::detail::MolCLJInfo&);
QDataStream& operator>>(QDataStream&, SireMM::detail::MolCLJInfo&);

QDataStream& operator<<(QDataStream&, const SireMM::detail::MolCLJInfoData&);
QDataStream& operator>>(QDataStream&, SireMM::detail::MolCLJInfoData&);

namespace SireMM
{

namespace detail
{

using SireMol::Molecule;

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
                   const QVector< QVector<ChargeParameter> > &chgs,
                   const QVector< QVector<LJParameter> > &ljs);

    /** Copy constructor */
    MolCLJInfoData(const MolCLJInfoData &other)
              : QSharedData(), molecule(other.molecule), coordinates(other.coordinates),
                chgparams(other.chgparams), ljparams(other.ljparams)
    {}

    ~MolCLJInfoData();

    void assertSameMolecule(const Molecule &mol) const;
    void assertSameMajorVersion(const Molecule &mol) const;
    void assertCompatibleParameters(
                        const QVector< QVector<ChargeParameter> > &chgparams) const;
    void assertCompatibleParameters(
                        const QVector< QVector<LJParameter> > &ljparams) const;

    void move(const Molecule &movedmol);

    void change(const Molecule &changedmol,
                const QVector< QVector<ChargeParameter> > &chgparams,
                const QVector< QVector<LJParameter> > &ljparams);

    /** The molecule whose parameters are contained in this object */
    Molecule molecule;

    /** The coordinates of the molecule, organised into CoordGroups
        an indexed by CutGroupID */
    QVector<CoordGroup> coordinates;

    /** The Charge parameters for all of the atoms in the molecule,
        organised into groups and indexed by CutGroupID */
    QVector< QVector<ChargeParameter> > chgparams;

    /** The LJ parameters for all of the atoms in the molecule,
        organised into groups and indexed by CutGroupID */
    QVector< QVector<LJParameter> > ljparams;
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
               const QVector< QVector<ChargeParameter> > &chgs,
               const QVector< QVector<LJParameter> > &ljs);

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
        'molecule' has changed, and that it now has the
        passed parameters

        \throw SireError::incompatible_error
    */
    void change(const Molecule &molecule,
                const QVector< QVector<ChargeParameter> > &chgparams,
                const QVector< QVector<LJParameter> > &ljparams)
    {
        d->change(molecule, chgparams, ljparams);
    }

private:
    /** Shared pointer to the data of this object */
    QSharedDataPointer<MolCLJInfoData> d;
};

}

}

SIRE_END_HEADER

#endif
