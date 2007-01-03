#ifndef SIREMM_TIP4P_H
#define SIREMM_TIP4P_H

#include "cljff.h"

#include "SireFF/parametermap.h"

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

using SireFF::ParameterMap;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::MoleculeID;

using SireVol::AABox;

using SireMaths::Vector;

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

    class SIREMM_EXPORT Components : public CLJFF::Components
    {
    public:
        Components();
        Components(const FFBase &ffbase, const Symbols &symbols);
        Components(const Components &other);

        ~Components();
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

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);

    bool add(const Molecule &molecule, const ParameterMap &map = ParameterMap());

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
