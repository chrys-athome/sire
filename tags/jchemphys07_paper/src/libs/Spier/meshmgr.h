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

#ifndef SPIER_MESHMGR_H
#define SPIER_MESHMGR_H

#include <QHash>

#include "glspheremesh.h"
#include "glcylindermesh.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This class provides a library of meshes - the class tries to reuse meshes as much as possible to save memory and thus hopefully improve rendering time.
 
@author Christopher Woods
*/
class SPIER_EXPORT MeshMgr
{
public:
    MeshMgr();
    ~MeshMgr();

    GLSphereMesh getSphere(double radius);
    GLCylinderMesh getCylinder(double length, double minrad, double maxrad);

private:

    /** Library of spheres (which are self-indexing) */
    QHash<GLSphereMesh, GLSphereMesh> spherelib;

    /** Library of cylinders (which are also self-indexing) */
    QHash<GLCylinderMesh, GLCylinderMesh> cylinderlib;

};

}

SIRE_END_HEADER

#endif
