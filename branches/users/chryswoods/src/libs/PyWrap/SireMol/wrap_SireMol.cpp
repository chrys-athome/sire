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

#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#include "wrap_SireMol.h"

namespace SireMol
{
void export_Atom();
void export_NewAtom();
void export_AtomSet();
void export_AtomVector();
void export_AtomList();
void export_AtomNumSet();
void export_AtomNumVector();
void export_AtomNumList();
void export_AtomSelection();
void export_CutGroup();
void export_CutGroupSet();
void export_IndexList();
void export_IndexSet();
void export_IndexVector();
void export_IDTypes();
void export_IDMolAtom();
void export_Internals();
void export_Bonds();
void export_Angles();
void export_Dihedrals();
void export_Impropers();
void export_EditMol();
void export_EditMolList();
void export_EditRes();
void export_Molecule();
void export_MoleculeBonds();
void export_MoleculeInfo();
void export_Residue();
void export_ResidueBonds();
void export_ResidueInfo();
void export_Property();

void export_QList_Molecule();

void SIREMOL_EXPORT export_SireMol()
{
    export_Atom();
    export_NewAtom();
    export_AtomList();
    export_AtomSet();
    export_AtomVector();
    export_AtomNumSet();
    export_AtomNumVector();
    export_AtomNumList();
    export_AtomSelection();
    export_CutGroup();
    export_CutGroupSet();
    export_EditMol();
    export_EditRes();
    export_IDMolAtom();
    export_IndexList();
    export_IndexSet();
    export_IndexVector();
    export_IDTypes();
    export_Internals();
    export_Bonds();
    export_Angles();
    export_Dihedrals();
    export_Impropers();
    export_Molecule();
    export_MoleculeBonds();
    export_MoleculeInfo();
    export_Property();
    export_Residue();
    export_ResidueBonds();
    export_ResidueInfo();

    export_QList_Molecule();
}

}
