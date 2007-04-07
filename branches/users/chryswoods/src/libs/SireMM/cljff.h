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

#ifndef SIREMM_CLJFF_H
#define SIREMM_CLJFF_H

#include <QSharedDataPointer>

#include "cljpair.h"
#include "combiningrules.h"
#include "switchingfunction.h"

#include "atomiccharges.h"
#include "atomicljs.h"

#include "SireVol/space.h"
#include "SireFF/ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atomselection.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJFF&);
QDataStream& operator>>(QDataStream&, SireMM::CLJFF&);

namespace SireMM
{

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::NewAtom;
using SireMol::AtomSelection;

using SireMol::CutGroupID;

using SireVol::Space;
using SireVol::CoordGroup;
using SireVol::DistMatrix;

using SireFF::FFBase;
using SireFF::ParameterName;
using SireFF::FFComponent;

using SireCAS::Symbols;

/** This is the base class of all forcefields that provide only
    the LJ energy of molecules.

    @author Christopher Woods
*/
class SIREMM_EXPORT CLJFF : public FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const CLJFF&);
friend QDataStream& ::operator>>(QDataStream&, CLJFF&);

public:
    CLJFF();

    CLJFF(const Space &space, const SwitchingFunction &switchfunc);

    CLJFF(const CLJFF &other);

    ~CLJFF();

    class CLJMolecule;
    class CLJMoleculeData;  //private implementation of CLJMolecule
    class ChangedCLJMolecule;

    class SIREMM_EXPORT Components : public FFBase::Components
    {
    public:
        Components();
        Components(const FFBase &ffbase, const Symbols &symbols = Symbols());

        Components(const Components &other);

        ~Components();

        Components& operator=(const Components &other);

        Components* clone() const
        {
            return new Components(*this);
        }

        const FFComponent& coulomb() const
        {
            return e_coul;
        }

        const FFComponent& lj() const
        {
            return e_lj;
        }

        static QString describe_coulomb();
        static QString describe_lj();

    protected:
        void setForceField(const FFBase &ffbase);

    private:
        /** The coulomb component */
        FFComponent e_coul;
        /** The LJ component */
        FFComponent e_lj;
    };

    class SIREMM_EXPORT Parameters : public FFBase::Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);

        ~Parameters();

        /** Return the default source of the coulomb parameters */
        const ParameterName& coulomb() const
        {
            return chg_params;
        }

        /** Return the default source of the LJ parameters */
        const ParameterName& lj() const
        {
            return lj_params;
        }

        static Parameters default_sources;

    private:
        /** The name and default source of the coulomb parameters */
        ParameterName chg_params;
        /** The name and default source of the LJ parameters */
        ParameterName lj_params;
    };

    const CLJFF::Parameters& parameters() const
    {
        return CLJFF::Parameters::default_sources;
    }

    class SIREMM_EXPORT Groups : public FFBase::Groups
    {
    public:
        Groups();
        Groups(const Groups &other);

        ~Groups();
    };

    const Space& space() const;
    const SwitchingFunction& switchingFunction() const;

    bool setProperty(const QString &name, const Property &value);
    Property getProperty(const QString &name) const;
    bool containsProperty(const QString &name) const;

    /** Return the object describing the components of this
        forcefield */
    const CLJFF::Components& components() const
    {
        return *components_ptr;
    }

    class SIREMM_EXPORT CLJEnergy
    {
    public:
        CLJEnergy(double cnrg=0, double ljnrg=0)
        {
            icnrg = cnrg;
            iljnrg = ljnrg;
        }

        CLJEnergy(const CLJEnergy &other)
                   : icnrg(other.icnrg), iljnrg(other.iljnrg)
        {}

        ~CLJEnergy()
        {}

        CLJEnergy& operator+=(const CLJEnergy &other)
        {
            icnrg += other.icnrg;
            iljnrg += other.iljnrg;
            return *this;
        }

        CLJEnergy& operator-=(const CLJEnergy &other)
        {
            icnrg -= other.icnrg;
            iljnrg -= other.iljnrg;
            return *this;
        }

        double coulomb() const
        {
            return icnrg;
        }

        double lj() const
        {
            return iljnrg;
        }

    private:
        double icnrg;
        double iljnrg;
    };

    static CLJEnergy calculateEnergy(const CLJMolecule &mol0,
                                     const CLJMolecule &mol1,
                                     const Space &space,
                                     const SwitchingFunction &switchfunc,
                                     DistMatrix &distmatrix,
                                     CLJMatrix &cljmatrix);

    static CLJEnergy calculateEnergy(const CLJMolecule &mol,
                                     const Space &space,
                                     const SwitchingFunction &switchfunc,
                                     DistMatrix &distmatrix,
                                     CLJMatrix &cljmatrix);

protected:

    static CLJEnergy calculateEnergy(const CoordGroup &group0,
                                     const QVector<ChargeParameter> &chg0,
                                     const QVector<LJParameter> &lj0,
                                     const CoordGroup &group1,
                                     const QVector<ChargeParameter> &chg1,
                                     const QVector<LJParameter> &lj1,
                                     const Space &space,
                                     const SwitchingFunction &switchfunc,
                                     DistMatrix &distmatrix,
                                     CLJMatrix &cljmatrix);

    static CLJEnergy calculateEnergy(const CoordGroup &group,
                                     const QVector<ChargeParameter> &chgs,
                                     const QVector<LJParameter> &ljs,
                                     const Space &space,
                                     DistMatrix &distmatrix,
                                     CLJMatrix &cljmatrix);

    void _pvt_copy(const FFBase &other);

