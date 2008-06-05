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
