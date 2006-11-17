
#include "SireCAS/qhash_sirecas.h"
#include "SireMol/qhash_siremol.h"

#include "intercljff.h"

#include "chargetable.h"
#include "ljtable.h"

#include "detail/molcljinfo.h"

#include "SireVol/coordgroup.h"

using namespace SireMM;
using namespace SireMM::detail;

using namespace SireDB;

/** Constructor */
InterCLJFF::InterCLJFF() : CLJFF()
{}

/** Construct a CLJ forcefield using the passed Space, combining rules and
    switching function */
InterCLJFF::InterCLJFF(const Space &space, const SwitchingFunction &switchfunc)
           : CLJFF(space, switchfunc)
{}

/** Copy constructor */
InterCLJFF::InterCLJFF(const InterCLJFF &other)
           : CLJFF(other), mols(other.mols)
{}

/** Destructor */
InterCLJFF::~InterCLJFF()
{}

/** Recalculate the total energy of this forcefield from scratch */
void InterCLJFF::recalculateTotalEnergy()
{
    //calculate the total CLJ energy of all molecule pairs...
    double cnrg = 0.0;
    double ljnrg = 0.0;

    int nmols = mols.count();

    const MolCLJInfo *molarray = mols.constData();

    //loop over all molecule pairs
    for (int i=0; i<nmols-1; ++i)
    {
        const MolCLJInfo &mol0 = molarray[i];

        for (int j=i+1; j<nmols; ++j)
        {
            const MolCLJInfo &mol1 = molarray[j];

            CLJFF::calculateEnergy( mol0, mol1, space(),
                                    switchingFunction(),
                                    workspace() );

            cnrg += workspace().cnrg;
            ljnrg += workspace().ljnrg;
        }
    }

    this->setComponent( coulomb(), cnrg );
    this->setComponent( lj(), ljnrg );
    this->setComponent( total(), cnrg+ljnrg );
}

