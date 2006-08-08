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
