#ifndef SQUIRE_MOLPROCALCULATOR_H
#define SQUIRE_MOLPROCALCULATOR_H

#include <QFileInfo>
#include <QDir>

#include <boost/shared_ptr.hpp>

#include "SireFF/ffcalculator.h"

#include "molproff.h"

SIRE_BEGIN_HEADER

namespace Squire
{

class MolproSession;

using SireFF::ForceField;
using SireCAS::Values;

/** This is the calculator used to calculate energies and forces
    via an external Molpro process.

    This is the main driver class behind the Molpro energy
    evaluation. The hierarchy of Molpro classes is as follows;

    MolproFF : The forcefield that uses Molpro to evaluate the QM/MM energy

    MolproProcessor : Processor that can run MolproFF derived forcefields.
                      A MolproFF derived forcefield must be placed on a
                      MolproProcessor so that it can be evaluated.

    MolproCalculator : Calculator that is used by MolproProcessor to
                       actually evaluate the MolproFF derived forcefield.

    MolproSession : An individual connection to a single running instance
                    of Molpro - this class provides the interface to the
                    running Molpro program that is used by MolproCalculator
                    and MolproFF to evaluate the QM/MM energies and forces.

    With this design, you will only need to override MolproFF if you wish
    to change the type of Molpro calculation.

    @author Christopher Woods
*/
class SQUIRE_EXPORT MolproCalculator : public SireFF::FFCalculatorBase
{
public:
    MolproCalculator(const ForceField &forcefield,
                     const QFileInfo &molpro_exe = "molpro",
                     const QDir &temp_dir = QDir::temp());

    ~MolproCalculator();

    double getEnergies(Values &values);

    const Molecule& molecule(MoleculeID molid) const;

    void calculateEnergy();

    bool move(const Molecule &molecule);

    bool move(const Residue &residue);

    bool setForceField(const ForceField &forcefield);

    ForceField forcefield() const;

private:

    void restartMolpro();

    /** The Molpro forcefield being evaluated */
    SireBase::SharedPolyPointer<MolproFF> molproff;

    /** The path to the molpro executable */
    QFileInfo molpro_exe;

    /** The directory that will be used to run the calculation (tmpdir) */
    QDir temp_dir;

    /** The total energy of the forcefield */
    double total_nrg;

    /** The values of the energy components */
    Values nrg_components;

    /** Shared pointer to the Molpro session that is
        used to calculate the energies */
    boost::shared_ptr<MolproSession> molpro_session;
};

}

SIRE_END_HEADER

#endif
