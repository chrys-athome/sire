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
