
#include "ThirdPartyGUI/glew.h"

#include "glsphere.h"
#include "glcanvas.h"
#include "materialmgr.h"
#include "meshmgr.h"
#include "material.h"
#include "camera.h"

#include "SireError/errors.h"

#include <QGLWidget>

using namespace SireStream;
using namespace Spier;
using namespace SireMaths;

/** Construct a null GLSphere */
GLSphere::GLSphere() : _parent(0)
{}

/** Construct a GLSphere which is to be drawn on the parent canvas */
GLSphere::GLSphere(GLCanvas *parent) : _parent(parent)
{
    getMesh();
}

/** Construct a GLSphere which represents 'sphere', to be drawn on 'parent' */
GLSphere::GLSphere(GLCanvas *parent, const Sphere &sphere) : _sphere(sphere), _parent(parent)
{
    getMesh();
}

/** Construct a GLSphere which represents  'sphere' using the specified material,
    and to be drawn on the parent canvas */
GLSphere::GLSphere(GLCanvas *parent, const Sphere &sphere, const MaterialPtr &material)
         : _sphere(sphere), _material(material), _parent(parent)
{
    getMesh();
}

/** Copy constructor - this is fast as the data in this class is implicitly shared */
GLSphere::GLSphere(const GLSphere &other)
         : _mesh(other._mesh), _sphere(other._sphere), _material(other._material),
           _parent(other._parent)
{}

/** Destructor */
GLSphere::~GLSphere()
{}

/** Get the mesh for this sphere for the current canvas */
void GLSphere::getMesh()
{
    if (_parent)
        _mesh = _parent->meshLibrary().getSphere( _sphere.radius() );
}

/** Assignment operator - this is fast as the data in this class is implicitly shared */
GLSphere& GLSphere::operator=(const GLSphere &other)
{
    _mesh = other._mesh;
    _sphere = other._sphere;
    _material = other._material;
    _parent = other._parent;
    return *this;
}

/** Set the material for this sphere */
void GLSphere::setMaterial(const MaterialPtr &material)
{
    _material = material;
}

/** Set the sphere for this sphere */
void GLSphere::setSphere(const Sphere &sphere)
{
    _sphere = sphere;
    getMesh();    
}

/** Set the position of this sphere */
void GLSphere::setPosition(const Vector &pos)
{
    _sphere.setPosition(pos);
}

/** Set the radius of this sphere */
void GLSphere::setRadius(double radius)
{
    _sphere.setRadius(radius);
    getMesh();    
}

/** Return the radius of this sphere */
double GLSphere::radius() const
{
    return _sphere.radius();
}

/** Return the mathematical representation of this sphere */
const Sphere& GLSphere::sphere() const
{
    return _sphere;
}

/** Return the position of the center of this sphere */
const Vector& GLSphere::position() const
{
    return _sphere.position();
}

/** Return the material used to render this sphere */
const MaterialPtr& GLSphere::material() const
{
    return _material;
}

/** Draw this sphere */
void GLSphere::draw() const
{
    if (not _parent)
        return;

    const Camera &cam = _parent->camera();
    
    double dist = cam.distance(_sphere.center());
    
    if ( cam.contains(_sphere) )
    {
        glPushMatrix();
    
        //translate to the center of the sphere
        glTranslatef( _sphere.position().x(), _sphere.position().y(), _sphere.position().z() );
    
        //bind the material of the sphere
        _parent->bind( _material );
    
        //draw the mesh
        _mesh.draw(dist);
    
        //done!
        glPopMatrix();
   }
}

/** Draw a basic representation of this sphere for selection mode */
void GLSphere::selectDraw() const
{
    if (not _parent)
        return;
        
    if ( _parent->camera().contains(_sphere) )
    {
        glPushMatrix();
        glTranslatef( _sphere.position().x(), _sphere.position().y(), _sphere.position().z() );
        _mesh.selectDraw();
        glPopMatrix();
    }
}
