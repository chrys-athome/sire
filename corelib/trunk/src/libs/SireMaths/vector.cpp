
#include "vector.h"
#include "angle.h"
#include "quaternion.h"
#include "matrix.h"

#include "SireUnits/units.h"
#include "SireStream/datastream.h"

#include <QString>
#include <QRegExp>

#include <cmath>

using namespace SireMaths;
using namespace SireStream;

static const RegisterMetaType<Vector> r_vector("SireMaths::Vector");

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const SireMaths::Vector &vec)
{
    writeHeader(ds, r_vector, 1) << vec.x() << vec.y() << vec.z();
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, SireMaths::Vector &vec)
{
    VersionID v = readHeader(ds, r_vector);

    if (v == 1)
    {
        double x,y,z;
        ds >> x >> y >> z;
    
        vec.set(x,y,z);
    }
    else
        throw version_error(v, "1", r_vector, CODELOC);
    
    return ds;
}

Vector::Vector(double val)
{
    for (int i=0; i<3; i++)
        sc[i] = val;
}

Vector::Vector(double x, double y, double z)
{
    sc[0] = x;
    sc[1] = y;
    sc[2] = z;
}

Vector::Vector( const tuple<double,double,double> &pos )
{
    sc[0] = pos.get<0>();
    sc[1] = pos.get<1>();
    sc[2] = pos.get<2>();
}

Vector::Vector(const Vector& p)
{
    for (int i=0; i<3; i++)
        sc[i] = p.sc[i];
}

Vector::~Vector()
{}

QString Vector::toString() const
{
    return QObject::tr("( %1, %2, %3 )").arg(sc[0]).arg(sc[1]).arg(sc[2]);
}

Vector Vector::fromString(const QString &str)
{
    QRegExp vecregexp("([0-9.-]+),\\s{0,}([0-9.-]+),\\s{0,}([0-9.-]+)");

    //use a regexp to extract the contents...
    if (vecregexp.indexIn(str) != -1)
    {
        return Vector(vecregexp.cap(1).toFloat(),
                      vecregexp.cap(2).toFloat(),
                      vecregexp.cap(3).toFloat());
    
   }
   else
       return Vector(0.0,0.0,0.0);
}

Angle Vector::bearing() const
{
    Vector t(x(),y(),0.0);

    if (t.x() == 0.0 and t.y() == 0.0)
        return Angle(0.0);

    if (t.y() == 0.0)
    {
       if (t.x() > 0.0)
         return Angle(1.5707963); // 90 degrees
       else
         return Angle(4.712389); // 270 degrees
    }
    else if (t.y() > 0.0)  // range from -90 to 180 to 90
    {
        return Angle(3.1415927 - std::atan(t.x()/t.y()));
    }
    else  // range from -90 to 0 to 90
    {
        return Angle(std::atan(-t.x()/t.y()));
    }
}

Angle Vector::bearingXY(const Vector &v) const
{
    Vector px( x(), y(), 0.0);
    Vector pv(v.x(), v.y(), 0.0);
    return angle(px,pv);
}

Angle Vector::bearingXZ(const Vector &v) const
{
    Vector px( x(), 0.0, z());
    Vector pv(v.x(), 0.0, v.z());
    return angle(px,pv);
}

Angle Vector::bearingYZ(const Vector &v) const
{
    Vector px( 0.0, y(), z());
    Vector pv(0.0, v.y(), v.z());
    return angle(px,pv);
}

Angle Vector::angle(const Vector &v0, const Vector &v1)
{
    double d = dot(v0,v1);

    double lt = v0.length() * v1.length();

    if (SireMaths::isZero(lt))
        return Angle(0.0);
    else
        return Angle( std::acos(d/lt) );
}

Angle Vector::angle(const Vector &v0, const Vector &v1, const Vector &v2)
{
    return angle( v0-v1, v2-v1 );
}

