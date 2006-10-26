
#include "SireMol/qhash_siremol.h"

#include "cljff.h"

#include "SireMol/molecule.h"
#include "SireMol/cutgroup.h"

#include "SireVol/simvolume.h"
#include "SireVol/cartesian.h"

#include "SireUnits/units.h"
#include "SireError/exceptions.h"
#include "SireError/getmagic.h"
#include "SireStream/txtstream.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireStream;
using namespace SireBase;
using namespace SireMaths;

static const MagicID CLJFFMagic = getMagic("SireFF::CLJFF");

/** Serialise this forcefield to a data stream */
QDataStream& operator<<(QDataStream &ds, const CLJFF &ff)
{
    //save the magic
    ds << CLJFFMagic;

    //there is a lot of stuff to save....

    //save the ForceField part of the class...
    ds << static_cast<const ForceField&>(ff);

    return ds;
}

/** Load this forcefield from a data stream */
QDataStream& operator>>(QDataStream &ds, CLJFF &ff)
{
    //test the magic
    MagicID id;
    ds >> id;

    if (id != CLJFFMagic)
        throw SireError::magic_error(id, CLJFFMagic, "SireFF:CLJFF", CODELOC);

    //there is a lot of stuff to load...

    //load the forcefield part of the class...
    ds >> static_cast<ForceField&>(ff);

    return ds;
}


/** This function is used to calculate the charge and LJ energy
    of two groups based on the inter-atomic inverse-square-distances stored in 'distmatrix'
    and using the combined CLJ parameters listed in 'cljmatrix', in the
    passed workspace. The total coulomb and LJ energies are returned in
    the workspace. */
void CLJFF::calculatePairEnergy(detail::CLJWorkspace &workspace)
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
            double invdist2 = distmat[j];
            const CLJPair &cljpair = cljmat[j];

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
void CLJFF::calculateSelfEnergy(detail::CLJWorkspace &workspace)
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
            double invdist2 = distmat[j];
            const CLJPair &cljpair = cljmat[j];

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
                            detail::CLJWorkspace &workspace)
{
    if ( not space.beyond(group0, group1, switchfunc.cutoffDistance()) )
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
                            detail::CLJWorkspace &workspace)
{
    combrules.combine(clj, workspace.cljmatrix);

    double mindist = space.calcInvDist2(group, workspace.distmatrix);

    CLJFF::calculateSelfEnergy(workspace);
}

/** Calculate the CLJ energy of interaction between two molecules, that
    are held, with their CLJ parameters, in mol0 and mol1, using the
    space 'space', combining rules 'combrules' and switching function
    'switchfunc'. The calculation will use provided workspace and will
    return the total coulomb and LJ energies in that workspace. */
