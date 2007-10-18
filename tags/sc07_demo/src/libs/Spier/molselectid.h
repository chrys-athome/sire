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
