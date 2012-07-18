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

#ifndef SPIER_GLSPHERE_H
#define SPIER_GLSPHERE_H

#include "glmesh.h"
#include "gldisplaylist.h"
#include "glrenderfunction.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class GLSphere;
class GLSphereRenderFunction;
}

QDataStream& operator<<(QDataStream&, const Spier::GLSphere&);
QDataStream& operator>>(QDataStream&, Spier::GLSphere&);

QDataStream& operator<<(QDataStream&, const Spier::GLSphereRenderFunction&);
QDataStream& operator>>(QDataStream&, Spier::GLSphereRenderFunction&);

namespace Spier
{

class GLSphereRenderFunction 
        : public SireBase::ConcreteProperty<GLSphereRenderFunction,GLRenderFunction>
{

friend QDataStream& ::operator<<(QDataStream&, const GLSphereRenderFunction&);
friend QDataStream& ::operator>>(QDataStream&, GLSphereRenderFunction&);

public:
    GLSphereRenderFunction();
    GLSphereRenderFunction(int resolution);
    
    GLSphereRenderFunction(const GLSphereRenderFunction &other);
    
    ~GLSphereRenderFunction();
    
    GLSphereRenderFunction& operator=(const GLSphereRenderFunction &other);
    
    bool operator==(const GLSphereRenderFunction &other) const;
    bool operator!=(const GLSphereRenderFunction &other) const;
    
    static const char* typeName();
    
    void operator()() const;

private:
    /** The resolution of the sphere */
    quint32 sphere_res;
};

/** This class holds a sphere which will be rendered with its
    origin at the current 3D 
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLSphere : public SireBase::ConcreteProperty<GLSphere,GLMesh>
{

friend QDataStream& ::operator<<(QDataStream&, const GLSphere&);
friend QDataStream& ::operator>>(QDataStream&, GLSphere&);

public:
    GLSphere();
    
    GLSphere(double radius);
    GLSphere(double radius, int hires, int midres, int lowres);
    
    GLSphere(const GLSphere &other);
    
    ~GLSphere();
    
    GLSphere& operator=(const GLSphere &other);
    
    bool operator==(const GLSphere &other) const;
    bool operator!=(const GLSphere &other) const;
    
    static const char* typeName();
    
    double radius() const;
    
    void render(GLRenderContext &render_context,
                float distance_from_camera) const;
    
private:
    void createSpheres();

    /** The display lists for different resolution unit spheres */
    GLDisplayList highres_sphere;
    GLDisplayList midres_sphere;
    GLDisplayList lowres_sphere;
    
    /** The radius of this sphere - this is so we can glScale the 
        sphere mesh to the right size */
    float sphere_radius;
};

}

Q_DECLARE_METATYPE( Spier::GLSphere )
Q_DECLARE_METATYPE( Spier::GLSphereRenderFunction )

SIRE_EXPOSE_CLASS( Spier::GLSphere )
SIRE_EXPOSE_CLASS( Spier::GLSphereRenderFunction )

SIRE_END_HEADER

#endif
