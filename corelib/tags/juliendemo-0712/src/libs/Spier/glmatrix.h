/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SPIER_GLMATRIX_H
#define SPIER_GLMATRIX_H

#include <QGLWidget>

#include "SireMaths/matrix.h"
#include "SireMaths/plane.h"
#include "SireMaths/quaternion.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMaths::Plane;

/** This class is an abstraction of an openGL transformation matrix. 
    This class represents openGL transformations (and rotations and scaling) 
    and can be constructed either from the SireMaths classes (SireMaths::Matrix 
    or SireMaths::Quaternion) or by loading from the current openGL render context.
 
    @author Christopher Woods
*/
class SPIER_EXPORT GLMatrix
{
public:
    GLMatrix();
    GLMatrix(const SireMaths::Matrix &mat);
    GLMatrix(const SireMaths::Quaternion &mat);
    GLMatrix(const GLMatrix &other);
    ~GLMatrix();

    Plane near() const;
    Plane far() const;
    Plane top() const;
    Plane bottom() const;
    Plane left() const;
    Plane right() const;

    static GLMatrix getModelView();
    static GLMatrix getProjection();
    static GLMatrix getModelProj();

    void glLoad() const;
    void glMult() const;

private:
    void set(const SireMaths::Matrix &mat);

    /** The actual data for the matrix */
    GLdouble mat[16];

};

}

SIRE_END_HEADER

#endif
