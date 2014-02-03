/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "SireMM/cljboxes.h"

#include "SireVol/aabox.h"
#include "SireVol/space.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

#include <QElapsedTimer>
#include <QDebug>

using namespace SireMM;
using namespace SireMaths;
using namespace SireVol;
using namespace SireStream;

///////////
/////////// Implementation of CLJBox
///////////

static const RegisterMetaType<CLJBox> r_cljbox(NO_ROOT);

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJBox &box)
{
    writeHeader(ds, r_cljbox, 1);
    
    SharedDataStream sds(ds);
    sds << box.atms;
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJBox &box)
{
    VersionID v = readHeader(ds, r_cljbox);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> box.atms;
    }
    else
        throw version_error(v, "1", r_cljbox, CODELOC);
    
    return ds;
}

/** Null constructor */
CLJBox::CLJBox()
{}

/** Construct a box that holds the passed atoms */
CLJBox::CLJBox(const CLJAtoms &atoms) : atms(atoms)
{}

/** Copy constructor */
CLJBox::CLJBox(const CLJBox &other) : atms(other.atms)
{}

/** Destructor */
CLJBox::~CLJBox()
{}

QString CLJBox::toString() const
{
    return QObject::tr("CLJBox( nAtoms() == %1 )").arg(nAtoms());
}

/** Copy assignment operator */
CLJBox& CLJBox::operator=(const CLJBox &other)
{
    atms = other.atms;
    return *this;
}

/** Comparison operator */
bool CLJBox::operator==(const CLJBox &other) const
{
    return atms == other.atms;
}

/** Comparison operator */
bool CLJBox::operator!=(const CLJBox &other) const
{
    return not operator==(other);
}

/** Return the number of atoms in the box */
int CLJBox::nAtoms() const
{
    return atms.count();
}

const char* CLJBox::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJBox>() );
}

const char* CLJBox::what() const
{
    return CLJBox::typeName();
}

///////////
/////////// Implementation of CLJBoxPtr
///////////

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJBoxPtr &ptr)
{
    SharedDataStream sds(ds);
    sds << ptr.d;
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJBoxPtr &ptr)
{
    SharedDataStream sds(ds);
    sds >> ptr.d;
    return ds;
}

static const CLJBoxPtr *null_box = 0;

/** Constructor */
CLJBoxPtr::CLJBoxPtr()
{
    if (null_box == 0)
    {
        null_box = new CLJBoxPtr( new CLJBox() );
    }
    
    this->operator=(*null_box);
}

/** Construct from the passed box. This takes over ownership of the pointer,
    and will delete it once there are no more CLJBoxPtrs pointing to this box */
CLJBoxPtr::CLJBoxPtr(CLJBox *box)
{
    if (box == 0)
    {
        if (null_box == 0)
        {
            null_box = new CLJBoxPtr( new CLJBox() );
        }
        
        this->operator=(*null_box);
    }
    else
    {
        d = box;
    }
}

/** Copy constructor */
CLJBoxPtr::CLJBoxPtr(const CLJBoxPtr &other) : d(other.d)
{}

/** Destructor */
CLJBoxPtr::~CLJBoxPtr()
{
    //this will delete the box if it is the last
    //reference to the box
}

/** Copy assignment operator */
CLJBoxPtr& CLJBoxPtr::operator=(const CLJBoxPtr &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool CLJBoxPtr::operator==(const CLJBoxPtr &other) const
{
    return d.constData() == other.d.constData() or
           *(d.constData()) == *(other.d.constData());
}

/** Comparison operator */
bool CLJBoxPtr::operator!=(const CLJBoxPtr &other) const
{
    return not operator==(other);
}

///////////
/////////// Implementation of CLJBoxIndex
///////////

static const RegisterMetaType<CLJBoxIndex> r_cljboxindex(NO_ROOT);

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJBoxIndex &index)
{
    writeHeader(ds, r_cljboxindex, 1);
    ds << index.v.index.ii << index.v.index.jj << index.v.index.kk << index.v.index.idx;
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJBoxIndex &index)
{
    VersionID v = readHeader(ds, r_cljboxindex);
    
    if (v == 1)
    {
        ds >> index.v.index.ii >> index.v.index.jj
           >> index.v.index.kk >> index.v.index.idx;
    }
    else
        throw version_error(v, "1", r_cljboxindex, CODELOC);
    
    return ds;
}

