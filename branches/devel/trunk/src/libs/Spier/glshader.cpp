
#include "ThirdPartyGUI/glew.h"

#include "glshader.h"

#include <QGLWidget>
#include <QSharedData>

namespace Spier
{

class GLShaderData : public QSharedData
{
public:
    GLShaderData();
    ~GLShaderData();

    void bind() const;

};
}

using namespace Spier;

GLShaderData::GLShaderData() : QSharedData()
{}

GLShaderData::~GLShaderData()
{}

void GLShaderData::bind() const
{}

/////////////////
///////////////// Implementation of GLShader 
/////////////////

GLShader::GLShader()
{}

GLShader::~GLShader()
{}

/** Assignment operator - this is fast as this class is implicitly shared */
GLShader& GLShader::operator=(const GLShader &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool GLShader::operator==(const GLShader &other) const
{
    return d.constData() == other.d.constData();
}

/** Comparison operator */
bool GLShader::operator!=(const GLShader &other) const
{
    return d.constData() != other.d.constData();
}

/** Bind this shader to the current openGL context. This will load, 
    compile and link the shader if necessary, and will then load it 
    onto the correct processor (vertex or fragment) */
void GLShader::bind() const
{
    if ( !d )
    {
        //this is a null shader - disable shaders
    }
    else
        d->bind();
}
