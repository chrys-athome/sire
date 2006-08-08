
#include "angle.h"

#include "SireUnits/units.h"
#include "SireStream/datastream.h"

#include <QObject>
#include <QString>
#include <cmath>

using namespace SireMaths;
using namespace SireStream;
using namespace SireUnits;

static const RegisterMetaType<Angle> r_angle("SireMaths::Angle");

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Angle &ang)
{
    writeHeader(ds, r_angle, 1) << ang.rads;
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Angle &ang)
{
    VersionID v = readHeader(ds, r_angle);
        
    if (v == 1)
    {
        ds >> ang.rads;
    }
    else
        throw version_error( v, "1", r_angle, CODELOC );
    
    return ds;
}

/** Fix the angle to lie between 0 and 360 degrees (0 and 2pi radians) */
inline void Angle::fixAngle()
{
    while (rads < -two_pi)
        rads += two_pi;
    while (rads >= two_pi)
        rads -= two_pi;
}

/** Create an angle that represents 0 degrees/radians */
Angle::Angle() : rads(0.0)
{}

/** Create a Angle from an angle in radians - the angle will be fixed to
    ensure that it lies between 0 and 360 degrees (0 and 2*pi radians) */
Angle::Angle(double rad) : rads(rad)
{
    this->fixAngle();
}

/** Create an angle from the coordinates 'x' and 'y'. This angle is calculated 
    with the assumption that 0 degrees corresponds to the y-axis (12 o'clock position)
    and positive angles are clockwise, and negative angles are anti-clockwise.

    The smallest angle will be returned, e.g. -180 < ang <= 180
*/
Angle::Angle(double x, double y)
{
    if (y == 0.0)
    {
        if (x == 0.0)
            rads = 0.0;
        else if (x > 0.0)
            rads = 0.5 * pi;
        else
            rads = -0.5 * pi;
    }
    else
    {
        rads = std::atan(x/y);
        
        if (x < 0.0 and y < 0.0)
            rads -= pi;
        else if (x >= 0.0 and y <= 0.0)
            rads += pi;
    }
}

/** Destructor */
Angle::~Angle()
{}

/** Return an angle of 'deg' degrees */
Angle Angle::degrees(double deg)
{
    return Angle(deg * SireUnits::degrees);
}

/** Return the angle in degrees */
double Angle::toDegrees() const
{
    return rads / SireUnits::degrees;
}

/** Return the smallest equivelent angle to this one (e.g. 270 degs == -90 degs, 
    so return -90) */
Angle Angle::small() const
{
    if (rads > pi)
        return Angle( rads - two_pi );
    else if (rads < -pi)
        return Angle( rads + two_pi );
    else
        return *this;
}

/** Return the largest equivelent angle to this one (e.g. 270 degs == -90 degs,
    so return -270) */
Angle Angle::large() const
{
    if (rads >= pi or rads <= -pi)
        return *this;
    else if (rads >= 0.0)
        return Angle( rads - two_pi );
    else
        return Angle( rads + two_pi );
}

/** Return the positive equivelent of the angle (0 <= ang < 360) */
Angle Angle::positive() const
{
    if (rads >= 0.0)
        return *this;
    else
        return Angle( rads + two_pi );
}

/** Return the negative equivelent of the angle (-360 < ang <= 0) */
Angle Angle::negative() const
{
    if (rads <= 0.0)
        return *this;
    else 
        return Angle( rads - two_pi );
}

/** Return the smallest difference between two angles. This is 
    guaranteed to lie between -180 <= ang <= 180 degrees */
Angle Angle::smallDifference(const Angle &ang) const
{
    return (ang - *this).small();
}
   
/** Return the largest difference between two angles. This is
    guaranteed to lie between 180 <= ang <= -180 */
Angle Angle::largeDifference(const Angle &ang) const
{
    return (ang - *this).large();
}

/** Return whether 'ang' is within 'delta' of this angle */
bool Angle::isWithin(const Angle &ang, const Angle &delta) const
{
    //get the difference between the angles
    Angle diff = *this - ang;
    return (diff.toRadians() < delta.toRadians());
}

/** Set the angle to theta degrees */
void Angle::setDegrees(double theta)
{
    rads = theta * SireUnits::degrees;
    this->fixAngle();
}

/** Set the angle to theta radians */
void Angle::setRadians(double theta)
{
    rads = theta * radians;
    this->fixAngle();
}

/** Return a string representation of the angle */
QString Angle::toString() const
{
    double deg = toDegrees();
    
    if (deg == 1.0)
        return QObject::tr("1 degree");
    else
        return QObject::tr("%1 degrees").arg(toDegrees());
}

/** unary version of + */
Angle& Angle::operator+=(const Angle &a)
{
    rads += a.rads;
    this->fixAngle();
    return *this;
}

/** unary version of - */
Angle& Angle::operator-=(const Angle &a)
{
    rads -= a.rads;
    this->fixAngle();
    return *this;
}

/** unary version of - */
Angle& Angle::operator-=(double c)
{
    rads -= c;
    this->fixAngle();
    return *this;
}

/** unary version of * */
Angle& Angle::operator*=(double c)
{
    rads *= c;
    this->fixAngle();
    return *this;
}

/** unary version of / */
Angle& Angle::operator/=(double c)
{
    if (c != 0)
    {
        rads /= c;
    }
    return *this;
}
