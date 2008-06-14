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
