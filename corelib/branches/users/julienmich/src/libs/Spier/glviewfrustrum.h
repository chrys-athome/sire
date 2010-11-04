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

#ifndef SPIER_GLVIEWFRUSTRUM_H
#define SPIER_GLVIEWFRUSTRUM_H

#include "camera.h"

#include "SireMaths/plane.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Sphere;
}

namespace Spier
{

/** This class holds information about the current view,
    and can be queried to see if objects will be visible
    in this view
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLViewFrustrum
{
public:
    GLViewFrustrum();
    
    GLViewFrustrum(const GLViewFrustrum &other);
    
    ~GLViewFrustrum();
    
    GLViewFrustrum& operator=(const GLViewFrustrum &other);
    
    bool operator==(const GLViewFrustrum &other) const;
    bool operator!=(const GLViewFrustrum &other) const;
    
    const Camera& camera() const;

    bool contains(const SireMaths::Vector &point) const;
    bool contains(const SireMaths::Sphere &sphere) const;

    double distance(const SireMaths::Vector &point) const;

    bool changeView(const Camera &camera, const QSize &new_size);

private:    
    /** Enum giving the indicies of the six planes */
    enum PLANEIDX{ _left=0, _right=1, _bottom=2, _top=3, _near=4, _far=5 };
    
    /** The six planes of the view frustrum (in world-space coordinates) */
    SireMaths::Plane fplanes[6];

    /** A copy of the camera that was used to create this view */
    CameraPtr cam;
    
    /** The current size of the viewport */
    QSize viewport_size;
};

}


SIRE_END_HEADER

#endif
