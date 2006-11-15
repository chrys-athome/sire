
#include <limits>
#include <cmath>

#include <QDebug>

#include "cartesian.h"
#include "coordgroup.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireVol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<Cartesian> r_cartesian("SireVol::Cartesian");

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, const Cartesian &cart)
{
    writeHeader(ds, r_cartesian, 1)
                 << static_cast<const SpaceBase&>(cart);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, Cartesian &cart)
{
    VersionID v = readHeader(ds, r_cartesian);

    if (v == 1)
    {
        ds >> static_cast<SpaceBase&>(cart);
    }
    else
        throw version_error(v, "1", r_cartesian, CODELOC);

    return ds;
}

/** Construct a default Cartesian volume */
Cartesian::Cartesian() : SpaceBase()
{}

/** Copy constructor */
Cartesian::Cartesian(const Cartesian &other) : SpaceBase(other)
{}

/** Destructor */
Cartesian::~Cartesian()
{}

/** Populate the matrix 'mat' with the distances between all points in
    the group 'group'. Return the shortest distance between points. */
double Cartesian::calcDist(const CoordGroup &group, DistMatrix &mat) const
{
    double mindist(std::numeric_limits<double>::max());
    double tmpdist;

    int n = group.count();
    const Vector *array = group.constData();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n,n);

    for (int i=0; i<n; ++i)
    {
        const Vector &point = array[i];
        mat.setOuterIndex(i);

        //zero diagonal
        mat[i] = 0;

        for (int j=i+1; j<n; ++j)
        {
            //calculate the distance between the two points
            tmpdist = Vector::distance(point,array[j]);

            //store the minimum distance
            mindist = qMin(tmpdist,mindist);

            //place this distance into the matrix
            mat[j] = tmpdist;
            mat(i,j) = tmpdist;
        }
    }

    return mindist;
}

/** Populate the matrix 'mat' with the distances^2 between all points in
    the group 'group'. Return the shortest distance between points. */
double Cartesian::calcDist2(const CoordGroup &group, DistMatrix &mat) const
{
    double mindist2(std::numeric_limits<double>::max());
    double tmpdist2;

    int n = group.count();
    const Vector *array = group.constData();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n,n);

    for (int i=0; i<n; ++i)
    {
        const Vector &point = array[i];
        mat.setOuterIndex(i);

        //zero diagonal
        mat[i] = 0;

        for (int j=i+1; j<n; ++j)
        {
            //calculate the distance between the two points
            tmpdist2 = Vector::distance2(point,array[j]);

            //store the minimum distance
            mindist2 = qMin(tmpdist2,mindist2);

            //place this distance into the matrix
            mat[j] = tmpdist2;
            mat(i,j) = tmpdist2;
        }
    }

    return sqrt(mindist2);
}

/** Populate the matrix 'mat' with the inverse distances between all points in
    the group 'group'. Return the smallest distance between points. */
double Cartesian::calcInvDist(const CoordGroup &group, DistMatrix &mat) const
{
    double mindist(0);
    double tmpdist;

    int n = group.count();
    const Vector *array = group.constData();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n,n);

    for (int i=0; i<n; ++i)
    {
        const Vector &point = array[i];
        mat.setOuterIndex(i);

        //zero diagonal
        mat[i] = 0;

        for (int j=i+1; j<n; ++j)
        {
            //calculate the distance between the two points
            tmpdist = Vector::invDistance(point,array[j]);

            //store the minimum distance
            mindist = qMax(tmpdist,mindist);

            //place this distance into the matrix
            mat[j] = tmpdist;
            mat(i,j) = tmpdist;
        }
    }

    return 1.0 / mindist;
}

/** Populate the matrix 'mat' with the inverse distances^2 between all points in
    the group 'group'. Return the smallest distance between points. */
double Cartesian::calcInvDist2(const CoordGroup &group, DistMatrix &mat) const
{
    double mindist2(0);
    double tmpdist2;

    int n = group.count();
    const Vector *array = group.constData();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n,n);

    for (int i=0; i<n; ++i)
    {
        const Vector &point = array[i];
        mat.setOuterIndex(i);

        //zero diagonal
        mat[i] = 0;

        for (int j=i+1; j<n; ++j)
        {
            //calculate the distance between the two points
            tmpdist2 = Vector::invDistance2(point,array[j]);

            //store the minimum distance
            mindist2 = qMax(tmpdist2,mindist2);

            //place this distance into the matrix
            mat[j] = tmpdist2;
            mat(i,j) = tmpdist2;
        }
    }

    return sqrt( 1.0 / mindist2 );
}

