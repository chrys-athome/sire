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

#include <QGLWidget>

#include "glsphere.h"
#include "glrendercontext.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

using namespace Spier;
using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of GLSphereRenderFunction
//////////

static const RegisterMetaType<GLSphereRenderFunction> r_glsphererender;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, 
                                     const GLSphereRenderFunction &glsphererender)
{
    writeHeader(ds, r_glsphererender, 1);
    
    ds << glsphererender.sphere_res
       << static_cast<const GLRenderFunction&>(glsphererender);
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, 
                                     GLSphereRenderFunction &glsphererender)
{
    VersionID v = readHeader(ds, r_glsphererender);
    
    if (v == 1)
    {
        ds >> glsphererender.sphere_res
           >> static_cast<GLRenderFunction&>(glsphererender);
    }
    else
        throw version_error( v, "1", r_glsphererender, CODELOC );
        
    return ds;
}

/** Constructor */
GLSphereRenderFunction::GLSphereRenderFunction()
                       : ConcreteProperty<GLSphereRenderFunction,GLRenderFunction>(),
                         sphere_res(5)
{}

/** Construct a sphere rendering function that will render a sphere with
    the specified resolution */
GLSphereRenderFunction::GLSphereRenderFunction(int resolution)
                       : ConcreteProperty<GLSphereRenderFunction,GLRenderFunction>(),
                         sphere_res(resolution)
{
    if (sphere_res < 3)
        sphere_res = 3;
    else if (sphere_res > 50)
        sphere_res = 50;
}

/** Copy constructor */
GLSphereRenderFunction::GLSphereRenderFunction(const GLSphereRenderFunction &other)
                       : ConcreteProperty<GLSphereRenderFunction,GLRenderFunction>(other),
                         sphere_res(other.sphere_res)
{}
                       
/** Destructor */
GLSphereRenderFunction::~GLSphereRenderFunction()
{}

/** Copy assignment operator */
GLSphereRenderFunction& GLSphereRenderFunction::operator=(
                                            const GLSphereRenderFunction &other)
{
    GLRenderFunction::operator=(other);
    sphere_res = other.sphere_res;
    
    return *this;
}

/** Comparison operator */
bool GLSphereRenderFunction::operator==(const GLSphereRenderFunction &other) const
{
    return sphere_res == other.sphere_res and
           GLRenderFunction::operator==(other);
}

/** Comparison operator */
bool GLSphereRenderFunction::operator!=(const GLSphereRenderFunction &other) const
{
    return not this->operator==(other);
}

const char* GLSphereRenderFunction::typeName()
{
    return QMetaType::typeName( qMetaTypeId<GLSphereRenderFunction>() );
}

/** This function renders a unit sphere with its origin at the current position */
void GLSphereRenderFunction::operator()() const
{
    GLUquadricObj *quadobj = gluNewQuadric();

    if (!quadobj)
        throw SireError::nullptr_error(QObject::tr(
                    "Could not create a new GLUQuadric to render a sphere!"), CODELOC);

    try
    {
        gluQuadricNormals(quadobj, GLU_SMOOTH);
        gluQuadricDrawStyle(quadobj, GLU_FILL);
        gluQuadricTexture(quadobj, GL_TRUE);

        gluSphere(quadobj, 1.0, sphere_res, sphere_res);
    }
    catch(...)
    {
        gluDeleteQuadric(quadobj);
        throw;
    }

    gluDeleteQuadric(quadobj);
}

//////////
////////// Implementation of GLSphere
//////////

static const RegisterMetaType<GLSphere> r_glsphere;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const GLSphere &glsphere)
{
    writeHeader(ds, r_glsphere, 1);
    
    SharedDataStream sds(ds);
    
    sds << glsphere.sphere_radius
        << glsphere.highres_sphere << glsphere.midres_sphere
        << glsphere.lowres_sphere;
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, GLSphere &glsphere)
{
    VersionID v = readHeader(ds, r_glsphere);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> glsphere.sphere_radius 
            >> glsphere.highres_sphere >> glsphere.midres_sphere
            >> glsphere.lowres_sphere;
    }
    else
        throw version_error( v, "1", r_glsphere, CODELOC );
        
    return ds;
}

