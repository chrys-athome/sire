/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include <QGLWidget>

#include "camera.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"

using namespace Spier;
using namespace SireMaths;
using namespace SireBase;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

/////////
///////// Implementation of Camera
/////////

static const RegisterMetaType<Camera> r_camera( MAGIC_ONLY, "Spier::Camera" );

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const Camera &camera)
{
    writeHeader(ds, r_camera, 1);
    
    double ang = camera.viewang.to(radians);
    
    ds << camera.loc << camera.viewvec << camera.upvec
       << camera.sidevec << ang << camera.rnge;
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, Camera &camera)
{
    VersionID v = readHeader(ds, r_camera);
    
    if (v == 1)
    {
        double ang;
    
        ds >> camera.loc >> camera.viewvec >> camera.upvec
           >> camera.sidevec >> ang >> camera.rnge;
           
        camera.viewang = ang*radians;
    }
    else
        throw version_error( v, "1", r_camera, CODELOC );
        
    return ds;
}

/** Construct the camera */
Camera::Camera() : Property(),
                   loc(0.0,0.0,0.0), viewvec(0.0,0.0,-1.0), upvec(0.0,1.0,0.0), 
                   sidevec(1.0,0.0,0.0), viewang( 45*degrees ), rnge(500.0)
{}

/** Copy constructor */
Camera::Camera(const Camera &other)
       : Property(other),
         loc(other.loc), viewvec(other.viewvec),
         upvec(other.upvec), sidevec(other.sidevec),
         viewang(other.viewang), rnge(other.rnge)
{}

/** Destructor */
Camera::~Camera()
{}

/** Copy assignment operator */
Camera& Camera::operator=(const Camera &other)
{
    if (this != &other)
    {
        loc = other.loc;
        viewvec = other.viewvec;
        upvec = other.upvec;
        sidevec = other.sidevec;
        viewang = other.viewang;
        rnge = other.rnge;
    }

    return *this;
}

/** Comparison operator */
bool Camera::operator==(const Camera &other) const
{
    return this == &other or
           ( loc == other.loc and viewvec == other.viewvec and 
             upvec == other.upvec and  sidevec == other.sidevec and 
             viewang == other.viewang and rnge == other.rnge );
}

/** Comparison operator */
bool Camera::operator!=(const Camera &other) const
{
    return not Camera::operator==(other);
}

/** Set the viewing angle of the camera */
void Camera::setViewAngle(const Angle &angle)
{
    viewang = angle;
}

/** Set the range of the camera */
void Camera::setViewRange(float range)
{
    rnge = range;
}

/** Set the camera view location to 'point' */
void Camera::setViewLocation(const Vector &point)
{
    loc = point;
}

/** Translate the view by 'delta' */
void Camera::translateView(const Vector &delta)
{
    loc += delta;
}

/** Rotate the view using the quaternion 'q' */
void Camera::rotateView(const Quaternion &q)
{
    viewvec = q.rotate(viewvec);
    upvec = q.rotate(viewvec);
    sidevec = q.rotate(viewvec);
}

/** Reset the camera */
void Camera::resetView()
{
    viewvec.set(0.0, 0.0, -1.0);
    upvec.set(0.0, 1.0, 0.0);
    sidevec.set(1.0, 0.0, 0.0);
    viewang = 45.0 * SireUnits::degrees;
    rnge = 500.0;
}

/** Return the location of the camera */
const Vector& Camera::location() const
{
    return loc;
}

/** Return the vector along which the camera is looking */
const Vector& Camera::viewVector() const
{
    return viewvec;
}
 
/** Return the vector pointing up from the camera */
const Vector& Camera::upVector() const
{
    return upvec;
}
    
/** Return the vector pointing to the right of the camera */
const Vector& Camera::sideVector() const
{
    return sidevec;
}

/** Return the viewing angle of the camera */
const Angle& Camera::viewAngle() const
{
    return viewang;
}

/** Return the viewing range of the camera */
float Camera::range() const
{
    return rnge;
}

/** Whether or not to depth cue */
bool Camera::depthCue() const
{
    return true;
}

/** Whether or not the view from this camera is different to 'other' */
bool Camera::differentView(const Camera &other) const
{
    return ( this != &other ) and
           ( loc != other.loc and viewvec != other.viewvec and
             upvec != other.upvec );
}

Q_GLOBAL_STATIC( NullCamera, nullCamera )

/** Return the global default camera */
const NullCamera& Camera::null()
{
    return *(nullCamera());
}

/////////
///////// Implementation of NullCamera
/////////

static const RegisterMetaType<NullCamera> r_nullcam;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const NullCamera &nullcam)
{
    writeHeader(ds, r_nullcam, 1);
    
    ds << static_cast<const Camera&>(nullcam);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, NullCamera &nullcam)
{
    VersionID v = readHeader(ds, r_nullcam);
    
    if (v == 1)
    {
        ds >> static_cast<Camera&>(nullcam);
    }
    else
        throw version_error( v, "1", r_nullcam, CODELOC );
        
    return ds;
}

/** Constructor */
NullCamera::NullCamera() : ConcreteProperty<NullCamera,Camera>()
{}

/** Copy constructor */
NullCamera::NullCamera(const NullCamera &other)
           : ConcreteProperty<NullCamera,Camera>(other)
{}

/** Destructor */
NullCamera::~NullCamera()
{}

