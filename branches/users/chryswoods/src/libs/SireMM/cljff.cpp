
#include "SireMol/qhash_siremol.h"
#include "SireCAS/qhash_sirecas.h"

#include "cljff.h"
#include "detail/molcljinfo.h"

#include "SireVol/coordgroup.h"

#include "SireUnits/units.h"
#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireMM::detail;
using namespace SireFF;
using namespace SireVol;
using namespace SireStream;

////////////
//////////// Implementation of CLJWorkspace
////////////

/** Constructor */
CLJWorkspace::CLJWorkspace() : cnrg(0), ljnrg(0)
{}

/** Destructor */
CLJWorkspace::~CLJWorkspace()
{}

////////////
//////////// Implementation of CLJFF
////////////

static const RegisterMetaType<CLJFF> r_cljff("SireMM::CLJFF", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJFF &cljff)
{
    writeHeader(ds, r_cljff, 1)
              << cljff.spce << cljff.combrules << cljff.switchfunc
              << static_cast<const FFBase&>(cljff);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJFF &cljff)
{
    VersionID v = readHeader(ds, r_cljff);

    if (v == 1)
    {
        ds >> cljff.spce >> cljff.combrules >> cljff.switchfunc
           >> static_cast<FFBase&>(cljff);
    }
    else
        throw version_error(v, "1", r_cljff, CODELOC);

    return ds;
}

/** This function is used to calculate the charge and LJ energy
    of two groups based on the inter-atomic inverse-square-distances stored in 'distmatrix'
    and using the combined CLJ parameters listed in 'cljmatrix', in the
    passed workspace. The total coulomb and LJ energies are returned in
    the workspace. */
void CLJFF::calculatePairEnergy(CLJWorkspace &workspace)
{
    DistMatrix &distmatrix = workspace.distmatrix;
    CLJPairMatrix &cljmatrix = workspace.cljmatrix;

    int nats0 = distmatrix.nOuter();
    int nats1 = distmatrix.nInner();

    double icnrg = 0;
    double iljnrg = 0;

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
            icnrg += SireUnits::one_over_four_pi_eps0 *
                                     cljpair.charge2() * std::sqrt(invdist2);

            //LJ energy
            iljnrg += 4 * cljpair.epsilon() *
                                     ( sig12_over_dist12 - sig6_over_dist6 );
        }
    }

    workspace.cnrg = icnrg;
    workspace.ljnrg = iljnrg;
}

/** This function is used to calculate the self-energy of a CutGroup,
    using the inverse-square-distances and parameters stored in the passed workspace.
    The total coulomb and LJ energies are returned in the workspace. */
