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

SIRE_BEGIN_HEADER

namespace Spier
{
class GLSphere;
}

QDataStream& operator<<(QDataStream&, const Spier::GLSphere&);
QDataStream& operator>>(QDataStream&, Spier::GLSphere&);

namespace Spier
{

class GLSphereRenderFunction 
        : public SireBase::ConcreteProperty<GLSphereRenderFunction,GLRenderFunction>
{

};

/** This class holds a sphere */
class SPIER_EXPORT GLSphere : public SireBase::ConcreteProperty<GLSphere,GLMesh>
{

friend QDataStream& ::operator<<(QDataStream&, const GLSphere&);
friend QDataStream& ::operator>>(QDataStream&, GLSphere&);

public:
    GLSphere();
    
    GLSphere(const GLSphere &other);
    
    ~GLSphere();
    
    void render(GLRenderContext &render_context) const;
    
private:
    /** The display lists for different resolution unit spheres */
    GLDisplayList highres_sphere;
    GLDisplayList midres_sphere;
    GLDisplayList lowres_sphere;
    
    /** The radius of this sphere - this is so we can glScale the 
        sphere mesh to the right size */
    double sphere_radius;
};

}

SIRE_END_HEADER

#endif