void InterCLJFF::calculateEnergy(const MolCLJInfo &mol0,
                                 const MolCLJInfo &mol1,
                                 const Space &space,
                                 const CombiningRules &combrules,
                                 const SwitchingFunction &switchfunc,
                                 detail::CLJWorkspace &workspace)
{
    int ncg0 = mol0.coords.count();
    int ncg1 = mol1.coords.count();

    if (ncg0 > 0 and ncg1 > 0)
    {
        double icnrg = 0;
        double iljnrg = 0;

        CoordGroup *cg0array = mol0.coords.constData();
        CoordGroup *cg1array = mol1.coords.constData();

        QVector<CLJParameter> *clj0array = mol0.params.constData();
        QVector<CLJParameter> *clj1array = mol1.params.constData();

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
    'switchfunc'. The calculation will use provided workspace and will
    return the total coulomb and LJ energies in that workspace. */
void InterCLJFF::calculateEnergy(const MolCLJInfo &mol,
                                 const Space &space,
                                 const CombiningRules &combrules,
                                 const SwitchingFunction &switchfunc,
                                 detail::CLJWorkspace &workspace)
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
            iljnrg += workspace.iljnrg;

            for (int j=i+1; j<ncg; ++j)
            {
                //calculate the group-group energy
                const CoordGroup &group1 = cgarray[j];
                const QVector<CLJParameter> &clj1 = cljarray[j];

                calculateEnergy(group0,clj0, group1,clj1,
                                space, combrules, switchfunc, workspace);

                icnrg += workspace.cnrg;
                iljnrg += workspace.iljnrg;
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

/** Construct a default, empty forcefield */
CLJFF::CLJFF() : ForceField()
{
    //by default we will use geometric combining rules...
    combineCLJ = &CLJFF::combineGeometric;

    //by default we will use an infinite cartesian volume
    volptr.reset( new SireVol::Cartesian() );

    //by default we will use 15A cutoff and 0.5A feather
    this->setCutoff( 15.0*SireUnits::angstrom, 0.5*SireUnits::angstrom );
}

/** Copy constructor - this is quite fast as most of the contents of this forcefield
    are implicitly shared */
CLJFF::CLJFF(const CLJFF &other)
      : ForceField(other),
        cljmutators(other.cljmutators),
        pertgroups(other.pertgroups),
        lambda(other.lambda),
        combineCLJ(other.combineCLJ),
        volptr(other.volptr),             //volptr points to an immutable object, so can be copied
        nbcut(other.nbcut), nbcut2(other.nbcut2),
        nbfeather(other.nbfeather), nbfeather2(other.nbfeather2)
{
    //cljparams is a hash of raw pointers (for run-time speed). These need to
    //be copied explicitly

    for (QHash< CutGroupID, CLJWindows* >::const_iterator it = other.cljparams.begin();
         it != other.cljparams.end();
         ++it)
    {
        //insert a deep copy of the clj params into our own hash
        if (it.value() != 0)
          cljparams.insert( it.key(), new CLJWindows(*(it.value())) );
    }
}

/** Destructor */
CLJFF::~CLJFF()
{}

/** Set the volume that is used to calculate the intermolecular distances. */
void CLJFF::setVolume(const SimVolume &vol)
{
    //clone the passed volume
    volptr = vol.clone();
}

/** Set the value of lambda for this forcefield. Note that this only updates the
    forcefield parameters for this value of lambda - it does not update the coordinates
    of the CutGroups. In general you should not be calling this function directly
    (as it should be called via the FFGroup, which will update the coordinates of
    the CutGroups) */
void CLJFF::setLambda(const Lambda &lam)
{
    if (lambda == lam)
        //no change, so nothing to do!
        return;

    //copy the value of lambda
    lambda = lam;

    //update all of the CLJ parameters
    QHashIterator<CutGroupID, CLJMutatorPtr> it(cljmutators);

    while (it.hasNext())
    {
        it.next();

        //get the CutGroupID of the perturbable group, and the
        //mutator used to perturb the group
        CutGroupID id = it.key();
        CLJMutatorPtr mutator = it.value();

        //use the key to get the CutGroup...
        CutGroupPtr cgroup = pertgroups.at_key(id);

        //now get the new CLJ parameters for this lambda value
        CLJWindows windows = mutator->get(cgroup->atoms(), lambda);

        //update the parameter list with the new parameters
        this->setParameters(cgroup.objID(), windows);
    }
}

/** Set the cutoff and feather distances. The feather distance is the distance
    over which you wish the non-bonded interactions to be feathered, e.g.
    0.5 would set the feathering to occur over the last 0.5 A of the cutoff.
    If the feather is larger than the cutoff then it will be set equal to the
    cutoff. */
void CLJFF::setCutoff(double cut, double feath)
{
    //make sure that the values are sane!
    if (cut <= 0.0)
        throw SireError::invalid_arg(QObject::tr(
                  "Cannot have a zero or negative cutoff distance! (%1)")
                                         .arg(cut), CODELOC);

    //save the values...
    nbcut = cut;
    nbfeather = cut - std::abs(feath);
    if (nbfeather < 0.0)
        nbfeather = 0.0;

    //now calculate derived properties
    nbcut2 = pow_2(nbcut);
    nbfeather2 = pow_2(nbfeather);
}

/** Add the parameters for the CutGroup 'group' to this forcefield - with the
    parameters coming from the CLJTable 'cljtable'. This table must contain
    parameters for all of the atoms in this CutGroup or an exception will be thrown. */
void CLJFF::setParameters(const CutGroupPtr &group, const CLJTable &cljtable)
{
    //get a CLJWindow for these parameters...
    CLJWindow cljwindow = cljtable.window(group->atoms());

    //save this window, copied for both the reference and perturbed states
    this->setParameters(group.objID(), CLJWindows(cljwindow));
}

/** Add the (possible perturbable) parameters for the CutGroup 'group' to this
    forcefield - with the parameters coming from the CLJMutator 'cljmutator'.
    This mutator must contain parameters for all of the atoms in the CutGroup or
    else an exception will be thrown. */
void CLJFF::setParameters(const CutGroupPtr &group, const CLJMutator &cljmutator)
{
    if (group.isNull())
        return;

    const AtomVector &atoms = group->atoms();

    if (cljmutator.arePerturbable(atoms))
    {
        //get a CLJGroup from this mutator - the resulting group may or may
        //not be perturbable
        CLJWindows windows = cljmutator.get(group->atoms(), lambda);

        //now add the CutGroup with its parameters to the forcefield
        this->setParameters(group.objID(), windows);

        //save the fact that this group is perturbable, and also save
        //the mutator
        cljmutators.insert( group.objID(), cljmutator.clone() );
    }
    //this is not a perturbable group - just use the parameters for lambda=0
    //(as they are the same as lambda=1)
    else
    {
        this->setParameters(group, cljmutator.table0());
    }
}


/** Internal function used to combine together the CLJ parameters 'cljwindow0' and 'cljwindow1'
    using geometric combining rules. The resulting CLJPairs are stored in the CLJPairMatrix 'cljmat'. */
void CLJFF::combineGeometric(const CLJWindow &cljwindow0, const CLJWindow &cljwindow1,
                             CLJPairMatrix &cljmat) const
{
    //get the size of the windows...
    int n0 = cljwindow0.count();
    int n1 = cljwindow1.count();

    //get pointers to the arrays containing the clj parameters
    const CLJParameter *clj0 = cljwindow0.constData();
    const CLJParameter *clj1 = cljwindow1.constData();

    //redimension the matrix so that it has sufficient space
    cljmat.redimension(n0, n1);

    //now calculate all of the CLJPairs
    for (int i=0; i<n0; ++i)
    {
        const CLJParameter &c0 = clj0[i];
        cljmat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //cljmat[j] = cljCombineFunc(c0, clj1[j]);
            cljmat[j] = CLJPair::geometric(c0, clj1[j]);
        }
    }
}

/** Internal function used to combine together the CLJ parameters 'cljwindow0' and 'cljwindow1'
    using arithmetic combining rules. The resulting CLJPairs are stored in the CLJPairMatrix 'cljmat'. */
void CLJFF::combineArithmetic(const CLJWindow &cljwindow0, const CLJWindow &cljwindow1,
                              CLJPairMatrix &cljmat) const
{
    //get the size of the windows...
    int n0 = cljwindow0.count();
    int n1 = cljwindow1.count();

    //get pointers to the arrays containing the clj parameters
    const CLJParameter *clj0 = cljwindow0.constData();
    const CLJParameter *clj1 = cljwindow1.constData();

    //redimension the matrix so that it has sufficient space
    cljmat.redimension(n0, n1);

    //now calculate all of the CLJPairs
    for (int i=0; i<n0; ++i)
    {
        const CLJParameter &c0 = clj0[i];
        cljmat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //cljmat[j] = cljCombineFunc(c0, clj1[j]);
            cljmat[j] = CLJPair::arithmetic(c0, clj1[j]);
        }
    }
}

/** Return the feather scaling factor for the distance^2 dist2.
    Overload this function if you want more control of the switching function
    for this forcefield. You should only call this function for
    featherdist2 <  dist2 <  cutoff2 */
double CLJFF::featherFactor(double dist2) const
{
    BOOST_ASSERT( (nbcut2 >= dist2) and (dist2 >= nbfeather2) );

    return (nbcut2 - dist2) / (nbcut2 - nbfeather2);
}

/** Internal function used to calculate and return the CLJ energy of CutGroups 'group0' and 'group1',
    which have clj parameters in 'clj0' and 'clj1'. The workspace for the calculation is provided
    by the distance matrix 'distmat', and the CLJPair matrix 'cljmat'. I return the energies
    in double references (cnrg and ljnrg) as I have speed tested, and this is the fastest way
    that I found to return two values. */
void CLJFF::cljEnergy(const CutGroup &group0, const CutGroup &group1,
                      const CLJWindow &clj0, const LambdaState &stat,
                      DistMatrix &distmat, CLJPairMatrix &cljmat,
                      double &cnrg, double &ljnrg) const
{
    //zero the interaction energies
    cnrg = 0.0;
    ljnrg = 0.0;

    //get all of the interatomic distances...
    double closest2 = volptr->calcInvDist2(group0, group1, stat, distmat);

    //is this within the cutoff distance?
    if (closest2 < nbcut2)
    {
        //calculate the CLJPair parameters
        (this->*combineCLJ)(clj0, this->getParameters(group1.ID(),stat), cljmat);

        //now run over each atom pair and calculate the clj energy
        int nats0 = distmat.nOuter();
        int nats1 = distmat.nInner();

        for (int i=0; i<nats0; ++i)
        {
            distmat.setOuterIndex(i);
            cljmat.setOuterIndex(i);

            for (int j=0; j<nats1; ++j)
            {
                //get the distance and CLJPair for this atom pair
                const double &invdist2 = distmat[j];
                const CLJPair &cljpair = cljmat[j];

                double sig2_over_dist2 = SireMaths::pow_2(cljpair.sigma()) * invdist2;
                double sig6_over_dist6 = SireMaths::pow_3(sig2_over_dist2);
                double sig12_over_dist12 = SireMaths::pow_2(sig6_over_dist6);

                //coulomb energy
                cnrg += SireUnits::one_over_four_pi_eps0 *
                                            cljpair.charge2() * std::sqrt(invdist2);

                //LJ energy
                ljnrg += 4.0 * cljpair.epsilon() *
                                         ( sig12_over_dist12 - sig6_over_dist6 );
            }
        }

        //does this energy need to be scaled by a non-bonded scale factor?
        if (closest2 > nbfeather2)
        {
            double fac = this->featherFactor(closest2);
            cnrg *= fac;
            ljnrg *= fac;
        }
    }
}