/** Null constructor */
CLJBoxIndex::CLJBoxIndex()
{
    v.val = 0;
}

/** Construct the index of the box at index i,j,k with (optionally supplied)
    index of a particular atom in the box */
CLJBoxIndex::CLJBoxIndex(qint16 ii, qint16 jj, qint16 kk, quint16 atom_idx)
{
    v.index.ii = ii;
    v.index.jj = jj;
    v.index.kk = kk;
    v.index.idx = atom_idx;
}

/** Copy constructor */
CLJBoxIndex::CLJBoxIndex(const CLJBoxIndex &other)
{
    v.val = other.v.val;
}

/** Destructor */
CLJBoxIndex::~CLJBoxIndex()
{}

/** Copy assignment operator */
CLJBoxIndex& CLJBoxIndex::operator=(const CLJBoxIndex &other)
{
    v.val = other.v.val;
    return *this;
}

const char* CLJBoxIndex::typeName()
{
    return "SireMM::CLJBoxIndex";
}

const char* CLJBoxIndex::what() const
{
    return CLJBoxIndex::typeName();
}

/** Return the AABox that describes this box (for a given box length of 'box_length') */
AABox CLJBoxIndex::box(Length box_length) const
{
    Vector length(box_length.value());
    
    Vector origin( i() * length.x(),
                   j() * length.y(),
                   k() * length.z() );
    
    return AABox(origin, 0.5*length);
}

/** Return a copy of this index that contains only the box index (not the atom index) */
CLJBoxIndex CLJBoxIndex::boxOnly() const
{
    return CLJBoxIndex( i(), j(), k() );
}

QString CLJBoxIndex::toString() const
{
    return QObject::tr("CLJBoxIndex( %1, %2, %3 : %4 )")
                .arg(v.index.ii)
                .arg(v.index.jj)
                .arg(v.index.kk)
                .arg(v.index.idx);
}

/** Create the index for the box that contains the point 'x,y,z' in a set of boxes
    of length 1 / inv_box_length */
CLJBoxIndex CLJBoxIndex::createWithInverseBoxLength(float x, float y, float z, float inv_length)
{
    int i = std::floor(x * inv_length + 0.5);
    int j = std::floor(y * inv_length + 0.5);
    int k = std::floor(z * inv_length + 0.5);
    
    const int min16 = std::numeric_limits<qint16>::min();
    const int max16 = std::numeric_limits<qint16>::max();
    
    if (i < min16 or i > max16 or
        j < min16 or j > max16 or
        k < min16 or k > max16)
    {
        throw SireError::program_bug( QObject::tr(
                "It is not possible to get the index of the box containing the point "
                "(%1,%2,%3) when the boxes have length %4 A, as the index lies outside "
                "the bounds of a 16bit integer (%5 to %6), (%7,%8,%9).")
                    .arg(x).arg(y).arg(z)
                    .arg(1 / inv_length)
                    .arg(min16).arg(max16)
                    .arg(i).arg(j).arg(k), CODELOC );
    }
    
    return CLJBoxIndex(i, j, k);
}

/** Create the index for the box that contains the point 'x,y,z' in a set of boxes
    of length 1 / inv_box_length */
CLJBoxIndex CLJBoxIndex::createWithInverseBoxLength(const Vector &coords, float inv_length)
{
    return createWithInverseBoxLength(coords.x(), coords.y(), coords.z(), inv_length);
}

/** Create the index for the box that contains the point 'x,y,z' in a set of boxes
    of length 'box_length' */
CLJBoxIndex CLJBoxIndex::createWithBoxLength(float x, float y, float z, Length box_length)
{
    return CLJBoxIndex::createWithInverseBoxLength(x, y, z, 1.0/box_length.value());
}