void GLSphere::createSpheres()
{
    if (sphere_radius == 0)
    {
        return;
    }
    else if (sphere_radius < 0.5)
    {
        lowres_sphere = GLDisplayList( GLSphereRenderFunction(7) );
        midres_sphere = GLDisplayList( GLSphereRenderFunction(10) );
        highres_sphere = GLDisplayList( GLSphereRenderFunction(20) );
    }
    else
    {
        lowres_sphere = GLDisplayList( GLSphereRenderFunction(8) );
        midres_sphere = GLDisplayList( GLSphereRenderFunction(15) );
        highres_sphere = GLDisplayList( GLSphereRenderFunction(25) );
    }
}

/** Construct a null sphere */
GLSphere::GLSphere()
         : ConcreteProperty<GLSphere,GLMesh>(), sphere_radius(0)
{}

/** Construct a sphere of radius 'radius' */
GLSphere::GLSphere(double radius)
         : ConcreteProperty<GLSphere,GLMesh>(),
           sphere_radius(radius)
{
    if (radius < 0)
        radius = 0;

    createSpheres();
}

/** Construct a sphere of specified radius and resolutions */
GLSphere::GLSphere(double radius, int hires, int midres, int lowres)
         : ConcreteProperty<GLSphere,GLMesh>(),
           highres_sphere( GLSphereRenderFunction(hires) ),
           midres_sphere( GLSphereRenderFunction(midres) ),
           lowres_sphere( GLSphereRenderFunction(lowres) ),
           sphere_radius(radius)
{
    if (radius < 0)
        radius = 0;
}

/** Copy constructor */
GLSphere::GLSphere(const GLSphere &other)
         : ConcreteProperty<GLSphere,GLMesh>(other),
           highres_sphere(other.highres_sphere),
           midres_sphere(other.midres_sphere),
           lowres_sphere(other.lowres_sphere),
           sphere_radius(other.sphere_radius)
{}

/** Destructor */
GLSphere::~GLSphere()
{}

/** Copy assignment operator */
GLSphere& GLSphere::operator=(const GLSphere &other)
{
    if (this != &other)
    {
        highres_sphere = other.highres_sphere;
        midres_sphere = other.midres_sphere;
        lowres_sphere = other.lowres_sphere;
        sphere_radius = other.sphere_radius;
        
        GLMesh::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool GLSphere::operator==(const GLSphere &other) const
{
    return this == &other or
           (highres_sphere == other.highres_sphere and
            midres_sphere == other.midres_sphere and
            lowres_sphere == other.lowres_sphere and
            sphere_radius == other.sphere_radius and
            GLMesh::operator==(other) );
}

/** Comparison operator */
bool GLSphere::operator!=(const GLSphere &other) const
{
    return not GLSphere::operator==(other);
}

const char* GLSphere::typeName()
{
    return QMetaType::typeName( qMetaTypeId<GLSphere>() );
}

/** Return the radius of the sphere */
double GLSphere::radius() const
{
    return sphere_radius;
}

/** Render the sphere to the passed context. This renders the sphere
    centered at the current render location */
void GLSphere::render(GLRenderContext &render_context, 
                      float distance_from_camera) const
{
    if (sphere_radius == 0)
        return;
    
    if (sphere_radius != 1)
    {
        glPushMatrix();
        glScalef(sphere_radius, sphere_radius, sphere_radius);
    }
                
    //work out the resolution
    if (sphere_radius < 0.5)
    {
        if (distance_from_camera > 15)
            render_context.render(lowres_sphere);
            
        else if (distance_from_camera > 7.5)
            render_context.render(midres_sphere);
        
        else
            render_context.render(lowres_sphere);
    }
    else
    {
        if (distance_from_camera > 30)
            render_context.render(lowres_sphere);
            
        else if (distance_from_camera > 15)
            render_context.render(midres_sphere);
            
        else
            render_context.render(highres_sphere);
    }
    
    if (sphere_radius != 1)
    {
        glPopMatrix();
    }
}
