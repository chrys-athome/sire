
#include "molselectid.h"
#include "moldisplaydata.h"

#include "SireMol/editmol.h"

#include "SireError/errors.h"

using namespace Spier;

/** Construct a null MolSelectID */
MolSelectID::MolSelectID() : SelectIDBase(), molselectobj(0)
{}

/** Construct a MolSelectID that represents atom 'atm' in the molecule
    represented by the MoleculeRep pointed to by 'obj'. 'obj' must point
    to a MoleculeRep or else an exception will be thrown. 
    
    \throw SireError::invalid_cast
*/
MolSelectID::MolSelectID(const ViewObjPtr &obj, const Atom &atm)
            : SelectIDBase(obj), molselectobj(0), selectatm(atm)
{
    //cast the object to a MoleculeRep...
    if (not this->isNull())
    {
        molselectobj = dynamic_cast<MoleculeRep*>(obj.get());
        
        if (not molselectobj)
        {
            //obj is not a pointer to a MoleculeRep - this is an error!
            throw SireError::invalid_cast(QObject::tr(
                  "Cannot create a MolSelectID from anything other than a MoleculeRep!"),CODELOC);
        }
    }
}

/** Destructor */
MolSelectID::~MolSelectID()
{}

/** Return the atom of this selection */
const Atom& MolSelectID::atom() const
{
    if (isNull() or molselectobj->isNull())
        throw SireError::nullptr_error(QObject::tr(
              "Cannot get the atom from a null MolSelectID"), CODELOC);

    return selectatm;
}

/** Return the molecule of this selection */
EditMol MolSelectID::molecule() const
{
    if (isNull() or molselectobj->isNull())
        throw SireError::nullptr_error(QObject::tr(
              "Cannot get the atom from a null MolSelectID"), CODELOC);

    return molselectobj->molecule();
}

/** Select the object */
void MolSelectID::select()
{
    if (isNull() or molselectobj->isNull())
        return;
    
    molselectobj->parent()->select(selectatm);
}

/** Deselect the object */
void MolSelectID::deselect()
{
    if (isNull() or molselectobj->isNull())
        return;
    
    molselectobj->parent()->deselect(selectatm);
}

/** Toggle the selection of the object */
void MolSelectID::toggle()
{
    if (isNull() or molselectobj->isNull())
        return;
        
    molselectobj->parent()->toggle(selectatm);
}

/** Highlight the object - this can only be called while rendering the scene (within Viewer::paintGL) */
void MolSelectID::highlight()
{
    if (isNull())
        return;
        
    molselectobj->highlight(selectatm);
}

/** Return whether or not this atom is selected */
bool MolSelectID::selected() const
{
    if (isNull() or molselectobj->isNull())
        return false;
    else
        return molselectobj->parent()->selected(selectatm);
}

/** Comparison operator */
bool MolSelectID::operator==(const SelectIDBase &other) const
{
    const MolSelectID *ptr = dynamic_cast<const MolSelectID*>(&other);
    
    if (ptr)
        return molselectobj == ptr->molselectobj and selectatm == ptr->selectatm;
    else
        return false;
}

/** Comparison operator */
bool MolSelectID::operator!=(const SelectIDBase &other) const
{
    return not (*this == other);
}

/** Return a string representation of this selection */
QString MolSelectID::toString() const
{
    if (isNull())
        return QObject::tr("Nothing");
    else
        return QObject::tr("Atom \"%1\" in residue \"%2 (%3)\" in molecule \"%4\"")
                      .arg(selectatm.name(), molselectobj->molecule().residueName(selectatm.resNum()))
                      .arg(selectatm.resNum().toString(),molselectobj->molecule().name());
}
