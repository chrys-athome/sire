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

#ifndef SPIER_CAMERA_H
#define SPIER_CAMERA_H

#include <QSize>

#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/plane.h"
#include "SireMaths/sphere.h"

#include "SireUnits/dimensions.h"

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class Camera;
class NullCamera;
class OrbitCamera;
}

QDataStream& operator<<(QDataStream&, const Spier::Camera&);
QDataStream& operator>>(QDataStream&, Spier::Camera&);

QDataStream& operator<<(QDataStream&, const Spier::NullCamera&);
QDataStream& operator>>(QDataStream&, Spier::NullCamera&);

QDataStream& operator<<(QDataStream&, const Spier::OrbitCamera&);
QDataStream& operator>>(QDataStream&, Spier::OrbitCamera&);

namespace Spier
{

using SireMaths::Vector;
using SireMaths::Quaternion;
using SireMaths::Plane;
using SireMaths::Sphere;

/** The Camera is the view onto the world!

    This is a basic camera which may be sub-classed to provide
    more functionality.
 
    @author Christopher Woods
*/
class SPIER_EXPORT Camera : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Camera&);
friend QDataStream& ::operator>>(QDataStream&, Camera&);

public:
    Camera();
    Camera(const Camera &other);
    
    virtual ~Camera();

    static const char* typeName()
    {
        return "Spier::Camera";
    }
    
    virtual Camera* clone() const=0;

    const Vector& location() const;

    const Vector& viewVector() const;
    const Vector& sideVector() const;
    const Vector& upVector() const;
    
    const SireUnits::Dimension::Angle& viewAngle() const;
    float range() const;

    bool differentView(const Camera &other) const;

    virtual bool depthCue() const;
    
    virtual void reset()=0;

    virtual void spin(const SireUnits::Dimension::Angle &delta)=0;

    virtual void rotate(const SireUnits::Dimension::Angle &dx, 
                        const SireUnits::Dimension::Angle &dy)=0;

    virtual void zoom(double delta)=0;
    virtual void translate(double dx, double dy)=0;
    
    virtual void lookAt(const Vector &lookat)=0;

    static const NullCamera& null();

protected:
    Camera& operator=(const Camera &other);
    
    bool operator==(const Camera &other) const;
    bool operator!=(const Camera &other) const;

    void resetView();

    void setViewLocation(const Vector &point);

    void translateView(const Vector &delta);
    void rotateView(const Quaternion &q);

    void setViewAngle(const SireUnits::Dimension::Angle &viewangle);
    void setViewRange(float range);

private:
    /** The point at which the camera is located */
    Vector loc;
    
    /** The normalised vector pointing straight ahead */
    Vector viewvec;
    
    /** The normalised vector pointing straight up */
    Vector upvec;
    
    /** The normalised vector pointing right */
    Vector sidevec;

    /** The viewing angle of the camera */
    SireUnits::Dimension::Angle viewang;

    /** The range of the camera */
    float rnge;
};

/** This is the null camera that just looks straight ahead */
class SPIER_EXPORT NullCamera : public SireBase::ConcreteProperty<NullCamera,Camera>
{

friend QDataStream& ::operator<<(QDataStream&, const NullCamera&);
friend QDataStream& ::operator>>(QDataStream&, NullCamera&);

public:
    NullCamera();
    
    NullCamera(const NullCamera &other);
    
    ~NullCamera();
    
    NullCamera& operator=(const NullCamera &other);
    
    bool operator==(const NullCamera &other) const;
    bool operator!=(const NullCamera &other) const;
    
    static const char* typeName();
    
    void reset();

    void spin(const SireUnits::Dimension::Angle &delta);
    void rotate(const SireUnits::Dimension::Angle &dx, 
                const SireUnits::Dimension::Angle &dy);

    void zoom(double delta);
    void translate(double dx, double dy);
    
    void lookAt(const Vector &lookat);
};

/** This is a camera that moves by orbiting a central point.

    @author Christopher Woods
*/
class SPIER_EXPORT OrbitCamera : public SireBase::ConcreteProperty<OrbitCamera,Camera>
{

friend QDataStream& ::operator<<(QDataStream&, const OrbitCamera&);
friend QDataStream& ::operator>>(QDataStream&, OrbitCamera&);

public:
    OrbitCamera();
    
    OrbitCamera(const Vector &lookat);
    
    OrbitCamera(const OrbitCamera &other);
    
    ~OrbitCamera();
    
    OrbitCamera& operator=(const OrbitCamera &other);
    
    bool operator==(const OrbitCamera &other) const;
    bool operator!=(const OrbitCamera &other) const;
    
    static const char* typeName();
    
    void reset();

    void spin(const SireUnits::Dimension::Angle &delta);
    void rotate(const SireUnits::Dimension::Angle &dx,
                const SireUnits::Dimension::Angle &dy);

    void zoom(double delta);
    void translate(double dx, double dy);
    
    void lookAt(const Vector &lookat);

private:
    void calculateViewVectors();

    /** The point in space that this camera is looking at
        (this is the point about which the camera view rotates) */
    Vector lookat;
    
    /** The x and y offsets of the camera (translation delta) */
    double offsetx,offsety;
    
    /** The signed distance from the plane containing the 
        point 'lookat' and perpendicular to 'viewvec', to the 
        current camera position */
    double zoomdistance;
};

typedef SireBase::PropPtr<Camera> CameraPtr;

}

Q_DECLARE_METATYPE( Spier::NullCamera )
Q_DECLARE_METATYPE( Spier::OrbitCamera )

SIRE_EXPOSE_CLASS( Spier::Camera )
SIRE_EXPOSE_CLASS( Spier::NullCamera )
SIRE_EXPOSE_CLASS( Spier::OrbitCamera )

SIRE_EXPOSE_PROPERTY( Spier::CameraPtr, Spier::Camera )

SIRE_END_HEADER

#endif
