/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SPIER_GLMESH_H
#define SPIER_GLMESH_H

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class GLMesh;
class NullGLMesh;
}

QDataStream& operator<<(QDataStream&, const Spier::GLMesh&);
QDataStream& operator>>(QDataStream&, Spier::GLMesh&);

QDataStream& operator<<(QDataStream&, const Spier::NullGLMesh&);
QDataStream& operator>>(QDataStream&, Spier::NullGLMesh&);

namespace Spier
{

/** This is the base class of all meshes that can be rendered. 

    A mesh provides a single 3D shape that is rendered using
    the same material and texture. The mesh may have multiple
    resolutions for different distances

    This mesh is rendered using the current model-view matrix.

    The mesh is rendered with its center at the current
    raster position.

    @author Christopher Woods
*/
class SPIER_EXPORT GLMesh : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const GLMesh&);
friend QDataStream& ::operator>>(QDataStream&, GLMesh&);

public:
    GLMesh();
    GLMesh(const GLMesh &other);
    
    virtual ~GLMesh();
    
    static const char* typeName()
    {
        return "Spier::GLMesh";
    }
    
    virtual GLMesh* clone() const=0;
    
    virtual void render(GLRenderContext &render_context) const=0;

    static const NullGLMesh& null();

protected:
    GLMesh& operator=(const GLMesh &other);
    
    bool operator==(const GLMesh &other) const;
    bool operator!=(const GLMesh &other) const;
};

typedef SireBase::PropPtr<GLMesh> GLMeshPtr;

}

Q_DECLARE_METATYPE( Spier::NullGLMesh )

SIRE_EXPOSE_CLASS( Spier::GLMesh )
SIRE_EXPOSE_CLASS( Spier::NullGLMesh )

SIRE_EXPOSE_PROPERTY( Spier::GLMeshPtr, Spier::GLMesh )

SIRE_END_HEADER

#endif
