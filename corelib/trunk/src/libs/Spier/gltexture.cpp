
#include "ThirdPartyGUI/glew.h"

#include "gltexture.h"

#include <QGLWidget>
#include <QSharedData>

namespace Spier
{
class GLTextureData : public QSharedData
{
public:
    GLTextureData();
    ~GLTextureData();
    
    void bind() const;
    
private:

};
}

using namespace Spier;

GLTextureData::GLTextureData() : QSharedData()
{}

GLTextureData::~GLTextureData()
{}

void GLTextureData::bind() const
{}

////////////////
//////////////// Implementation of GLTexture
////////////////

GLTexture::GLTexture()
{}

GLTexture::~GLTexture()
{}

/** Assignment operator - this will be fast as this class is implicitly shared */
GLTexture& GLTexture::operator=(const GLTexture &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool GLTexture::operator==(const GLTexture &other) const
{
    return d.constData() == other.d.constData();
}

/** Comparison operator */
bool GLTexture::operator!=(const GLTexture &other) const
{
    return d.constData() != other.d.constData();
}

/** Bind this texture to the current openGL context */
void GLTexture::bind() const
{
    if ( !d )
    {
        //this is a null texture - disable texturing
    }
    else
        d->bind();
}
