#ifndef SIREMOL_RESID_H
#define SIREMOL_RESID_H

#include "index.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This is the ID type for a residue (index into an array of residues) 
    Be careful not to confuse this with ResNum, which is the identifying
    number given to the residue by the user, and used internally to 
    identify the residue
*/
typedef Index ResID;

}

SIRE_END_HEADER

#endif
