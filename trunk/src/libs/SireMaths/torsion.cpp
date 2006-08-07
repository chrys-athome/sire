
#include "torsion.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMaths;

static RegisterMetaType<Torsion> r_torsion("SireMaths::Torsion");

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Torsion &torsion)
{
    writeHeader(ds, r_torsion, 1) << torsion.points[0] << torsion.points[1]
                                  << torsion.points[2] << torsion.points[3];
       
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Torsion &torsion)
{
    VersionID v = readHeader(ds, r_torsion);
        
    if (v == 1)
    {
        ds >> torsion.points[0] >> torsion.points[1]
           >> torsion.points[2] >> torsion.points[3];
    }
    else
        throw version_error(v, "1", r_torsion, CODELOC);
       
    return ds;
}

/** Construct a zero torsion */
Torsion::Torsion()
{}

/** Construct a torsion from the points 0 to 4 */
Torsion::Torsion(const Vector &point0, const Vector &point1,
                 const Vector &point2, const Vector &point3)
{
    points[0] = point0;
    points[1] = point1;
    points[2] = point2;
    points[3] = point3;
}

/** Destructor */
Torsion::~Torsion()
{}

/** Return a string representation of this torsion */
QString Torsion::toString() const
{
    return QObject::tr("Torsion: Angle %1, length03 = %2")
                  .arg(angle().toString()).arg(vector03().length());
}
