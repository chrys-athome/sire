#ifndef SPIER_GLSHADER_H
#define SPIER_GLSHADER_H

#include <QSharedDataPointer>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class GLShaderData;

/**
This class abstracts an openGL vertex and pixel shader. This class contains all of the code necessary to load and bind these shaders. This class will be ignored if shaders are not supported or are disabled.
 
@author Christopher Woods
*/
class SPIER_EXPORT GLShader
{
public:
    GLShader();
    ~GLShader();

    void bind() const;

    GLShader& operator=(const GLShader &other);

    bool operator==(const GLShader &other) const;
    bool operator!=(const GLShader &other) const;

private:
    /** Pointer to the implicitly shared data for this shader */
    QSharedDataPointer<GLShaderData> d;
};

}

SIRE_END_HEADER

#endif
