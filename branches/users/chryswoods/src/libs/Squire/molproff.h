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

#ifndef SQUIRE_MOLPROFF_H
#define SQUIRE_MOLPROFF_H

#include <QVector>
#include <QDir>
#include <QFileInfo>

#include <boost/tuple/tuple.hpp>

#include "SireBase/majversion.h"

#include "SireFF/ffbase.h"

#include "SireVol/space.h"
#include "SireVol/coordgroup.h"

#include "SireMol/molecule.h"
#include "SireMol/element.h"

#include "SireMM/atomiccharges.h"
#include "SireMM/switchingfunction.h"

class QTextStream;

SIRE_BEGIN_HEADER

namespace Squire
{
class MolproFF;
}

QDataStream& operator<<(QDataStream&, const Squire::MolproFF&);
QDataStream& operator>>(QDataStream&, Squire::MolproFF&);

namespace Squire
{

class MolproCalculator;
class MolproSession;

using boost::tuple;

using SireBase::MajVersion;

using SireCAS::Symbols;
using SireCAS::Values;

using SireMol::Molecule;
using SireMol::MoleculeID;
using SireMol::Residue;
using SireMol::NewAtom;
using SireMol::Element;
using SireMol::CutGroupID;

using SireFF::FFComponent;
using SireFF::ParameterMap;
using SireFF::ParameterName;

using SireVol::Space;
using SireVol::CoordGroup;

using SireMM::AtomicCharges;
using SireMM::SwitchingFunction;

/** This class implements a forcefield that calculates the QM energy
    of atoms in the QM region as they are polarised by the charges
    of atoms in the MM region. This forcefield uses Molpro to perform
    the energy calculation. It is most efficient to place this
    forcefield on a MolproProcessor, as this allows the forcefield
    to use a single instance of Molpro, rather than starting
    and stopping Molpro for each energy evaluation.

    Note that the current version only
    supports entire molecules in the QM or MM regions. Later
    versions of this forcefield will expand this support to
    include parts of molecules, and defined link atoms.

    @author Christopher Woods
*/
class SQUIRE_EXPORT MolproFF : public SireFF::FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const MolproFF&);
friend QDataStream& ::operator>>(QDataStream&, MolproFF&);

friend class MolproSession;    //friend so can call all protected functions
friend class MolproCalculator; //used to recalculate the energy

public:
    MolproFF();

    MolproFF(const Space &space,
             const SwitchingFunction &switchfunc);

    MolproFF(const MolproFF &other);

    ~MolproFF();

    MolproFF& operator=(const MolproFF &other);

    class SQUIRE_EXPORT Components : public SireFF::FFBase::Components
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

        const FFComponent& qm() const
        {
            return e_qm;
        }

        static QString describe_qm();

    protected:
        void setForceField(const FFBase &ffbase);

    private:
        /** The QM component */
        FFComponent e_qm;
    };

    class SQUIRE_EXPORT Parameters : public SireFF::FFBase::Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);

        ~Parameters();

        /** Return the default source of the coulomb parameters (charges) */
        const ParameterName& coulomb() const
        {
            return coulomb_params;
        }

        static Parameters default_sources;

     private:
        /** The name and default source property of the coulomb parameters
            of the MM region */
        ParameterName coulomb_params;
    };

    const Parameters& parameters() const
    {
        return MolproFF::Parameters::default_sources;
    }

    const MolproFF::Components& components() const
    {
        return *components_ptr;
    }

    static const char* typeName()
    {
        return "Squire::MolproFF";
    }

    const char* what() const
    {
        return MolproFF::typeName();
    }

    MolproFF* clone() const
    {
        return new MolproFF(*this);
    }

    const QFileInfo& molproExe() const;
    const QDir& molproTempDir() const;

    void setMolproExe(const QFileInfo &molpro);
    void setMolproTempDir(const QDir &tempdir);

    virtual void setEnergyOrigin(double nrg_zero);

    const Space& space() const;
    const SwitchingFunction& switchingFunction() const;

    quint32 qmVersion() const;

    bool addToQM(const Molecule &molecule);
    bool addToMM(const Molecule &molecule,
                 const ParameterMap &map = ParameterMap());

    bool addToQM(const QList<Molecule> &molecules);
    bool addToMM(const QList<Molecule> &molecules,
                 const ParameterMap &map = ParameterMap());

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);
    bool change(const NewAtom &atom);

    bool remove(const Molecule &molecule);

    virtual QString molproCommandInput();

    const QVector<double>& mmCoordsAndChargesArray() const;
    const QVector<double>& qmCoordsArray() const;

protected:
    void recalculateEnergy();

    bool updateArrays();

    //protected functions designed to be overloaded by child classes, and
    //only called by MolproCalculator
    virtual Values recalculateEnergy(MolproSession &session);

