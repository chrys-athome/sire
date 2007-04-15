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

#ifndef SIREMM_COULOMBFF_H
#define SIREMM_COULOMBFF_H

#include <QSharedDataPointer>

#include "atomiccharges.h"
#include "combiningrules.h"
#include "switchingfunction.h"

#include "atomicljs.h"

#include "SireVol/space.h"
#include "SireFF/ffbase.h"

#include "SireMol/partialmolecule.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CoulombFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::CoulombFF&);
QDataStream& operator>>(QDataStream&, SireMM::CoulombFF&);

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
using SireFF::FFComponent;

using SireCAS::Symbols;

/** This is the base class of all forcefields that provide only
    the Coulomb energy of molecules.

    @author Christopher Woods
*/
class SIREMM_EXPORT CoulombFF : public FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const CoulombFF&);
friend QDataStream& ::operator>>(QDataStream&, CoulombFF&);

public:
    CoulombFF();

    CoulombFF(const Space &space, const SwitchingFunction &switchfunc);

    CoulombFF(const CoulombFF &other);

    ~CoulombFF();

    class CoulombMolecule;
    class CoulombMoleculeData;  //private implementation of CoulombMolecule
    class ChangedCoulombMolecule;

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
            return e_coulomb;
        }

        static QString describe_coulomb();

    protected:
        void setForceField(const FFBase &ffbase);

    private:
        /** The Coulomb component */
        FFComponent e_coulomb;
    };

    class SIREMM_EXPORT Parameters : public FFBase::Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);

        ~Parameters();

        /** Return the default source of the charge parameters */
        const ParameterName& coulomb() const
        {
            return chg_params;
        }

        static Parameters default_sources;

    private:
        /** The name and default source of the charge parameters */
        ParameterName chg_params;
    };

    const CoulombFF::Parameters& parameters() const
    {
        return CoulombFF::Parameters::default_sources;
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

    /** Return the object describing the components of this
        forcefield */
    const CoulombFF::Components& components() const
    {
        return *components_ptr;
    }

    static double calculateEnergy(const CoulombMolecule &mol0,
                                  const CoulombMolecule &mol1,
                                  const Space &space,
                                  const SwitchingFunction &switchfunc,
                                  DistMatrix &distmatrix);

    static double calculateEnergy(const CoulombMolecule &mol,
                                  const Space &space,
                                  const SwitchingFunction &switchfunc,
                                  DistMatrix &distmatrix);

protected:

    static double calculateEnergy(const CoordGroup &group0,
                                  const QVector<ChargeParameter> &chg0,
                                  const CoordGroup &group1,
                                  const QVector<ChargeParameter> &chg1,
                                  const Space &space,
                                  const SwitchingFunction &switchfunc,
                                  DistMatrix &distmatrix);

    static double calculateEnergy(const CoordGroup &group,
                                  const QVector<ChargeParameter> &chgs,
                                  const Space &space,
                                  DistMatrix &distmatrix);

    DistMatrix& distanceMatrix();

private:

    static double calculatePairEnergy(DistMatrix &distmatrix,
                                      const QVector<ChargeParameter> &chgs0,
                                      const QVector<ChargeParameter> &chgs1);

    static double calculateSelfEnergy(DistMatrix &distmatrix,
                                      const QVector<ChargeParameter> &chgs);

    void registerComponents();

    /** The space in which the molecules in this forcefield reside */
    Space spce;

    /** The switching function used to truncate the Coulomb interactions */
    SwitchingFunction switchfunc;

    /** Pointer to the object containing the components of
        this forcefield */
    const CoulombFF::Components *components_ptr;
};

} // end of namespace SireMM

QDataStream& operator<<(QDataStream&, const SireMM::CoulombFF::CoulombMolecule&);
QDataStream& operator>>(QDataStream&, SireMM::CoulombFF::CoulombMolecule&);

QDataStream& operator<<(QDataStream&, const SireMM::CoulombFF::ChangedCoulombMolecule&);
QDataStream& operator>>(QDataStream&, SireMM::CoulombFF::ChangedCoulombMolecule&);