/** Populate the matrix 'mat' with the distances between all of the
    points of the two CoordGroups. Return the shortest distance between the two
    CoordGroups. */
double Cartesian::calcDist(const CoordGroup &group0, const CoordGroup &group1,
                           DistMatrix &mat) const
{
    double mindist(std::numeric_limits<double>::max());
    double tmpdist;

    int n0 = group0.count();
    int n1 = group1.count();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n0, n1);

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    for (int i=0; i<n0; ++i)
    {
        const Vector& point0 = array0[i];
        mat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //calculate the distance between the two points
            tmpdist = Vector::distance(point0,array1[j]);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            mindist = qMin(tmpdist,mindist);

            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }

    //return the minimum distance
    return mindist;
}

/** Populate the matrix 'mat' with the distances^2 between all of the
    points of the two CoordGroups. Return the shortest distance between the
    two CoordGroups. */
double Cartesian::calcDist2(const CoordGroup &group0, const CoordGroup &group1,
                            DistMatrix &mat) const
{
    double mindist2(std::numeric_limits<double>::max());
    double tmpdist;

    int n0 = group0.count();
    int n1 = group1.count();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n0, n1);

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    for (int i=0; i<n0; ++i)
    {
        const Vector& point0 = array0[i];
        mat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //calculate the distance between the two points
            tmpdist = Vector::distance2(point0,array1[j]);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            mindist2 = qMin(tmpdist,mindist2);

            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }

    //return the minimum distance
    return sqrt(mindist2);
}

/** Populate the matrix 'mat' with the inverse distances between all of the
    points of the two CoordGroups. Return the shortest distance between
    the two CoordGroups. */
double Cartesian::calcInvDist(const CoordGroup &group0, const CoordGroup &group1,
                              DistMatrix &mat) const
{
    double maxinvdist(0);
    double tmpdist;

    int n0 = group0.count();
    int n1 = group1.count();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n0, n1);

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    for (int i=0; i<n0; ++i)
    {
        const Vector &point0 = array0[i];
        mat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //calculate the distance between the two points
            tmpdist = Vector::invDistance(point0,array1[j]);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            maxinvdist = qMax(tmpdist,maxinvdist);

            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }

    return 1.0 / maxinvdist;
}

/** Populate the matrix 'mat' with the inverse distances^2 between all of the
    points of the two CoordGroups. Return the shortest distance between
    the two CoordGroups. */
double Cartesian::calcInvDist2(const CoordGroup &group0, const CoordGroup &group1,
                               DistMatrix &mat) const
{
    double maxinvdist2(0);
    double tmpdist;

    int n0 = group0.count();
    int n1 = group1.count();

    //redimension the matrix to hold all of the pairs
    mat.redimension(n0, n1);

    //get raw pointers to the arrays - this provides more efficient access
    const Vector *array0 = group0.constData();
    const Vector *array1 = group1.constData();

    for (int i=0; i<n0; ++i)
    {
        const Vector& point0 = array0[i];
        mat.setOuterIndex(i);

        for (int j=0; j<n1; ++j)
        {
            //calculate the distance between the two points
            tmpdist = Vector::invDistance2(point0, array1[j]);

            //store the minimum distance, the value expected to be the minimum
            //value is most efficiently placed as the second argument
            maxinvdist2 = qMax(tmpdist,maxinvdist2);

            //place this distance into the matrix
            mat[j] = tmpdist;
        }
    }

    return sqrt( 1.0 / maxinvdist2 );
}

/** Return whether or not these two groups are definitely beyond the cutoff distance. */
bool Cartesian::beyond(double dist, const CoordGroup &group0,
                       const CoordGroup &group1) const
{
    const AABox &box0 = group0.aaBox();
    const AABox &box1 = group1.aaBox();

    return Vector::distance2(box0.center(),box1.center()) >
                      SireMaths::pow_2(dist + box0.radius() + box1.radius());
}
