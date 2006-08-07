#ifndef SPIER_GLTEXTURE_H
#define SPIER_GLTEXTURE_H

#include <QSharedDataPointer>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class GLTextureData;

/**
This class abstracts an openGL texture. The texture may be 1, 2 or 3 dimensional. This class contains all of the code necessary to load and bind the texture.
 
@author Christopher Woods
*/
class SPIER_EXPORT GLTexture
{
public:
    GLTexture();
    ~GLTexture();

    void bind() const;
    
    GLTexture& operator=(const GLTexture &other);
    
    bool operator==(const GLTexture &other) const;
    bool operator!=(const GLTexture &other) const;
    
private:
    /** Pointer to the implicitly shared data for this texture */
    QSharedDataPointer<GLTextureData> d;
};

}

SIRE_END_HEADER

#endif
