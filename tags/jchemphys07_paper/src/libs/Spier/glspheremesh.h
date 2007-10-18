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

#ifndef SPIER_GLSPHEREMESH_H
#define SPIER_GLSPHEREMESH_H

#include <QSharedDataPointer>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class GLSphereMeshData;

/**
This is an implicitly shared class that holds the mesh of verticies/normals/texcoords for an openGL sphere.
The sphere mesh is centered on the origin, and the only adjustable parameter is its radius.
 
@author Christopher Woods
*/
class SPIER_EXPORT GLSphereMesh
{
public:
    GLSphereMesh(double radius=1.0);
    GLSphereMesh(const GLSphereMesh &other);
    ~GLSphereMesh();

    GLSphereMesh& operator=(const GLSphereMesh &other);

    bool operator==(const GLSphereMesh &other) const;
    bool operator!=(const GLSphereMesh &other) const;

    double radius() const;

    void draw(double distance=0.0) const;
    void selectDraw(double distance=0.0) const;

private:

    QSharedDataPointer<GLSphereMeshData> d;
};

}

/** Hash a GLSphereMesh - this is useful as this allows a sphere to act as its own 
    key in a QHash */
inline uint qHash(const Spier::GLSphereMesh &sphere)
{
    return uint( 100.0 * sphere.radius() );
}

SIRE_END_HEADER

#endif
