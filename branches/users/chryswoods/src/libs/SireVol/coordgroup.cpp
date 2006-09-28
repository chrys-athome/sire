/**
  * @file
  *
  * C++ Implementation: CoordGroupBase, CoordGroup, CoordGroupEditor and CoordGroupPvt
  *
  * Description:
  * Implementation of the public CoordGroupBase, CoordGroup and CoordGroupEditor classes,
  * and the private CoordGroupPvt class.
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QSharedData>
#include <QVector>

#include <boost/scoped_array.hpp>

#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/rotate.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "coordgroup.h"

using namespace SireMaths;
using namespace SireStream;

static const RegisterMetaType<SireVol::CoordGroupPvt>
                    r_grouppvt("SireVol::CoordGroupPvt", MAGIC_ONLY);

QDataStream& operator<<(QDataStream&, const SireVol::CoordGroupPvt&);
QDataStream& operator>>(QDataStream&, SireVol::CoordGroupPvt&);

namespace SireVol
{

/** Private class which holds the data for CoordGroup and CoordGroupEditor

    @author Christopher Woods
*/
class CoordGroupPvt : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const CoordGroupPvt&);
friend QDataStream& ::operator>>(QDataStream&, CoordGroupPvt&);

public:
    CoordGroupPvt();
    CoordGroupPvt(int size);
    CoordGroupPvt(int size, const Vector &value);
    CoordGroupPvt(const QVector<Vector> &coords);

    CoordGroupPvt(const CoordGroupPvt &other);

    ~CoordGroupPvt();

    CoordGroupPvt& operator=(const CoordGroupPvt &other);

    bool operator==(const CoordGroupPvt &other) const;
    bool operator!=(const CoordGroupPvt &other) const;

    const Vector& at(int i) const;

    const Vector& operator[](int i) const;
    Vector& operator[](int i);

    bool isEmpty() const;

    int count() const;
    int size() const;

    const AABox& aaBox() const;

    const Vector* constData() const;
    const Vector* data() const;
    Vector* data();

    void translate(const Vector &delta);
    void translate(int i, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const Matrix &rotmat, const Vector &point);

    void rotate(int i, const Quaternion &quat, const Vector &point);
    void rotate(int i, const Matrix &rotmat, const Vector &point);

    bool needsUpdate() const;
    void update();

    void setCoordinates(const QVector<Vector> &newcoords);

    void assertSameSize(const QVector<Vector> &newcoords) const;
    void assertSameSize(const CoordGroupBase &newcoords) const;

private:
    void checkIndex(int i) const;

    void assertSameSize(int n) const;

    /** Pointer to the array of coordinates */
    boost::scoped_array<Vector> coords;
    /** The number of coordinates in the group */
    qint32 sz;

    /** Whether or not the AABox is in sync with the coordinates */
    bool needsupdate;

    /** The AABox which should completely enclose all of the points */
    AABox aabox;
};

}

/////////////
///////////// Implementation of CoordGroupPvt
/////////////

using namespace SireVol;

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const CoordGroupPvt &coordgroup)
{
    qint32 sz = coordgroup.size();

    writeHeader(ds, r_grouppvt, 1) << sz;

    if (sz > 0)
    {
        const Vector *array = coordgroup.constData();

        for (int i=0; i<sz; ++i)
        {
            //don't stream each vector individually, as this will
            //fill the stream with metadata!
            const Vector &coords = array[i];
            double x = coords.x();
            double y = coords.y();
            double z = coords.z();

            ds << x << y << z;
        }
    }

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, CoordGroupPvt &coordgroup)
{
    VersionID v = readHeader(ds, r_grouppvt);

    if (v == 1)
    {
        qint32 sz;
        ds >> sz;

        if (sz > 0)
        {
            boost::scoped_array<Vector> new_array( new Vector[sz] );

            double x,y,z;

            for (int i=0; i<sz; ++i)
            {
                Vector &coords = new_array[i];

                ds >> x >> y >> z;
                coords.set(x,y,z);
            }

            //now everything has been successfully loaded, we can
            //update 'coordgroup' (*maintain the invariant*)
            coordgroup.coords.swap(new_array);
            coordgroup.sz = sz;
            coordgroup.needsupdate = true;
            coordgroup.update();
        }
        else
        {
            coordgroup.coords.reset();
            coordgroup.aabox = AABox();
            coordgroup.sz = 0;
            coordgroup.needsupdate = false;
        }
    }
    else
        throw version_error(v, "1", r_grouppvt, CODELOC);

    return ds;
}