namespace SireMM
{

/** This class is used to represent a Molecule or part of a Molecule
    in this forcefield. It contains all of the coordinates of the molecule,
    together with the Coulomb parameters of the atoms. */
class SIREMM_EXPORT CoulombFF::CoulombMolecule
{

friend QDataStream& ::operator<<(QDataStream&, const CoulombFF::CoulombMolecule&);
friend QDataStream& ::operator>>(QDataStream&, CoulombFF::CoulombMolecule&);

public:
    CoulombMolecule();

    CoulombMolecule(const PartialMolecule &molecule, const QString &chgproperty);

    CoulombMolecule(const CoulombMolecule &other, const QSet<CutGroupID> &groups);

    CoulombMolecule(const CoulombMolecule &other);

    ~CoulombMolecule();

    CoulombMolecule& operator=(const CoulombMolecule &other);

    bool operator==(const CoulombMolecule &other) const;
    bool operator!=(const CoulombMolecule &other) const;

    bool isEmpty() const;

    const PartialMolecule& molecule() const;

    ChangedCoulombMolecule change(const PartialMolecule &molecule,
                                  const QString &chgproperty = QString::null) const;

    ChangedCoulombMolecule add(const PartialMolecule &molecule,
                               const QString &chgproperty = QString::null) const;

    ChangedCoulombMolecule remove(const PartialMolecule &molecule) const;

    const QString& chargeProperty() const;

    const QVector<CoordGroup>& coordinates() const;
    const AtomicCharges& charges() const;

    bool isWholeMolecule() const;

private:
    /** Implicitly shared pointer to the data of this class */
    QSharedDataPointer<CoulombMoleculeData> d;
};

/** This class holds information on how a molecule has changed
    since the last energy evaluation.
*/
class SIREMM_EXPORT CoulombFF::ChangedCoulombMolecule
{

friend QDataStream& ::operator<<(QDataStream&, const CoulombFF::ChangedCoulombMolecule&);
friend QDataStream& ::operator>>(QDataStream&, CoulombFF::ChangedCoulombMolecule&);

public:
    ChangedCoulombMolecule();

    ChangedCoulombMolecule(const CoulombMolecule &mol);

    ChangedCoulombMolecule(const CoulombMolecule &oldmol, const CoulombMolecule &newmol);

    ChangedCoulombMolecule(const CoulombMolecule &oldmol, const CoulombMolecule &newmol,
                           const QSet<CutGroupID> &changed_groups);

    ChangedCoulombMolecule(const ChangedCoulombMolecule &other);

    ~ChangedCoulombMolecule();

    ChangedCoulombMolecule& operator=(const ChangedCoulombMolecule &other);

    bool operator==(const ChangedCoulombMolecule &other) const;
    bool operator!=(const ChangedCoulombMolecule &other) const;

    bool isEmpty() const;

    ChangedCoulombMolecule change(const PartialMolecule &molecule,
                                  const QString &chgproperty = QString::null) const;

    ChangedCoulombMolecule add(const PartialMolecule &molecule,
                               const QString &chgproperty = QString::null) const;

    ChangedCoulombMolecule remove(const PartialMolecule &molecule) const;

    bool changedAll() const;

    const QSet<CutGroupID>& changedGroups() const;

    const CoulombMolecule& oldMolecule() const;
    const CoulombMolecule& newMolecule() const;

    const CoulombMolecule& oldParts() const;
    const CoulombMolecule& newParts() const;

private:
    /** The old version of the molecule */
    CoulombMolecule oldmol;

    /** The new version of the molecule */
    CoulombMolecule newmol;

    /** The old version of the parts of the molecule that have
        changed */
    CoulombMolecule oldparts;

    /** The new version of the parts of the molecule that have
        changed */
    CoulombMolecule newparts;

    /** The CutGroupIDs of all of the CutGroups that have changed.
        This is empty if all of the CutGroups have changed */
    QSet<CutGroupID> changed_cgids;
};


/** Return the space within which the Coulomb energy is calculated */
inline const Space& CoulombFF::space() const
{
    return spce;
}

/** Return the switching function that is used to apply the non-bonded
    cutoff */
inline const SwitchingFunction& CoulombFF::switchingFunction() const
{
    return switchfunc;
}

} // end of namespace SireMM

Q_DECLARE_METATYPE(SireMM::CoulombFF::CoulombMolecule);
Q_DECLARE_METATYPE(SireMM::CoulombFF::ChangedCoulombMolecule);

SIRE_END_HEADER

#endif
