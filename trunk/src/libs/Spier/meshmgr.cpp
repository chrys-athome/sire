
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
