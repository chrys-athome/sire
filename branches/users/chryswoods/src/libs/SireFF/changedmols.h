#ifndef SIREFF_CHANGEDMOLS_H
#define SIREFF_CHANGEDMOLS_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

/**
This holds the list of molecules that have changed. This includes molecules whose parameters have changed, and molecules that have been added or removed.

@author Christopher Woods
*/
class SIREFF_EXPORT ChangedMols
{
public:
    ChangedMols();
    ~ChangedMols();

};

}

SIRE_END_HEADER

#endif
