/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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
