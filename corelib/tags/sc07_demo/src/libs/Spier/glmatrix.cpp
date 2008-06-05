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

#include "glmatrix.h"

#include "SireMaths/vector.h"

using namespace Spier;
using namespace SireMaths;

/** Construct an identity GLMatrix */
GLMatrix::GLMatrix()
{
    for (int i=0; i<16; ++i)
        mat[i] = 0.0;
        
    mat[0] = 1.0;
    mat[5] = 1.0;
    mat[10] = 1.0;
    mat[15] = 1.0;
}

/** Copy constructor */
GLMatrix::GLMatrix(const GLMatrix &other)
{
    for (int i=0; i<16; ++i)
        mat[i] = other.mat[i];
}

/** Construct a GLMatrix which is a copy of the SireMaths::Matrix */
GLMatrix::GLMatrix(const Matrix &matrix)
{
    this->set(matrix);
}

/** Construct a GLMatrix which is a representation of the rotation implied
    by the passed Quaternion */
GLMatrix::GLMatrix(const Quaternion &quat)
{
    this->set(quat.toMatrix());
}

/** Destructor */
GLMatrix::~GLMatrix()
{}

/** Set this matrix equal to the passed SireMaths::Matrix */
void GLMatrix::set(const Matrix &matrix)
{
    Vector col0 = matrix.column0();
    Vector col1 = matrix.column1();
    Vector col2 = matrix.column2();
    
    mat[0] = col0.x();
    mat[1] = col0.y();
    mat[2] = col0.z();
    mat[3] = 0.0;
    
    mat[4] = col1.x();
    mat[5] = col1.y();
    mat[6] = col1.z();
    mat[7] = 0.0;
    
    mat[8] = col2.x();
    mat[9] = col2.y();
    mat[10] = col2.z();
    mat[11] = 0.0;

    mat[12] = 0.0;
    mat[13] = 0.0;
    mat[14] = 0.0;
    mat[15] = 1.0;
}

/** Return the current openGL model-view matrix. This should only be called
    while rendering */
GLMatrix GLMatrix::getModelView()
{
    GLMatrix matrix;
    
    glGetDoublev(GL_MODELVIEW_MATRIX, matrix.mat);
    return matrix;
}

/** Return the current openGL projection matrix. This should only be called 
    while rendering */
GLMatrix GLMatrix::getProjection()
{
    GLMatrix matrix;
    glGetDoublev(GL_PROJECTION_MATRIX, matrix.mat);
    return matrix;
}

/** Get the product of the current projection and modelview matricies */
GLMatrix GLMatrix::getModelProj()
{
    GLMatrix modelmat, projmat;
    
    //get the projection and modelview matricies
    glGetDoublev(GL_MODELVIEW_MATRIX, modelmat.mat);
    glGetDoublev(GL_PROJECTION_MATRIX, projmat.mat);
   
    //now calculate their product
    GLMatrix result;
   
    double p00 = projmat.mat[0];
    double p01 = projmat.mat[1];
    double p02 = projmat.mat[2];
    double p03 = projmat.mat[3];
    
    double p10 = projmat.mat[4];
    double p11 = projmat.mat[5];
    double p12 = projmat.mat[6];
    double p13 = projmat.mat[7];
    
    double p20 = projmat.mat[8];
    double p21 = projmat.mat[9];
    double p22 = projmat.mat[10];
    double p23 = projmat.mat[11];
    
    double p30 = projmat.mat[12];
    double p31 = projmat.mat[13];
    double p32 = projmat.mat[14];
    double p33 = projmat.mat[15];

    for (int i=0; i<4; ++i) 
    {
        int i0 = i*4;
        int i1 = i0+1;
        int i2 = i0+2;
        int i3 = i0+3;
    
        double mi0 = modelmat.mat[i0];
        double mi1 = modelmat.mat[i1];
        double mi2 = modelmat.mat[i2];
        double mi3 = modelmat.mat[i3];
    
        result.mat[i0] = mi0*p00 + mi1*p10 + mi2*p20 + mi3*p30;
        result.mat[i1] = mi0*p01 + mi1*p11 + mi2*p21 + mi3*p31;
        result.mat[i2] = mi0*p02 + mi1*p12 + mi2*p22 + mi3*p32;
        result.mat[i3] = mi0*p03 + mi1*p13 + mi2*p23 + mi3*p33;
    }
    
    return result;
}
