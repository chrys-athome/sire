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
