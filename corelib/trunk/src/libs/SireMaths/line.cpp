
#include <QDataStream>

#include "SireStream/datastream.h"

#include "line.h"

using namespace SireMaths;
using namespace SireStream;

static const RegisterMetaType<Line> r_line("SireMaths::Line");

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const SireMaths::Line &line)
{
    writeHeader(ds, r_line, 1) << line.points[0] << line.points[1];
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, SireMaths::Line &line)
{
    VersionID v = readHeader(ds, r_line);
    
    if (v == 1)
    {
        ds >> line.points[0] >> line.points[1];
    }
    else
        throw version_error(v, "1", r_line, CODELOC);
    
    return ds;
}

/** Construct a zero line */
Line::Line()
{}

/** Construct a line from point0 to point1 */
Line::Line(const Vector &point0, const Vector &point1)
{
    points[0] = point0;
    points[1] = point1;
}

/** Destructor */
Line::~Line()
{}

/** Return a string representation of the line */
QString Line::toString() const
{
    return QObject::tr("Line from %1 to %2, length = %3")
            .arg(point(0).toString(),point(1).toString()).arg(length());
}
