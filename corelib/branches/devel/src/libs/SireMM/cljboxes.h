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

#ifndef SIREMM_CLJBOXES_H
#define SIREMM_CLJBOXES_H

#include "SireMM/cljatoms.h"

#include <QHash>
#include <QSharedDataPointer>
#include <QSharedData>
#include <QStack>

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJBox;
class CLJBoxIndex;
class CLJBoxPtr;
class CLJBoxes;
class CLJBoxDistance;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJBox&);
QDataStream& operator>>(QDataStream&, SireMM::CLJBox&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJBoxIndex&);
QDataStream& operator>>(QDataStream&, SireMM::CLJBoxIndex&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJBoxPtr&);
QDataStream& operator>>(QDataStream&, SireMM::CLJBoxPtr&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJBoxes&);
QDataStream& operator>>(QDataStream&, SireMM::CLJBoxes&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJBoxDistance&);
QDataStream& operator>>(QDataStream&, SireMM::CLJBoxDistance&);

namespace SireVol
{
class AABox;
class Space;
}

namespace SireMM
{

using SireUnits::Dimension::Length;
using SireVol::Space;

/** This class represents a single box of CLJ atoms. The CLJ calculation
    works by dividing space into a series of boxes and working out which
    boxes are close enough to be within cutoff distance
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CLJBox : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const CLJBox&);
friend QDataStream& ::operator>>(QDataStream&, CLJBox&);

public:
    CLJBox();
    CLJBox(const CLJAtoms &atoms);
    CLJBox(const CLJBox &other);
    
    ~CLJBox();
    
    CLJBox& operator=(const CLJBox &other);
    
    bool operator==(const CLJBox &other) const;
    bool operator!=(const CLJBox &other) const;
    
    CLJBox operator+(const CLJBox &other) const;
    
    static const char* typeName();
    const char* what() const;
    
    CLJAtom operator[](int i) const;
    
    CLJAtom at(int i) const;
    CLJAtom getitem(int i) const;

    int count() const;
    int size() const;
    
    bool isEmpty() const;
    
    QString toString() const;
    
    const CLJAtoms& atoms() const;

    int nAtoms() const;

    CLJBox squeeze() const;

    QVector<int> add(const CLJAtoms &atoms);

    void remove(int atom);
    void remove(const QList<int> &atoms);

    CLJAtom take(int atom);
    CLJAtom takeNegative(int atom);

private:
    void findGaps();

    /** The actual atoms in the box */
    CLJAtoms atms;
    
    /** The indicies of gaps (dummy atoms) in the box */
    QStack<int> gaps;
};

/** A simple implicitly shared pointer used to hold each CLJBox */
class SIREMM_EXPORT CLJBoxPtr
{

friend QDataStream& ::operator<<(QDataStream&, const CLJBoxPtr&);
friend QDataStream& ::operator>>(QDataStream&, CLJBoxPtr&);

public:
    CLJBoxPtr();
    CLJBoxPtr(CLJBox *box);
    CLJBoxPtr(const CLJBox &box);
    
    CLJBoxPtr(const CLJBoxPtr &other);
    
    ~CLJBoxPtr();
    
    CLJBoxPtr& operator=(const CLJBoxPtr &other);
    
    bool operator==(const CLJBoxPtr &other) const;
    bool operator!=(const CLJBoxPtr &other) const;
    
    const CLJBox& read() const;
    CLJBox& write();
    
private:
    /** Implicitly shared pointer to the box */
    QSharedDataPointer<CLJBox> d;
};

/** This class provides a simple i,j,k index of a box in the grid,
    with, optionally, the index of a particular atom in the grid box */
class SIREMM_EXPORT CLJBoxIndex
{

friend QDataStream& ::operator<<(QDataStream&, const CLJBoxIndex&);
friend QDataStream& ::operator>>(QDataStream&, CLJBoxIndex&);

public:
    CLJBoxIndex();
    CLJBoxIndex(qint16 i, qint16 j, qint16 k, qint16 atom_idx=-1);
    
    CLJBoxIndex(const CLJBoxIndex &other);
    
    ~CLJBoxIndex();
    
    CLJBoxIndex& operator=(const CLJBoxIndex &other);
    
    bool operator==(const CLJBoxIndex &other) const;
    bool operator!=(const CLJBoxIndex &other) const;

    bool operator<(const CLJBoxIndex &other) const;
    bool operator<=(const CLJBoxIndex &other) const;
    
    bool operator>(const CLJBoxIndex &other) const;
    bool operator>=(const CLJBoxIndex &other) const;
    
    static const char* typeName();
    const char* what() const;
    
    QString toString() const;
    
    qint16 i() const;
    qint16 j() const;
    qint16 k() const;
    
