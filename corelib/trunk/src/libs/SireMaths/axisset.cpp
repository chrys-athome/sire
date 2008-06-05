
#include <QDataStream>

#include "SireStream/datastream.h"

#include "axisset.h"

using namespace SireMaths;
using namespace SireStream;

static const RegisterMetaType<AxisSet> r_axisset("SireMaths::AxisSet");

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const AxisSet &ax)
{
    writeHeader(ds, r_axisset, 1) << ax.mat << ax.invmat << ax.orgn;
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, AxisSet &ax)
{
    VersionID v = readHeader(ds, r_axisset);
    
    if (v == 1)
    {
        ds >> ax.mat >> ax.invmat >> ax.orgn;
    }
    else
        throw version_error(v, "1", r_axisset, CODELOC);
    
    return ds;
}

/** Construct an empty AxisSet. This represents the standard cartesian axes, centered
    on the origin */
AxisSet::AxisSet() : mat(), invmat(), orgn()
{}

/** Construct an AxisSet using matrix 'matrx', and origin 'orign' */
AxisSet::AxisSet(const Matrix &matrx, Vector vec) 
        : mat(matrx), invmat(mat.inverse()), orgn(vec)
{}

/** Copy constructor */
AxisSet::AxisSet(const AxisSet &other)
        : mat(other.mat), invmat(other.invmat), orgn(other.orgn)
{}

/** Destructor */
AxisSet::~AxisSet()
{}

/** Convert a vector from the cartesian frame to this coordinate frame */
Vector AxisSet::fromIdentity(const Vector &vec) const
{
    return (mat * vec) + orgn;
}

/** Convert a vector to the cartesian frame from this coordinate frame */
Vector AxisSet::toIdentity(const Vector &vec) const
{
    return invmat * (vec - orgn);
}

/** Convert a vector from the frame 'frame' to this coordinate frame */
Vector AxisSet::fromFrame(const AxisSet &frame, const Vector &vec) const
{
    return ( mat *  (frame.invmat * (vec - frame.orgn)) ) + orgn;
}

/** Convert a vector to the frame 'frame' from this coordinate frame */
Vector AxisSet::toFrame(const AxisSet &frame, const Vector &vec) const
{
    return ( frame.mat * (invmat * (vec - orgn)) ) + frame.orgn;
}

/** Return a string representation of the AxisSet */
QString AxisSet::toString() const
{
    return QObject::tr("AxisSet, %1, %2").arg(orgn.toString(),mat.toString());
}