/** Create the index for the box that contains the point 'x,y,z' in a set of boxes
    of length 'box_length' */
CLJBoxIndex CLJBoxIndex::createWithBoxLength(const Vector &coords, Length box_length)
{
    return CLJBoxIndex::createWithInverseBoxLength(coords.x(), coords.y(),
                                                   coords.z(), 1.0/box_length.value());
}

///////////
/////////// Implementation of CLJBoxDistance
///////////

static const RegisterMetaType<CLJBoxDistance> r_boxdist(NO_ROOT);

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJBoxDistance &boxdist)
{
    writeHeader(ds, r_boxdist, 1);
    
    ds << boxdist.b0 << boxdist.b1 << boxdist.dist;
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJBoxDistance &boxdist)
{
    VersionID v = readHeader(ds, r_boxdist);
    
    if (v == 1)
    {
        ds >> boxdist.b0 >> boxdist.b1 >> boxdist.dist;
    }
    else
        throw version_error(v, "1", r_boxdist, CODELOC);
    
    return ds;
}

/** Constructor */
CLJBoxDistance::CLJBoxDistance() : dist(0)
{}

/** Construct saying that the minimum distance between the box with index 'box0'
    and the box with index 'box1' is 'distance' */
CLJBoxDistance::CLJBoxDistance(const CLJBoxIndex &box0, const CLJBoxIndex &box1, float distance)
               : b0(box0), b1(box1), dist(distance)
{}

/** Copy constructor */
CLJBoxDistance::CLJBoxDistance(const CLJBoxDistance &other)
               : b0(other.b0), b1(other.b1), dist(other.dist)
{}

/** Destructor */
CLJBoxDistance::~CLJBoxDistance()
{}

/** Copy assignment operator */
CLJBoxDistance& CLJBoxDistance::operator=(const CLJBoxDistance &other)
{
    if (this != &other)
    {
        b0 = other.b0;
        b1 = other.b1;
        dist = other.dist;
    }
    
    return *this;
}

/** Comparison operator. Note that this will compare equal only if
    the distances are the same and if either, box0() == other.box() and
    box1() == other.box1() */
bool CLJBoxDistance::operator==(const CLJBoxDistance &other) const
{
    return dist == other.dist and
           b0 == other.b0 and b1 == other.b1;
}

/** Comparison operator */
bool CLJBoxDistance::operator!=(const CLJBoxDistance &other) const
{
    return not operator==(other);
}

/** Distance comparison operator - compares less if the distance
    is less than 'other'. This is used to allow CLJBoxDistance objects
    to be sorted according to their minimum distances */
bool CLJBoxDistance::operator<(const CLJBoxDistance &other) const
{
    return dist < other.dist;
}

/** Distance comparison operator - compares greater if the distance
    is greater than 'other'. This is used to allow CLJBoxDistance objects
    to be sorted according to their minimum distances */
bool CLJBoxDistance::operator>(const CLJBoxDistance &other) const
{
    return dist > other.dist;
}

const char* CLJBoxDistance::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJBoxDistance>() );
}

QString CLJBoxDistance::toString() const
{
    return QObject::tr("CLJBoxDistance( %1->%2, %3 A )")
                .arg(b0.toString(), b1.toString())
                .arg(dist);
}

const char* CLJBoxDistance::what() const
{
    return CLJBoxDistance::typeName();
}


///////////
/////////// Implementation of CLJBoxes
///////////

static const RegisterMetaType<CLJBoxes> r_cljboxes(NO_ROOT);

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJBoxes &boxes)
{
    writeHeader(ds, r_cljboxes, 1);
    
    SharedDataStream sds(ds);
    
    sds << boxes.bxs << boxes.box_length;
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJBoxes &boxes)
{
    VersionID v = readHeader(ds, r_cljboxes);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> boxes.bxs >> boxes.box_length;
    }
    else
        throw version_error(v, "1", r_cljboxes, CODELOC);
    
    return ds;
}

