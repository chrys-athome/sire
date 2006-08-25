#ifndef SIREFF_MOVEDMOLS_H
#define SIREFF_MOVEDMOLS_H

#include "moverecord.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class MovedMols;
}

QDataStream& operator<<(QDataStream&, const SireFF::MovedMols&);
QDataStream& operator>>(QDataStream&, SireFF::MovedMols&);

namespace SireFF
{

/**
This class is used by the forcefield classes to record which molecules or residues have moved. This stores which molecules or residues have moved, together with the new coordinates of those molecules or residues.


@author Christopher Woods
*/
class SIREFF_EXPORT MovedMols
{

friend QDataStream& ::operator<<(QDataStream&, const MovedMols&);
friend QDataStream& ::operator>>(QDataStream&, MovedMols&);

public:
    MovedMols();
    ~MovedMols();

    void move(const Molecule &molecule);
    void move(const Residue &residue);

    template<class T>
    void move(const T &mols_or_res);

private:
    /** Hash of the new coordinates of moved molecules,
        indexed by the MoleculeID of the molecule */
    QHash< MoleculeID, MoveRecord > molcoords;
};

}

Q_DECLARE_METATYPE(SireFF::MovedMols)

SIRE_END_HEADER

#endif
