
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
//////////// Implementation of CLJFF::Parameters
////////////

/** Constructor - by default the coulomb parameters come
    from the 'charges' property, while the LJ parameters
    come from the 'ljs' property */
CLJFF::Parameters::Parameters()
      : FFBase::Parameters(), coulomb_params("coulomb", "charges"), lj_params("lj", "ljs")
{}

/** Copy constructor */
CLJFF::Parameters::Parameters(const CLJFF::Parameters &other)
      : FFBase::Parameters(other), coulomb_params(other.coulomb_params), lj_params(other.lj_params)
{}

/** Destructor */
CLJFF::Parameters::~Parameters()
{}

/** Static object returned by CLJFF::parameters() */
CLJFF::Parameters CLJFF::Parameters::default_sources;

////////////
//////////// Implementation of CLJFF::Components
////////////

/** Constructor */
CLJFF::Components::Components() : FFBase::Components()
{}

/** Construct using the supplied basename */
CLJFF::Components::Components(const QString &basename) : FFBase::Components()
{
    CLJFF::Components::setBaseName(basename);
}

/** Copy constructor */
CLJFF::Components::Components(const CLJFF::Components &other)
      : FFBase::Components(other),
        e_coulomb(other.e_coulomb),
        e_lj(other.e_lj)
{}

/** Destructor */
CLJFF::Components::~Components()
{}

/** Set the names of the functions from the passed base name */
void CLJFF::Components::setBaseName(const QString &basename)
{
    this->unregisterFunction(e_coulomb);
    this->unregisterFunction(e_lj);

    e_coulomb = getFunction(basename, "coulomb");
    e_lj = getFunction(basename, "lj");

    this->registerFunction(e_coulomb);
    this->registerFunction(e_lj);

    FFBase::Components::setBaseName(basename);
}

/** Describe the coulomb component */
QString CLJFF::Components::describe_coulomb()
{
    return QObject::tr("The total coulomb (electrostatic) energy of the forcefield.");
}

/** Describe the LJ component */
QString CLJFF::Components::describe_lj()
{
    return QObject::tr("The total Lennard-Jones (van der waals) energy of the forcefield.");
}

////////////
//////////// Implementation of CLJFF
////////////

