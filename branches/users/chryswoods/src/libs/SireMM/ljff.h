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

#ifndef SIREMM_LJFF_H
#define SIREMM_LJFF_H

#include <QSharedDataPointer>

#include "ljpair.h"
#include "combiningrules.h"
#include "switchingfunction.h"

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
class LJFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::LJFF&);
QDataStream& operator>>(QDataStream&, SireMM::LJFF&);

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
class SIREMM_EXPORT LJFF : public FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const LJFF&);
friend QDataStream& ::operator>>(QDataStream&, LJFF&);

public:
    LJFF();

    LJFF(const Space &space, const SwitchingFunction &switchfunc);

    LJFF(const LJFF &other);

    ~LJFF();

    class LJMolecule;
    class LJMoleculeData;  //private implementation of LJMolecule
    class ChangedLJMolecule;

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

        const FFComponent& lj() const
        {
            return e_lj;
        }

        static QString describe_lj();

    protected:
        void setForceField(const FFBase &ffbase);

    private:
        /** The LJ component */
        FFComponent e_lj;
    };

    class SIREMM_EXPORT Parameters : public FFBase::Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);

        ~Parameters();

        /** Return the default source of the LJ parameters */
        const ParameterName& lj() const
        {
            return lj_params;
        }

        static Parameters default_sources;

    private:
        /** The name and default source of the LJ parameters */
        ParameterName lj_params;
    };

    const LJFF::Parameters& parameters() const
    {
        return LJFF::Parameters::default_sources;
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

    /** Return the object describing the components of this
        forcefield */
    const LJFF::Components& components() const
    {
        return *components_ptr;
    }

    static double calculateEnergy(const LJMolecule &mol0,
                                  const LJMolecule &mol1,
                                  const Space &space,
                                  const SwitchingFunction &switchfunc,
                                  DistMatrix &distmatrix,
                                  LJMatrix &ljmatrix);

    static double calculateEnergy(const LJMolecule &mol,
                                  const Space &space,
                                  const SwitchingFunction &switchfunc,
                                  DistMatrix &distmatrix,
                                  LJMatrix &ljmatrix);

protected:

    static double calculateEnergy(const CoordGroup &group0,
                                  const QVector<LJParameter> &lj0,
                                  const CoordGroup &group1,
                                  const QVector<LJParameter> &lj1,
                                  const Space &space,
                                  const SwitchingFunction &switchfunc,
                                  DistMatrix &distmatrix,
                                  LJMatrix &ljmatrix);

    static double calculateEnergy(const CoordGroup &group,
                                  const QVector<LJParameter> &ljs,
                                  const Space &space,
                                  DistMatrix &distmatrix,
                                  LJMatrix &ljmatrix);

    DistMatrix& distanceMatrix();
    LJMatrix& ljMatrix();

private:

    static double calculatePairEnergy(DistMatrix &distmatrix,
                                      LJMatrix &ljmatrix);

    static double calculateSelfEnergy(DistMatrix &distmatrix,
                                      LJMatrix &ljmatrix);

    void registerComponents();

    /** Workspace for the distance calculations */
    DistMatrix distmat;

    /** Workspace for the combination of LJ parameters */
    LJMatrix ljmat;

    /** The space in which the molecules in this forcefield reside */
    Space spce;

    /** The switching function used to truncate the LJ interactions */
    SwitchingFunction switchfunc;

    /** Pointer to the object containing the components of
        this forcefield */
    const LJFF::Components *components_ptr;
};

} // end of namespace SireMM

QDataStream& operator<<(QDataStream&, const SireMM::LJFF::LJMolecule&);
QDataStream& operator>>(QDataStream&, SireMM::LJFF::LJMolecule&);

QDataStream& operator<<(QDataStream&, const SireMM::LJFF::ChangedLJMolecule&);
QDataStream& operator>>(QDataStream&, SireMM::LJFF::ChangedLJMolecule&);

namespace SireMM
{

/** This class is used to represent a Molecule or part of a Molecule
    in this forcefield. It contains all of the coordinates of the molecule,
    together with the LJ parameters of the atoms. */
class SIREMM_EXPORT LJFF::LJMolecule
{

friend QDataStream& ::operator<<(QDataStream&, const LJFF::LJMolecule&);
friend QDataStream& ::operator>>(QDataStream&, LJFF::LJMolecule&);

public:
    LJMolecule();

    LJMolecule(const Molecule &molecule, const QString &ljproperty);
    LJMolecule(const Residue &residue, const QString &ljproperty);
    LJMolecule(const NewAtom &atom, const QString &ljproperty);

    LJMolecule(const Molecule &molecule, const AtomSelection &selected_atoms,
               const QString &ljproperty);

    LJMolecule(const LJMolecule &other, const QSet<CutGroupID> &groups);

    LJMolecule(const LJMolecule &other);

    ~LJMolecule();

    LJMolecule& operator=(const LJMolecule &other);

    bool operator==(const LJMolecule &other) const;
    bool operator!=(const LJMolecule &other) const;

    bool isEmpty() const;

    const Molecule& molecule() const;