const double default_box_length = 10;

/** Null constructor */
CLJBoxes::CLJBoxes() : box_length(default_box_length)
{}

/** Construct from the passed set of atoms */
void CLJBoxes::constructFrom(const CLJAtoms &atoms)
{
    QElapsedTimer t;
    t.start();
    
    const float inv_length = 1.0 / box_length;
    
    QMap< CLJBoxIndex,QList<CLJAtom> > boxed_atoms;
    
    for (int i=0; i<atoms.count(); ++i)
    {
        const CLJAtom atom = atoms[i];

        if (atom.ID() != 0)
        {
            CLJBoxIndex cljindex = CLJBoxIndex::createWithInverseBoxLength(
                                                        atom.coordinates(), inv_length);
            
            boxed_atoms[cljindex].append(atom);
        }
    }

    //now build the CLJAtoms for each box
    for (QMap< CLJBoxIndex,QList<CLJAtom> >::const_iterator it = boxed_atoms.constBegin();
         it != boxed_atoms.constEnd();
         ++it)
    {
        //qDebug() << "Box" << it.key().box(Length(box_length)).toString() << it.value().count();
        bxs.insert( it.key(), new CLJBox(CLJAtoms(it.value())) );
    }
    
    quint64 ns = t.nsecsElapsed();
    qDebug() << "Boxing up" << atoms.count() << "atoms took"
             << (0.000001*ns) << "ms";

    qDebug() << "number of boxes ==" << boxed_atoms.count();
}

/** Box up the passed set of atoms into boxes of the default box size
    (5 angstroms) */
CLJBoxes::CLJBoxes(const CLJAtoms &atoms) : box_length(default_box_length)
{
    constructFrom(atoms);
}

/** Box up the passed set of atoms into boxes of specified box size */
CLJBoxes::CLJBoxes(const CLJAtoms &atoms, Length box_size)
         : box_length(box_size.value())
{
    if (box_length <= 0)
        box_length = default_box_length;
    
    constructFrom(atoms);
}

/** Copy constructor */
CLJBoxes::CLJBoxes(const CLJBoxes &other)
         : bxs(other.bxs), box_length(other.box_length)
{}

/** Destructor */
CLJBoxes::~CLJBoxes()
{}

/** Copy assignment operator */
CLJBoxes& CLJBoxes::operator=(const CLJBoxes &other)
{
    if (this != &other)
    {
        bxs = other.bxs;
        box_length = other.box_length;
    }
    
    return *this;
}

/** Comparison operator */
bool CLJBoxes::operator==(const CLJBoxes &other) const
{
    return box_length == other.box_length and bxs == other.bxs;
}

/** Comparison operator */
bool CLJBoxes::operator!=(const CLJBoxes &other) const
{
    return not operator==(other);
}

QString CLJBoxes::toString() const
{
    return QObject::tr("CLJBoxes( nAtoms() == %1, nOccupiedBoxes() == %2 )")
                .arg(nAtoms()).arg(nOccupiedBoxes());
}

/** Return the indicies of all occupied boxes */
QVector<CLJBoxIndex> CLJBoxes::occupiedBoxIndicies() const
{
    return bxs.keys().toVector();
}

/** Return the box at index 'index' */
CLJBox CLJBoxes::boxAt(const CLJBoxIndex &index) const
{
    return bxs.value( index.boxOnly() ).read();
}

/** Return the AABox that describes the boundary of the box at index 'index' */
AABox CLJBoxes::boxDimensionsAt(const CLJBoxIndex &index) const
{
    return index.box( Length(box_length) );
}

/** Return the box that contains the point with coordinates 'coords' */
CLJBox CLJBoxes::boxAt(const Vector &coords) const
{
    return this->boxAt( CLJBoxIndex::createWithInverseBoxLength(coords, 1.0/box_length) );
}

/** Return the dimensions of the box that contains the point with coordinates 'coords' */
AABox CLJBoxes::boxDimensionsAt(const Vector &coords) const
{
    return this->boxDimensionsAt( CLJBoxIndex::createWithInverseBoxLength(coords, 1.0/box_length));
}

