
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

using namespace SireVol;
using namespace SireDB;

///////////
/////////// Implementation of Tip4PFF::Components
///////////

/** Constructor */
Tip4PFF::Components::Components() : CLJFF::Components()
{}

/** Copy constructor */
Tip4PFF::Components::Components(const Components &other)
           : CLJFF::Components(other)
{}

/** Construct with a specified name */
Tip4PFF::Components::Components(const QString &basename)
           : CLJFF::Components(basename)
{}

/** Destructor */
Tip4PFF::Components::~Components()
{}

///////////
/////////// Implementation of Tip4PFF
///////////

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

/** Calculate the CLJ energy of interaction of group0, with CLJ parameters
    in 'clj0' and group1, with CLJ parameters in 'clj1', using the
    space 'space', and using the provided workspace. The total
    coulomb and LJ energies are returned in the workspace. */
double Tip4PFF::calculateEnergy(const Vector *array0, int nats0,
                                const Vector *array1, int nats1,
                                const CLJParameter *cljarray,
                                const Space &space)
{
    double maxinvdist2(0);
    double tmpdist;

    double inrg = 0;

    //loop over all pairs of atoms
    for (int i=0; i<nats0; ++i)
    {
        const Vector &v0 = array0[i];

        const CLJParameter &cljparam0 = cljarray[i];

        for (int j=0; j<nats1; ++j)
        {
            const Vector &v1 = array1[j];

            const CLJParameter &cljparam1 = cljarray[j];

            double invdist2 = Vector::invDistance2(v0,v1);

            maxinvdist2 = qMax(maxinvdist2, invdist2);

            double sig2_over_dist2 = SireMaths::pow_2(cljparam0.sqrtSigma()*cljparam1.sqrtSigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

            //coulomb energy
            inrg += SireUnits::one_over_four_pi_eps0 *
                                    cljparam0.charge() *cljparam1.charge() * std::sqrt(invdist2);

            //LJ energy
            inrg += 4 * cljparam0.sqrtEpsilon() * cljparam1.sqrtEpsilon() *
                                   ( sig12_over_dist12 - sig6_over_dist6 );
        }
    }

    double mindist = sqrt( 1.0 / maxinvdist2 );

    if (mindist < 14.5)
        return inrg;
    else if (mindist < 15.0)
        return inrg * (225.0 - mindist*mindist) * 0.0677966101;
    else
        return 0;
}

/** Recalculate the total energy of this forcefield from scratch */
void Tip4PFF::recalculateEnergy()
{
    int nmols = mols.count();

    const QVector<Vector> *molarray = mols.constData();
    const AABox *boxarray = aaboxes.constData();

    const CLJParameter *cljarray = cljparams.constData();

    double nrg = 0;

    const Space &myspace = space();

    int nats = 4;

    //loop over all molecule pairs
    for (int i=0; i<nmols-1; ++i)
    {
        const Vector *array0 = molarray[i].constData();

        const AABox &box0 = boxarray[i];

        for (int j=i+1; j<nmols; ++j)
        {
            const AABox &box1 = boxarray[j];

            if (not ( Vector::distance2(box0.center(),box1.center()) >
                      SireMaths::pow_2(15.0 + box0.radius() + box1.radius())) )
            {
                const Vector *array1 = molarray[j].constData();

                nrg += Tip4PFF::calculateEnergy( array0, nats,
                                                 array1, nats,
                                                 cljarray,
                                                 myspace);
            }
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

    return Molecule();
}

/** Temporary function used to add a molecule with passed charge and LJ
    parameters */
void Tip4PFF::add(const Molecule &mol, const ChargeTable &charges,
                  const LJTable &ljs)
{
    if (cljparams.isEmpty())
    {
        charges.assertCompatibleWith(mol);
        ljs.assertCompatibleWith(mol);

        QVector< ParameterGroup<ChargeParameter> > chargeparams = charges.parameterGroups();
        QVector< ParameterGroup<LJParameter> > ljparams = ljs.parameterGroups();

        int ncg = chargeparams.count();

        BOOST_ASSERT(ncg == 1);

        int nparams = chargeparams[0].parameters().count();

        BOOST_ASSERT(nparams == 4);

        const ChargeParameter *chargearray = chargeparams[0].parameters().constData();
        const LJParameter *ljarray = ljparams[0].parameters().constData();

        cljparams.reserve(nparams);

        for (int j=0; j<nparams; ++j)
        {
            cljparams.append( CLJParameter(chargearray[j],ljarray[j]) );
        }
    }

    //add this molecule to the list
    CoordGroup cgroup = mol.coordGroups()[0];

    int nats = cgroup.count();

    QVector<Vector> coords;
    coords.reserve(nats);

    const Vector *array = cgroup.constData();

    for (int i=0; i<nats; ++i)
        coords.append( array[i] );

    mols.append( coords );
    aaboxes.append( cgroup.aaBox() );
}

/** Move the molecule 'molecule' */
bool Tip4PFF::move(const Molecule &molecule)
{
    return false;
}

bool Tip4PFF::move(const Residue &residue)
{
    return false;
}
