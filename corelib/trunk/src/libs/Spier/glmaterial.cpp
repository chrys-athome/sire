
#include "ThirdPartyGUI/glew.h"

#include <QSharedData>
#include <QGLWidget>

#include "glmaterial.h"

namespace Spier
{

/** This private class is used internally by GLMaterial to hold all of the data
    for the material (implicitly shared pimpl class) */
class GLMaterialData : public QSharedData
{
public:
    GLMaterialData();
    ~GLMaterialData();

    void setAmbient(const Vector &color, double opacity=1.0);
    void setDiffuse(const Vector &color, double opacity=1.0);
    void setAmbientDiffuse(const Vector &color, double opacity=1.0);
    
    void setSpecular(const Vector &color, double opacity=1.0);
    void setEmission(const Vector &color, double opacity=1.0);

    void setShininess(double shininess);

    void bind() const;

private:
    void clearList();    

    /** The ambient colour of the material */
    GLfloat amb[4];
    
    /** The diffuse colour */
    GLfloat diff[4];
    
    /** The specular colour */
    GLfloat spec[4];
    
    /** The emissive colour of the material */
    GLfloat emiss[4];
    
    /** The specular exponent (shininess) */
    GLfloat shiny;
    
    /** The material is stored in a display list - this is the 
        index to the display list. If this material is not bound, 
        then this index will be zero */
    GLuint idx;
};

}

using namespace Spier;

/** Create a default, boring grey colour */
GLMaterialData::GLMaterialData() : QSharedData(), shiny(125.0), idx(0)
{
    Vector boringgrey(0.5);
    this->setAmbient(boringgrey);
    this->setDiffuse(boringgrey);
    this->setSpecular( Vector(1.0) );
    this->setEmission( Vector(0.0) );
}

/** Clear the display list (if one is currently used) */
inline void GLMaterialData::clearList()
{
    if (idx)
    {
        glDeleteLists(idx, 1);
        idx = 0;
    }
}

/** Destructor */
GLMaterialData::~GLMaterialData()
{
    this->clearList();
}

/** Set the ambient component of the material */
void GLMaterialData::setAmbient(const Vector &color, double opacity)
{
    clearList();
    amb[0] = color.r();
    amb[1] = color.g();
    amb[2] = color.b();
    amb[3] = opacity;
}

/** Set the diffuse component of the material */
void GLMaterialData::setDiffuse(const Vector &color, double opacity)
{
    clearList();
    diff[0] = color.r();
    diff[1] = color.g();
    diff[2] = color.b();
    diff[3] = opacity;
}

/** Set the ambient and diffuse components of the material */
void GLMaterialData::setAmbientDiffuse(const Vector &color, double opacity)
{
    this->setAmbient(color,opacity);
    this->setDiffuse(color,opacity);
}
    
/** Set the specular component of the material */
void GLMaterialData::setSpecular(const Vector &color, double opacity)
{
    clearList();
    spec[0] = color.r();
    spec[1] = color.g();
    spec[2] = color.b();
    spec[3] = opacity;
}

/** Set the emissive component of the material */
void GLMaterialData::setEmission(const Vector &color, double opacity)
{
    clearList();
    emiss[0] = color.r();
    emiss[1] = color.g();
    emiss[2] = color.b();
    emiss[3] = opacity;
}

/** Set the shininess of the material */
void GLMaterialData::setShininess(double shininess)
{
    clearList();
    shiny = shininess;
}

/** Bind this material to the current openGL context */
void GLMaterialData::bind() const
{
    if (idx)
    {
        //execute the display list holding the material
        glCallList(idx);
    }
    else
    {
        //create a display list to hold this material
        const_cast<GLMaterialData*>(this)->idx = glGenLists(1);
        
        //if idx == 0 then the display list was not created - we
        //will thus just render the material directly
        if (idx)
            glNewList(idx, GL_COMPILE_AND_EXECUTE);
            
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emiss);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
            
        if (idx)
            glEndList();
    }
}

