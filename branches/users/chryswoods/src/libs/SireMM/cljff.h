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
#include "SireFF/parametermap.h"

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
using SireMol::PartialMolecule;
using SireMol::Residue;
using SireMol::NewAtom;
using SireMol::AtomSelection;

using SireMol::CutGroupID;

using SireVol::Space;
using SireVol::CoordGroup;
using SireVol::DistMatrix;

using SireFF::FFBase;
using SireFF::ParameterName;
using SireFF::ParameterMap;
using SireFF::FFComponent;

using SireCAS::Symbols;

struct CLJFactor;
class CLJNBPairs;

template<class T>
class CGAtomPairs;

typedef CGAtomPairs<CLJFactor> CLJCGNBPairs;

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

    class Molecule;
    class MoleculeData;  //private implementation of CLJFF::Molecule
    class ChangedMolecule;
    class Energy;

    typedef CLJPairMatrix ParamPairMatrix;

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

    virtual bool setSpace(const Space &space);
    virtual bool setSwitchingFunction(const SwitchingFunction &switchfunc);

    bool setProperty(const QString &name, const Property &value);
    Property getProperty(const QString &name) const;
    bool containsProperty(const QString &name) const;

    QHash<QString,Property> properties() const;

    /** Return the object describing the components of this
        forcefield */
    const CLJFF::Components& components() const
    {
        return *components_ptr;
    }

    class SIREMM_EXPORT Energy
    {
    public:
        Energy(double cnrg=0, double ljnrg=0)
        {
            icnrg = cnrg;
            iljnrg = ljnrg;
        }

        Energy(const Energy &other)
                   : icnrg(other.icnrg), iljnrg(other.iljnrg)
        {}

        ~Energy()
        {}

        Energy& operator+=(const Energy &other)
        {
            icnrg += other.icnrg;
            iljnrg += other.iljnrg;
            return *this;
        }

        Energy& operator-=(const Energy &other)
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

protected:

    static CLJFF::Energy calculateEnergy(const CLJFF::Molecule &mol0,
                                         const CLJFF::Molecule &mol1,
                                         const Space &space,
                                         const SwitchingFunction &switchfunc,
                                         DistMatrix &distmatrix,
                                         CLJPairMatrix &cljmatrix);

    static CLJFF::Energy calculateEnergy(const CLJFF::Molecule &mol,
                                         const Space &space,
                                         const SwitchingFunction &switchfunc,
                                         DistMatrix &distmatrix,
                                         CLJPairMatrix &cljmatrix);

    static CLJFF::Molecule createMolecule(const PartialMolecule &molecule,
                                          const ParameterMap &map);

    static void assertCompatibleParameters(const CLJFF::Molecule &mol,
                                           const ParameterMap &map);

    void setEnergy(const CLJFF::Energy &energy);
    void updateEnergy(const CLJFF::Energy &energy);

    void _pvt_copy(const FFBase &other);

private:

    static Energy calculateEnergy(const CoordGroup &group0,
                                     const QVector<ChargeParameter> &chg0,
                                     const QVector<LJParameter> &lj0,
                                     const CoordGroup &group1,
                                     const QVector<ChargeParameter> &chg1,
                                     const QVector<LJParameter> &lj1,
                                     const Space &space,
                                     const SwitchingFunction &switchfunc,
                                     DistMatrix &distmatrix,
                                     CLJPairMatrix &cljmatrix);

    static Energy calculateEnergy(const CoordGroup &group0,
                                     const QVector<ChargeParameter> &chg0,
                                     const QVector<LJParameter> &lj0,
                                     const CoordGroup &group1,
                                     const QVector<ChargeParameter> &chg1,
                                     const QVector<LJParameter> &lj1,
                                     const CLJCGNBPairs &nbpairs,
                                     const Space &space,
                                     const SwitchingFunction &switchfunc,
                                     DistMatrix &distmatrix,
                                     CLJPairMatrix &cljmatrix);

    static Energy calculateEnergy(const CoordGroup &group,
                                     const QVector<ChargeParameter> &chgs,
                                     const QVector<LJParameter> &ljs,
                                     const CLJCGNBPairs &nbpairs,
                                     const Space &space,
                                     DistMatrix &distmatrix,
                                     CLJPairMatrix &cljmatrix);

    static Energy calculatePairEnergy(DistMatrix &distmatrix,
                                         CLJPairMatrix &cljmatrix);

    static Energy calculatePairEnergy(DistMatrix &distmatrix,
                                         CLJPairMatrix &cljmatrix,
                                         CLJCGNBPairs &nbpairs);

    static Energy calculateSelfEnergy(DistMatrix &distmatrix,
                                         CLJPairMatrix &cljmatrix,
                                         CLJCGNBPairs &nbpairs);

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

QDataStream& operator<<(QDataStream&, const SireMM::CLJFF::Molecule&);
QDataStream& operator>>(QDataStream&, SireMM::CLJFF::Molecule&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJFF::ChangedMolecule&);
QDataStream& operator>>(QDataStream&, SireMM::CLJFF::ChangedMolecule&);

namespace SireMM
{

/** This class is used to represent a Molecule or part of a Molecule
    in this forcefield. It contains all of the coordinates of the molecule,
    together with the LJ parameters of the atoms. */
class SIREMM_EXPORT CLJFF::Molecule
{

friend QDataStream& ::operator<<(QDataStream&, const CLJFF::Molecule&);
friend QDataStream& ::operator>>(QDataStream&, CLJFF::Molecule&);

public:
    Molecule();

    Molecule(const PartialMolecule &molecule,
             const QString &chgproperty, const QString &ljproperty,
             const QString &nbsclproperty = QString::null);

    Molecule(const Molecule &other);

    ~Molecule();

    Molecule& operator=(const Molecule &other);

    bool operator==(const Molecule &other) const;
    bool operator!=(const Molecule &other) const;

    bool isEmpty() const;

    const PartialMolecule& molecule() const;

    CLJFF::Molecule change(const PartialMolecule &molecule) const;
    CLJFF::Molecule add(const PartialMolecule &molecule) const;
    CLJFF::Molecule remove(const PartialMolecule &molecule) const;

    CLJFF::Molecule changeParameters(const QString &chgproperty,
                                     const QString &ljproperty,
                                     const QString &nbsclproperty) const;

    CLJFF::Molecule getDifferences(const CLJFF::Molecule &other) const;

    const QString& chargeProperty() const;
    const QString& ljProperty() const;
    const QString& nbsclProperty() const;

    const QVector<CoordGroup>& coordinates() const;

    const AtomicCharges& charges() const;
    const AtomicLJs& ljParameters() const;
    const CLJNBPairs& intraNBScaleFactors() const;

    bool isWholeMolecule() const;

private:
    /** Implicitly shared pointer to the data of this class */
    QSharedDataPointer<CLJFF::MoleculeData> d;
};

/** This class holds information on how a molecule has changed
    since the last energy evaluation.
*/
class SIREMM_EXPORT CLJFF::ChangedMolecule
{

friend QDataStream& ::operator<<(QDataStream&, const CLJFF::ChangedMolecule&);
friend QDataStream& ::operator>>(QDataStream&, CLJFF::ChangedMolecule&);

public:
    ChangedMolecule();

    ChangedMolecule(const CLJFF::Molecule &mol);

    ChangedMolecule(const CLJFF::Molecule &oldmol, const CLJFF::Molecule &newmol);

    ChangedMolecule(const CLJFF::ChangedMolecule &other);

    ~ChangedMolecule();

    ChangedMolecule& operator=(const CLJFF::ChangedMolecule &other);

    bool operator==(const ChangedMolecule &other) const;
    bool operator!=(const ChangedMolecule &other) const;

    bool isEmpty() const;

    CLJFF::ChangedMolecule change(const PartialMolecule &molecule) const;
    CLJFF::ChangedMolecule add(const PartialMolecule &molecule) const;
    CLJFF::ChangedMolecule remove(const PartialMolecule &molecule) const;

    CLJFF::ChangedMolecule changeParameters(const QString &chgproperty,
                                            const QString &ljproperty,
                                            const QString &nbsclproperty) const;

    bool changedAll() const;
    bool nothingChanged() const;

    const CLJFF::Molecule& oldMolecule() const;
    const CLJFF::Molecule& newMolecule() const;

    const CLJFF::Molecule& oldParts() const;
    const CLJFF::Molecule& newParts() const;

private:
    /** The old version of the molecule */
    CLJFF::Molecule oldmol;

    /** The new version of the molecule */
    CLJFF::Molecule newmol;

    /** The old version of the parts of the molecule that have
        changed */
    CLJFF::Molecule oldparts;

    /** The new version of the parts of the molecule that have
        changed */
    CLJFF::Molecule newparts;
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

Q_DECLARE_METATYPE(SireMM::CLJFF::Molecule);
Q_DECLARE_METATYPE(SireMM::CLJFF::ChangedMolecule);

SIRE_END_HEADER

#endif
