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

#ifndef SPIER_GLCYLINDERMESH_H
#define SPIER_GLCYLINDERMESH_H

#include <QSharedDataPointer>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class GLCylinderMeshData;

/**
This class abstracts a cylinder mesh (as GLSphereMesh abstracts a sphere). This class is implicitly shared to allow the same geometry to be reused by multiple objects in a scene. The cylinder is centered at the origin, aligned along the z-axis, and has the larger end at the bottom.
 
@author Christopher Woods
*/
class SPIER_EXPORT GLCylinderMesh
{
public:
    GLCylinderMesh(double length=1.0, double lowrad=1.0, double hirad=1.0);
    GLCylinderMesh(const GLCylinderMesh &other);
    ~GLCylinderMesh();

    GLCylinderMesh& operator=(const GLCylinderMesh &other);

    bool operator==(const GLCylinderMesh &other) const;
    bool operator!=(const GLCylinderMesh &other) const;

    double length() const;
    double minRadius() const;
    double maxRadius() const;

    void draw(double distance=0.0) const;
    void selectDraw(double distance=0.0) const;

private:

    QSharedDataPointer<GLCylinderMeshData> d;
    
};

}

/** Hash a GLCylinderMesh - this is used so that the GLCylinderMesh can act as its own index
    in a QHash */
inline uint qHash(const Spier::GLCylinderMesh &cyl)
{
    return uint( 100.0 * cyl.length() + 10000.0*cyl.minRadius() + 1000000.0*cyl.maxRadius() );
}

SIRE_END_HEADER

#endif
