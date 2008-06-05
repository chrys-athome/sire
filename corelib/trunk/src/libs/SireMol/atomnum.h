#ifndef SIREMOL_ATOMNUM_H
#define SIREMOL_ATOMNUM_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify an atom by the user-supplied
    atom number (this is typically the number assigned to the 
    atom from the PDB or other coordinate file)
    
    Be careful not to confuse this with AtomID, which is the
    index of the atom in the residue or CutGroup (e.g. the
    fifth atom in the residue would have AtomID '4' but has
    whatever AtomNum the user supplied.
    
    Each atom can be given an identifying number in a molecule, much like how every 
    residue is given an identifying number (ResNum). However, the atom number doesn't 
    have the same importance, as the atoms are identified by name. The atom number 
    may sometimes be used for parametisation, or by file readers to identify bonds.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomNum : public ID<AtomNum>
{

public:
    AtomNum() : ID<AtomNum>()
    {}
    
    AtomNum(quint32 id) : ID<AtomNum>(id)
    {}
    
    AtomNum(const AtomNum &other) : ID<AtomNum>(other)
    {}
    
    ~AtomNum()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::AtomNum, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
