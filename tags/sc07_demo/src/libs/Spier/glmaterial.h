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

#ifndef SPIER_GLMATERIAL_H
#define SPIER_GLMATERIAL_H

#include <QSharedDataPointer>

#include "SireMaths/vector.h"

#include "color.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMaths::Vector;

class GLMaterialData;

/**
This class abstract an openGL material (the collection of diffuse, ambient, specular, shininess and emission properties, set via the glMaterial command).
 
@author Christopher Woods
*/
class SPIER_EXPORT GLMaterial
{
public:
    GLMaterial();
    GLMaterial(const Vector &basecolor);
    GLMaterial(const Vector &ambient, const Vector &diffuse, const Vector &specular,
               const Vector &emission, double shininess, double opacity=1.0);
    
    GLMaterial(const Color &basecolor);
    GLMaterial(const Color &ambient, const Color &diffuse, const Color &specular, 
               const Color &emission, double shininess);
    
    GLMaterial(const GLMaterial &other);
    
    ~GLMaterial();

    GLMaterial& operator=(const GLMaterial &other);

    void setAmbient(const Vector &color, double opacity=1.0);
    void setDiffuse(const Vector &color, double opacity=1.0);
    void setAmbientDiffuse(const Vector &color, double opacity=1.0);
    
    void setSpecular(const Vector &color, double opacity=1.0);
    void setEmission(const Vector &color, double opacity=1.0);

    void setAmbient(const Color &color);
    void setDiffuse(const Color &color);
    void setAmbientDiffuse(const Color &color);
    
    void setSpecular(const Color &color);
    void setEmission(const Color &color);

    void setShininess(double shininess);

    void bind() const;

    bool operator==(const GLMaterial &other) const;
    bool operator!=(const GLMaterial &other) const;

private:
    /** Implicitly shared pointer to the data of this class */
    QSharedDataPointer<GLMaterialData> d;
};

}

SIRE_END_HEADER

#endif
