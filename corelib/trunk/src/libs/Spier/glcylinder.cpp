
#include "ThirdPartyGUI/glew.h"

#include <QGLWidget>

#include "SireMaths/sphere.h"

#include "glcanvas.h"
#include "glcylinder.h"
#include "meshmgr.h"
#include "camera.h"

#include "SireError/errors.h"

using namespace Spier;
using namespace SireMaths;

/** Construct a null GLCylinder */
GLCylinder::GLCylinder() : _parent(0)
{}

/** Construct a cylinder that is a child of the GLCanvas */
GLCylinder::GLCylinder(GLCanvas *parent) : _parent(parent)
{}

/** Construct a GLCylinder with specified GLCanvas parent, and specified dimensions */
GLCylinder::GLCylinder(GLCanvas *parent, double length, double lowrad, double hirad)
           : _length(length), _rad0(lowrad), _rad1(hirad), _parent(parent)
{
    getMesh();
}

/** Copy constructor */
GLCylinder::GLCylinder(const GLCylinder &other)
           : _mesh(other._mesh), _material(other._material),
             _center(other._center), _orient(other._orient), _glmat(other._glmat),
             _length(other._length), _rad0(other._rad0), _rad1(other._rad1),
             _parent(other._parent)
{}

/** Destructor */
GLCylinder::~GLCylinder()
{}

/** Assignment operator */
GLCylinder& GLCylinder::operator=(const GLCylinder &other)
{
    _mesh = other._mesh;
    _material = other._material;
    _center = other._center;
    _orient = other._orient;
    _glmat = other._glmat;
    _length = other._length;
    _rad0 = other._rad0;
    _rad1 = other._rad1;
    _parent = other._parent;
    
    return *this;
}

/** Get the mesh for this cylinder */
void GLCylinder::getMesh()
{
    if (_parent)
        _mesh = _parent->meshLibrary().getCylinder(_length, _rad0, _rad1);
}

/** Set the position of the center of the cylinder */
void GLCylinder::setPosition(const Vector &position)
{
    _center = position;
}

/** Set the position of the center of the cylinder */
void GLCylinder::setCenter(const Vector &center)
{
    _center = center;
}

/** Set the orientation of the cylinder */
void GLCylinder::setOrientation(const Quaternion &orient)
{
    _orient = orient;
    _glmat = GLMatrix(_orient);
}

/** Set the dimensions of the cylinder */
void GLCylinder::setDimensions(double length, double rad0, double rad1)
{
    _length = length;
    _rad0 = rad0;
    _rad1 = rad1;
    
    getMesh();
}

/** Set the material for this cylinder */
void GLCylinder::setMaterial(const MaterialPtr &material)
{
    _material = material;
}

/** Return the position of the center of the cylinder */
const Vector& GLCylinder::position() const
{
    return _center;
}

/** Return the position of the center of the cylinder */
const Vector& GLCylinder::center() const
{
    return _center;
}

/** Return a quaternion representing the orientation of the cylinder */
const Quaternion& GLCylinder::orientation() const
{
    return _orient;
}

/** Return the length of the cylinder */
double GLCylinder::length() const
{
    return _length;
}

/** Return the minimum radius of the cylinder */
double GLCylinder::minRadius() const
{
    return qMin(_rad0, _rad1);
}

/** Return the maximum radius of the cylinder */
double GLCylinder::maxRadius() const
{
    return qMax(_rad0, _rad1);
}

/** Draw this cylinder */
void GLCylinder::draw() const
{
    if (_parent == 0)
        return;

    const Camera &cam = _parent->camera();
    
    if (cam.contains(Sphere(_center, 0.5*_length)))
    {
        glPushMatrix();
        glTranslatef(_center.x(), _center.y(), _center.z());
        _glmat.glMult();
    
        _parent->bind(_material);
    
        _mesh.draw( cam.distance(_center) );
    
        glPopMatrix();
    }
}

/** Draw a simplified version of the cylinder for selection mode */
void GLCylinder::selectDraw() const
{
    if (_parent == 0)
        return;
        
    if (_parent->camera().contains(Sphere(_center, 0.5*_length)))
    {
        glPushMatrix();
        glTranslatef(_center.x(), _center.y(), _center.z());
        _glmat.glMult();
    
        _mesh.selectDraw();
    
        glPopMatrix();
    }
}
