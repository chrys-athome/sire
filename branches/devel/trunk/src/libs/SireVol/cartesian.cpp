
#include <limits>
#include <cmath>

#include "SireMol/qhash_siremol.h"

#include "cartesian.h"

#include "SireMol/atom.h"
#include "SireMol/aabox.h"
#include "SireMol/cutgroup.h"

#include "SireError/errors.h"

using namespace SireVol;
using namespace SireMol;
using namespace SireBase;

/** Construct a default Cartesian volume */
Cartesian::Cartesian() : SimVolume()
{}

/** Copy constructor */
Cartesian::Cartesian(const Cartesian &other) : SimVolume(other)
{}

/** Destructor */
Cartesian::~Cartesian()
{}

/** Return a clone of this Cartesian volume */
SimVolumePtr Cartesian::clone() const
{
    return SimVolumePtr(new Cartesian(*this));
}

/** Populate the matrix 'mat' with the distances between all of the
    atoms of the two CutGroups. Return the shortest distance^2 between the two
    CutGroups. */
double Cartesian::calcDist(const CutGroup &group0, const CutGroup &group1,
                           DistMatrix &mat) const
{
    double mindist(std::numeric_limits<double>::max());
    double tmpdist;
    
    int nats0 = group0.nAtoms();
    int nats1 = group1.nAtoms();
    
    //redimension the matrix to hold all of the pairs
    mat.redimension(nats0, nats1);

    //get raw pointers to the arrays - this provides more efficient access
    const Atom *array0 = group0.atoms().constData();
    const Atom *array1 = group1.atoms().constData();
        
    for (int i=0; i<nats0; i++)
    {
        const Atom& atom0 = array0[i];
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
double Cartesian::calcDist2(const CutGroup &group0, const CutGroup &group1,
                            DistMatrix &mat) const
{
    double mindist2(std::numeric_limits<double>::max());
    double tmpdist;
    
    int nats0 = group0.nAtoms();
    int nats1 = group1.nAtoms();
    
    //redimension the matrix to hold all of the pairs
    mat.redimension(nats0, nats1);

    //get raw pointers to the arrays - this provides more efficient access
    const Atom *array0 = group0.atoms().constData();
    const Atom *array1 = group1.atoms().constData();
        
    for (int i=0; i<nats0; i++)
    {
        const Atom& atom0 = array0[i];
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
double Cartesian::calcInvDist(const CutGroup &group0, const CutGroup &group1,
                              DistMatrix &mat) const
{
    double maxinvdist(0.0);
    double tmpdist;
    
    int nats0 = group0.nAtoms();
    int nats1 = group1.nAtoms();
    
    //redimension the matrix to hold all of the pairs
    mat.redimension(nats0, nats1);

    //get raw pointers to the arrays - this provides more efficient access
    const Atom *array0 = group0.atoms().constData();
    const Atom *array1 = group1.atoms().constData();
        
    for (int i=0; i<nats0; i++)
    {
        const Atom& atom0 = array0[i];
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
double Cartesian::calcInvDist2(const CutGroup &group0, const CutGroup &group1,
                               DistMatrix &mat) const
{
    double maxinvdist2(0.0);
    double tmpdist;
    
    int nats0 = group0.nAtoms();
    int nats1 = group1.nAtoms();
    
    //redimension the matrix to hold all of the pairs
    mat.redimension(nats0, nats1);

    //get raw pointers to the arrays - this provides more efficient access
    const Atom *array0 = group0.atoms().constData();
    const Atom *array1 = group1.atoms().constData();
        
    for (int i=0; i<nats0; ++i)
    {
        const Atom& atom0 = array0[i];
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
bool Cartesian::beyond(const double &dist, const CutGroup &group0, 
                       const CutGroup &group1) const
{
    const AABox &box0 = group0.aaBox();
    const AABox &box1 = group1.aaBox();
    
    return Vector::distance2(box0.center(),box1.center()) >
                      SireMaths::pow_2(dist + box0.radius() + box1.radius());
}