/** Null constructor */
CoordGroupPvt::CoordGroupPvt() : QSharedData(), sz(0), needsupdate(false)
{}

/** Construct a CoordGroup to hold 'size' coordinate */
CoordGroupPvt::CoordGroupPvt(int size)
              : QSharedData(), sz(size), needsupdate(false)
{
    if (size > 0)
        coords.reset( new Vector[size] );
}

/** Construct a CoordGroup to hold 'size' coordinates, all of which have the
    value 'value' */
CoordGroupPvt::CoordGroupPvt(int size, const Vector &value)
              : QSharedData(), sz(size), needsupdate(false)
{
    if (size > 0)
    {
        coords.reset( new Vector[size] );

        for (int i=0; i<size; ++i)
            coords[i] = value;

        aabox = AABox(value, Vector(0));
    }
}

/** Create from a vector of coordinates */
CoordGroupPvt::CoordGroupPvt(const QVector<Vector> &coords)
              : QSharedData(), sz(0), needsupdate(false)
{
    setCoordinates(coords);
}

/** Copy constructor - deep-copies the coordinates of 'other' */
CoordGroupPvt::CoordGroupPvt(const CoordGroupPvt &other)
              : QSharedData(), sz(other.sz),
                needsupdate(other.needsupdate), aabox(other.aabox)
{
    if (sz > 0)
    {
        coords.reset( new Vector[sz] );

        //use memcpy to copy the vector (possible as this
        //is a movable class) (actually use the qMemCopy function provided by Qt
        //in qglobal.h)
        void *output = qMemCopy( coords.get(), other.coords.get(), sz * sizeof(Vector) );

        BOOST_ASSERT( output == coords.get() );
    }
}

/** Destructor */
CoordGroupPvt::~CoordGroupPvt()
{}

/** Assignment operator */
CoordGroupPvt& CoordGroupPvt::operator=(const CoordGroupPvt &other)
{
    if (other.sz > 0)
    {
        sz = other.sz;
        needsupdate = other.needsupdate;
        aabox = other.aabox;

        coords.reset( new Vector[sz] );

        void *output = qMemCopy( coords.get(), other.coords.get(), sz * sizeof(Vector) );

        BOOST_ASSERT( output = coords.get() );
    }
    else
    {
        sz = 0;
        coords.reset();
        aabox = AABox();
        needsupdate = false;
    }

    return *this;
}

/** Comparison operator */
bool CoordGroupPvt::operator==(const CoordGroupPvt &other) const
{
    if (this == &other)
        return true;
    else if (sz != other.sz)
        return false;
    else
    {
        for (int i=0; i<sz; ++i)
        {
            if (other.coords[i] != coords[i])
                return false;
        }

        return true;
    }
}

/** Comparison operator */
bool CoordGroupPvt::operator!=(const CoordGroupPvt &other) const
{
    return not (*this == other);
}

/** Set the coordinates of this group to be the same as those in 'newcoords' */
void CoordGroupPvt::setCoordinates(const QVector<Vector> &newcoords)
{
    int n = newcoords.size();
    if (n > 0)
    {
        sz = n;
        coords.reset( new Vector[sz] );

        void *output = qMemCopy( coords.get(), newcoords.constData(),
                                 sz * sizeof(Vector) );

        BOOST_ASSERT( output = coords.get() );

        needsupdate = true;
        this->update();
    }
    else
    {
        sz = 0;
        coords.reset();
        aabox = AABox();
        needsupdate = false;
    }
}

/** Check that an index is valid - else throw an exception

    \throw SireError::invalid_index
*/
inline void CoordGroupPvt::checkIndex(int i) const
{
    if (i < 0 or i >= sz)
        throw SireError::invalid_index( QObject::tr(
                "Invalid index in CoordGroup: i == %1 while size() == %2")
                    .arg(i).arg(sz), CODELOC );
}

/** Return the 'ith' coordinate in the group - this will throw an
    exception if 'i' refers to an invalid index.

    \throw SireError::invalid_index
*/
inline const Vector& CoordGroupPvt::at(int i) const
{
    checkIndex(i);
    return coords[i];
}