    ChangedLJMolecule change(const Molecule &molecule) const;
    ChangedLJMolecule change(const Residue &residue) const;
    ChangedLJMolecule change(const NewAtom &newatom) const;

    ChangedLJMolecule add(const Molecule &molecule,
                          const QString &ljproperty = QString::null) const;
    ChangedLJMolecule add(const Residue &residue,
                          const QString &ljproperty = QString::null) const;
    ChangedLJMolecule add(const NewAtom &newatom,
                          const QString &ljproperty = QString::null) const;
    ChangedLJMolecule add(const AtomSelection &selected_atoms,
                          const QString &ljproperty = QString::null) const;

    ChangedLJMolecule remove(const Molecule &molecule) const;
    ChangedLJMolecule remove(const Residue &residue) const;
    ChangedLJMolecule remove(const NewAtom &atom) const;
    ChangedLJMolecule remove(const AtomSelection &selected_atoms) const;

    const QString& ljProperty() const;

    const QVector<CoordGroup>& coordinates() const;
    const AtomicLJs& ljParameters() const;

    bool isWholeMolecule() const;
    const AtomSelection& selectedAtoms() const;

private:
    ChangedLJMolecule _pvt_change(const Molecule &molecule,
                                  const QSet<CutGroupID> &cgids,
                                  const QString &ljproperty = QString::null) const;

    ChangedLJMolecule _pvt_change(const Molecule &molecule,
                                  const QSet<CutGroupID> &cgids,
                                  const AtomSelection &selected_atoms,
                                  const QString &ljproperty = QString::null) const;

    /** Implicitly shared pointer to the data of this class */
    QSharedDataPointer<LJMoleculeData> d;
};

/** This class holds information on how a molecule has changed
    since the last energy evaluation.
*/
class SIREMM_EXPORT LJFF::ChangedLJMolecule
{

friend QDataStream& ::operator<<(QDataStream&, const LJFF::ChangedLJMolecule&);
friend QDataStream& ::operator>>(QDataStream&, LJFF::ChangedLJMolecule&);

public:
    ChangedLJMolecule();

    ChangedLJMolecule(const LJMolecule &mol);

    ChangedLJMolecule(const LJMolecule &oldmol, const LJMolecule &newmol);

    ChangedLJMolecule(const LJMolecule &oldmol, const LJMolecule &newmol,
                      const QSet<CutGroupID> &changed_groups);

    ChangedLJMolecule(const ChangedLJMolecule &other);

    ~ChangedLJMolecule();

    ChangedLJMolecule& operator=(const ChangedLJMolecule &other);

    bool operator==(const ChangedLJMolecule &other) const;
    bool operator!=(const ChangedLJMolecule &other) const;

    bool isEmpty() const;

    ChangedLJMolecule change(const Molecule &molecule) const;
    ChangedLJMolecule change(const Residue &residue) const;
    ChangedLJMolecule change(const NewAtom &atom) const;

    ChangedLJMolecule add(const Molecule &molecule,
                          const QString &ljproperty = QString::null) const;
    ChangedLJMolecule add(const Residue &residue,
                          const QString &ljproperty = QString::null) const;
    ChangedLJMolecule add(const NewAtom &atom,
                          const QString &ljproperty = QString::null) const;
    ChangedLJMolecule add(const AtomSelection &selected_atoms,
                          const QString &ljproperty = QString::null) const;

    ChangedLJMolecule remove(const Molecule &molecule) const;
    ChangedLJMolecule remove(const Residue &residue) const;
    ChangedLJMolecule remove(const NewAtom &atom) const;
    ChangedLJMolecule remove(const AtomSelection &selected_atoms) const;

    bool changedAll() const;

    const QSet<CutGroupID>& changedGroups() const;

    const LJMolecule& oldMolecule() const;
    const LJMolecule& newMolecule() const;

    const LJMolecule& oldParts() const;
    const LJMolecule& newParts() const;

private:
    /** The old version of the molecule */
    LJMolecule oldmol;

    /** The new version of the molecule */
    LJMolecule newmol;

    /** The old version of the parts of the molecule that have
        changed */
    LJMolecule oldparts;

    /** The new version of the parts of the molecule that have
        changed */
    LJMolecule newparts;

    /** The CutGroupIDs of all of the CutGroups that have changed.
        This is empty if all of the CutGroups have changed */
    QSet<CutGroupID> changed_cgids;
};


/** Return the space within which the LJ energy is calculated */
inline const Space& LJFF::space() const
{
    return spce;
}

/** Return the switching function that is used to apply the non-bonded
    cutoff */
inline const SwitchingFunction& LJFF::switchingFunction() const
{
    return switchfunc;
}

/** Return a reference to the workspace used for the distance calculations */
inline DistMatrix& LJFF::distanceMatrix()
{
    return distmat;
}

/** Return a reference to the workspace used for the LJ parameter combination */
inline LJMatrix& LJFF::ljMatrix()
{
    return ljmat;
}

} // end of namespace SireMM

Q_DECLARE_METATYPE(SireMM::LJFF::LJMolecule);
Q_DECLARE_METATYPE(SireMM::LJFF::ChangedLJMolecule);

SIRE_END_HEADER

#endif