private:
    void registerComponents();

    QString energyCmdString() const;
    QString qmCoordString() const;
    QString mmCoordAndChargesString();

    int nQMAtomsInArray() const;
    int nMMAtomsInArray() const;
    int nAtomsInArray() const;

    enum { NOCHANGE = 0x0000, CHANGE = 0x0001, ADD = 0x0010 };

    int _pvt_addToQM(const Molecule &molecule);
    int _pvt_addToMM(const Molecule &molecule, const ParameterMap &map);

    void rebuildQMCoordGroup();

    template<class T>
    bool _pvt_change(const T &obj);

    class QMMolecule
    {
    public:
        QMMolecule();
        QMMolecule(const Molecule &molecule);

        QMMolecule(const QMMolecule &other);

        ~QMMolecule();

        QMMolecule& operator=(const QMMolecule &other);

        const Molecule& molecule() const;

        bool change(const Molecule &molecule);
        bool change(const Residue &residue);
        bool change(const NewAtom &atom);

        void addTo(QVector<double> &qm_array);
        void update(QVector<double> &qm_array) const;

        QString coordString() const;

        int nAtomsInArray() const;

    private:
        /** The molecule itself */
        Molecule mol;

        /** The element types of all of the atoms */
        QVector< QVector<Element> > elements;

        /** The index in the qm_coords array of each atom
            in this molecule */
        QVector< QVector<int> > indicies;
    };

    class MMMolecule
    {
    public:
        MMMolecule();
        MMMolecule(const Molecule &molecule,
                   const QString &charge_property);

        MMMolecule(const MMMolecule &other);

        ~MMMolecule();

        MMMolecule& operator=(const MMMolecule &other);

        const Molecule& molecule() const;

        bool change(const Molecule &molecule);
        bool change(const Residue &residue);
        bool change(const NewAtom &atom);

        void update(const CoordGroup &qm_coordgroup,
                    const Space &space, const SwitchingFunction &switchfunc);

        void addTo(QVector<double> &mm_coords_and_charges, int start_idx);
        void update(QVector<double> &mm_coords_and_charges);

        int nAtomsInArray() const;

        const QString& chargeProperty() const;

        QString coordAndChargesString() const;

    private:
        /** The molecule itself */
        Molecule mol;

        /** The name of the property containing the
            atomic charges of the atoms. */
        QString chg_property;

        /** The partial charges on the atoms */
        AtomicCharges chgs;

        /** The replicas of each CoordGroup of this molecule,
            within the cutoff, together with their closest
            distance to the QM molecule */
        QVector< QList< tuple<double,CoordGroup> > > coords;

        /** The IDs of CutGroups that need rebuilding. Empty
            means that all of them do! */
        QSet<CutGroupID> cgids_to_be_rebuilt;

        /** The index of the first atom
            of this molecule in the MM coords
            and charges array - equals -1 if this molecule
            is not currently in the array. */
        int idx;

        /** The number of atoms of this molecule
            that are in the MM coords and charges
            array (could be more than number of atoms
            in molecule, as multiple copies of the
            molecule may be included... */
        uint nats;

        /** Whether or not all of the CutGroups need rebuilding */
        bool rebuild_all;
    };

    /** The space for this forcefield - this is used to map
        to infinite cartesian space for the QM calculation
        (only maps the position of the MM parts) */
    Space spce;

    /** The switching function used to scale down the MM atom
        charges */
    SwitchingFunction switchfunc;

    /** The full name and path to the molpro executable */
    QFileInfo molpro_exe;

    /** The temporary directory in which to run the Molpro calculation.
        Each calculation will be carried out within a new unique directory
        within this directory. */
    QDir molpro_tmpdir;

    /** All of the coordinates of the QM atoms */
    QVector<double> qm_coords;

    /** Array containing all of the coordinates and charges
        of the MM atoms */
    QVector<double> mm_coords_and_charges;

    /** Hash of all of the QM molecules in this forcefield, indexed
        by their MoleculeID */
    QHash<MoleculeID, QMMolecule> qm_mols;

    /** CoordGroup containing all of the atoms of all of the
        QM molecules... */
    CoordGroup qm_coordgroup;

    /** Hash of all of the MM molecules in this forcefield, indexed
        by their MoleculeID */
    QHash<MoleculeID, MMMolecule> mm_mols;

    /** Version number which tracks changes in the QM molecules.
        This version number is incremented whenever a QM molecule
        is added or removed. */
    MajVersion qm_version;

    /** The 'zero' energy - this is held in hartrees and is subtracted
        from the energy returned by Molpro before it is converted
        to kcal mol-1. This is to help avoid numerical instability
        caused by taking the difference of very large kcal mol-1 values. */
    double zero_nrg;

    /** Set of MoleculeIDs of MM molecules that need rebuilding */
    QSet<MoleculeID> rebuild_mm;

    /** Do we need to rebuild all of the coordinates?
        (we do whenever the QM coordinates change) */
    bool rebuild_all;

    /** Do we really need to recalculate the QM/MM energy? We
        only need to do so if the QM or MM arrays change */
    bool need_recalculate_qmmm;

    /** Pointer to the object containing the components of
        this forcefield */
    const MolproFF::Components *components_ptr;
};

/** Return the full name and path of the molpro executable
    used to calculate the energy. */
inline const QFileInfo& MolproFF::molproExe() const
{
    return molpro_exe;
}

/** Return the directory in which Molpro calculations
    will be run. */
inline const QDir& MolproFF::molproTempDir() const
{
    return molpro_tmpdir;
}

/** Return the version number of the qm region - this is
    incremented whenever molecules are added or removed from
    the QM region */
inline quint32 MolproFF::qmVersion() const
{
    return qm_version;
}

/** Return the space within which the QM and MM atoms exist */
inline const Space& MolproFF::space() const
{
    return spce;
}

/** Return the switching function used to scale the charges of the
    MM atoms */
inline const SwitchingFunction& MolproFF::switchingFunction() const
{
    return switchfunc;
}

/** Return the current array containing MM coordinates and charges. Call
    updateArrays() before this function to ensure that the returned
    array is up to date. */
inline const QVector<double>& MolproFF::mmCoordsAndChargesArray() const
{
    return mm_coords_and_charges;
}

/** Return the current array containing QM coordinates. Call
    updateArrays() before this function to ensure that the returned
    array is up to date. */
inline const QVector<double>& MolproFF::qmCoordsArray() const
{
    return qm_coords;
}

}

Q_DECLARE_METATYPE(Squire::MolproFF);

SIRE_END_HEADER

#endif