    qint16 index() const;
    
    bool isNull() const;
    
    SireVol::AABox box(Length box_length) const;

    CLJBoxIndex boxOnly() const;

    uint hash() const;

    static CLJBoxIndex null();

    bool hasAtomIndex() const;

    static CLJBoxIndex createWithBoxLength(float x, float y, float z, Length box_length);
    static CLJBoxIndex createWithInverseBoxLength(float x, float y, float z, float inv_length);

    static CLJBoxIndex createWithBoxLength(const Vector &coords, Length box_length);
    static CLJBoxIndex createWithInverseBoxLength(const Vector &coords, float inv_length);

private:
    union
    {
        quint64 val;
        
        struct
        {
            qint16 ii;
            qint16 jj;
            qint16 kk;
            qint16 idx;
        } index;

    } v;
};

/** This simple class holds the minimum distance between the two
    specified CLJBoxes
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CLJBoxDistance
{

friend QDataStream& ::operator<<(QDataStream&, const CLJBoxDistance&);
friend QDataStream& ::operator>>(QDataStream&, CLJBoxDistance&);

public:
    CLJBoxDistance();
    CLJBoxDistance(const CLJBoxIndex &box0, const CLJBoxIndex &box1, float distance);
    
    CLJBoxDistance(const CLJBoxDistance &other);
    
    ~CLJBoxDistance();
    
    CLJBoxDistance& operator=(const CLJBoxDistance &other);
    
    bool operator==(const CLJBoxDistance &other) const;
    bool operator!=(const CLJBoxDistance &other) const;
    
    bool operator<(const CLJBoxDistance &other) const;
    bool operator>(const CLJBoxDistance &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    QString toString() const;
    
    const CLJBoxIndex& box0() const;
    const CLJBoxIndex& box1() const;
    
    float distance() const;

private:
    CLJBoxIndex b0;
    CLJBoxIndex b1;
    float dist;
};

/** The set of CLJBox boxes that contain all of the atoms in the system.
    Space is divided into a series of cubic boxes, and atoms are divided
    between these so as to speed up the CLJ calculation (atoms in boxes
    that are separated by more than the cutoff distance do not need to
    have their interactions evaluated, and there is a natural parallelism
    between calculating interaction energies between pairs of boxes rather
    than pairs of atoms)
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CLJBoxes
{

friend QDataStream& ::operator<<(QDataStream&, const CLJBoxes&);
friend QDataStream& ::operator>>(QDataStream&, CLJBoxes&);

public:
    CLJBoxes();
    CLJBoxes(Length box_size);
    
    CLJBoxes(const CLJAtoms &atoms);
    CLJBoxes(const CLJAtoms &atoms0, const CLJAtoms &atoms1);
    
    CLJBoxes(const CLJAtoms &atoms, Length box_size);
    CLJBoxes(const CLJAtoms &atoms0, const CLJAtoms &atoms1, Length box_size);
    
    CLJBoxes(const CLJBoxes &other);
    
    ~CLJBoxes();
    
    CLJBoxes& operator=(const CLJBoxes &other);
    
    bool operator==(const CLJBoxes &other) const;
    bool operator!=(const CLJBoxes &other) const;

    CLJAtom operator[](const CLJBoxIndex &idx) const;
    
    CLJAtom at(const CLJBoxIndex &idx) const;
    CLJAtom getitem(const CLJBoxIndex &idx) const;

    CLJBoxes operator+(const CLJBoxes &other) const;

    static const char* typeName();

    const char* what() const;
    
    QString toString() const;
    
    bool isEmpty() const;
    
    QVector<CLJBoxIndex> occupiedBoxIndicies() const;
    
    const QHash<CLJBoxIndex,CLJBoxPtr>& occupiedBoxes() const;
    
    CLJBox boxAt(const CLJBoxIndex &index) const;
    SireVol::AABox boxDimensionsAt(const CLJBoxIndex &index) const;
    
    CLJBox boxAt(const Vector &coords) const;
    SireVol::AABox boxDimensionsAt(const Vector &coords) const;
    
    QVector<CLJBox> boxes() const;
    QVector<SireVol::AABox> boxDimensions() const;
    
    float getDistance(const Space &space, const CLJBoxIndex &box0, const CLJBoxIndex &box1) const;
    
    static QVector<CLJBoxDistance> getDistances(const Space &space, const CLJBoxes &boxes);
    static QVector<CLJBoxDistance> getDistances(const Space &space,
                                                const CLJBoxes &boxes, Length cutoff);
    
    static QVector<CLJBoxDistance> getDistances(const Space &space,
                                                const CLJBoxes &boxes0, const CLJBoxes &boxes1);
    static QVector<CLJBoxDistance> getDistances(const Space &space,
                                                const CLJBoxes &boxes0, const CLJBoxes &boxes1,
                                                Length cutoff);
    
    QVector<CLJBoxIndex> add(const CLJAtoms &atoms);

    void remove(const QVector<CLJBoxIndex> &atoms);
    
    CLJAtoms take(const QVector<CLJBoxIndex> &atoms);
    CLJAtoms takeNegative(const QVector<CLJBoxIndex> &atoms);
    
    CLJAtoms atoms() const;
    
    Length length() const;
    
    int nAtoms() const;
    
    int nOccupiedBoxes() const;
    
    CLJBoxes squeeze() const;
    
private:
    void constructFrom(const CLJAtoms &atoms0, const CLJAtoms &atoms1);

    /** All of the boxes that contain atoms */
    QHash<CLJBoxIndex,CLJBoxPtr> bxs;
    
    /** The size of the box */
    float box_length;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Return a read-only reference to the contained object */
