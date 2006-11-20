#ifndef SIREMM_TIP4P_H
#define SIREMM_TIP4P_H

#include "cljff.h"

#include "SireVol/aabox.h"

namespace SireMM
{
class Tip4PFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::Tip4PFF&);
QDataStream& operator>>(QDataStream&, SireMM::Tip4PFF&);

namespace SireMM
{

class ChargeTable;
class LJTable;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::MoleculeID;

using SireVol::AABox;

using SireMaths::Vector;

using SireFF::ChangedMols;
using SireFF::MovedMols;

using SireDB::ParameterTable;

/** A Tip4PFF is a forcefield that calculates the intermolecular coulomb and
    Lennard Jones energies of all contained TIP4P water molecules. An Tip4PFF is perhaps
    the most expensive type of MM forcefield, as it must calculate the full
    pair-pair interactions between all pairs of molecules in the forcefield
    that are within the cutoff distance.

    \author Christopher Woods
*/
class SIREMM_EXPORT Tip4PFF : public CLJFF
{

friend QDataStream& ::operator<<(QDataStream&, const Tip4PFF&);
friend QDataStream& ::operator>>(QDataStream&, Tip4PFF&);

public:
    Tip4PFF();

    Tip4PFF(const Space &space, const SwitchingFunction &switchfunc);

    Tip4PFF(const Tip4PFF &other);

    ~Tip4PFF();

    static const char* typeName()
    {
        return "SireMM::Tip4PFF";
    }

    const char* what() const
    {
        return Tip4PFF::typeName();
    }

    Tip4PFF* clone() const
    {
        return new Tip4PFF(*this);
    }

    const Molecule& molecule(MoleculeID molid) const;

    void move(const Molecule &molecule);
    void move(const Residue &residue);
    void move(const MovedMols &movedmols);

    void change(const Molecule &molecule, const ParameterTable &params);
    void change(const Residue &residue, const ParameterTable &params);
    void change(const ChangedMols &changedmols);

    void add(const Molecule &molecule, const ParameterTable &params,
             int groupid);
    void add(const Residue &residue, const ParameterTable &params,
             int groupid);

    void add(const Molecule &molecule,
             const ChargeTable &charges, const LJTable &ljs);

    void remove(const Molecule &molecule);
    void remove(const Residue &residue);

protected:
    void recalculateEnergy();

    static double calculateEnergy(const Vector *array0, int nats0,
                                  const Vector *array1, int nats1,
                                  const CLJParameter *cljarray,
                                  const Space &space);

    /** Information about every molecule contained in this forcefield */
    QVector< QVector<Vector> > mols;

    /** Vector of all of the AABoxes */
    QVector<AABox> aaboxes;

    /** The CLJ parameters for Tip4P */
    QVector<CLJParameter> cljparams;
};

}


#endif
