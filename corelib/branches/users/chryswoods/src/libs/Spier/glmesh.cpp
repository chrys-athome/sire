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

#include "glmesh.h"
#include "glrendercontext.h"

#include "SireStream/datastream.h"

using namespace Spier;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of GLMesh
/////////

static const RegisterMetaType<GLMesh> r_glmesh( MAGIC_ONLY, GLMesh::typeName() );

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const GLMesh &glmesh)
{
    writeHeader(ds, r_glmesh, 1);
    
    ds << static_cast<const Property&>(glmesh);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, GLMesh &glmesh)
{
    VersionID v = readHeader(ds, r_glmesh);
    
    if (v == 1)
    {
        ds >> static_cast<Property&>(glmesh);
    }
    else
        throw version_error( v, "1", r_glmesh, CODELOC );
        
    return ds;
}

/** Constructor */
GLMesh::GLMesh() : Property()
{}

/** Copy constructor */
GLMesh::GLMesh(const GLMesh &other) : Property(other)
{}

/** Destructor */
GLMesh::~GLMesh()
{}

/** Copy assignment operator */
GLMesh& GLMesh::operator=(const GLMesh &other)
{
    Property::operator=(other);
    return *this;
}

/** Comparison operator */
bool GLMesh::operator==(const GLMesh &other) const
{
    return Property::operator==(other);
}

/** Comparison operator */
bool GLMesh::operator!=(const GLMesh &other) const
{
    return Property::operator!=(other);
}

/** Render this mesh to the passed context, but without providing
    any distance information about how far the center of this mesh is from the camera */
void GLMesh::render(GLRenderContext &render_context) const
{
    this->render(render_context, 0);
}

Q_GLOBAL_STATIC( NullGLMesh, nullGLMesh )

/** Return the global null mesh */
const NullGLMesh& GLMesh::null()
{
    return *(nullGLMesh());
}

/////////
///////// Implementation of NullGLMesh
/////////

static const RegisterMetaType<NullGLMesh> r_nullglmesh;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const NullGLMesh &nullglmesh)
{
    writeHeader(ds, r_nullglmesh, 1);
    
    ds << static_cast<const GLMesh&>(nullglmesh);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, NullGLMesh &nullglmesh)
{
    VersionID v = readHeader(ds, r_nullglmesh);
    
    if (v == 1)
    {
        ds >> static_cast<GLMesh&>(nullglmesh);
    }
    else
        throw version_error( v, "1", r_nullglmesh, CODELOC );
        
    return ds;
}

/** Constructor */
NullGLMesh::NullGLMesh() : ConcreteProperty<NullGLMesh,GLMesh>()
{}

/** Copy constructor */
NullGLMesh::NullGLMesh(const NullGLMesh &other)
           : ConcreteProperty<NullGLMesh,GLMesh>(other)
{}

/** Destructor */
NullGLMesh::~NullGLMesh()
{}

/** Copy assignment operator */
NullGLMesh& NullGLMesh::operator=(const NullGLMesh &other)
{
    GLMesh::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullGLMesh::operator==(const NullGLMesh&) const
{
    return true;
}

/** Comparison operator */
bool NullGLMesh::operator!=(const NullGLMesh&) const
{
    return false;
}

const char* NullGLMesh::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullGLMesh>() );
}

/** A NullGLMesh does nothing */
void NullGLMesh::render(GLRenderContext&, float) const
{}