/** Return the 'ith' coordinate in the group - this will throw an
    exception if 'i' refers to an invalid index.

    \throw SireError::invalid_index
*/
inline const Vector& CoordGroupPvt::operator[](int i) const
{
    checkIndex(i);
    return coords[i];
}

/** Return the 'ith' coordinate in the group - this will throw an
    exception if 'i' refers to an invalid index.

    \throw SireError::invalid_index
*/
inline Vector& CoordGroupPvt::operator[](int i)
{
    checkIndex(i);
    return coords[i];
}

/** Return whether this CoordGroup is empty */
inline bool CoordGroupPvt::isEmpty() const
{
    return sz == 0;
}

/** Return the number of coordinates in the group */
inline int CoordGroupPvt::count() const
{
    return sz;
}

/** Return the number of coordinates in the group */
inline int CoordGroupPvt::size() const
{
    return sz;
}

/** Return whether or not the AABox is out of sync with the
    coordinates */
inline bool CoordGroupPvt::needsUpdate() const
{
    return needsupdate;
}

/** Return the AABox that encloses the coordinates - you should
    always ensure that the box is up-to-date (e.g. 'update()' has
    been called after any editing of the coordinates) */
inline const AABox& CoordGroupPvt::aaBox() const
{
    return aabox;
}

/** Return a const-pointer to the array of coordinates */
inline const Vector* CoordGroupPvt::constData() const
{
    return coords.get();
}

/** Return a const-pointer to the array of coordinates */
inline const Vector* CoordGroupPvt::data() const
{
    return coords.get();
}

/** Return a modifiable pointer to the array of coordinates -
    *do not delete this pointer* */
inline Vector* CoordGroupPvt::data()
{
    return coords.get();
}

/** Translate the coordinates by 'delta' */
inline void CoordGroupPvt::translate(const Vector &delta)
{
    if (not delta.isZero())
    {
        for (int i=0; i<sz; ++i)
            coords[i] += delta;

        //translate the AABox
        aabox.translate(delta);
    }
}

/** Translate the 'ith' atom by 'delta' */
inline void CoordGroupPvt::translate(int i, const Vector &delta)
{
    checkIndex(i);
    coords[i] += delta;
    needsupdate = true;
}

/** Rotate (and possibly shear and scale) the coordinates about the point 'point'
    by the matrix 'rotmat' */
inline void CoordGroupPvt::rotate(const Matrix &rotmat, const Vector &point)
{
    for (int i=0; i<sz; ++i)
    {
        coords[i] = SireMaths::rotate( coords[i], rotmat, point );
    }
    needsupdate = true;
}

/** Rotate (and possibly shear and scale) the coordinates of the 'ith' point
    using the matrix 'rotmat' about the point 'point'

    \throw SireError::invalid_index
*/
inline void CoordGroupPvt::rotate(int i, const Matrix &rotmat, const Vector &point)
{
    checkIndex(i);
    coords[i] = SireMaths::rotate( coords[i], rotmat, point );
    needsupdate = true;
}

/** Rotate the coordinates about 'point' by the Quaternion 'quat' */
inline void CoordGroupPvt::rotate(const Quaternion &quat, const Vector &point)
{
    this->rotate(quat.toMatrix(), point);
}

/** Rotate the coordinates of the 'ith' point using the quaternion 'quat'
    about the point 'point'

    \throw SireError::invalid_index
*/
inline void CoordGroupPvt::rotate(int i, const Quaternion &quat, const Vector &point)
{
    this->rotate(i, quat.toMatrix(), point);
}

/** Update the AABox so that it encloses the coordinates. */
inline void CoordGroupPvt::update()
{
    if (needsupdate and sz > 0)
    {
        //need to call protected function - messy, but this needs to be
        //as fast as possible, and I don't want to expose CoordGroupPvt to
        //AABox
        aabox.recalculate(coords.get(), sz);
        needsupdate = false;
    }
}

/** Assert that the number of coordinates in this group equals 'n'

    \throw SireError::incompatible_error
*/
inline void CoordGroupPvt::assertSameSize(int n) const
{
    if (sz != n)
        throw SireError::incompatible_error( QObject::tr(
                "There are not %1 coordinates in this CoordGroup - the number "
                "of coordinates equals %2")
                    .arg(n).arg(sz), CODELOC );
}

