#ifndef SIREMOL_ATOMID_H
#define SIREMOL_ATOMID_H

#include "index.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify an Atom via its index. 
    (e.g. the fifth atom in a residue)
    
    Be careful not to confuse this with AtomNum, which is a 
    user-supplied ID number for an atom (AtomID is to 
    AtomNum as ResID is to ResNum)
    
    @author Christopher Woods
*/
typedef Index AtomID;

}

SIRE_END_HEADER

#endif
