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

SIRE_BEGIN_HEADER

namespace Squire
{
class MolproFF;
class MolproProcessor;
}

QDataStream& operator<<(QDataStream&, const Squire::MolproFF&);
QDataStream& operator>>(QDataStream&, Squire::MolproFF&);

namespace SireMM
{
class ChargeTable;
}

namespace Squire
{

using SireMol::Molecule;
using SireMol::MoleculeID;
using SireMol::Residue;

using SireMM::ChargeTable;
using SireMM::ChargeParameter;

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

friend class MolproProcessor; //friend so can all protected functions
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

protected:
    void recalculateEnergy(MolproProcessor &processor);

    void recalculateEnergy();  //throw an exception

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
    QVector<Molecule> qm_molecules;

    /** The coordinates of all of the QM molecules, placed into
        a single array. The coordinates are in order of the molecules
        in qm_molecules, then the atoms in AtomID order. The coordinates
        are stored in units of bohr radii */
    QVector<double> qm_coords;

    /** Hash mapping MoleculeID to the index in qm_molecules */
    QHash<MoleculeID, int> qm_molid_to_index;

    /** The MM molecules */
    QVector<Molecule> mm_molecules;

    /** The charges on all of the atoms of the MM molecules */
    QVector< QVector<ChargeParameter> > mm_charges;

    /** The coordinates and charges of all of the MM molecules,
        placed into a single array. The coordinates and charges are
        placed in the order of the molecules in mm_molecules, then
        the atoms in AtomID order. The coordinates are in units of
        bohr radii, while the charges are in atomic charge units */
    QVector<double> mm_coords_and_charges;

    /** Hash mapping MoleculeID to the index in mm_molecules */
    QHash<MoleculeID, int> mm_molid_to_index;

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

}

Q_DECLARE_METATYPE(Squire::MolproFF)

SIRE_END_HEADER

#endif
