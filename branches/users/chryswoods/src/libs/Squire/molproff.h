/**
  * @file
  *
  * C++ Interface: MolproFF
  *
  * Description: Interface for MolproFF
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SQUIRE_MOLPROFF_H
#define SQUIRE_MOLPROFF_H

#include <QMutex>
#include <QVector>

#include "SireFF/ffbase.h"
#include "SireMM/chargeparameter.h"

#include "SireStream/versionid.h"

class QTextStream;

SIRE_BEGIN_HEADER

namespace Squire
{
class MolproFF;
}

QDataStream& operator<<(QDataStream&, const Squire::MolproFF&);
QDataStream& operator>>(QDataStream&, Squire::MolproFF&);

namespace SireMM
{
class ChargeTable;
}

namespace Squire
{

class MolproCalculator;
class MolproSession;

using SireMol::Molecule;
using SireMol::MoleculeID;
using SireMol::Residue;

using SireMM::ChargeTable;
using SireMM::ChargeParameter;

using SireCAS::Values;

using SireStream::VersionID;

/** This class implements a forcefield that calculates the QM energy
    of atoms in the QM region as they are polarised by the charges
    of atoms in the MM region. This forcefield uses Molpro to perform
    the energy calculation, and therefore must be placed on a
    MolproProcessor to work. Note that the current version only
    supports entire molecules in the QM or MM regions. Later
    versions of this forcefield will expand this support to
    include parts of molecules, and defined link atoms.

    @author Christopher Woods
*/
class SQUIRE_EXPORT MolproFF : public SireFF::FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const MolproFF&);
friend QDataStream& ::operator>>(QDataStream&, MolproFF&);

friend class MolproCalculator; //friend so can all protected functions
                               //used to recalculate the energy

public:
    MolproFF();
    MolproFF(const QString &name);

    MolproFF(const MolproFF &other);

    ~MolproFF();

    MolproFF& operator=(const MolproFF &other);

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

    void addToQM(const Molecule &molecule);
    void addToMM(const Molecule &molecule, const ChargeTable &charges);

    void addToQM(const QList<Molecule> &molecules);
    void addToMM(const QList<Molecule> &molecules, const QList<ChargeTable> &charges);

    const QVector<double>& qmCoordinates() const;

    const QVector<double>& mmCoordsAndCharges() const;

    const Molecule& molecule(MoleculeID molid) const;

    bool move(const Molecule &mol);
    bool move(const Residue &res);

    int ID() const;
    VersionID version() const;

protected:
    void recalculateEnergy();  //throw an exception

    //protected functions designed to be overloaded by child classes, and
    //only called by MolproCalculator
    virtual void initialise(QTextStream &ts) const;
    virtual Values recalculateEnergy(MolproSession &session);

private:

    static int getUniqueID();

    void _pvt_addToQM(const Molecule &molecule);
    void _pvt_addToMM(const Molecule &molecule, const ChargeTable &charges);

    void reconstructIndexAndArrays();
    void reconstructMMArray();
    void reconstructQMArray();

    void getNewID();
    void incrementVersion();

    static QMutex id_mutex;
    static int lastid;

    /** The QM molecules */
    QVector< detail::MolproQMMol > qm_molecules;

    /** The coordinates of all of the QM molecules, placed into
        a single array. The coordinates are in order of the molecules
        in qm_molecules, then the atoms in AtomID order. The coordinates
        are stored in units of bohr radii */
    QVector<double> qm_coords;

    /** Hash mapping MoleculeID to the index in qm_molecules */
    QHash<MoleculeID, int> qm_molid_to_index;

    /** The QM molecules that have changed since the last time the
        qm_coords array was constructed */
    QVector< detail::ChangedMolproQMMols > moved_qm_mols;

    /** Hash mapping MoleculeID to changed QM molecule */
    QHash< MoleculeID, int > molid_to_moved_qm_mol;

    /** The MM molecules */
    QVector< detail::MolproMMMol > mm_molecules;

    /** The coordinates and charges of all of the MM molecules,
        placed into a single array. The coordinates and charges are
        placed in the order of the molecules in mm_molecules, then
        the atoms in AtomID order. The coordinates are in units of
        bohr radii, while the charges are in atomic charge units */
    QVector<double> mm_coords_and_charges;

    /** Hash mapping MoleculeID to the index in mm_molecules */
    QHash<MoleculeID, int> mm_molid_to_index;

    /** The MM molecules that have changed since the last time the
        mm_coords_and_charges array was constructed */
    QVector< detail::ChangedMolproMMMols >  moved_mm_mols;

    /** Hash mapping MoleculeID to changed MM molecule */
    QHash< MoleculeID, int > molid_to_moved_mm_mol;

    /** An ID number that uniquely identifies this MolproFF
        (from other MolproFFs) */
    int ffid;

    /** An ID number that identifies the version of this MolproFF
        (this is incremented when molecules are added, removed or
        changed, and is necessary as such changes require a restart
        of the MolproProcessor) */
    VersionID ff_version;
};

/** Return the vector containing the coordinates of the QM atoms,
    arranged into an array of doubles, as
    atom0(x,y,z), atom1(x,y,z) ... atomn(x,y,z), and with the
    coordinates in bohr radii */
inline const QVector<double>& MolproFF::qmCoordinates() const
{
    return qm_coords;
}

/** Return the vector containing the coordinates and charges of the
    MM atoms, arranged into an array of doubles, as
    atom0(x,y,z,q), atom1(x,y,z,q) .... atomn(x,y,z,q)
    with coordinates in bohr radii and charges in atomic charge units */
inline const QVector<double>& MolproFF::mmCoordsAndCharges() const
{
    return mm_coords_and_charges;
}

/** Return the ID number of this Molpro forcefield - this should be unique */
inline int MolproFF::ID() const
{
    return ffid;
}

/** Return the version number of this forcefield - this is incremented whenever
    the molecules in this forcefield are changed (note changed - not moved!)*/
inline VersionID MolproFF::version() const
{
    return ff_version;
}

}

Q_DECLARE_METATYPE(Squire::MolproFF)

SIRE_END_HEADER

#endif
