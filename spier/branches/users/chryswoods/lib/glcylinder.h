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

#ifndef SPIER_GLCYLINDER_H
#define SPIER_GLCYLINDER_H

#include "glcylindermesh.h"
#include "materialptr.h"
#include "glmatrix.h"

#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMaths::Vector;
using SireMaths::Quaternion;

class GLCanvas;

/**
This class abstracts a cylinder (as GLSphere abstracts a sphere). This class is implicitly shared to allow the same geometry to be reused by multiple objects in a scene.
 
@author Christopher Woods
*/
class SPIER_EXPORT GLCylinder
{
public:
    GLCylinder();
    GLCylinder(GLCanvas *parent);
    GLCylinder(GLCanvas *parent, double length, double lowrad, double hirad);
    GLCylinder(const GLCylinder &other);
    ~GLCylinder();

    GLCylinder& operator=(const GLCylinder &other);

    void setPosition(const Vector &position);
    void setCenter(const Vector &center);
    void setOrientation(const Quaternion &orient);
    void setDimensions(double length, double rad0, double rad1);

    void setMaterial(const MaterialPtr &material);

    const Vector& position() const;
    const Vector& center() const;
    const Quaternion& orientation() const;

    double length() const;
    double minRadius() const;
    double maxRadius() const;

    void draw() const;
    void selectDraw() const;

    const GLCanvas* parent() const;

private:

    void getMesh();

    /** The mesh for the cylinder */
    GLCylinderMesh _mesh;
    
    /** The material for the cylinder */
    MaterialPtr _material;

    /** The position of the center of the cylinder */
    Vector _center;
    /** The orientation of the cylinder */
    Quaternion _orient;
    /** GLMatrix version of the orientation of the cylinder */
    GLMatrix _glmat;
    
    /** The dimensions of the cylinder */
    double _length, _rad0, _rad1;
    
    /** The parent canvas on which this cylinder will be drawn */
    GLCanvas* _parent;
};

/** Return the parent GLCanvas of this sphere */
inline const GLCanvas* GLCylinder::parent() const
{
    return _parent;
}

}

/** Hash a GLCylinder - this is used so that the GLCylinder can act as its own index
    in a QHash */
inline uint qHash(const Spier::GLCylinder &cyl)
{
    return uint( 100.0 * cyl.length() + 10000.0*cyl.minRadius() + 1000000.0*cyl.maxRadius() );
}

SIRE_END_HEADER

#endif