static const RegisterMetaType<CLJFF> r_cljff("SireMM::CLJFF", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJFF &cljff)
{
    writeHeader(ds, r_cljff, 1)
              << cljff.spce << cljff.switchfunc
              << static_cast<const FFBase&>(cljff);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJFF &cljff)
{
    VersionID v = readHeader(ds, r_cljff);

    if (v == 1)
    {
        ds >> cljff.spce >> cljff.switchfunc
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
void CLJFF::calculatePairEnergy(CLJWorkspace &workspace,
                                const QVector<ChargeParameter> &chg0,
                                const QVector<LJParameter> &lj0,
                                const QVector<ChargeParameter> &chg1,
                                const QVector<LJParameter> &lj1)
{
    DistMatrix &distmatrix = workspace.distmatrix;

    int nats0 = distmatrix.nOuter();
    int nats1 = distmatrix.nInner();

    double icnrg = 0;
    double iljnrg = 0;

    const ChargeParameter *chg0array = chg0.constData();
    const LJParameter *lj0array = lj0.constData();

    const ChargeParameter *chg1array = chg1.constData();
    const LJParameter *lj1array = lj1.constData();

    //loop over all pairs of atoms
    for (int i=0; i<nats0; ++i)
    {
        distmatrix.setOuterIndex(i);

        double chg0param = chg0array[i].charge();
        const LJParameter &lj0param = lj0array[i];

        for (int j=0; j<nats1; ++j)
        {
            const LJParameter &lj1param = lj1array[j];
            double chg2 = SireUnits::one_over_four_pi_eps0 * chg0param * chg1array[j].charge();

            //get the distance and CLJPair for this atom pair
            double invdist2 = distmatrix[j];

            double sig2_over_dist2 = SireMaths::pow_2(lj0param.sqrtSigma()*
                                                      lj1param.sqrtSigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

            //coulomb energy
            icnrg +=  chg2 * std::sqrt(invdist2);

            //LJ energy
            iljnrg += 4 * lj0param.sqrtEpsilon()*lj1param.sqrtEpsilon() *
                                     ( sig12_over_dist12 - sig6_over_dist6 );
        }
    }

    workspace.cnrg = icnrg;
    workspace.ljnrg = iljnrg;
}

/** This function is used to calculate the self-energy of a CutGroup,
    using the inverse-square-distances and parameters stored in the passed workspace.
    The total coulomb and LJ energies are returned in the workspace. */
void CLJFF::calculateSelfEnergy(CLJWorkspace &workspace,
                                const QVector<ChargeParameter> &chgs,
                                const QVector<LJParameter> &ljs)
{
    DistMatrix &distmatrix = workspace.distmatrix;

    int nats = distmatrix.nOuter();

    double icnrg = 0;
    double iljnrg = 0;

    const ChargeParameter *chgarray = chgs.constData();
    const LJParameter *ljarray = ljs.constData();

    for (int i=0; i<nats-1; ++i)
    {
        distmatrix.setOuterIndex(i);

        const ChargeParameter &chg0param = chgarray[i];
        const LJParameter &lj0param = ljarray[i];

        for (int j=i+1; j<nats; ++j)
        {
            const LJParameter &lj1param = ljarray[j];

            //get the distance and CLJPair for this atom pair
            double invdist2 = distmatrix[j];

            double sig2_over_dist2 = SireMaths::pow_2(lj0param.sqrtSigma() *
                                                      lj1param.sqrtSigma()) * invdist2;
            double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
            double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

            //coulomb energy
            icnrg += SireUnits::one_over_four_pi_eps0 *
                                     chg0param.charge() *
                                     chgarray[j].charge() * std::sqrt(invdist2);

            //LJ energy
            iljnrg += 4 * lj0param.sqrtEpsilon() * lj1param.sqrtEpsilon() *
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
                            const QVector<ChargeParameter> &chg0,
                            const QVector<LJParameter> &lj0,
                            const CoordGroup &group1,
                            const QVector<ChargeParameter> &chg1,
                            const QVector<LJParameter> &lj1,
                            const Space &space,
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
            CLJFF::calculatePairEnergy(workspace, chg0, lj0, chg1, lj1);

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
                            const QVector<ChargeParameter> &chgs,
                            const QVector<LJParameter> &ljs,
                            const Space &space,
                            CLJWorkspace &workspace)
{
    space.calcInvDist2(group, workspace.distmatrix);

    CLJFF::calculateSelfEnergy(workspace, chgs, ljs);
}

/** Calculate the CLJ energy of interaction between two molecules, that
    are held, with their CLJ parameters, in mol0 and mol1, using the
    space 'space', combining rules 'combrules' and switching function
    'switchfunc'. The calculation will use provided workspace and will
    return the total coulomb and LJ energies in that workspace. */
void CLJFF::calculateEnergy(const MolCLJInfo &mol0,
                            const MolCLJInfo &mol1,
                            const Space &space,
                            const SwitchingFunction &switchfunc,
                            CLJWorkspace &workspace)
{
    int ncg0 = mol0.coordinates().count();
    int ncg1 = mol1.coordinates().count();

    if (ncg0 == 1 and ncg1 == 1)
    {
        calculateEnergy( mol0.coordinates().constData()[0],
                         mol0.chargeParameters().constData()[0],
                         mol0.ljParameters().constData()[0],
                         mol1.coordinates().constData()[0],
                         mol1.chargeParameters().constData()[0],
                         mol1.ljParameters().constData()[0],

                         space, switchfunc, workspace );
    }
    else if (ncg0 > 0 and ncg1 > 0)
    {
        double icnrg = 0;
        double iljnrg = 0;

        const CoordGroup *cg0array = mol0.coordinates().constData();
        const CoordGroup *cg1array = mol1.coordinates().constData();

        const QVector<ChargeParameter> *chg0array = mol0.chargeParameters().constData();
        const QVector<LJParameter> *lj0array = mol0.ljParameters().constData();

        const QVector<ChargeParameter> *chg1array = mol1.chargeParameters().constData();
        const QVector<LJParameter> *lj1array = mol1.ljParameters().constData();

        for (int i=0; i<ncg0; ++i)
        {
            const CoordGroup &group0 = cg0array[i];
            const QVector<ChargeParameter> &chg0 = chg0array[i];
            const QVector<LJParameter> &lj0 = lj0array[i];

            for (int j=0; j<ncg1; ++j)
            {
                const CoordGroup &group1 = cg1array[j];
                const QVector<ChargeParameter> &chg1 = chg1array[j];
                const QVector<LJParameter> &lj1 = lj1array[j];

                calculateEnergy(group0, chg0, lj0, group1, chg1, lj1,
                                space, switchfunc, workspace);

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
                            const SwitchingFunction &switchfunc,
                            CLJWorkspace &workspace)
{
    int ncg = mol.coordinates().count();

    if (ncg == 1)
    {
        //calculate only the self-energy of the first CutGroup
        calculateEnergy(mol.coordinates().constData()[0],
                        mol.chargeParameters().constData()[0],
                        mol.ljParameters().constData()[0],
                        space, workspace);
    }
    else if (ncg > 1)
    {
        double icnrg = 0;
        double iljnrg = 0;

        const CoordGroup *cgarray = mol.coordinates().constData();
        const QVector<ChargeParameter> *chgarray = mol.chargeParameters().constData();
        const QVector<LJParameter> *ljarray = mol.ljParameters().constData();

        for (int i=0; i<ncg-1; ++i)
        {
            const CoordGroup &group0 = cgarray[i];
            const QVector<ChargeParameter> &chg0 = chgarray[i];
            const QVector<LJParameter> &lj0 = ljarray[i];

            //add on the self-energy
            calculateEnergy(group0, chg0, lj0, space, workspace);

            icnrg += workspace.cnrg;
            iljnrg += workspace.ljnrg;

            for (int j=i+1; j<ncg; ++j)
            {
                //calculate the group-group energy
                const CoordGroup &group1 = cgarray[j];
                const QVector<ChargeParameter> &chg1 = chgarray[j];
                const QVector<LJParameter> &lj1 = ljarray[j];

                calculateEnergy(group0,chg0, lj0, group1, chg1, lj1,
                                space, switchfunc, workspace);

                icnrg += workspace.cnrg;
                iljnrg += workspace.ljnrg;
            }
        }

        //have to add on the self-energy of the last CutGroup of the molecule
        const CoordGroup &group = cgarray[ncg-1];
        const QVector<ChargeParameter> &chg = chgarray[ncg-1];
        const QVector<LJParameter> &lj = ljarray[ncg-1];

        calculateEnergy(group, chg, lj, space, workspace);

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

/** Construct a CLJFF using the specified space and switching function */
CLJFF::CLJFF(const Space &space, const SwitchingFunction &switchingfunction)
      : FFBase(),
        spce(space), switchfunc(switchingfunction)
{
    this->registerComponents();
}

/** Copy constructor */
CLJFF::CLJFF(const CLJFF &other)
      : FFBase(other),
        spce(other.spce), switchfunc(other.switchfunc)
{
    //get the pointer from the base class...
    components_ptr = dynamic_cast<const CLJFF::Components*>( &(FFBase::components()) );
    BOOST_ASSERT( components_ptr != 0 );
}

/** Destructor */
CLJFF::~CLJFF()
{}

/** Register the energy components associated with this forcefield
    (the Coulomb and LJ components) */
void CLJFF::registerComponents()
{
    std::auto_ptr<CLJFF::Components> ptr( new CLJFF::Components(name()) );

    FFBase::registerComponents(ptr.get());

    components_ptr = ptr.release();
}