private:

    static CLJEnergy calculatePairEnergy(DistMatrix &distmatrix,
                                         CLJMatrix &cljmatrix);

    static CLJEnergy calculateSelfEnergy(DistMatrix &distmatrix,
                                         CLJMatrix &cljmatrix);

    void registerComponents();

    /** The space in which the molecules in this forcefield reside */
    Space spce;

    /** The switching function used to truncate the LJ interactions */
    SwitchingFunction switchfunc;

    /** Pointer to the object containing the components of
        this forcefield */
    const CLJFF::Components *components_ptr;
};

} // end of namespace SireMM

QDataStream& operator<<(QDataStream&, const SireMM::CLJFF::CLJMolecule&);
QDataStream& operator>>(QDataStream&, SireMM::CLJFF::CLJMolecule&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJFF::ChangedCLJMolecule&);
QDataStream& operator>>(QDataStream&, SireMM::CLJFF::ChangedCLJMolecule&);

namespace SireMM
{

/** This class is used to represent a Molecule or part of a Molecule
    in this forcefield. It contains all of the coordinates of the molecule,
    together with the LJ parameters of the atoms. */
class SIREMM_EXPORT CLJFF::CLJMolecule
{

friend QDataStream& ::operator<<(QDataStream&, const CLJFF::CLJMolecule&);
friend QDataStream& ::operator>>(QDataStream&, CLJFF::CLJMolecule&);

public:
    CLJMolecule();

    CLJMolecule(const PartialMolecule &molecule,
                const QString &chgproperty, const QString &ljproperty);

    CLJMolecule(const CLJMolecule &other, const QSet<CutGroupID> &groups);

    CLJMolecule(const CLJMolecule &other);

    ~CLJMolecule();

    CLJMolecule& operator=(const CLJMolecule &other);

    bool operator==(const CLJMolecule &other) const;
    bool operator!=(const CLJMolecule &other) const;

    bool isEmpty() const;

    const PartialMolecule& molecule() const;

    ChangedCLJMolecule change(const PartialMolecule &molecule) const;

    ChangedCLJMolecule add(const PartialMolecule &molecule,
                           const QString &chgproperty = QString::null,
                           const QString &ljproperty = QString::null) const;

    ChangedCLJMolecule remove(const PartialMolecule &molecule) const;

    const QString& chgProperty() const;
    const QString& ljProperty() const;

    const QVector<CoordGroup>& coordinates() const;

    const AtomicCharges& charges() const;
    const AtomicLJs& ljParameters() const;

    bool isWholeMolecule() const;

private:
    /** Implicitly shared pointer to the data of this class */
    QSharedDataPointer<CLJMoleculeData> d;
};

/** This class holds information on how a molecule has changed
    since the last energy evaluation.
*/
class SIREMM_EXPORT CLJFF::ChangedCLJMolecule
{

friend QDataStream& ::operator<<(QDataStream&, const CLJFF::ChangedCLJMolecule&);
friend QDataStream& ::operator>>(QDataStream&, CLJFF::ChangedCLJMolecule&);

public:
    ChangedCLJMolecule();

    ChangedCLJMolecule(const CLJMolecule &mol);

    ChangedCLJMolecule(const CLJMolecule &oldmol, const CLJMolecule &newmol);

    ChangedCLJMolecule(const CLJMolecule &oldmol, const CLJMolecule &newmol,
                       const QSet<CutGroupID> &changed_groups);

    ChangedCLJMolecule(const ChangedCLJMolecule &other);

    ~ChangedCLJMolecule();

    ChangedCLJMolecule& operator=(const ChangedCLJMolecule &other);

    bool operator==(const ChangedCLJMolecule &other) const;
    bool operator!=(const ChangedCLJMolecule &other) const;

    bool isEmpty() const;

    ChangedCLJMolecule change(const PartialMolecule &molecule) const;

    ChangedCLJMolecule add(const PartialMolecule &molecule,
                           const QString &chgproperty = QString::null,
                           const QString &ljproperty = QString::null) const;

    ChangedCLJMolecule remove(const PartialMolecule &molecule) const;

    bool changedAll() const;

    const QSet<CutGroupID>& changedGroups() const;

    const CLJMolecule& oldMolecule() const;
    const CLJMolecule& newMolecule() const;

    const CLJMolecule& oldParts() const;
    const CLJMolecule& newParts() const;

private:
    /** The old version of the molecule */
    CLJMolecule oldmol;

    /** The new version of the molecule */
    CLJMolecule newmol;

    /** The old version of the parts of the molecule that have
        changed */
    CLJMolecule oldparts;

    /** The new version of the parts of the molecule that have
        changed */
    CLJMolecule newparts;

    /** The CutGroupIDs of all of the CutGroups that have changed.
        This is empty if all of the CutGroups have changed */
    QSet<CutGroupID> changed_cgids;
};


/** Return the space within which the LJ energy is calculated */
inline const Space& CLJFF::space() const
{
    return spce;
}

/** Return the switching function that is used to apply the non-bonded
    cutoff */
inline const SwitchingFunction& CLJFF::switchingFunction() const
{
    return switchfunc;
}

} // end of namespace SireMM

Q_DECLARE_METATYPE(SireMM::CLJFF::CLJMolecule);
Q_DECLARE_METATYPE(SireMM::CLJFF::ChangedCLJMolecule);

SIRE_END_HEADER

#endif