/** Return an array containing all occupied boxes. This is in the same order
    as the box dimensions returned by 'boxDimensions' */
QVector<CLJBox> CLJBoxes::boxes() const
{
    if (bxs.isEmpty())
        return QVector<CLJBox>();

    QVector<CLJBox> b( bxs.count() );
    CLJBox *ba = b.data();
    
    int idx = 0;
    
    for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = bxs.constBegin();
         it != bxs.constEnd();
         ++it)
    {
        ba[idx] = it.value().read();
        idx += 1;
    }
    
    return b;
}

/** Return an array containing all occupied box dimensions. This is in the same order
    as the boxes returned by 'boxes' */
QVector<AABox> CLJBoxes::boxDimensions() const
{
    if (bxs.isEmpty())
        return QVector<AABox>();

    QVector<AABox> b( bxs.count() );
    AABox *ba = b.data();
    
    int idx = 0;
    
    for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = bxs.constBegin();
         it != bxs.constEnd();
         ++it)
    {
        ba[idx] = it.key().box(Length(box_length));
        idx += 1;
    }
    
    return b;
}

/** Return the number of occupied boxes */
int CLJBoxes::nOccupiedBoxes() const
{
    return bxs.count();
}

/** Return the number of atoms in the boxes */
int CLJBoxes::nAtoms() const
{
    int n = 0;
    
    for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = bxs.constBegin();
         it != bxs.constEnd();
         ++it)
    {
        n += it.value().read().nAtoms();
    }
    
    return n;
}

/** Return all of the atoms in all of the boxes (these may
    be returned with a lot of padding) */
CLJAtoms CLJBoxes::atoms() const
{
    CLJAtoms atms;
    
    for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = bxs.constBegin();
         it != bxs.constEnd();
         ++it)
    {
        atms += it.value().read().atoms();
    }
    
    return atms;
}

/** Return the length of each side of each box */
Length CLJBoxes::length() const
{
    return Length(box_length);
}

const char* CLJBoxes::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJBoxes>() );
}

const char* CLJBoxes::what() const
{
    return CLJBoxes::typeName();
}

// this is the collection of square roots of integers between 0 and 31
// This is used as a lookup table for the getBoxDistance function, which
// we know will always have integer distances
static const int nsqrts = 32;
static const float sqrts[] = { 0, 1, 1.41421356237, 1.73205080757,
                               2.0, 2.2360679775, 2.44948974278,
                               2.64575131106, 2.82842712475,
                               3.0, 3.16227766017, 3.31662479036,
                               3.46410161514, 3.60555127546,
                               3.74165738677, 3.87298334621,
                               4.0, 4.12310562562, 4.24264068712,
                               4.35889894354, 4.472135955, 4.58257569496,
                               4.69041575982, 4.79583152331, 4.89897948557,
                               5.0, 5.09901951359, 5.19615242271, 5.29150262213,
                               5.38516480713, 5.47722557505, 5.56776436283 };

/** Return the distance between boxes, in box length units, if they 
    are separated by dx, dy and dz box lengths */
inline float getBoxDistance(const int dx, const int dy, const int dz)
{
    //sort the deltas into numerically decreasing value
    const int sum = dx*dx + dy*dy + dz*dz;
    
    if (sum < nsqrts)
    {
        return sqrts[sum];
    }
    else
        return std::sqrt( float(sum) );
}

/** Get the number of box lengths separating boxes 0 and 1 at indicies
    i0 and i1 */
inline int getDelta( const int i0, const int i1 )
{
    // the below single-line expression is doing
    // if (i0 == i1)
    // {
    //     return 0;
    // }
    // else if (i0 < i1)
    // {
    //     return i1 - i0 - 1;
    // }
    // else
    // {
    //     return i0 - i1 - 1;
    // }
    //
    // I found on my mac that the single line compiled to faster code
    // than the above if statements.

    return (i0 == i1) ? 0 : (i0 < i1) ? (i1-i0-1) : (i0-i1-1);
}

