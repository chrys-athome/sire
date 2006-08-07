#ifndef SPIER_QHASH_SPIER_H
#define SPIER_QHASH_SPIER_H

#include <QtGlobal>

namespace Spier
{
class GLSphereMesh;
class GLCylinderMesh;
}

uint qHash(const Spier::GLSphereMesh&);
uint qHash(const Spier::GLCylinderMesh&);


#endif