void CLJFF::calculateSelfEnergy(CLJWorkspace &workspace)
{
    DistMatrix &distmatrix = workspace.distmatrix;
    CLJPairMatrix &cljmatrix = workspace.cljmatrix;

    int nats = distmatrix.nOuter();

    double icnrg = 0;
    double iljnrg = 0;

    for (int i=0; i<nats-1; ++i)
    {
        distmatrix.setOuterIndex(i);
        cljmatrix.setOuterIndex(i);

        for (int j=i+1; j<nats; ++j)
        {
            //get the distance and CLJPair for this atom pair
            double invdist2 = distmatrix[j];
            const CLJPair &cljpair = cljmatrix[j];

            double sig2_over_dist2 = SireMaths::pow_2(cljpair.sigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

            //coulomb energy
            icnrg += SireUnits::one_over_four_pi_eps0 *
                                     cljpair.charge2() * std::sqrt(invdist2);

            //LJ energy
            iljnrg += 4 * cljpair.epsilon() *
                                     ( sig12_over_dist12 - sig6_over_dist6 );
        }
    }

    workspace.cnrg = icnrg;
    workspace.ljnrg = iljnrg;
}

/** Calculate the CLJ energy of interaction of group0, with CLJ parameters
    in 'clj0' and group1, with CLJ parameters in 'clj1', using the
    space 'space', and using the provided workspace. The total
    coulomb and LJ energies are returned in the workspace. */
void CLJFF::calculateEnergy(const CoordGroup &group0,
                            const QVector<CLJParameter> &clj0,
                            const CoordGroup &group1,
                            const QVector<CLJParameter> &clj1,
                            const Space &space,
                            const CombiningRules &combrules,
                            const SwitchingFunction &switchfunc,
                            CLJWorkspace &workspace)
{
    if ( not space.beyond(switchfunc.cutoffDistance(), group0, group1) )
    {
        double mindist = space.calcInvDist2(group0, group1, workspace.distmatrix);

        double sclcoul = switchfunc.electrostaticScaleFactor(mindist);
        double scllj = switchfunc.vdwScaleFactor(mindist);

        if (sclcoul != 0 or scllj != 0)
        {
            combrules.combine(clj0, clj1, workspace.cljmatrix);

            CLJFF::calculatePairEnergy(workspace);

            workspace.cnrg *= sclcoul;
            workspace.ljnrg *= scllj;

            return;
        }
    }

    workspace.cnrg = 0;
    workspace.ljnrg = 0;
}

/** Calculate the CLJ energy of interaction of the atoms within the
    group 'group', with CLJ parameters in 'clj', using the space 'space',
    combining rules in 'combrules', and working in the workspace 'workspace'.
    The coulomb and LJ energies are returned in the workspace. */
void CLJFF::calculateEnergy(const CoordGroup &group,
                            const QVector<CLJParameter> &clj,
                            const Space &space,
                            const CombiningRules &combrules,
                            CLJWorkspace &workspace)
{
    combrules.combine(clj, workspace.cljmatrix);

    space.calcInvDist2(group, workspace.distmatrix);

    CLJFF::calculateSelfEnergy(workspace);
}

/** Calculate the CLJ energy of interaction between two molecules, that
    are held, with their CLJ parameters, in mol0 and mol1, using the
    space 'space', combining rules 'combrules' and switching function
    'switchfunc'. The calculation will use provided workspace and will
    return the total coulomb and LJ energies in that workspace. */
void CLJFF::calculateEnergy(const MolCLJInfo &mol0,
                            const MolCLJInfo &mol1,
                            const Space &space,
                            const CombiningRules &combrules,
                            const SwitchingFunction &switchfunc,
                            CLJWorkspace &workspace)
{
    int ncg0 = mol0.coordinates().count();
    int ncg1 = mol1.coordinates().count();

    if (ncg0 > 0 and ncg1 > 0)
    {
        double icnrg = 0;
        double iljnrg = 0;

        const CoordGroup *cg0array = mol0.coordinates().constData();
        const CoordGroup *cg1array = mol1.coordinates().constData();

        const QVector<CLJParameter> *clj0array = mol0.parameters().constData();
        const QVector<CLJParameter> *clj1array = mol1.parameters().constData();

        for (int i=0; i<ncg0; ++i)
        {
            const CoordGroup &group0 = cg0array[i];
            const QVector<CLJParameter> &clj0 = clj0array[i];

            for (int j=0; j<ncg1; ++j)
            {
                const CoordGroup &group1 = cg1array[j];
                const QVector<CLJParameter> &clj1 = clj1array[j];

                calculateEnergy(group0, clj0, group1, clj1,
                                space, combrules, switchfunc, workspace);

                icnrg += workspace.cnrg;
                iljnrg += workspace.ljnrg;
            }
        }

        workspace.cnrg = icnrg;
        workspace.ljnrg = iljnrg;
    }
    else
    {
        workspace.cnrg = 0;
        workspace.ljnrg = 0;
    }
}

/** Calculate the CLJ energy of interaction between the pairs
    of atoms within a single molecule, which is held, with its CLJ parameters,
    in mol, using the space 'space', combining rules 'combrules' and switching function
    'switchfunc'. The calculation will use the provided workspace and will
    return the total coulomb and LJ energies in that workspace. */
void CLJFF::calculateEnergy(const MolCLJInfo &mol,
                            const Space &space,
                            const CombiningRules &combrules,
                            const SwitchingFunction &switchfunc,
                            CLJWorkspace &workspace)
{
    int ncg = mol.coordinates().count();

    if (ncg == 1)
    {
        //calculate only the self-energy of the first CutGroup
        calculateEnergy(mol.coordinates().constData()[0],
                        mol.parameters().constData()[0],
                        space, combrules, workspace);
    }
    else if (ncg > 1)
    {
        double icnrg = 0;
        double iljnrg = 0;

        const CoordGroup *cgarray = mol.coordinates().constData();
        const QVector<CLJParameter> *cljarray = mol.parameters().constData();

        for (int i=0; i<ncg-1; ++i)
        {
            const CoordGroup &group0 = cgarray[i];
            const QVector<CLJParameter> &clj0 = cljarray[i];

            //add on the self-energy
            calculateEnergy(group0, clj0, space, combrules, workspace);

            icnrg += workspace.cnrg;
            iljnrg += workspace.ljnrg;

            for (int j=i+1; j<ncg; ++j)
            {
                //calculate the group-group energy
                const CoordGroup &group1 = cgarray[j];
                const QVector<CLJParameter> &clj1 = cljarray[j];

                calculateEnergy(group0,clj0, group1,clj1,
                                space, combrules, switchfunc, workspace);

                icnrg += workspace.cnrg;
                iljnrg += workspace.ljnrg;
            }
        }

        //have to add on the self-energy of the last CutGroup of the molecule
        const CoordGroup &group = cgarray[ncg-1];
        const QVector<CLJParameter> &clj = cljarray[ncg-1];

        calculateEnergy(group, clj, space, combrules, workspace);

        workspace.cnrg += icnrg;
        workspace.ljnrg += iljnrg;
    }
    else
    {
        workspace.cnrg = 0;
        workspace.ljnrg = 0;
    }
}

/** Null constructor */
CLJFF::CLJFF() : FFBase()
{
    this->registerComponents();
}

/** Construct a CLJFF using the specified space, combining rules and
    switching function */
CLJFF::CLJFF(const Space &space, const CombiningRules &combiningrules,
             const SwitchingFunction &switchingfunction)
      : FFBase(),
        spce(space), combrules(combiningrules), switchfunc(switchingfunction)
{
    this->registerComponents();
}

/** Copy constructor */
CLJFF::CLJFF(const CLJFF &other)
      : FFBase(other),
        spce(other.spce), combrules(other.combrules), switchfunc(other.switchfunc)
{}

/** Destructor */
CLJFF::~CLJFF()
{}

/** Register the energy components associated with this forcefield
    (the Coulomb and LJ components) */
void CLJFF::registerComponents()
{
    this->registerComponent( CLJFF::COULOMB(), "coul",
                             QObject::tr("The total electrostatic (coulomb) energy.") );

    this->registerComponent( CLJFF::LJ(), "lj",
                             QObject::tr("The total vdw (Lennard Jones) energy.") );
}

/** Return the function representing the coulomb energy component of this
    forcefield */
const Function& CLJFF::coulomb() const
{
    return this->component(CLJFF::COULOMB());
}

/** Return the function representing the LJ energy component of this
    forcefield */
const Function& CLJFF::lj() const
{
    return this->component(CLJFF::LJ());
}
