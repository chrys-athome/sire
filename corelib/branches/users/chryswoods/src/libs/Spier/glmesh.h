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

#include <QSharedData>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/** This is the base class of all meshes that can be rendered 

    @author Christopher Woods
*/
class SPIER_EXPORT GLMesh : public QSharedData
{
public:
    GLMesh();
    GLMesh(const GLMesh &other);
    
    virtual ~GLMesh();
    
    static const char* typeName()
    {
        return "Spier::GLMesh";
    }
    
    virtual const char* what() const=0;
    
    virtual GLMesh* clone() const=0;
    
    virtual void render(const QGLContext *render_context) const=0;

};

}

SIRE_END_HEADER

#endif
