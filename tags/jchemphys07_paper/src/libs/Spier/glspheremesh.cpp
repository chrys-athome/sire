/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#include "ThirdPartyGUI/glew.h"

#include "glspheremesh.h"

#include <QGLWidget>
#include <QSharedData>

#include "SireError/errors.h"

namespace Spier
{

class GLSphereMeshData : public QSharedData
{
public:
    GLSphereMeshData(double radius);
    ~GLSphereMeshData();
    
    void draw(double dist) const;
    void selectDraw(double dist) const;
    
    double radius() const;
    
    bool operator==(const GLSphereMeshData &other) const;
    
private:
    /** Index to the display list that contains this sphere, for
        several different distances */
    GLuint idx[4];
    
    /** Index to the display list holding the sphere drawn for
        selection (reduced resolution sphere) */
    GLuint select_idx;

    /** The radius of this sphere */
    double rad;
};
}

using namespace Spier;

GLSphereMeshData::GLSphereMeshData(double radius) : select_idx(0), rad(radius)
{
    for (int i=0; i<4; ++i)
        idx[i] = 0;

    if (rad < 0.0)
        rad = 0.0;
}

GLSphereMeshData::~GLSphereMeshData()
{
    for (int i=0; i<4; ++i)
        if (idx[i])
            glDeleteLists(idx[i], 1);
    
    if (select_idx)
        glDeleteLists(select_idx, 1);
}

inline double GLSphereMeshData::radius() const
{
    return rad;
}

inline bool GLSphereMeshData::operator==(const GLSphereMeshData &other) const
{
    return rad == other.rad;
}

inline void GLSphereMeshData::draw(double dist) const
{
    int i = 0;
    int res = 30;
    
    if (rad < 0.5)
    {
        if (dist > 30.0)
        {
            i = 3;
            res = 7;
        }
        else if (dist > 20.0)
        {
            i = 2;
            res = 10;
        }
        else if (dist > 7.5)
        {
            i = 1;
            res = 20;
        }
    }
    else
    {
        if (dist > 50.0)
        {
            i = 3;
            res = 8;
        }
        else if (dist > 30.0)
        {
            i = 2;
            res = 15;
        }
        else if (dist > 15.0)
        {
            i = 1;
            res = 20;
        }
    }
    
    if (idx[i])
    {
        //execute the display list holding the sphere
        glCallList(idx[i]);
    }
    else
    {
    
        //create a display list to hold this sphere
        const_cast<GLSphereMeshData*>(this)->idx[i] = glGenLists(1);
        
        //if idx == 0 then the display list was not created - we
        //will thus just render the sphere directly
        if (idx[i])
            glNewList(idx[i], GL_COMPILE_AND_EXECUTE);
            
        GLUquadricObj *quadobj = gluNewQuadric();
        if (!quadobj)
            throw SireError::nullptr_error(QObject::tr("Could not create a new GLUQuadric!"),
                                          CODELOC);
        
        gluQuadricNormals(quadobj, GLU_SMOOTH);
        gluQuadricDrawStyle(quadobj, GLU_FILL);
        gluQuadricTexture(quadobj, GL_TRUE);
        
        gluSphere(quadobj, rad, res, res);
        
        gluDeleteQuadric(quadobj);
        
        if (idx[i])
            glEndList();
    }
}

inline void GLSphereMeshData::selectDraw(double) const
{
    if (select_idx)
    {
        //execute the display list holding the sphere
        glCallList(select_idx);
    }
    else
    {
        //create a display list to hold this sphere
        const_cast<GLSphereMeshData*>(this)->select_idx = glGenLists(1);
        
        //if idx == 0 then the display list was not created - we
        //will thus just render the sphere directly
        if (select_idx)
            glNewList(select_idx, GL_COMPILE_AND_EXECUTE);
            
        GLUquadricObj *quadobj = gluNewQuadric();
        if (!quadobj)
            throw SireError::nullptr_error(QObject::tr("Could not create a new GLUQuadric!"),
                                          CODELOC);
        
        gluQuadricNormals(quadobj, GLU_NONE);
        gluQuadricDrawStyle(quadobj, GLU_FILL);
        gluQuadricTexture(quadobj, GL_FALSE);
        
        gluSphere(quadobj, rad, 8, 8);
        
        gluDeleteQuadric(quadobj);
        
        if (select_idx)
            glEndList();
    }
}

/////////////
///////////// Implementation of GLSphereMesh
/////////////

GLSphereMesh::GLSphereMesh(double radius) : d( new GLSphereMeshData(radius) )
{}

GLSphereMesh::GLSphereMesh(const GLSphereMesh &other) : d( other.d )
{}

GLSphereMesh::~GLSphereMesh()
{}

GLSphereMesh& GLSphereMesh::operator=(const GLSphereMesh &other)
{
    d = other.d;
    return *this;
}

void GLSphereMesh::draw( double distance ) const
{
    d->draw(distance);
}

void GLSphereMesh::selectDraw( double distance ) const
{
    d->selectDraw(distance);
}

double GLSphereMesh::radius() const
{
    return d->radius();
}

bool GLSphereMesh::operator==(const GLSphereMesh &other) const
{
    return d->operator==( *(other.d) );
}

bool GLSphereMesh::operator!=(const GLSphereMesh &other) const
{
    return not d->operator==( *(other.d) );
}
