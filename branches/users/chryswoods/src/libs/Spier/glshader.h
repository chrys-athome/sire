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
