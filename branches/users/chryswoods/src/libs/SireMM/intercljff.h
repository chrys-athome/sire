#ifndef SIREMM_INTERCLJFF_H
#define SIREMM_INTERCLJFF_H

#include "cljff.h"

namespace SireMM
{
class InterCLJFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::InterCLJFF&);
QDataStream& operator>>(QDataStream&, SireMM::InterCLJFF&);

namespace SireMM
{

class ChargeTable;
class LJTable;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::MoleculeID;

using SireFF::ChangedMols;
using SireFF::MovedMols;

using SireDB::ParameterTable;

/** An InterCLJFF is a forcefield that calculates the intermolecular coulomb and
    Lennard Jones energies of all contained molecules. An InterCLJFF is perhaps
    the most expensive type of MM forcefield, as it must calculate the full
    pair-pair interactions between all pairs of molecules in the forcefield
    that are within the cutoff distance.

    \author Christopher Woods
*/
class SIREMM_EXPORT InterCLJFF : public CLJFF
{

friend QDataStream& ::operator<<(QDataStream&, const InterCLJFF&);
friend QDataStream& ::operator>>(QDataStream&, InterCLJFF&);

public:
    InterCLJFF();

    InterCLJFF(const Space &space, const SwitchingFunction &switchfunc);

    InterCLJFF(const InterCLJFF &other);

    ~InterCLJFF();

    class SIREMM_EXPORT Components : public CLJFF::Components
    {
    public:
        Components();
        Components(const Components &other);
        Components(const QString &basename);

        ~Components();

        Components* clone() const
        {
            return new Components(*this);
        }
        
    };

    static const char* typeName()
    {
        return "SireMM::InterCLJFF";
    }

    const char* what() const
    {
        return InterCLJFF::typeName();
    }

    InterCLJFF* clone() const
    {
        return new InterCLJFF(*this);
    }

    const Molecule& molecule(MoleculeID molid) const;

    void add(const Molecule &mol, const ChargeTable &chargetable,
             const LJTable &ljtable);

    bool move(const Molecule &molecule);
    bool move(const Residue &residue);

protected:
    void recalculateViaDelta();
    void recalculateTotalEnergy();

    void recalculateEnergy();

    /** Information about every molecule contained in this forcefield */
    QVector<detail::MolCLJInfo> mols;

    /** Hash mapping MoleculeID to index in 'mols' */
    QHash<MoleculeID, int> molid_to_molindex;

    /** Information about all of the changed molecules since the last
        energy update */
    QVector<detail::ChangedMolCLJInfo> movedmols;

    /** Hash mapping MoleculeID to index in 'movedmols' */
    QHash<MoleculeID, int> molid_to_movedindex;

    /** The IDs of all of the molecules that were removed since the last
        energy update */
    QSet<MoleculeID> removedmols;
};

}


#endif
