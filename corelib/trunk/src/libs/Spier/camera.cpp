
#include "ThirdPartyGUI/glew.h"
#include <QGLWidget>

#include "camera.h"
#include "glmatrix.h"

#include "SireMaths/plane.h"

#include "SireUnits/units.h"

using namespace Spier;
using namespace SireMaths;

/** Construct the camera */
Camera::Camera(QObject *parent) 
       : QObject(parent), lookat(0.0,0.0,0.0),
         viewvec(0.0,0.0,-1.0), upvec(0.0,1.0,0.0), sidevec(1.0,0.0,0.0), 
         viewang( Angle::degrees(45.0) ),
         rnge(500.0), offsetx(0.0), offsety(0.0), zoomdistance(10.0),
         viewchanged(true), sizechanged(true)
{
    this->calculateViewVectors();
}

/** Destructor */
Camera::~Camera()
{}

/** Reset the camera to be looking at the origin */
void Camera::reset()
{
    lookat.set(0.0, 0.0, 0.0);
    viewvec.set(0.0, 0.0, -1.0);
    upvec.set(0.0, 1.0, 0.0);
    sidevec.set(1.0, 0.0, 0.0);
    viewang = 45.0 * SireUnits::degrees;
    rnge = 500.0;
    offsetx = 0.0;
    offsety = 0.0;
    zoomdistance = 10.0;
    sizechanged = true;
    
    this->calculateViewVectors();
}

/** Calculate all of the vectors of the camera */
void Camera::calculateViewVectors()
{
    //first, ensure that viewvec, upvec and sidevec form a 
    //right-angled set of vectors - this should correct for any
    //rounding error...

    //viewing is based on the plane that is perpendicular to 'viewvec'
    //but that also contains 'lookat'

    //the camera position is zoomdistance*viewvec away from this point
    loc = lookat - (zoomdistance * viewvec) - (offsetx*sidevec) - (offsety*upvec);

    viewchanged = true;  
}

/** Set the size of the canvas */
void Camera::setSize(const QSize &size)
{
    if (size != sz)
    {
        sz = size;
        sizechanged = true;
    }
}

/** Set the new look-at point for the camera */
void Camera::lookAt(const Vector &point)
{
    lookat = point;
    offsetx = 0.0;
    offsety = 0.0;
    calculateViewVectors();
}

/** Zoom in on the object */
void Camera::zoom(double delta)
{
    //decrease 'zoomdistance' by delta
    zoomdistance -= delta;
    
    if (zoomdistance < 1.0)
        zoomdistance = 1.0;

    calculateViewVectors();
}

/** Spin around, while still looking at the object */
void Camera::spin(const Angle &delta)
{
    Quaternion q(delta, viewvec);
    
    upvec = q.rotate(upvec);
    sidevec = q.rotate(sidevec);

    calculateViewVectors();
}

/** Rotate around, while still looking at the object */
void Camera::rotate(const Angle &delta, const Vector &axis)
{
    //rotate viewvec and upvec around this vector
    Quaternion q(delta, axis);
    
    viewvec = q.rotate(viewvec);
    upvec = q.rotate(upvec);
    sidevec = q.rotate(sidevec);
    
    calculateViewVectors();
}

/** Translate the view by dx and dy, but don't change the center  */
void Camera::translate(double dx, double dy)
{
    offsetx += dx;
    offsety += dy;
    
    calculateViewVectors();
}

/** Perform the openGL transformation commands necessary to 
    look through the camera - this also gets the frustrum planes 
    of the resulting view volume (in world coordinates).

    This returns whether or not it has actually changed anything.
*/
bool Camera::look(bool force)
{
    bool changed = false;

    if (force)
    {
        viewchanged = true;
        sizechanged = true;
    }
    
    if (sizechanged)
    {
        GLfloat viewratio = (GLfloat)sz.width() / (GLfloat)sz.height();

        glViewport(0,0,sz.width(),sz.height());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(viewAngle().toDegrees(),viewratio,0.5,range());
        glMatrixMode(GL_MODELVIEW);
        glFogf(GL_FOG_END,range()*10.0);
        changed = true;
        sizechanged = false;
    }
    
    if (viewchanged)
    {
        //look through the camera
        glLoadIdentity();
        gluLookAt(loc.x(),loc.y(),loc.z(),
                  loc.x()+viewvec.x(), loc.y()+viewvec.y(), loc.z()+viewvec.z(),
                  upvec.x(), upvec.y(), upvec.z());
        viewchanged = false;
        
        changed = true;
    }

    if (changed)
    {
        //we need to recalculate the planes of the viewing frustrum...
        GLMatrix mat = GLMatrix::getModelProj();

        fplanes[_left] = mat.left();
        fplanes[_right] = mat.right();
        fplanes[_bottom] = mat.bottom();
        fplanes[_top] = mat.top();
        fplanes[_near] = mat.near();
        fplanes[_far] = mat.far();
        
        return true;
    }
    else
        return false;
}

/** Return whether the view frustrum of this camera contains the point 'point' */
bool Camera::contains(const Vector &point) const
{
    return fplanes[_left].distance(point) >= 0.0 and 
           fplanes[_right].distance(point) >= 0.0 and 
           fplanes[_top].distance(point) >= 0.0 and 
           fplanes[_bottom].distance(point) >= 0.0 and 
           fplanes[_near].distance(point) >= 0.0 and 
           fplanes[_far].distance(point) >= 0.0;
}

/** Return whether the view frustrum (even partially) contains the sphere 'sphere' */
bool Camera::contains(const Sphere &sphere) const
{
    double rad = -sphere.radius();
    const Vector &point = sphere.center();

    return fplanes[_left].distance(point) >= rad and 
           fplanes[_right].distance(point) >= rad and
           fplanes[_top].distance(point) >= rad and
           fplanes[_bottom].distance(point) >= rad and
           fplanes[_near].distance(point) >= rad and
           fplanes[_far].distance(point) >= rad;
}