/** Return the distances between all of the occupied boxes in 'boxes'
    based on the space 'space' */
QVector<CLJBoxDistance> CLJBoxes::getDistances(const Space &space, const CLJBoxes &boxes)
{
    QVector<CLJBoxDistance> dists;
    dists.reserve(1024);
    
    if (space.isCartesian())
    {
        for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = boxes.bxs.constBegin();
             it != boxes.bxs.constEnd();
             ++it)
        {
            const CLJBoxIndex &box0 = it.key();
            
            for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it2 = it;
                 it2 != boxes.bxs.constEnd();
                 ++it2)
            {
                const CLJBoxIndex &box1 = it2.key();
                
                const int dx = getDelta(box0.i(), box1.i());
                const int dy = getDelta(box0.j(), box1.j());
                const int dz = getDelta(box0.k(), box1.k());
                
                const float dist = getBoxDistance(dx, dy, dz);
                dists.append( CLJBoxDistance(box0, box1, dist*boxes.box_length) );
            }
        }
    }
    else
    {
        
        const Length l(boxes.box_length);
        
        for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = boxes.bxs.constBegin();
             it != boxes.bxs.constEnd();
             ++it)
        {
            const AABox box0 = it.key().box(l);
        
            for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it2 = it;
                 it2 != boxes.bxs.constEnd();
                 ++it2)
            {
                const AABox box1 = it2.key().box(l);
                
                dists.append( CLJBoxDistance(it.key(), it2.key(), space.minimumDistance(box0,box1)) );
            }
        }
    }
    
    return dists;
}

/** Return the distances between all of the occupied boxes in 'boxes'
    based on the space 'space', only returning boxes that are separated    
    by distances of less than 'cutoff' */
QVector<CLJBoxDistance> CLJBoxes::getDistances(const Space &space, const CLJBoxes &boxes,
                                               Length cutoff)
{
    QElapsedTimer t;
    t.start();

    QVector<CLJBoxDistance> dists;
    dists.reserve(1024);

    if (space.isCartesian())
    {
        const float box_cutoff = cutoff.value() / boxes.box_length;
        const int int_box_cutoff = std::ceil(box_cutoff);
        
        for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = boxes.bxs.constBegin();
             it != boxes.bxs.constEnd();
             ++it)
        {
            const CLJBoxIndex &box0 = it.key();
            
            for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it2 = it;
                 it2 != boxes.bxs.constEnd();
                 ++it2)
            {
                const CLJBoxIndex &box1 = it2.key();
                
                const int dx = getDelta(box0.i(), box1.i());
                const int dy = getDelta(box0.j(), box1.j());
                const int dz = getDelta(box0.k(), box1.k());
                
                if ((dx+dy+dz) <= int_box_cutoff)
                {
                    //the box-pair are within cutoff
                    const float dist = getBoxDistance(dx, dy, dz);
                    
                    if (dist < box_cutoff)
                        dists.append( CLJBoxDistance(box0, box1, dist*boxes.box_length) );
                }
            }
        }
    }
    else
    {
        const Length l(boxes.box_length);
        
        for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = boxes.bxs.constBegin();
             it != boxes.bxs.constEnd();
             ++it)
        {
            const AABox box0 = it.key().box(l);
        
            for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it2 = it;
                 it2 != boxes.bxs.constEnd();
                 ++it2)
            {
                const AABox box1 = it2.key().box(l);
                
                const float dist = space.minimumDistance(box0,box1);
                
                if (dist < cutoff.value())
                    dists.append( CLJBoxDistance(it.key(), it2.key(), dist) );
            }
        }
    }
    
    quint64 ns = t.nsecsElapsed();
    qDebug() << "Getting box distances took" << (0.000001*ns) << "ms";
    
    return dists;
}

/** Return the distances between all pairs of occupied boxes between the boxes in 
    'boxes0' and the boxes in 'boxes1' */
