
#include "ThirdPartyGUI/glew.h"

#include <QGLWidget>
#include <QSharedData>

#include "glcylindermesh.h"

#include "SireError/errors.h"

namespace Spier
{

class GLCylinderMeshData : public QSharedData
{
public:
    GLCylinderMeshData(double length, double minradius, double maxradius);
    ~GLCylinderMeshData();
    
    void draw(double distance) const;
    void selectDraw(double distance) const;
    
    double length() const;
    double minRadius() const;
    double maxRadius() const;
    
    bool operator==(const GLCylinderMeshData &other) const;

private:
    /** Index to the display list that contains the geometry
        for this cylinder */
    GLuint idx[4];
    
    /** Index to the display list that contains the geometry
        for this cylinder when it is rendered in selection mode */
    GLuint selectidx;
    
    /** The length of this cylinder */
    double lgth;
    
    /** The minimum radius of the cylinder */
    double minrad;
    
    /** The maximum radius of the cylinder */
    double maxrad;
};

}

using namespace Spier;

GLCylinderMeshData::GLCylinderMeshData(double length, double minradius, double maxradius)
               : QSharedData(), selectidx(0),
                 lgth(length), minrad(minradius), maxrad(maxradius)
{
    for (int i=0; i<4; ++i)
        idx[i] = 0;

    if (lgth < 0.0)
        lgth = 0.0;
    
    if (minrad < 0.0)
        minrad = 0.0;
        
    if (maxrad < 0.0)
        maxrad = 0.0;
    
    if (minrad > maxrad)
        qSwap(minrad, maxrad);
}

GLCylinderMeshData::~GLCylinderMeshData()
{
    for (int i=0; i<4; ++i)
        if (idx[i])
            glDeleteLists(idx[i],1);
    
    if (selectidx)
        glDeleteLists(selectidx,1);
}

inline double GLCylinderMeshData::length() const
{
    return lgth;
}

inline double GLCylinderMeshData::minRadius() const
{
    return minrad;
}

inline double GLCylinderMeshData::maxRadius() const
{
    return maxrad;
}

inline bool GLCylinderMeshData::operator==(const GLCylinderMeshData &other) const
{
    return lgth == other.lgth and minrad == other.minrad and maxrad == other.maxrad;
}

inline void GLCylinderMeshData::draw(double dist) const
{
    int i = 0;
    int cres = 30;
    int hres = 10;

    if (dist > 30.0)
    {
        i = 3;
        cres = 6;
        hres = 2;
    }
    else if (dist > 15.0)
    {
        i = 2;
        cres = 10;
        hres = 4;
    }
    else if (dist > 5.0)
    {
        i = 1;
        cres = 20;
        hres = 6;
    }

    if (idx[i])
    {
        //execute the display list holding the cylinder
        glCallList(idx[i]);
    }
    else
    {
        //create a display list to hold this cylinder
        const_cast<GLCylinderMeshData*>(this)->idx[i] = glGenLists(1);
        
        //if idx == 0 then the display list was not created - we
        //will thus just render the cylinder directly
        if (idx[i])
            glNewList(idx[i], GL_COMPILE_AND_EXECUTE);
            
        GLUquadricObj *quadobj = gluNewQuadric();
        if (!quadobj)
            throw SireError::nullptr_error(QObject::tr("Could not create a new GLUQuadric!"),
                                          CODELOC);
        
        gluQuadricNormals(quadobj, GLU_SMOOTH);
        gluQuadricDrawStyle(quadobj, GLU_FILL);
        gluQuadricTexture(quadobj, GL_TRUE);
        
        //translate so that the center of the cylinder is at the origin, and the 
        //cylinder is orientated along the z-axis, with the maximum radius at the bottom,
        //and the minimum radius at the top
        glPushMatrix();
        glTranslatef(0.0,0.0, -0.5 * lgth);
        
        gluCylinder(quadobj, maxrad, minrad, lgth, cres, hres);
        
        glPopMatrix();
        
        gluDeleteQuadric(quadobj);
        
        if (idx[i])
            glEndList();
    }
}

inline void GLCylinderMeshData::selectDraw(double) const
{
    if (selectidx)
    {
        //execute the display list holding the cylinder
        glCallList(selectidx);
    }
    else
    {
        //create a display list to hold this cylinder
        const_cast<GLCylinderMeshData*>(this)->selectidx = glGenLists(1);
        
        //if idx == 0 then the display list was not created - we
        //will thus just render the cylinder directly
        if (selectidx)
            glNewList(selectidx, GL_COMPILE_AND_EXECUTE);
            
        GLUquadricObj *quadobj = gluNewQuadric();
        if (!quadobj)
            throw SireError::nullptr_error(QObject::tr("Could not create a new GLUQuadric!"),
                                          CODELOC);
        
        gluQuadricNormals(quadobj, GLU_NONE);
        gluQuadricDrawStyle(quadobj, GLU_FILL);
        gluQuadricTexture(quadobj, GL_FALSE);
        
        //translate so that the center of the cylinder is at the origin, and the 
        //cylinder is orientated along the z-axis, with the maximum radius at the bottom,
        //and the minimum radius at the top
        glPushMatrix();
        glTranslatef(0.0,0.0, -0.5 * lgth);
        
        gluCylinder(quadobj, maxrad, minrad, lgth, 8, 3);
        
        glPopMatrix();
        
        gluDeleteQuadric(quadobj);
        
        if (selectidx)
            glEndList();
    }
}

////////////////////
//////////////////// Implementation of GLCylinderMesh
////////////////////

GLCylinderMesh::GLCylinderMesh(double length, double lowrad, double hirad)
           : d( new GLCylinderMeshData(length, lowrad, hirad) )
{}

GLCylinderMesh::GLCylinderMesh(const GLCylinderMesh &other) : d( other.d )
{}

GLCylinderMesh::~GLCylinderMesh()
{}

GLCylinderMesh& GLCylinderMesh::operator=(const GLCylinderMesh &other)
{
    d = other.d;
    return *this;
}

bool GLCylinderMesh::operator==(const GLCylinderMesh &other) const
{
    return d->operator==( *(other.d) );
}

bool GLCylinderMesh::operator!=(const GLCylinderMesh &other) const
{
    return not d->operator==( *(other.d) );
}

double GLCylinderMesh::length() const
{
    return d->length();
}

double GLCylinderMesh::minRadius() const
{
    return d->minRadius();
}

double GLCylinderMesh::maxRadius() const
{
    return d->maxRadius();
}

void GLCylinderMesh::draw(double distance) const
{
    d->draw(distance);
}

void GLCylinderMesh::selectDraw(double distance) const
{
    d->selectDraw(distance);
}