Angle Vector::dihedral(const Vector &v0, const Vector &v1, const Vector &v2, const Vector &v3)
{
    //     v0        v3
    //      \       /
    //      v1----v2  
    //
    //    Dihedral angle is the plane of intersection between the planes
    //    formed by v0,v1,v2 and v1,v2,v3.
    //
    //    This is equivelent to the angle of intersection of the normals to those
    //    planes - thus we must calculate those normals!
    //
    //    norm0 = cross(vec(v1->v0), vec(v2->v1))
    //    norm1 = cross(vec(v2->v1), vec(v3->v2))

    //get the first plane normal...    
    Vector v0_1 = v0 - v1;
    Vector v2_1 = v2 - v1;
    Vector norm0 = Vector::cross(v0_1,v2_1);
    
    //now get the second plane normal...
    Vector v1_2 = v1 - v2;
    Vector v3_2 = v3 - v2;
    Vector norm1 = Vector::cross(v1_2,v3_2);
    
    //now get the angle between the normals - this is based directly on the dot product 
    //(as the normals have unit length)
    double cos_ang = Vector::dot(norm0,norm1);
    
    //ensure that cos_ang lies between -1 and 1
    cos_ang = SIRE_MIN(cos_ang,1.0);
    cos_ang = SIRE_MAX(cos_ang,-1.0);
    
    //now get the angle
    Angle ang(std::acos(cos_ang));
    
    //this angle will only lie between 0 and 180. To work out what 
    //side of 180 this angle is we construct the plane formed by v2_1 and norm0.
    //We then calculate the angle between this new plane and that defined by norm1.
    //If this angle is < 90 degrees, then the dihedral angle is > 180 degrees

    Vector norm2 = Vector::cross(norm0,v2_1);
    cos_ang = Vector::dot(norm1,norm2);
    cos_ang = SIRE_MIN(cos_ang,1.0);
    cos_ang = SIRE_MAX(cos_ang,-1.0);
    
    Angle ang2(std::acos(cos_ang));

    if (ang2.toRadians() < SireMaths::pi_over_two)
        return Angle(SireMaths::two_pi - ang.toRadians());
    else
        return ang;
}

Vector Vector::generate(double dst, const Vector &v1, const Angle &ang, const Vector &v2,
                        const Angle &dih, const Vector &v3)
{

//     v3        v0 (generating coords of v0!)
//       \      /
//       v2---v1

//    first create a set of x/y/z orthonormal vectors, with y perpendicular
//    vec(v3-v1) and vec(v2-v1), x perpendicular to vec(v2-v1) and y, and z 
//    perpendicular to x and y. Do this via cross products...
      
      Vector v31 = v3 - v1;
      Vector v21 = v2 - v1;
      
      Vector vy = Vector::cross(v31,v21);
      Vector vx = Vector::cross(v21,vy);
      Vector vz = Vector::cross(vx,vy);
      
//    now we have the x/y/z vectors, we can generate the new coordinates
//    from this basis set...
//    thus x/y/z in the basis set is given by
//    xbs = lgth * sin(ang) * cos(dih)
//    ybs = lgth * sin(ang) * sin(dih)
//    zbs = -lgth * cos(ang)

      double sinval = std::sin(ang.toRadians());
      double xbs = dst * sinval * std::cos(dih.toRadians());
      double ybs = dst * sinval * std::sin(dih.toRadians());
      double zbs = -dst * std::cos(ang.toRadians());

//    Then we map the coordinates in this basis set to our cartesian coordinates
//    via...
//    x = xbs*vx(1) + ybs*vy(1) + zbs*vz(1)
//    y = xbs*vx(2) + ybs*vy(2) + zbs*vz(2)
//    z = xbs*vx(3) + ybs*vy(3) + zbs*vz(3)
//
//    These coordinates are based at the origin - they need to be based from 
//    the coordinates of the bond atom by adding on v1.
//
//    (we combine the last two steps together for speed)

      double nx = vx.x()*xbs + vy.x()*ybs + vz.x()*zbs + v1.x();
      double ny = vx.y()*xbs + vy.y()*ybs + vz.y()*zbs + v1.y();
      double nz = vx.z()*xbs + vy.z()*ybs + vz.z()*zbs + v1.z();
      
      return Vector(nx,ny,nz);
}

Vector Vector::cross(const Vector &v0, const Vector &v1)
{
    double nx = v0.sc[1]*v1.sc[2] - v0.sc[2]*v1.sc[1];
    double ny = v0.sc[2]*v1.sc[0] - v0.sc[0]*v1.sc[2];
    double nz = v0.sc[0]*v1.sc[1] - v0.sc[1]*v1.sc[0];

    Vector vec(nx,ny,nz);
    
    if ( vec.isZero() )
    {
        //these two vectors are parallel - we need to choose just
        //one perpendicular vector 
        if (v0.isZero())
        {
            if (v1.isZero())
                //both are null vectors. Just return a unit vector along the x axis
                return Vector(1.0, 0.0, 0.0);
            else
            {
                //do this by creating a copy of v0 with two elements swapped
                if ( SireMaths::isZero(v1.x()) )
                    return Vector(v1.x(),v1.z(),v1.y()).normalise();
                else
                    return Vector(v1.y(),v1.x(),v1.z()).normalise();
            }
        }
        else
        {
            //do this by creating a copy of v0 with two elements swapped
            if ( SireMaths::isZero(v0.x()) )
                return Vector(v0.x(),v0.z(),v0.y()).normalise();
            else
                return Vector(v0.y(),v0.x(),v0.z()).normalise();
        }
    }
    else
        return Vector(nx,ny,nz).normalise();
}

double Vector::manhattanLength() const
{
    return std::abs(sc[0])+std::abs(sc[1])+std::abs(sc[2]);
}
