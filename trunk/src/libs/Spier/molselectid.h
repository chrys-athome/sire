#ifndef SPIER_MOLSELECTID_H
#define SPIER_MOLSELECTID_H

#include "selectid.h"
#include "moleculerep.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This is a specialisation of SelectIDBase that is used to represent a selection within a Molecule.
 
@author Christopher Woods
*/
class MolSelectID : public SelectIDBase
{
public:
    MolSelectID();
    MolSelectID(const ViewObjPtr &viewobj, const Atom &atom);
    
    ~MolSelectID();

    void highlight();
    
    void select();
    void deselect();
    void toggle();
    
    bool selected() const;

    bool operator==(const SelectIDBase &other) const;
    bool operator!=(const SelectIDBase &other) const;

    QString toString() const;

    const Atom& atom() const;
    EditMol molecule() const;

private:

    /** Raw pointer version of 'selectobj' that is cast to a
        MoleculeRep */
    MoleculeRep *molselectobj;
    
    /** The selected atom */
    Atom selectatm;

};

}

SIRE_END_HEADER

#endif
