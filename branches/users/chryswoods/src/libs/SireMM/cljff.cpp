
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