/** Assert that the number of coordinates in 'newcoords' is the same as the
    number of coordinates in this group

    \throw SireError::incompatible_error
*/
inline void CoordGroupPvt::assertSameSize(const CoordGroupBase &other) const
{
    this->assertSameSize(other.size());
}

/** Assert that the number of coordinates in 'newcoords' is the same as the
    number of coordinates in this group

    \throw SireError::incompatible_error
*/
inline void CoordGroupPvt::assertSameSize(const QVector<Vector> &other) const
{
    this->assertSameSize(other.size());
}

/** Static, shared null CoordGroup */
static QSharedDataPointer<CoordGroupPvt> shared_null( new CoordGroupPvt() );

/////////////
///////////// Implementation of CoordGroupBase
/////////////

/** Null constructor */
CoordGroupBase::CoordGroupBase() : d(shared_null)
{}

/** Construct a CoordGroup that holds 'size' coordinates */
CoordGroupBase::CoordGroupBase(int size) : d( new CoordGroupPvt(size) )
{}

/** Construct a CoordGroup that holds 'size' coordinates, all with the
    value 'value' */
CoordGroupBase::CoordGroupBase(int size, const Vector &value)
               : d( new CoordGroupPvt(size, value) )
{}

/** Create from a vector of points */
CoordGroupBase::CoordGroupBase(const QVector<Vector> &coords)
               : d( new CoordGroupPvt(coords) )
{}

/** Copy constructor - copying is fast as this class is implicitly shared */
CoordGroupBase::CoordGroupBase(const CoordGroupBase &other)
               : d( other.d )
{}

/** Destructor */
CoordGroupBase::~CoordGroupBase()
{}

/** Assignment operator - copying is fast as this class is implicitly shared */
CoordGroupBase& CoordGroupBase::operator=(const CoordGroupBase &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool CoordGroupBase::operator==(const CoordGroupBase &other) const
{
    return d == other.d or
           *d == *(other.d);
}

/** Comparison operator */
bool CoordGroupBase::operator!=(const CoordGroupBase &other) const
{
    return d != other.d and
           *d != *(other.d);
}

/** Return the 'ith' coordinate in the group - this will throw an exception
    if 'i' refers to an invalid index

    \throw SireError::invalid_index
*/
const Vector& CoordGroupBase::at(int i) const
{
    return d->at(i);
}

/** Return the 'ith' coordinate in the group - this will throw an exception
    if 'i' refers to an invalid index

    \throw SireError::invalid_index
*/
const Vector& CoordGroupBase::operator[](int i) const
{
    return d->operator[](i);
}

/** Return whether or not this is an empty CoordGroup */
bool CoordGroupBase::isEmpty() const
{
    return d->isEmpty();
}

/** Return the number of coordinates in this group */
int CoordGroupBase::count() const
{
    return d->count();
}

/** Return the number of coordinates in this group */
int CoordGroupBase::size() const
{
    return d->size();
}

/** Return the AABox which is guaranteed to always exactly enclose
    the coordinates in this group */
const AABox& CoordGroupBase::aaBox() const
{
    return d->aaBox();
}

/** Return a const-pointer to the array holding the coordinates in
    this group. */
const Vector* CoordGroupBase::constData() const
{
    return d->constData();
}

/////////////
///////////// Implementation of CoordGroup
/////////////

static const RegisterMetaType<CoordGroup> r_cgroup("SireVol::CoordGroup");

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, const CoordGroup &cgroup)
{
    writeHeader(ds, r_cgroup, 1);

    SharedDataStream(ds) << cgroup.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, CoordGroup &cgroup)
{
    VersionID v = readHeader(ds, r_cgroup);

    if (v == 1)
    {
        SharedDataStream(ds) >> cgroup.d;
    }
    else
        throw version_error(v, "1", r_cgroup, CODELOC);

    return ds;
}

/** Null constructor */
CoordGroup::CoordGroup() : CoordGroupBase()
{}

/** Construct a CoordGroup that holds 'size' coordinates */
CoordGroup::CoordGroup(int size) : CoordGroupBase(size)
{}

/** Construct a CoordGroup that holds 'size' coordinates, all with the
    value 'value' */
CoordGroup::CoordGroup(int size, const Vector &value)
           : CoordGroupBase(size, value)
{}

