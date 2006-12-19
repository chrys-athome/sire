
#include "SireCAS/qhash_sirecas.h"
#include "SireMol/qhash_siremol.h"

#include <QDebug>

#include "intercljff.h"

#include "chargetable.h"
#include "ljtable.h"

#include "detail/molcljinfo.h"

#include "SireVol/coordgroup.h"

#include "SireMol/errors.h"

using namespace SireMM;
using namespace SireMM::detail;

using namespace SireDB;

///////////
/////////// Implementation of InterCLJFF::Components
///////////

/** Constructor */
InterCLJFF::Components::Components() : CLJFF::Components()
{}

/** Copy constructor */
InterCLJFF::Components::Components(const Components &other)
           : CLJFF::Components(other)
{}

/** Construct with a specified name */
InterCLJFF::Components::Components(const QString &basename)
           : CLJFF::Components(basename)
{}

/** Destructor */
InterCLJFF::Components::~Components()
{}

///////////
/////////// Implementation of InterCLJFF
///////////

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

    //clear the list of moved molecules
    movedmols.clear();
    molid_to_movedindex.clear();
    removedmols.clear();
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
        //all of the molecules have been deleted! Set the energy to
        //zero and clear the moved list
        this->setComponent( coulomb(), 0 );
        this->setComponent( lj(), 0 );
        this->setComponent( total(), 0 );

        movedmols.clear();
        removedmols.clear();
        molid_to_movedindex.clear();

        return;
    }

    //temp components to hold the change in energies
    double icnrg = 0;
    double iljnrg = 0;

    //now loop over all of the molecules in the system
    const MolCLJInfo *molarray = mols.constData();
    const ChangedMolCLJInfo *movedarray = movedmols.constData();

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
                const ChangedMolCLJInfo &movedmol = movedarray[j];

                //calculate the energy of the new configuration
                calculateEnergy(mol, movedmol.newParts(), space(),
                                switchingFunction(), workspace());

                icnrg += workspace().cnrg;
                iljnrg += workspace().ljnrg;

                calculateEnergy(mol, movedmol.oldParts(), space(),
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

            const ChangedMolCLJInfo &movedmol0 = movedarray[idx];

            if (movedmol0.movedAll())
            {
                //the whole of this molecule has been moved - calculate the
                //change in energy of the whole of this molecule with the whole
                //of the other moved molecules
                for (int j=idx+1; j<nmoved; ++j)
                {
                    const ChangedMolCLJInfo &movedmol1 = movedarray[j];

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
                    const ChangedMolCLJInfo &movedmol1 = movedarray[j];

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
                        calculateEnergy(movedmol0.newParts(), movedmol1.newMol(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace().cnrg;
                        iljnrg += workspace().ljnrg;

                        calculateEnergy(movedmol0.oldParts(), movedmol1.oldMol(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace().cnrg;
                        iljnrg -= workspace().ljnrg;

                        //now the moved part of mol1 with all of mol0
                        calculateEnergy(movedmol0.newMol(), movedmol1.newParts(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace().cnrg;
                        iljnrg += workspace().ljnrg;

                        calculateEnergy(movedmol0.oldMol(), movedmol1.oldParts(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace().cnrg;
                        iljnrg -= workspace().ljnrg;

                        //finally, remove the doubly-counted contribution of the
                        //moved part of mol0 with the moved part of mol1
                        calculateEnergy(movedmol0.newParts(), movedmol1.newParts(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace().cnrg;   // subtract doubly-counted contribution
                        iljnrg -= workspace().ljnrg;

                        calculateEnergy(movedmol0.oldParts(), movedmol1.oldParts(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace().cnrg;
                        iljnrg += workspace().ljnrg;
                    }
                }
            }
        }
    }

    //finally, loop over all of the molecules that have been removed - the energy
    //of non-moved molecules with removed molecules has already been calculated,
    //as has the energy of moved molecules that are before the removed molecules
    //in the moved list. We only now have to calculate the energy of the removed
    //molecules with all of the molecules that lie above us in the moved list
    for (QSet<MoleculeID>::const_iterator it = removedmols.constBegin();
         it != removedmols.constEnd();
         ++it)
    {
        //get the index of the removed molecule in the moved list
        int idx = molid_to_movedindex.value(*it);

        const ChangedMolCLJInfo &removedmol = movedarray[idx];

        //calculate the change in energy associated with removing this molecule
        //(only have to do the 'old' energy, as the new energy is zero)

        for (int j=idx+1; j<nmoved; ++j)
        {
            const ChangedMolCLJInfo &movedmol = movedarray[j];

            calculateEnergy(removedmol.oldMol(), movedmol.oldMol(),
                            space(), switchingFunction(), workspace());

            icnrg -= workspace().cnrg;
            iljnrg -= workspace().ljnrg;
        }
    }

    //clear the list of moved molecules
    movedmols.clear();
    molid_to_movedindex.clear();
    removedmols.clear();

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
    QHash<MoleculeID,int>::const_iterator it = molid_to_molindex.find(molid);

    if (it == molid_to_molindex.end())
        throw SireMol::missing_molecule( QObject::tr(
              "There is no molecule with ID == %1 in the forcefield "
              "\"%2\" (of type %3)")
                  .arg(molid).arg(this->name()).arg(this->what()), CODELOC );

    return mols.constData()[it.value()].molecule();
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
    molid_to_molindex.insert( mol.ID(), mols.count()-1 );
}

/** Move the molecule 'molecule' */
bool InterCLJFF::move(const Molecule &molecule)
{
    //try to find this molecule in this forcefield,
    //based on its ID number
    MoleculeID molid = molecule.ID();

    if ( not molid_to_molindex.contains(molid) )
        //the molecule has either been removed or
        //did not exist in this forcefield. Moving it
        //will thus not change the energy
        return isDirty();

    if ( molid_to_movedindex.contains(molid) )
    {
        //this molecule has already been changed since the
        //last update - move it
        movedmols[ molid_to_movedindex.value(molid) ].move(molecule);
    }
    else
    {
        //this molecule has not been moved since the last energy
        //evaluation - create a ChangedMolCLJInfo that describes the
        //move

        int idx = molid_to_molindex.value(molid);

        //get the existing copy of the molecule
        MolCLJInfo& oldinfo = mols[idx];

        BOOST_ASSERT( oldinfo.molecule().ID() == molid );

        oldinfo.molecule().assertSameMajorVersion(molecule);

        MolCLJInfo newinfo( molecule, oldinfo.chargeParameters(),
                            oldinfo.ljParameters() );

        //save the old and new molecule
        movedmols.append( ChangedMolCLJInfo(oldinfo,newinfo) );

        molid_to_movedindex.insert(molid, movedmols.count()-1);

        //update the current state of the molecule in the forcefield
        oldinfo = newinfo;
    }

    this->setDirty();
    return isDirty();
}

/** Move the residue 'residue' */
bool InterCLJFF::move(const Residue &residue)
{
    //get the molecule containing this residue
    Molecule molecule = residue.molecule();

    //try to find this molecule in this forcefield,
    //based on its ID number
    MoleculeID molid = molecule.ID();

    if ( not molid_to_movedindex.contains(molid) )
        //the molecule containing this residue has either been removed or
        //did not exist in this forcefield. Moving it
        //will thus not change the energy
        return isDirty();

    if ( molid_to_movedindex.contains(molid) )
    {
        //the molecule containing this residue has already been changed
        //since the last update - move it
        movedmols[ molid_to_movedindex.value(molid) ].move(residue);
    }
    else
    {
        //the molecule containing this residue has not been moved since
        //the last energy evaluation - create a ChangedMolCLJInfo that
        //describes the move

        int idx = molid_to_molindex.value(molid);

        //get the existing copy of the molecule
        MolCLJInfo& oldinfo = mols[idx];

        BOOST_ASSERT( oldinfo.molecule().ID() == molid );

        oldinfo.molecule().assertSameMajorVersion(molecule);

        MolCLJInfo newinfo( molecule, oldinfo.chargeParameters(),
                            oldinfo.ljParameters() );

        //save the old and new molecule
        movedmols.append( ChangedMolCLJInfo(oldinfo,newinfo,
                                            residue.info().cutGroupIDs()) );

        molid_to_movedindex.insert(molid, movedmols.count()-1);

        //update the current state of the molecule in the forcefield
        oldinfo = newinfo;
    }

    this->setDirty();
    return isDirty();
}
