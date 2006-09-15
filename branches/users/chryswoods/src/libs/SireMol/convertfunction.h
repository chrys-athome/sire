#ifndef SIREMOL_CONVERTFUNCTION_H
#define SIREMOL_CONVERTFUNCTION_H
/**
  * @file
  *
  * C++ Interface: ConvertFunction
  *
  * Description: Interface for ConvertFunction
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/**
This is the base class of the function classes that are used to convert
EditMols into Molecules.

@author Christopher Woods
*/
class SIREMOL_EXPORT ConvertFunction
{
public:
    ConvertFunction();

    ~ConvertFunction();

};

}

SIRE_END_HEADER

#endif
