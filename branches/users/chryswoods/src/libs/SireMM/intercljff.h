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
class InterCLJFF : public CLJFF
{

friend QDataStream& ::operator<<(QDataStream&, const InterCLJFF&);
friend QDataStream& ::operator>>(QDataStream&, InterCLJFF&);

public:
    InterCLJFF();
    
    InterCLJFF(const InterCLJFF &other);
    
    ~InterCLJFF();

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
    
    void remove(const Molecule &molecule);
    void remove(const Residue &residue);

protected:
    void recalculateEnergy();

    /** Information about every molecule contained in this forcefield */
    QVector<detail::MolCLJInfo> mols;

};

}


#endif
