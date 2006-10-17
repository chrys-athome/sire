#ifndef SIREFF_FORCETABLE_H
#define SIREFF_FORCETABLE_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

/**
A ForceTable is a workspace within which all of the forces acting on a molecule may be stored. A ForceTable is used as storing the forces requires lots of memory, and continually creating a deleting such large amouts of memory would be inefficient. Also, using a ForceTable allows for forces to be accumalated directly, rather than requiring intermediate storage space for the individual components.

@author Christopher Woods

*/
class ForceTable
{
public:
    ForceTable();
    ~ForceTable();

};

}

SIRE_END_HEADER

#endif
