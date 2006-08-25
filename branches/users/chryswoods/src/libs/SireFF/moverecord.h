#ifndef SIREFF_MOVERECORD_H
#define SIREFF_MOVERECORD_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

/**
This class holds a record of the new coordinates of a moved molecule. This can hold the new coordinates of the whole molecule, as well as the new coordinates of individual residues that may be moved after the whole molecule has moved (or instead of moving the whole molecule). This class is designed to work with MovedMols.

@author Christopher Woods
*/
class MoveRecord
{
public:
    MoveRecord();
    ~MoveRecord();

private:
};

}

SIRE_END_HEADER

#endif
