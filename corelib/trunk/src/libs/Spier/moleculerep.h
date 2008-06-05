#ifndef SPIER_MOLECULEREP_H
#define SPIER_MOLECULEREP_H

#include <QVector>

#include "viewobj.h"

#include "glsphere.h"
#include "glcylinder.h"
#include "materialptr.h"
#include "glmatrix.h"

#include "moldisplaydataptr.h"

#include "SireMol/editmol.h"
#include "SireMol/atom.h"
#include "SireMol/atomset.h"
#include "SireMol/bond.h"
#include "SireMol/bondset.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMol::EditMol;
using SireMol::AtomSet;
using SireMol::Atom;
using SireMol::AtomIndex;
using SireMol::BondList;

/**
This is the base class of all molecular representations.
 
@author Christopher Woods
*/
class SPIER_EXPORT MoleculeRep : public ViewObj
{

friend class MolSelectID;  //so can call 'highlight'

public:
    MoleculeRep(const MolDisplayDataPtr &ptr);
    //MoleculeRep(const MolDisplayDataPtr &ptr, const AtomIndexSet &dispatms);
    ~MoleculeRep();

    AtomSet renderAtoms() const;

    MolDisplayDataPtr parent() const;
    EditMol molecule() const;
    
    virtual void select(const AtomIndex &atom)=0;
    virtual void deselect(const AtomIndex &atom)=0;
    
    bool isNull() const;

protected:

    virtual void highlight(const Atom &atom)=0;

    /** Weak pointer to the parent MolDisplayData that holds the information
        about the molecule */
    MolDisplayDataWeakPtr prnt;

    /** All of the atoms that are to be rendered from this molecule */
    AtomSet renderatms;

};

/** Return the pointer to the parent MolDisplayData - this will be null
    if there is no parent */
inline MolDisplayDataPtr MoleculeRep::parent() const
{
    return prnt.lock();
}

/** Return the list of atoms that are rendered in this representation */
inline AtomSet MoleculeRep::renderAtoms() const
{
    if (not isNull())
        return renderatms;
    else
        return AtomSet();
}

}

SIRE_END_HEADER

#endif
