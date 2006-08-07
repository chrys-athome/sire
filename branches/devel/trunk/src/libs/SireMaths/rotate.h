#ifndef SIREMATHS_ROTATE_H
#define SIREMATHS_ROTATE_H

#include "vector.h"
#include "matrix.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{

/** Function that rotates the input vector 'input' by the rotation 
    matrix 'rotmat' about the point 'point'. The output is returned. */
inline Vector rotate(const Vector &input, const Matrix &rotmat, const Vector &point)
{
    return point + rotmat*(input-point);
}

}

SIRE_END_HEADER

#endif
