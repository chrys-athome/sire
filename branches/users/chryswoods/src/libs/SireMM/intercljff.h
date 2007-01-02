#ifndef SIREMM_INTERCLJFF_H
#define SIREMM_INTERCLJFF_H

#include "cljff.h"

#include "SireFF/parametermap.h"

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

using SireFF::ParameterMap;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::MoleculeID;

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

        ~Components();
        
        Components& operator=(const Components &other);
    };

    class SIREMM_EXPORT Parameters : public CLJFF::Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);

        ~Parameters();
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

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);
    
    bool add(const Molecule &mol, const ParameterMap &map = ParameterMap());

    bool remove(const Molecule &mol);

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
    QVector<detail::ChangedMolCLJInfo> changedmols;

    /** Hash mapping MoleculeID to index in 'changedmols' */
    QHash<MoleculeID, int> molid_to_changedindex;

    /** The IDs of all of the molecules that were removed since the last
        energy update */
    QSet<MoleculeID> removedmols;
};

}


#endif
