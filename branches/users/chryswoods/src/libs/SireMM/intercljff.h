#ifndef SIREFF_INTERCLJFF_H
#define SIREFF_INTERCLJFF_H

#include "cljff.h"

namespace SireFF
{
class InterCLJFF;
}

QDataStream& operator<<(QDataStream&, const SireFF::InterCLJFF&);
QDataStream& operator>>(QDataStream&, SireFF::InterCLJFF&);

namespace SireFF
{

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

    const char* what() const
    {
        return "SireFF::InterCLJFF";
    }

protected:
    void recalculateEnergy();

    /** Information about every molecule contained in this forcefield */
    QVector<MolCLJInfo> mols;

};

}


#endif
