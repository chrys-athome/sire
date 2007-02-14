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

#include "qhash_spier.h"

#include "meshmgr.h"

using namespace Spier;

/** Construct the Mesh Manager */
MeshMgr::MeshMgr()
{}

/** Destructor */
MeshMgr::~MeshMgr()
{}

/** Return a sphere with radius 'radius'. You could just create one for yourself, 
    but using the mesh manager ensures that all spheres of the same radius use the 
    same underlying object, thus saving memory (in particular graphics memory, 
    as the sphere's geometry is stored on the graphics card) */
GLSphereMesh MeshMgr::getSphere(double radius)
{
    GLSphereMesh newsphere(radius);
    
    if (spherelib.contains(newsphere))
        return spherelib.value(newsphere);
    else
    {
        spherelib.insert(newsphere, newsphere);
        return newsphere;
    }
}

/** Return a cylinder with the specified parameters (see GLCylinder). 
    You could just create one for yourself, 
    but using the mesh manager ensures that all cylinders with the same parameters use the 
    same underlying object, thus saving memory (in particular graphics memory, 
    as the cylinder's geometry is stored on the graphics card) */
GLCylinderMesh MeshMgr::getCylinder(double length, double minrad, double maxrad)
{
    GLCylinderMesh newcyl(length, minrad, maxrad);
    
    if (cylinderlib.contains(newcyl))
        return cylinderlib.value(newcyl);
    else
    {
        cylinderlib.insert(newcyl, newcyl);
        return newcyl;
    }
}
