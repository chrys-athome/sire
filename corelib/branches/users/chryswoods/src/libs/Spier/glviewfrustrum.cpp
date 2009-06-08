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

#include "glviewfrustrum.h"
#include "camera.h"
#include "glmatrix.h"

#include "SireMaths/sphere.h"

#include "SireUnits/units.h"

using namespace Spier;
using namespace SireMaths;
using namespace SireUnits;

/** Null constructor */
GLViewFrustrum::GLViewFrustrum()
{}

/** Copy constructor */
GLViewFrustrum::GLViewFrustrum(const GLViewFrustrum &other)
               : cam(other.cam)
{
    for (int i=0; i<6; ++i)
    {
        fplanes[i] = other.fplanes[i];
    }
}

/** Destructor */
GLViewFrustrum::~GLViewFrustrum()
{}

/** Copy assignment operator */
GLViewFrustrum& GLViewFrustrum::operator=(const GLViewFrustrum &other)
{
    if (this != &other)
    {
        cam = other.cam;
        
        for (int i=0; i<6; ++i)
        {
            fplanes[i] = other.fplanes[i];
        }
    }
    
    return *this;
}

/** Comparison operator */
bool GLViewFrustrum::operator==(const GLViewFrustrum &other) const
{
    return cam == other.cam;
}

/** Comparison operator */
bool GLViewFrustrum::operator!=(const GLViewFrustrum &other) const
{
    return cam != other.cam;
}

/** Return the camera that creates this view */
const Camera& GLViewFrustrum::camera() const
{
    return cam;
}

/** Return whether the view frustrum of this camera contains the point 'point' */
bool GLViewFrustrum::contains(const Vector &point) const
{
    return fplanes[_left].distance(point) >= 0.0 and 
           fplanes[_right].distance(point) >= 0.0 and 
           fplanes[_top].distance(point) >= 0.0 and 
           fplanes[_bottom].distance(point) >= 0.0 and 
           fplanes[_near].distance(point) >= 0.0 and 
           fplanes[_far].distance(point) >= 0.0;
}

/** Return whether the view frustrum (even partially) contains the sphere 'sphere' */
bool GLViewFrustrum::contains(const Sphere &sphere) const
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

/** Return the (signed) distance from the camera to the specified point. A positive
    distance is returned if the point is in front of the camera, while a negative
    distance is returned if the point is behind the camera */
double GLViewFrustrum::distance(const Vector &point) const
{
    return fplanes[_near].distance(point);
}

/** Perform the openGL transformation commands necessary to 
    look through the camera - this also gets the frustrum planes 
    of the resulting view volume (in world coordinates).

    This returns whether or not it has actually changed anything.
*/
bool GLViewFrustrum::changeView(const Camera &new_camera, const QSize &new_size)
{
    const Camera &old_camera = cam.read();

    /*if ( &(new_camera) == &(old_camera) or old_camera.equals(new_camera) )
    {
        //same camera, so nothing has changed
        return false;
    }*/
    
    bool changed = false;
    
    /*if ( viewport_size != new_size or
         old_camera.viewAngle() != new_camera.viewAngle() or
         old_camera.depthCue() != new_camera.depthCue() or
         old_camera.range() != new_camera.range() )*/
    {
        GLfloat viewratio = (GLfloat)(new_size.width()) / 
                            (GLfloat)(new_size.height());

        glViewport( 0,0, new_size.width(), new_size.height() );

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective( new_camera.viewAngle().to(degrees),
                        viewratio, 0.5, new_camera.range() );
                        
        glMatrixMode(GL_MODELVIEW);
        
        if (new_camera.depthCue())
            glFogf( GL_FOG_END, new_camera.range()*10.0 );
        
        changed = true;
    }
    
    //if (old_camera.differentView(new_camera))
    {
        //look through the camera
        const Vector &location = new_camera.location();
        Vector lookat = location + new_camera.viewVector();
        const Vector &upvec = new_camera.upVector();

        glLoadIdentity();
        gluLookAt(location.x(),location.y(),location.z(),
                  lookat.x(), lookat.y(), lookat.z(),
                  upvec.x(), upvec.y(), upvec.z());
        
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

        cam = new_camera;
        viewport_size = new_size;
        
        return true;
    }
    else
        return false;
}
