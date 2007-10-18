/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SPIER_GLSPHERE_H
#define SPIER_GLSPHERE_H

#include "SireMaths/sphere.h"

#include "materialptr.h"
#include "glspheremesh.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMaths::Sphere;
using SireMaths::Vector;

class GLCanvas;

/**
This class draws a sphere of specified radius around the origin. The resolution of the sphere increases the closer the sphere is to the camera. The sphere is cached so that it can be drawn quickly (especially if there are multiple copies in the scene)
 
@author Christopher Woods
*/
class SPIER_EXPORT GLSphere
{
public:
    GLSphere();
    GLSphere(GLCanvas *parent);
    GLSphere(GLCanvas *parent, const Sphere &sphere);
    GLSphere(GLCanvas *parent, const Sphere &sphere, const MaterialPtr &material);
    
    GLSphere(const GLSphere &other);
    
    ~GLSphere();

    GLSphere& operator=(const GLSphere &other);

    void setMaterial(const MaterialPtr &material);
    void setSphere(const Sphere &sphere);
    void setPosition(const Vector &pos);
    void setRadius(double radius);

    double radius() const;
    const Sphere& sphere() const;
    const Vector& position() const;
    const MaterialPtr& material() const;

    void draw() const;
    void selectDraw() const;
    
    const GLCanvas* parent() const;
    
private:

    void getMesh();

    /** The mesh representing the sphere */
    GLSphereMesh _mesh;

    /** The mathematical representation of the sphere */
    Sphere _sphere;
    
    /** The material used to render the sphere */
    MaterialPtr _material;

    /** The parent GLCanvas on which this sphere will be drawn */
    GLCanvas* _parent;
};

/** Return the parent GLCanvas of this sphere */
inline const GLCanvas* GLSphere::parent() const
{
    return _parent;
}

}

SIRE_END_HEADER

#endif