//////////////
////////////// Now the implementation of GLMaterial
//////////////

/** Construct a default material */
GLMaterial::GLMaterial() : d( new GLMaterialData() )
{}

/** Construct a material that is based on the colour 'basecolor' */
GLMaterial::GLMaterial(const Vector &basecolor) : d( new GLMaterialData() )
{
    this->setAmbientDiffuse(basecolor);
}
    
/** Construct a material that is based on the colour 'basecolor' */
GLMaterial::GLMaterial(const Color &basecolor) : d( new GLMaterialData() )
{
    this->setAmbientDiffuse(basecolor);
}
    
/** Construct a material that has the specified components and opacity */
GLMaterial::GLMaterial(const Vector &ambient, const Vector &diffuse, const Vector &specular,
                       const Vector &emission, double shininess, double opacity)
           : d( new GLMaterialData() )
{
    this->setAmbient(ambient, opacity);
    this->setDiffuse(diffuse, opacity);
    this->setSpecular(specular, opacity);
    this->setEmission(emission, opacity);
    this->setShininess(shininess);
}
    
/** Construct a material that has the specified components */
GLMaterial::GLMaterial(const Color &ambient, const Color &diffuse, const Color &specular,
                       const Color &emission, double shininess)
           : d( new GLMaterialData() )
{
    this->setAmbient(ambient);
    this->setDiffuse(diffuse);
    this->setSpecular(specular);
    this->setEmission(emission);
    this->setShininess(shininess);
} 
    
/** Copy constructor - this class is implicitly shared so copying is very fast */
GLMaterial::GLMaterial(const GLMaterial &other) : d( other.d )
{}
    
/** Destructor */
GLMaterial::~GLMaterial()
{}

/** Assignment operator - this is quick as this class is implicitly shared */
GLMaterial& GLMaterial::operator=(const GLMaterial &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool GLMaterial::operator==(const GLMaterial &other) const
{
    return d.constData() == other.d.constData();
}

/** Comparison operator */
bool GLMaterial::operator!=(const GLMaterial &other) const
{
    return d.constData() != other.d.constData();
}

/** Set the ambient component to 'color', with the specified opacity */
void GLMaterial::setAmbient(const Vector &color, double opacity)
{
    d->setAmbient(color, opacity);
}

/** Set the diffuse component to 'color', with the specified opacity */
void GLMaterial::setDiffuse(const Vector &color, double opacity)
{
    d->setDiffuse(color, opacity);
}

/** Set the ambient and diffuse components to 'color', with the specified opacity */
void GLMaterial::setAmbientDiffuse(const Vector &color, double opacity)
{
    d->setAmbientDiffuse(color, opacity);
}

/** Set the specular component to 'color', with the specified opacity */
void GLMaterial::setSpecular(const Vector &color, double opacity)
{
    d->setSpecular(color, opacity);
}

/** Set the emissive component to 'color', with the specified opacity */
void GLMaterial::setEmission(const Vector &color, double opacity)
{
    d->setEmission(color, opacity);
}

/** Set the ambient component to 'color' */
void GLMaterial::setAmbient(const Color &color)
{
    d->setAmbient(color, color.a());
}

/** Set the diffuse component to 'color' */
void GLMaterial::setDiffuse(const Color &color)
{
    d->setDiffuse(color, color.a());
}

/** Set the ambient and diffuse components to 'color' */
void GLMaterial::setAmbientDiffuse(const Color &color)
{
    d->setAmbientDiffuse(color, color.a());
}

/** Set the specular component to 'color' */
void GLMaterial::setSpecular(const Color &color)
{
    d->setSpecular(color, color.a());
}

/** Set the emissive component to 'color' */
void GLMaterial::setEmission(const Color &color)
{
    d->setEmission(color, color.a());
}

/** Set the shininess of the material (the specular exponent) */
void GLMaterial::setShininess(double shiny)
{
    d->setShininess(shiny);
}

/** Bind this material to the current openGL context */
void GLMaterial::bind() const
{
    d->bind();
}
