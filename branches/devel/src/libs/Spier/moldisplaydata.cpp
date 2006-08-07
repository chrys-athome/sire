
#include "SireMol/qhash_siremol.h"

#include "moldisplaydata.h"
#include "viewobjptr.h"
#include "glcanvas.h"
#include "cpkrep.h"
#include "viewobj.h"

#include "SireMol/editmol.h"

using namespace Spier;
using namespace SireMol;
using namespace SireStream;

MolDisplayDataPtr MolDisplayData::create(GLCanvas *canvas, const EditMol &molecule)
{
    MolDisplayDataPtr moldata = (new MolDisplayData(canvas))->self();
    
    moldata->mol = molecule;
    
    //create a CPKRep by default
    
    ViewObjPtr dispobj = (new CPKRep(moldata))->self();
    
    //add it to the list of reps
    moldata->reps.insert(QObject::tr("default"), 
              boost::dynamic_pointer_cast<MoleculeRep,ViewObj>(dispobj));
  
    //add this displayable object to the canvas
    if (canvas)
        canvas->addToDisplay(dispobj);
    
    return moldata;
}

/** Create a MolDisplayData that holds the display data for the molecule pointed to
    by 'molecule', and to display that molecule on the GLCanvas 'parent'. */
MolDisplayData::MolDisplayData(GLCanvas *parent) : cnvs(parent)
{
}

/** Destructor */
MolDisplayData::~MolDisplayData()
{
    if (cnvs)
    {
        //remove all representations from the canvas that are associated 
        //with this molecule
        QHashIterator<QString,MoleculeRepPtr> it(reps);
        while(it.hasNext())
        {
            it.next();
            cnvs->removeFromDisplay(it.value());
        }
    }
}

/** Return whether or not this is a null object (has no valid molecule) */
bool MolDisplayData::isNull() const
{
    return mol.isEmpty();
}

/** Return the shared_ptr pointer to this object. Note that this object must
    have been created via the 'new' (i.e. on the heap) or else undefined
    things will happen! */
MolDisplayDataPtr MolDisplayData::self() const
{
    MolDisplayDataPtr ptr = selfptr.lock();
    
    if (ptr.get() == 0)
    {
        MolDisplayData *nonconst_this = const_cast<MolDisplayData*>(this);
        ptr.reset(nonconst_this);
        nonconst_this->selfptr = ptr;
    }
    
    return ptr;
}

/** Select the specified atom. This does nothing if the atom 
    is not contained in the molecule, or if the atom is already
    selected. */
void MolDisplayData::select(const AtomIndex &atom)
{
    if (isNull() or selectatms.contains(atom))
        return;
        
    if (mol.contains(atom))
    {
        //add this atom to the list of selected atoms
        selectatms.insert(atom);
        
        //now change *all* representations of the molecule to show that 
        //this atom is selected
        QMutableHashIterator<QString,MoleculeRepPtr> it(reps);
        
        while (it.hasNext())
        {
            it.next();
            it.value()->select(atom);
        }
    }
}
    
/** Deselect the specified atom. This does nothing if this atom
    is not selected. */
void MolDisplayData::deselect(const AtomIndex &atom)
{
    if (selectatms.contains(atom))
    {
        //remove this atom from the list of selected atoms
        selectatms.remove(atom);
        
        //now change *all* representations of the molecule to show 
        //that this atom is no longer selected.
        QMutableHashIterator<QString,MoleculeRepPtr> it(reps);
        
        while (it.hasNext())
        {
            it.next();
            it.value()->deselect(atom);
        }
    }
}

/** Toggle the selection of this atom. This does nothing if this
    atom is not contained in the molecule. */
void MolDisplayData::toggle(const AtomIndex &atom)
{
    if (selectatms.contains(atom))
        this->deselect(atom);
    else
        this->select(atom);
}
    
/** Return whether or not this atom is selected. This will return 
    false if this atom is not in the molecule. */
bool MolDisplayData::selected(const AtomIndex &atom)
{
    return selectatms.contains(atom);
}

/** Clear the list of selected atoms. */
void MolDisplayData::clearSelection()
{
    //deselect all of the selected atoms
    QList<AtomIndex> selected = selectatms.toList();
        
    foreach( AtomIndex atom, selected )
    {
        this->deselect(atom);
    }
}