/** Recalculate the energy by using a delta from the old configuration */
void InterCLJFF::recalculateViaDelta()
{
    int nmols = mols.count();
    int nmoved = movedmols.count();

    if (nmoved == 0)
    {
        //no moved molecules, so just recalculate the total energy
        recalculateTotalEnergy();
        return;
    }
    else if (nmols == 0)
    {
        //there is no old configuration - add all of the moved
        //molecules and recalculate from scratch
        updateChangedMolecules();
        recalculateTotalEnergy();
        return;
    }

    //temp components to hold the change in energies
    double icnrg = 0;
    double iljnrg = 0;

    //now loop over all of the molecules in the system
    const MolCLJInfo *molarray = mols.constData();
    const MovedMolCLJInfo *movedarray = movedmols.constData();

    QHash<MoleculeID,int>::const_iterator it;

    for (int i=0; i<nmols; ++i)
    {
        const MolCLJInfo &mol = molarray[i];

        //is this one of the moved molecules?
        it = molid_to_movedindex.constFind(mol.molecule().ID());

        if (it == molid_to_movedindex.constEnd())
        {
            //this isn't one of the moved molecules.
            //Calculate the difference in energy between this molecule
            //and the old and new configurations of all of the moved
            //molecules
            for (int j=0; j<nmoved; ++j)
            {
                const MovedMolCLJInfo &movedmol = movedmols[j];

                //calculate the energy of the new configuration
                calculateEnergy(mol, movedmol.newPart(), space(),
                                switchingFunction(), workspace());

                icnrg += workspace().cnrg;
                iljnrg += workspace().ljnrg;

                calculateEnergy(mol, movedmol.oldPart(), space(),
                                switchingFunction(), workspace());

                icnrg -= workspace().cnrg;
                iljnrg -= workspace().ljnrg;
            }
        }
        else
        {
            //calculate the energy of the moved molecule with all moved
            //molecules that are after it in the moved mols array
            int idx = it.value();

            const MovedMolCLJInfo &movedmol0 = movedmols[idx];

            if (movedmol0.movedAll())
            {
                //the whole of this molecule has been moved - calculate the
                //change in energy of the whole of this molecule with the whole
                //of the other moved molecules
                for (int j=idx+1; j<nmoved; ++j)
                {
                    const MovedMolCLJInfo &movedmol1 = movedmols[j];

                    calculateEnergy(movedmol0.newMol(), movedmol1.newMol(),
                                    space(), switchingFunction(), workspace());

                    icnrg += workspace().cnrg;
                    iljnrg += workspace().ljnrg;

                    calculateEnergy(movedmol0.oldMol(), movedmol1.oldMol(),
                                    space(), switchingFunction(), workspace());

                    icnrg -= workspace().cnrg;
                    iljnrg -= workspace().ljnrg;
                }
            }
            else
            {
                //only part of this molecule has been moved...
                for (int j=idx+1; j<nmoved; ++j)
                {
                    const MovedMolCLJInfo &movedmol1 = movedmols[j];

                    if (movedmol1.movedAll())
                    {
                        //the whole of the other molecule has moved, so we need
                        //to calculate the change in energy of the whole molecule
                        //with this molecule
                        calculateEnergy(movedmol0.newMol(), movedmol1.newMol(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace().cnrg;
                        iljnrg += workspace().ljnrg;

                        calculateEnergy(movedmol0.oldMol(), movedmol1.oldMol(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace().cnrg;
                        iljnrg -= workspace().ljnrg;
                    }
                    else
                    {
                        //this is the complicated, but hopefully much rarer edge case
                        // - part of this molecule *and* part of the other molecule have
                        // moved. For now, I will do the less efficient method of calculating
                        // the change in energy, namely calculating the energy of the part
                        // of mol0 that moved with all of mol1, then the energy of the part
                        // of mol1 that moved with all of mol0, then finally, subtracting the
                        // energy of the part of mol0 with the part of mol1 (which would
                        // otherwise be double-counted).

                        //ok, so first the moved part of mol0 with all of mol1
                        calculateEnergy(movedmol0.newPart(), movedmol1.newMol(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace.cnrg;
                        iljnrg += workspace.ljnrg;

                        calculateEnergy(movedmol0.oldPart(), movedmol1.oldMol(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace.cnrg;
                        iljnrg -= workspace.ljnrg;

                        //now the moved part of mol1 with all of mol0
                        calculateEnergy(movedmol0.newMol(), movedmol1.newPart(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace.cnrg;
                        iljnrg += workspace.ljnrg;

                        calculateEnergy(movedmol0.oldMol(), movedmol1.oldPart(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace.cnrg;
                        iljnrg -= workspace.ljnrg;

                        //finally, remove the doubly-counted contribution of the
                        //moved part of mol0 with the moved part of mol1
                        calculateEnergy(movedmol0.newPart(), movedmol1.newPart(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace.cnrg;   // subtract doubly-counted contribution
                        iljnrg -= workspace.ljnrg;

                        calculateEnergy(movedmol0.oldPart(), movedmol1.oldPart(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace.cnrg;
                        iljnrg += workspace.ljnrg;
                    }
                }
            }
        }
    }

    //merge the moved molecules back into the main list
    this->updateChangedMolecules();

    //save the new total energy of this forcefield
    this->changeComponent( coulomb(), icnrg );
    this->changeComponent( lj(), iljnrg );
    this->changeComponent( total(), icnrg+iljnrg );
}

/** Calculate the total energy of this forcefield. This will either
    calculate the energy from scratch, or it will calculate it as a
    delta based on the parts of the forcefield that have moved since
    the last evaluation. */
void InterCLJFF::recalculateEnergy()
{
    if (movedmols.isEmpty())
    {
        this->recalculateTotalEnergy();
    }
    else
    {
        this->recalculateViaDelta();
    }
}

/** Return the molecule in this forcefield that has the ID 'molid'

    \throw SireMol::missing_molecule
*/
const Molecule& InterCLJFF::molecule(MoleculeID molid) const
{
    throw SireError::incomplete_code( QObject::tr(
        "Need to write InterCLJFF::molecule(molid)"), CODELOC );

    return mols[0].molecule();
}

/** Temporary function used to add a molecule with passed charge and LJ
    parameters */
void InterCLJFF::add(const Molecule &mol, const ChargeTable &chargetable,
                     const LJTable &ljtable)
{
    chargetable.assertCompatibleWith(mol);
    ljtable.assertCompatibleWith(mol);

    QVector< ParameterGroup<ChargeParameter> > chargeparams = chargetable.parameterGroups();
    QVector< ParameterGroup<LJParameter> > ljparams = ljtable.parameterGroups();

    QVector< QVector<ChargeParameter> > chgs;
    QVector< QVector<LJParameter> >  ljs;

    int ncg = chargeparams.count();
    BOOST_ASSERT(ljparams.count() == ncg);

    chgs.reserve(ncg);
    ljs.reserve(ncg);

    for (int i=0; i<ncg; ++i)
    {
        chgs.append( chargeparams.constData()[i].parameters() );
        ljs.append( ljparams.constData()[i].parameters() );
    }

    //add this molecule to the list
    mols.append( MolCLJInfo(mol, chgs, ljs) );
}

/** Move the molecule 'molecule' */
void InterCLJFF::move(const Molecule &molecule)
{}

void InterCLJFF::move(const Residue &residue)
{}

void InterCLJFF::move(const MovedMols &movedmols)
{}

void InterCLJFF::change(const Molecule &molecule, const ParameterTable &params)
{}

void InterCLJFF::change(const Residue &residue, const ParameterTable &params)
{}

void InterCLJFF::change(const ChangedMols &changedmols)
{}

void InterCLJFF::add(const Molecule &molecule, const ParameterTable &params,
                     int groupid)
{}

void InterCLJFF::add(const Residue &residue, const ParameterTable &params,
                     int groupid)
{}

void InterCLJFF::remove(const Molecule &molecule)
{}

void InterCLJFF::remove(const Residue &residue)
{}
