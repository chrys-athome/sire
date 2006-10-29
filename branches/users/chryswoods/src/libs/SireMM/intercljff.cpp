
#include "SireCAS/qhash_sirecas.h"
#include "SireMol/qhash_siremol.h"

#include "intercljff.h"

#include "detail/molcljinfo.h"

#include "SireVol/coordgroup.h"

using namespace SireMM;
using namespace SireMM::detail;

/** Constructor */
InterCLJFF::InterCLJFF()
{}

/** Copy constructor */
InterCLJFF::InterCLJFF(const InterCLJFF &other)
{}

/** Destructor */
InterCLJFF::~InterCLJFF()
{}

/** Recalculate the total energy of this forcefield from scratch */
void InterCLJFF::recalculateEnergy()
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
                                    combiningRules(), switchingFunction(),
                                    workspace() );

            cnrg += workspace().cnrg;
            ljnrg += workspace().ljnrg;
        }
    }

    this->setComponent( coulomb(), cnrg );
    this->setComponent( lj(), ljnrg );
    this->setComponent( total(), cnrg+ljnrg );
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
