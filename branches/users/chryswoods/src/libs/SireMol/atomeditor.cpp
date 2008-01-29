/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "atomeditor.h"
#include "atom.h"

#include "moleditor.h"

#include "selector.hpp"
#include "mover.hpp"

#include "residx.h"
#include "cgidx.h"
#include "segidx.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

//fully instantiate the Editor<Atom> and Editor< Selector<Atom> > classes
template class Editor<Atom>;
template class Editor< Selector<Atom> >;

/////////
///////// Implementation of AtomEditor
/////////

static const RegisterMetaType<AtomEditor> r_atomeditor;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const AtomEditor &atomeditor)
{
    writeHeader(ds, r_atomeditor, 1);
    
    ds << static_cast<const Editor<Atom>&>(atomeditor);
    
    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       AtomEditor &atomeditor)
{
    VersionID v = readHeader(ds, r_atomeditor);
    
    if (v == 1)
    {
        ds >> static_cast<Editor<Atom>&>(atomeditor);
    }
    else
        throw version_error(v, "1", r_atomeditor, CODELOC);

    return ds;
}

/** Null constructor */
AtomEditor::AtomEditor() : Editor<Atom>()
{}

/** Construct an editor that edits a copy of 'atom' */
AtomEditor::AtomEditor(const Atom &atom)
           : Editor<Atom>(atom)
{}

/** Copy constructor */
AtomEditor::AtomEditor(const AtomEditor &other)
           : Editor<Atom>(other)
{}

/** Destructor */
AtomEditor::~AtomEditor()
{}

/** Set this editor so that it is editing a copy of 'atom' */
AtomEditor& AtomEditor::operator=(const Atom &atom)
{
    Editor<Atom>::operator=(atom);
    return *this;
}

/** Copy assignment operator */
AtomEditor& AtomEditor::operator=(const AtomEditor &other)
{
    Editor<Atom>::operator=(other);
    return *this;
}

/** Remove this atom from the molecule, returning an editor
    that can further edit the structure of the molecule */
MolStructureEditor AtomEditor::remove() const
{
    MolStructureEditor moleditor( this->data() );    
    moleditor.remove(this->index());
    
    return moleditor;
}

/** Reparent this atom so that it will be placed into the CutGroup
    with index 'cgidx' - this returns the updated atom in 
    an AtomStructureEditor, which is optimised for further
    editing of the molecule structure
    
    \throw SireError::invalid_index
*/
AtomStructureEditor AtomEditor::reparent(CGIdx cgidx) const
{
    AtomStructureEditor editor(*this);
    editor.reparent( cgidx );
    return editor;
}

/** Reparent this atom so that it will be placed into the CutGroup
    with ID 'cgid' - this returns the updated atom in 
    an AtomStructureEditor, which is optimised for further
    editing of the molecule structure
    
    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
    \throw SireError::invalid_index
*/
AtomStructureEditor AtomEditor::reparent(const CGID &cgid) const
{
    AtomStructureEditor editor(*this);
    editor.reparent(cgid);
    return editor;
}

/** Reparent this atom so that it will be placed into the residue
    with index 'residx' - this returns the updated atom in 
    an AtomStructureEditor, which is optimised for further
    editing of the molecule structure
    
    \throw SireError::invalid_index
*/
AtomStructureEditor AtomEditor::reparent(ResIdx residx) const
{
    AtomStructureEditor editor(*this);
    editor.reparent(residx);
    return editor;
}

/** Reparent this atom so that it will be placed into the residue
    with ID 'resid' - this returns the updated atom in 
    an AtomStructureEditor, which is optimised for further
    editing of the molecule structure
    
    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
AtomStructureEditor AtomEditor::reparent(const ResID &resid) const
{
    AtomStructureEditor editor(*this);
    editor.reparent(resid);
    return editor;
}

/** Reparent this atom so that it will be placed into the segment
    with index 'segidx' - this returns the updated atom in 
    an AtomStructureEditor, which is optimised for further
    editing of the molecule structure
    
    \throw SireError::invalid_index
*/
AtomStructureEditor AtomEditor::reparent(SegIdx segidx) const
{
    AtomStructureEditor editor(*this);
    editor.reparent(segidx);
    return editor;
}

/** Reparent this atom so that it will be placed into the segment
    with ID 'segid' - this returns the updated atom in 
    an AtomStructureEditor, which is optimised for further
    editing of the molecule structure
    
    \throw SireMol::missing_segment
    \throw SireMol::duplicate_segment
    \throw SireError::invalid_index
*/
AtomStructureEditor AtomEditor::reparent(const SegID &segid) const
{
    AtomStructureEditor editor(*this);
    editor.reparent(segid);
    return editor;
}

/////////
///////// Implementation of AtomStructureEditor
/////////