/** Copy assignment operator */
NullCamera& NullCamera::operator=(const NullCamera &other)
{
    Camera::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullCamera::operator==(const NullCamera&) const
{
    return true;
}

/** Comparison operator */
bool NullCamera::operator!=(const NullCamera&) const
{
    return false;
}

const char* NullCamera::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullCamera>() );
}

/** Reset this camera */
void NullCamera::reset()
{}

/** NullCamera does not move */
void NullCamera::spin(const Angle&)
{}

/** NullCamera does not move */
void NullCamera::rotate(const Angle&, const Vector&)
{}

/** NullCamera does not move */
void NullCamera::zoom(double)
{}

/** NullCamera does not move */
void NullCamera::translate(double, double)
{}

/** NullCamera does not move */
void NullCamera::lookAt(const Vector&)
{}

/////////
///////// Implementation of OrbitCamera
/////////

static const RegisterMetaType<OrbitCamera> r_orbcam;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const OrbitCamera &orbcam)
{
    writeHeader(ds, r_orbcam, 1);
    
    ds << orbcam.lookat << orbcam.offsetx << orbcam.offsety
       << orbcam.zoomdistance << static_cast<const Camera&>(orbcam);
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, OrbitCamera &orbcam)
{
    VersionID v = readHeader(ds, r_orbcam);
    
    if (v == 1)
    {
        ds >> orbcam.lookat >> orbcam.offsetx >> orbcam.offsety
           >> orbcam.zoomdistance >> static_cast<Camera&>(orbcam);
    }
    else
        throw version_error(v, "1", r_orbcam, CODELOC);
        
    return ds;
}

/** Constructor */
OrbitCamera::OrbitCamera() 
            : ConcreteProperty<OrbitCamera,Camera>(),
              lookat(0,0,0),
              offsetx(0), offsety(0), zoomdistance(20)
{
    OrbitCamera::calculateViewVectors();
}

/** Construct an orbit camera looking at 'lookat' */
OrbitCamera::OrbitCamera(const Vector &p)
            : ConcreteProperty<OrbitCamera,Camera>(),
              lookat(p), offsetx(0), offsety(0), zoomdistance(20)
{
    OrbitCamera::calculateViewVectors();
}

/** Copy constructor */
OrbitCamera::OrbitCamera(const OrbitCamera &other)
            : ConcreteProperty<OrbitCamera,Camera>(other),
              lookat(other.lookat), offsetx(other.offsetx),
              offsety(other.offsety), zoomdistance(other.zoomdistance)
{}

/** Destructor */
OrbitCamera::~OrbitCamera()
{}

/** Copy assignment operator */
OrbitCamera& OrbitCamera::operator=(const OrbitCamera &other)
{
    if (this != &other)
    {
        Camera::operator=(other);
        
        lookat = other.lookat;
        offsetx = other.offsetx;
        offsety = other.offsety;
        zoomdistance = other.zoomdistance;
    }
    
    return *this;
}

/** Comparison operator */
bool OrbitCamera::operator==(const OrbitCamera &other) const
{
    return Camera::operator==(other) and 
           lookat == other.lookat and
           offsetx == other.offsetx and
           offsety == other.offsety and
           zoomdistance == other.zoomdistance;
}

/** Comparison operator */
bool OrbitCamera::operator!=(const OrbitCamera &other) const
{
    return not OrbitCamera::operator==(other);
}

const char* OrbitCamera::typeName()
{
    return QMetaType::typeName( qMetaTypeId<OrbitCamera>() );
}

/** Calculate all of the vectors of the camera */
void OrbitCamera::calculateViewVectors()
{
    //viewing is based on the plane that is perpendicular to 'viewvec'
    //but that also contains 'lookat'

    //the camera position is zoomdistance*viewvec away from this point
    Camera::setViewLocation(
            lookat - (zoomdistance * Camera::viewVector()) - 
                     (offsetx * Camera::sideVector()) - 
                     (offsety * Camera::upVector()) );
}

/** Set the new look-at point for the camera */
void OrbitCamera::lookAt(const Vector &point)
{
    lookat = point;
    offsetx = 0.0;
    offsety = 0.0;
    calculateViewVectors();
}

/** Zoom in on the object */
void OrbitCamera::zoom(double delta)
{
    //decrease 'zoomdistance' by delta
    zoomdistance -= delta;
    
    if (zoomdistance < 1.0)
        zoomdistance = 1.0;

    calculateViewVectors();
}

/** Spin around, while still looking at the object */
void OrbitCamera::spin(const Angle &delta)
{
    Quaternion q(delta, Camera::viewVector());
    
    Camera::rotateView(q);

    calculateViewVectors();
}

/** Rotate around, while still looking at the object */
void OrbitCamera::rotate(const Angle &delta, const Vector &axis)
{
    Quaternion q(delta, axis);
    
    Camera::rotateView(q);
    
    calculateViewVectors();
}

/** Translate the view by dx and dy, but don't change the center  */
void OrbitCamera::translate(double dx, double dy)
{
    offsetx += dx;
    offsety += dy;
    
    calculateViewVectors();
}

/** Reset the camera to be looking at the origin */
void OrbitCamera::reset()
{
    Camera::resetView();

    lookat.set(0.0, 0.0, 0.0);

    offsetx = 0.0;
    offsety = 0.0;

    zoomdistance = 10.0;
    
    this->calculateViewVectors();
}
