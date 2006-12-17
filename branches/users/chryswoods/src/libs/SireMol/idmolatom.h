#ifndef SIREMOL_IDMOLATOM_H
#define SIREMOL_IDMOLATOM_H

#include "idtypes.h"
#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class MoleculeInfo;

/** This small and simple class is used to provide a single
    argument for Molecule functions that expect an Atom
    identification (e.g. anything specifying or returning
    the property of a single atom). Using this class,
    the Molecule class can expose only a single function,
    thereby preventing an explosion of over-ridden functions.
    
    This class is designed to be used only by functions
    of Molecule and MoleculeData, and is not safe for 
    use by any other class.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT IDMolAtom
{
public:
    IDMolAtom(const CGAtomID &cgatomid);
    IDMolAtom(const CGNumAtomID &cgatomid);
    IDMolAtom(const ResNumAtomID &resatomid);
    IDMolAtom(const ResIDAtomID &resatomid);
    IDMolAtom(const AtomIndex &atomindex);
    IDMolAtom(const AtomID &atomid);
    
    IDMolAtom(const IDMolAtom &other);
    
    ~IDMolAtom();
    
    const CGAtomID& index(const MoleculeInfo &molinfo) const;
    
    bool contains(const MoleculeInfo &molinfo) const;
    
private:

    enum IndexType { CGIDATOMID    = 0,
                     CGNUMATOMID   = 1,
                     RESNUMATOMID  = 2,
                     RESIDATOMID   = 3,
                     ATOMINDEX     = 4,
                     ATOMID        = 5
                   };

    /** The type of index held */
    IndexType indextype;

    /** Pointer to the index */
    union
    {
        const CGAtomID *cgatomid;
        const CGNumAtomID *cgnumatomid;
        const ResNumAtomID *resnumatomid;
        const ResIDAtomID *residatomid;
        const AtomIndex *atomindex;
        const AtomID *atomid;
    } d;
};

}

SIRE_END_HEADER

#endif
