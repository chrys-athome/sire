
#include "SireCAS/qhash_sirecas.h"
#include "SireMol/qhash_siremol.h"

#include "SireUnits/units.h"

#include "tip4pff.h"

#include "chargetable.h"
#include "ljtable.h"

#include "detail/molcljinfo.h"

#include "SireVol/coordgroup.h"

using namespace SireMM;
using namespace SireMM::detail;

using namespace SireDB;

/** Constructor */
Tip4PFF::Tip4PFF() : CLJFF()
{}

/** Construct a CLJ forcefield using the passed Space and
    switching function (combining rules don't affect TIP4P) */
Tip4PFF::Tip4PFF(const Space &space, const SwitchingFunction &switchfunc)
           : CLJFF(space, switchfunc)
{}

/** Copy constructor */
Tip4PFF::Tip4PFF(const Tip4PFF &other)
        : CLJFF(other), mols(other.mols)
{}

/** Destructor */
Tip4PFF::~Tip4PFF()
{}

/** This function is used to calculate the charge and LJ energy
    of two groups based on the inter-atomic inverse-square-distances stored in 'distmatrix'
    and using the combined CLJ parameters listed in 'cljmatrix', in the
    passed workspace. The total coulomb and LJ energies are returned in
    the workspace. */
double calculatePairEnergy(CLJWorkspace &workspace)
{
    DistMatrix &distmatrix = workspace.distmatrix;
    CLJPairMatrix &cljmatrix = workspace.cljmatrix;

    int nats0 = distmatrix.nOuter();
    int nats1 = distmatrix.nInner();

    double nrg = 0;

    //loop over all pairs of atoms
    for (int i=0; i<nats0; ++i)
    {
        distmatrix.setOuterIndex(i);
        cljmatrix.setOuterIndex(i);

        for (int j=0; j<nats1; ++j)
        {
            //get the distance and CLJPair for this atom pair
            double invdist2 = distmatrix[j];
            const CLJPair &cljpair = cljmatrix[j];

            double sig2_over_dist2 = SireMaths::pow_2(cljpair.sigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

            //coulomb energy
            nrg += SireUnits::one_over_four_pi_eps0 *
                                     cljpair.charge2() * std::sqrt(invdist2);

            //LJ energy
            nrg += 4 * cljpair.epsilon() *
                                     ( sig12_over_dist12 - sig6_over_dist6 );
        }
    }

    return nrg;
}

/** Calculate the CLJ energy of interaction of group0, with CLJ parameters
    in 'clj0' and group1, with CLJ parameters in 'clj1', using the
    space 'space', and using the provided workspace. The total
    coulomb and LJ energies are returned in the workspace. */
double calculateEnergy(const CoordGroup &group0,
                       const QVector<CLJParameter> &clj0,
                       const CoordGroup &group1,
                       const Space &space,
                       const SwitchingFunction &switchfunc,
                       CLJWorkspace &workspace)
{
    double nrg = 0;

    DistMatrix &distmatrix = workspace.distmatrix;

    if ( not space.beyond(15.0, group0, group1) )
    {
        double mindist = space.calcInvDist2(group0, group1, distmatrix);

        double scl = 0;

        if (mindist <= 14.5)
            scl = 1;
        else if (mindist < 15.0)
            scl = (225.0 - mindist*mindist) * 0.0677966101;

        if (scl != 0)
        {
            double inrg = 0;

            //get combined parameters directly...
            workspace.cljmatrix.redimension(4,4);

            for (int i=0; i<4; ++i)
            {
                workspace.cljmatrix.setOuterIndex(i);

                const CLJParameter *cljarray = clj0.constData();

                const CLJParameter &cljparam0 = cljarray[i];

                for (int j=0; j<4; ++j)
                {
                    const CLJParameter &cljparam1 = cljarray[j];

                    workspace.cljmatrix[j] = CLJPair::arithmetic(cljparam0,cljparam1);
                }
            }

            CLJPairMatrix &cljmatrix = workspace.cljmatrix;

            int nats0 = distmatrix.nOuter();
            int nats1 = distmatrix.nInner();

            //loop over all pairs of atoms
            for (int i=0; i<nats0; ++i)
            {
                distmatrix.setOuterIndex(i);
                cljmatrix.setOuterIndex(i);

                for (int j=0; j<nats1; ++j)
                {
                    //get the distance and CLJPair for this atom pair
                    double invdist2 = distmatrix[j];
                    const CLJPair &cljpair = cljmatrix[j];

                    double sig2_over_dist2 = SireMaths::pow_2(cljpair.sigma()) * invdist2;
                    double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
                    double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

                    //coulomb energy
                    inrg += SireUnits::one_over_four_pi_eps0 *
                                           cljpair.charge2() * std::sqrt(invdist2);

                    //LJ energy
                    inrg += 4 * cljpair.epsilon() *
                                           ( sig12_over_dist12 - sig6_over_dist6 );
                }
            }

            nrg += scl * inrg;
        }
    }

    return nrg;
}

/** Recalculate the total energy of this forcefield from scratch */
void Tip4PFF::recalculateEnergy()
{
    //calculate the total CLJ energy of all molecule pairs...
    double cnrg = 0.0;
    double ljnrg = 0.0;

    int nmols = mols.count();

    const MolCLJInfo *molarray = mols.constData();

    double nrg;

    const Space &myspace = space();
    const SwitchingFunction &switchfunc = switchingFunction();
    CLJWorkspace &wspace = workspace();

    //loop over all molecule pairs
    for (int i=0; i<nmols-1; ++i)
    {
        const MolCLJInfo &mol0 = molarray[i];

        const CoordGroup &group0 = mol0.coordinates().constData()[0];
        const QVector<CLJParameter> &cljparams = mol0.parameters().constData()[0];

        for (int j=i+1; j<nmols; ++j)
        {
            const MolCLJInfo &mol1 = molarray[j];

            nrg += ::calculateEnergy( group0,
                                      cljparams,
                                      mol1.coordinates().constData()[0],
                                      myspace,
                                      switchfunc, wspace );
        }
    }

    //this->setComponent( coulomb(), cnrg );
    //this->setComponent( lj(), ljnrg );
    this->setComponent( total(), nrg );
}

/** Return the molecule in this forcefield that has the ID 'molid'

    \throw SireMol::missing_molecule
*/
const Molecule& Tip4PFF::molecule(MoleculeID molid) const
{
    throw SireError::incomplete_code( QObject::tr(
        "Need to write Tip4PFF::molecule(molid)"), CODELOC );

    return mols[0].molecule();
}

/** Temporary function used to add a molecule with passed charge and LJ
    parameters */
void Tip4PFF::add(const Molecule &mol, const ChargeTable &charges,
                  const LJTable &ljs)
{
    charges.assertCompatibleWith(mol);
    ljs.assertCompatibleWith(mol);

    //extract arrays of the CLJ parameters
    QVector< QVector<CLJParameter> > cljparams;

    QVector< ParameterGroup<ChargeParameter> > chargeparams = charges.parameterGroups();
    QVector< ParameterGroup<LJParameter> > ljparams = ljs.parameterGroups();

    int ncg = chargeparams.count();

    cljparams.reserve(ncg);

    for (int i=0; i<ncg; ++i)
    {
        QVector<CLJParameter> cljs;

        int nparams = chargeparams[i].parameters().count();

        const ChargeParameter *chargearray = chargeparams[i].parameters().constData();
        const LJParameter *ljarray = ljparams[i].parameters().constData();

        cljs.reserve(nparams);

        for (int j=0; j<nparams; ++j)
        {
            cljs.append( CLJParameter(chargearray[j],ljarray[j]) );
        }

        cljparams.append(cljs);
    }

    //add this molecule to the list
    mols.append( MolCLJInfo(mol, cljparams) );
}

/** Move the molecule 'molecule' */
void Tip4PFF::move(const Molecule &molecule)
{}

void Tip4PFF::move(const Residue &residue)
{}

void Tip4PFF::move(const MovedMols &movedmols)
{}

void Tip4PFF::change(const Molecule &molecule, const ParameterTable &params)
{}

void Tip4PFF::change(const Residue &residue, const ParameterTable &params)
{}

void Tip4PFF::change(const ChangedMols &changedmols)
{}

void Tip4PFF::add(const Molecule &molecule, const ParameterTable &params,
                     int groupid)
{}

void Tip4PFF::add(const Residue &residue, const ParameterTable &params,
                     int groupid)
{}

void Tip4PFF::remove(const Molecule &molecule)
{}

void Tip4PFF::remove(const Residue &residue)
{}