inline const CLJBox& CLJBoxPtr::read() const
{
    return *d;
}
    
/** Return the i value of the index */
inline qint16 CLJBoxIndex::i() const
{
    return v.index.ii;
}

/** Return the j value of the index */
inline qint16 CLJBoxIndex::j() const
{
    return v.index.jj;
}

/** Return the k value of the index */
inline qint16 CLJBoxIndex::k() const
{
    return v.index.kk;
}

/** Return a hash of the box index (used for QHash) */
inline uint CLJBoxIndex::hash() const
{
    return v.val;
}

inline uint qHash(const CLJBoxIndex &index)
{
    return index.hash();
}

/** Return the (optionally supplied) index of a particular atom in the box */
inline qint16 CLJBoxIndex::index() const
{
    return v.index.idx;
}

/** Return whether or not this contains an atom index */
inline bool CLJBoxIndex::hasAtomIndex() const
{
    return v.index.idx >= 0;
}

/** Comparison operator */
inline bool CLJBoxIndex::operator==(const CLJBoxIndex &other) const
{
    return v.val == other.v.val;
}

/** Comparison operator */
inline bool CLJBoxIndex::operator!=(const CLJBoxIndex &other) const
{
    return not operator==(other);
}

/** Comparison operator */
inline bool CLJBoxIndex::operator<(const CLJBoxIndex &other) const
{
    return v.val < other.v.val;
}

/** Comparison operator */
inline bool CLJBoxIndex::operator<=(const CLJBoxIndex &other) const
{
    return v.val <= other.v.val;
}

/** Comparison operator */
inline bool CLJBoxIndex::operator>(const CLJBoxIndex &other) const
{
    return v.val > other.v.val;
}

/** Comparison operator */
inline bool CLJBoxIndex::operator>=(const CLJBoxIndex &other) const
{
    return v.val >= other.v.val;
}

/** Return a read/write reference to the contained object */
inline CLJBox& CLJBoxPtr::write()
{
    return *d;
}

/** Return the atoms contained in the box */
inline const CLJAtoms& CLJBox::atoms() const
{
    return atms;
}

/** Return the set of all occupied boxes, indexed by their CLJBoxIndex */
inline const QHash<CLJBoxIndex,CLJBoxPtr>& CLJBoxes::occupiedBoxes() const
{
    return bxs;
}

/** Return the index of the first box */
inline const CLJBoxIndex& CLJBoxDistance::box0() const
{
    return b0;
}

/** Return the index of the second box */
inline const CLJBoxIndex& CLJBoxDistance::box1() const
{
    return b1;
}

/** Return the minimum distance between the boxes */
inline float CLJBoxDistance::distance() const
{
    return dist;
}

/** Return whether or not this is empty (contains no atoms) */
inline bool CLJBoxes::isEmpty() const
{
    return bxs.isEmpty();
}

/** Return whether or not the passed box is empty */
inline bool CLJBox::isEmpty() const
{
    return atms.isEmpty();
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE( SireMM::CLJBox )
Q_DECLARE_METATYPE( SireMM::CLJBoxIndex )
Q_DECLARE_METATYPE( SireMM::CLJBoxes )
Q_DECLARE_METATYPE( SireMM::CLJBoxDistance )

Q_DECLARE_TYPEINFO( SireMM::CLJBoxIndex, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireMM::CLJBoxDistance, Q_MOVABLE_TYPE );

SIRE_EXPOSE_CLASS( SireMM::CLJBox )
SIRE_EXPOSE_CLASS( SireMM::CLJBoxIndex )
SIRE_EXPOSE_CLASS( SireMM::CLJBoxes )
SIRE_EXPOSE_CLASS( SireMM::CLJBoxDistance )

SIRE_END_HEADER

#endif