/** Construct a CoordGroup from a vector of points */
CoordGroup::CoordGroup(const QVector<Vector> &coords)
           : CoordGroupBase(coords)
{}

/** Copy constructor - copying is fast as this class is implicitly shared */
CoordGroup::CoordGroup(const CoordGroupBase &other) : CoordGroupBase(other)
{}

/** Destructor */
CoordGroup::~CoordGroup()
{}

/** Assignment operator - copying is fast as this class is implicitly shared */
CoordGroup& CoordGroup::operator=(const CoordGroup &other)
{
    CoordGroupBase::operator=(other);
    return *this;
}

/** Return an editor that can edit a copy of the coordinates in this group */
CoordGroupEditor CoordGroup::edit()
{
    return CoordGroupEditor(*this);
}

/////////////
///////////// Implementation of CoordGroupEditor
/////////////

/** Null constructor */
CoordGroupEditor::CoordGroupEditor() : CoordGroupBase()
{}

/** Copy constructor */
CoordGroupEditor::CoordGroupEditor(const CoordGroupBase &other)
                 : CoordGroupBase(other)
{}

/** Destructor */
CoordGroupEditor::~CoordGroupEditor()
{}

/** Assignment operator */
CoordGroupEditor& CoordGroupEditor::operator=(const CoordGroupBase &other)
{
    CoordGroupBase::operator=(other);
    return *this;
}

/** Return a modifiable reference to the 'ith' coordinate in the group
    - this will throw an exception if 'i' refers to an invalid index

    \throw SireError::invalid_index
*/
Vector& CoordGroupEditor::operator[](int i)
{
    return d->operator[](i);
}

/** Return a modifiable pointer to the array of coordinates -
    *do not delete this pointer* */
Vector* CoordGroupEditor::data()
{
    return d->data();
}

/** Translate this CoordGroup by 'delta' */
void CoordGroupEditor::translate(const Vector &delta)
{
    d->translate(delta);
}

/** Translate the 'ith' point in the group by 'delta'

    \throw SireError::invalid_index
*/
void CoordGroupEditor::translate(int i, const Vector &delta)
{
    d->translate(i, delta);
}

/** Rotate this group by the Quaternion 'quat' about the point 'point' */
void CoordGroupEditor::rotate(const Quaternion &quat, const Vector &point)
{
    d->rotate(quat, point);
}

/** Rotate the 'ith' point in the group using the quaternion 'quat' about the
    point 'point'

    \throw SireError::index
*/
void CoordGroupEditor::rotate(int i, const Quaternion &quat, const Vector &point)
{
    d->rotate(i, quat, point);
}

/** Rotate (and perhaps scale and shear) this group by the matrix 'rotmat'
    about the point 'point' */
void CoordGroupEditor::rotate(const Matrix &rotmat, const Vector &point)
{
    d->rotate(rotmat, point);
}

/** Rotate the 'ith' point in the group using the matrix 'rotmat' about the
    point 'point'

    \throw SireError::index
*/
void CoordGroupEditor::rotate(int i, const Matrix &rotmat, const Vector &point)
{
    d->rotate(i, rotmat, point);
}

/** Set the coordinates of the CoordGroup to 'newcoords' - this must
    have the same number of points as this CoordGroup or an exception
    will be thrown

    \throw SireError::incompatible_error
*/
void CoordGroupEditor::setCoordinates(const QVector<Vector> &newcoords)
{
    d->assertSameSize(newcoords);
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the CoordGroup to 'newcoords' - this
    must have the same number of points as this CoordGroup or an
    exception will be thrown

    \throw SireError::incompatible_error
*/
void CoordGroupEditor::setCoordinates(const CoordGroupBase &newcoords)
{
    d->assertSameSize(newcoords);
    *this = newcoords;
}

/** Const function used to check whether or not the AABox is out of
    sync with the coordinates (and thus needs to be updated) */
bool CoordGroupEditor::needsUpdate() const
{
    return d->needsUpdate();
}

/** Return a CoordGroup which is a copy of this group. This will update the
    AABox before making the copy, thus ensuring that the AABox of the returned
    CoordGroup is consistent with its coordinates. */
CoordGroup CoordGroupEditor::commit()
{
    if (this->needsUpdate())
    {
        //update the AABox
        d->update();
    }

    //return a copy of this CoordGroup
    return CoordGroup(*this);
}
