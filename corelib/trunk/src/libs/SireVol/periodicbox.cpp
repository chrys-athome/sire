
#include <limits>
#include <cmath>

#include "SireMol/qhash_siremol.h"

#include "periodicbox.h"

#include "SireMol/atom.h"
#include "SireMol/aabox.h"
#include "SireMol/cutgroup.h"

#include "SireError/errors.h"

using namespace SireVol;
using namespace SireMol;
using namespace SireBase;
using namespace SireMaths;

/** Construct a default PeriodicBox volume (zero volume) */
PeriodicBox::PeriodicBox() : SimVolume()
{}

/** Construct a PeriodicBox volume that goes from min to max */
PeriodicBox::PeriodicBox(const Vector &min, const Vector &max)
            : SimVolume()
{
    this->setDimension(min,max);
}

/** Copy constructor */
PeriodicBox::PeriodicBox(const PeriodicBox &other) 
            : SimVolume(other), mincoords(other.mincoords), maxcoords(other.maxcoords),
              boxlength(other.boxlength), halflength(other.halflength), invlength(other.invlength)
{}

/** Destructor */
PeriodicBox::~PeriodicBox()
{}

/** Set the dimensions of this box so that it is the smallest possible that contains
    the points 'min' and 'max'. The minimum coordinates of this box will be set to 
    the minimum of the coordinates of these two points, and the maximum coordinates
    will be set to the maximum of the two points. */
void PeriodicBox::setDimension(const Vector &min, const Vector &max)
{
    mincoords = min;
    mincoords.setMin(max);
    
    maxcoords = max;
    maxcoords.setMax(min);
    
    boxlength = maxcoords - mincoords;
    
    for (int i=0; i<3; ++i)
    {
        invlength.set(i, 1.0/boxlength[i]);
        halflength.set(i, 0.5 * boxlength[i]);
    }
}

/** Return a clone of this PeriodicBox volume */
SimVolumePtr PeriodicBox::clone() const
{
    return SimVolumePtr(new PeriodicBox(*this));
}

/** Populate the matrix 'mat' with the distances between all of the
    atoms of the two CutGroups. Return the shortest distance^2 between the two
    CutGroups. */
double PeriodicBox::calcDist(const CutGroup &group0, const CutGroup &group1,
                             DistMatrix &mat) const
{
    double mindist(std::numeric_limits<double>::max());
    double tmpdist;
    
    int nats0 = group0.nAtoms();
    int nats1 = group1.nAtoms();
    
    //redimension the matrix to hold all of the pairs
    mat.redimension(nats0, nats1);

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    //get raw pointers to the arrays - this provides more efficient access
    const Atom *array0 = group0.atoms().constData();
    const Atom *array1 = group1.atoms().constData();
        
    for (int i=0; i<nats0; i++)
    {
        //add the delta to the coordinates of atom0
        Vector atom0 = array0[i] + wrapdelta;
        mat.setOuterIndex(i);
        
        for (int j=0; j<nats1; j++)
        {
            //calculate the distance between the two atoms
            tmpdist = Vector::distance(atom0,array1[j]);
            
            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            mindist = SIRE_MIN(tmpdist,mindist);
            
            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }
    
    //return the minimum distance^2
    return mindist*mindist;
}

/** Populate the matrix 'mat' with the distances^2 between all of the
    atoms of the two CutGroups. Return the shortest distance^2 between the
    two CutGroups. */
double PeriodicBox::calcDist2(const CutGroup &group0, const CutGroup &group1,
                              DistMatrix &mat) const
{
    double mindist2(std::numeric_limits<double>::max());
    double tmpdist;
    
    int nats0 = group0.nAtoms();
    int nats1 = group1.nAtoms();
    
    //redimension the matrix to hold all of the pairs
    mat.redimension(nats0, nats1);

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    //get raw pointers to the arrays - this provides more efficient access
    const Atom *array0 = group0.atoms().constData();
    const Atom *array1 = group1.atoms().constData();
        
    for (int i=0; i<nats0; i++)
    {
        //add the delta to the coordinates of atom0
        Vector atom0 = array0[i] + wrapdelta;
        mat.setOuterIndex(i);
        
        for (int j=0; j<nats1; j++)
        {
            //calculate the distance between the two atoms
            tmpdist = Vector::distance2(atom0,array1[j]);
            
            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            mindist2 = SIRE_MIN(tmpdist,mindist2);
            
            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }
    
    //return the minimum distance^2
    return mindist2;
}

/** Populate the matrix 'mat' with the inverse distances between all of the
    atoms of the two CutGroups. Return the shortest distance^2 between the two CutGroups. */
double PeriodicBox::calcInvDist(const CutGroup &group0, const CutGroup &group1,
                                DistMatrix &mat) const
{
    double maxinvdist(0.0);
    double tmpdist;
    
    int nats0 = group0.nAtoms();
    int nats1 = group1.nAtoms();
    
    //redimension the matrix to hold all of the pairs
    mat.redimension(nats0, nats1);

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    //get raw pointers to the arrays - this provides more efficient access
    const Atom *array0 = group0.atoms().constData();
    const Atom *array1 = group1.atoms().constData();
        
    for (int i=0; i<nats0; i++)
    {
        //add the delta to the coordinates of atom0
        Vector atom0 = array0[i] + wrapdelta;
        mat.setOuterIndex(i);
        
        for (int j=0; j<nats1; j++)
        {
            //calculate the distance between the two atoms
            tmpdist = Vector::invDistance(atom0,array1[j]);
            
            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            maxinvdist = SIRE_MAX(tmpdist,maxinvdist);
            
            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }
    
    //return the minimum distance^2
    return 1.0 / (maxinvdist*maxinvdist);
}

/** Populate the matrix 'mat' with the inverse distances^2 between all of the
    atoms of the two CutGroups. Return the shortest distance^2 between the two CutGroups. */
double PeriodicBox::calcInvDist2(const CutGroup &group0, const CutGroup &group1,
                                 DistMatrix &mat) const
{
    double maxinvdist2(0.0);
    double tmpdist;
    
    int nats0 = group0.nAtoms();
    int nats1 = group1.nAtoms();
    
    //redimension the matrix to hold all of the pairs
    mat.redimension(nats0, nats1);

    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(group0.aaBox().center(), group1.aaBox().center());

    //get raw pointers to the arrays - this provides more efficient access
    const Atom *array0 = group0.atoms().constData();
    const Atom *array1 = group1.atoms().constData();
        
    for (int i=0; i<nats0; ++i)
    {
        //add the delta to the coordinates of atom0
        //Vector atom0 = array0[i] + wrapdelta;
        Vector atom0 = array0[i] + wrapdelta;
        mat.setOuterIndex(i);
        
        for (int j=0; j<nats1; ++j)
        {
            //calculate the distance between the two atoms
            tmpdist = Vector::invDistance2(atom0, array1[j]);
            
            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            maxinvdist2 = SIRE_MAX(tmpdist,maxinvdist2);
            
            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }
    
    //return the minimum distance
    return 1.0 / maxinvdist2;
}

/** Return whether or not these two groups are definitely beyond the cutoff distance. */
bool PeriodicBox::beyond(const double &dist, const CutGroup &group0, 
                         const CutGroup &group1) const
{
    const AABox &box0 = group0.aaBox();
    const AABox &box1 = group1.aaBox();
    
    //see if we need to wrap the coordinates...
    Vector wrapdelta = this->wrapDelta(box0.center(), box1.center());

    return Vector::distance2( box0.center()+wrapdelta, box1.center() ) >
                      SireMaths::pow_2(dist + box0.radius() + box1.radius());
}
