#ifndef SPIER_CAMERA_H
#define SPIER_CAMERA_H

#include <QSize>

#include "SireMaths/angle.h"
#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/plane.h"
#include "SireMaths/sphere.h"

#include <QObject>

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMaths::Angle;
using SireMaths::Vector;
using SireMaths::Quaternion;
using SireMaths::Plane;
using SireMaths::Sphere;

/**
The Camera is the view onto the world!
 
@author Christopher Woods
*/
class SPIER_EXPORT Camera : public QObject
{

Q_OBJECT

public:
    Camera(QObject *parent=0);
    ~Camera();

    const Vector& position() const;

    const Vector& viewVec() const;
    const Vector& sideVec() const;
    const Vector& upVec() const;
    
    const Angle& viewAngle() const;
    const float& range() const;

    bool look(bool force=false);

    double distance(const Vector &point) const;

    bool contains(const Vector &point) const;
    bool contains(const Sphere &sphere) const;

    bool viewChanged() const;
    bool sizeChanged() const;
    bool needRepaint() const;

public slots:
    
    void reset();
    void zoom(double delta);
    void spin(const Angle &delta);
    void rotate(const Angle &delta, const Vector &axis);    
    void translate(double dx, double dy);
    
    void lookAt(const Vector &lookat);
    
    void setSize(const QSize &size);

protected:

    void calculateViewVectors();

    /** The point at which the camera is located */
    Vector loc;
    
    /** The point in space that this camera is looking at
        (this is the point about which the camera view rotates) */
    Vector lookat;
    
    /** The normalised vector pointing straight ahead */
    Vector viewvec;
    
    /** The normalised vector pointing straight up */
    Vector upvec;
    
    /** The normalised vector pointing right */
    Vector sidevec;

    /** The viewing angle of the camera */
    Angle viewang;

    /** The range of the camera */
    float rnge;
    
    /** The x and y offsets of the camera (translation delta) */
    double offsetx,offsety;
    
    /** The signed distance from the plane containing the 
        point 'lookat' and perpendicular to 'viewvec', to the 
        current camera position */
    double zoomdistance;
    
    /** The size of the canvas */
    QSize sz;
    
    /** Enum giving the indicies of the six planes */
    enum PLANEIDX{ _left=0, _right=1, _bottom=2, _top=3, _near=4, _far=5 };
    
    /** The six planes of the view frustrum (in world-space coordinates) */
    Plane fplanes[6];

    /** Whether or not the view has changed */
    bool viewchanged;
    
    /** Whether or not the size of the canvas has changed */
    bool sizechanged;
};

inline const Vector& Camera::position() const
{
    return loc;
}

inline const Vector& Camera::viewVec() const
{
    return viewvec;
}
 
inline const Vector& Camera::upVec() const
{
    return upvec;
}
    
inline const Vector& Camera::sideVec() const
{
    return sidevec;
}
    
inline const Angle& Camera::viewAngle() const
{
    return viewang;
}

inline const float& Camera::range() const
{
    return rnge;
}


/** Return the (signed) distance from the camera to the specified point. A positive
    distance is returned if the point is in front of the camera, while a negative
    distance is returned if the point is behind he camera */
inline double Camera::distance(const Vector &point) const
{
    return fplanes[_near].distance(point);
}

/** Return whether or not the camera view has changed since the last call
    to 'Camera::look' */
inline bool Camera::viewChanged() const
{
    return viewchanged;
}

/** Return whether or not the size of the canvas has changed since the last
    call to 'Camera::look' */
inline bool Camera::sizeChanged() const
{
    return sizechanged;
}

/** Return whether a change in the camera state necessitates the canvas to be 
    repainted */
inline bool Camera::needRepaint() const
{
    return viewchanged or sizechanged;
}

}

SIRE_END_HEADER

#endif