QVector<CLJBoxDistance> CLJBoxes::getDistances(const Space &space, const CLJBoxes &boxes0,
                                               const CLJBoxes &boxes1)
{
    QVector<CLJBoxDistance> dists;
    dists.reserve(1024);
    
    if (space.isCartesian() and (boxes0.box_length == boxes1.box_length))
    {
        for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = boxes0.bxs.constBegin();
             it != boxes0.bxs.constEnd();
             ++it)
        {
            const CLJBoxIndex &box0 = it.key();
            
            for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it2 = boxes1.bxs.constBegin();
                 it2 != boxes1.bxs.constEnd();
                 ++it2)
            {
                const CLJBoxIndex &box1 = it2.key();
                
                const int dx = getDelta(box0.i(), box1.i());
                const int dy = getDelta(box0.j(), box1.j());
                const int dz = getDelta(box0.k(), box1.k());
                
                //the box-pair are within cutoff
                const float dist = getBoxDistance(dx, dy, dz);
                dists.append( CLJBoxDistance(box0, box1, dist*boxes0.box_length) );
            }
        }
    }
    else
    {
        const Length l0(boxes0.box_length);
        const Length l1(boxes1.box_length);
        
        for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = boxes0.bxs.constBegin();
             it != boxes0.bxs.constEnd();
             ++it)
        {
            const AABox box0 = it.key().box(l0);
            
            for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it2 = boxes1.bxs.constBegin();
                 it2 != boxes1.bxs.constEnd();
                 ++it2)
            {
                const AABox box1 = it2.key().box(l1);
                
                dists.append( CLJBoxDistance(it.key(), it2.key(),
                                             space.minimumDistance(box0,box1)) );
            }
        }
    }
    
    return dists;
}

/** Return the distances between all pairs of occupied boxes between the boxes in 
    'boxes0' and the boxes in 'boxes1' */
QVector<CLJBoxDistance> CLJBoxes::getDistances(const Space &space, const CLJBoxes &boxes0,
                                               const CLJBoxes &boxes1, Length cutoff)
{
    QElapsedTimer t;
    t.start();

    QVector<CLJBoxDistance> dists;
    dists.reserve( 1024 );

    if (space.isCartesian() and (boxes0.box_length == boxes1.box_length))
    {
        const float box_cutoff = cutoff.value() / boxes0.box_length;
        const int int_box_cutoff = std::ceil(box_cutoff);
        
        for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = boxes0.bxs.constBegin();
             it != boxes0.bxs.constEnd();
             ++it)
        {
            const CLJBoxIndex &box0 = it.key();
            
            for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it2 = boxes1.bxs.constBegin();
                 it2 != boxes1.bxs.constEnd();
                 ++it2)
            {
                const CLJBoxIndex &box1 = it2.key();
                
                const int dx = getDelta(box0.i(), box1.i());
                const int dy = getDelta(box0.j(), box1.j());
                const int dz = getDelta(box0.k(), box1.k());
                
                if ((dx+dy+dz) <= int_box_cutoff)
                {
                    //the box-pair are within cutoff
                    const float dist = getBoxDistance(dx, dy, dz);
                    
                    if (dist < box_cutoff)
                        dists.append( CLJBoxDistance(box0, box1, dist*boxes0.box_length) );
                }
            }
        }
    }
    else
    {
        const Length l0(boxes0.box_length);
        const Length l1(boxes1.box_length);
        
        for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = boxes0.bxs.constBegin();
             it != boxes0.bxs.constEnd();
             ++it)
        {
            const AABox box0 = it.key().box(l0);
            
            for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it2 = boxes1.bxs.constBegin();
                 it2 != boxes1.bxs.constEnd();
                 ++it2)
            {
                const AABox box1 = it2.key().box(l1);
                
                const float dist = space.minimumDistance(box0, box1);
                
                if (dist < cutoff.value())
                    dists.append( CLJBoxDistance(it.key(), it2.key(), dist) );
            }
        }
    }
    
    quint64 ns = t.nsecsElapsed();
    qDebug() << "Getting box distances took" << (0.000001*ns) << "ms";
    
    return dists;
}
