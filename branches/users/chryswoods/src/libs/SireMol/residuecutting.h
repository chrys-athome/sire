/**
  * @file
  *
  * C++ Interface: ResidueCutting
  *
  * Description: Interface for ResidueCutting
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIREMOL_RESIDUECUTTING_H
#define SIREMOL_RESIDUECUTTING_H

#include "convertfunction.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/**
This is the ConvertFunction that is used to create a molecule that uses
residue-based cutting - i.e. each residue is in its own CutGroup.

@author
*/
class SIREMOL_EXPORT ResidueCutting
{
public:
    ResidueCutting();

    ~ResidueCutting();

};

};

SIRE_END_HEADER

#endif
