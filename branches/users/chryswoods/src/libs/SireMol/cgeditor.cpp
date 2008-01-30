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

#include "cgeditor.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

// fully instantiate Editor<CutGroup>
template class Editor<CutGroup>;
//template class Selector< Editor<CutGroup> >;

////////
//////// Implementation of CGEditor
////////

static const RegisterMetaType<CGEditor> r_cgeditor;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const CGEditor &cgeditor)
{
    writeHeader(ds, r_cgeditor, 1);
    
    ds << static_cast<const Editor<CutGroup>&>(cgeditor);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       CGEditor &cgeditor)
{
    VersionID v = readHeader(ds, r_cgeditor);
    
    if (v == 1)
    {
        ds >> static_cast<Editor<CutGroup>&>(cgeditor);
    }
    else
        throw version_error( v, "1", r_cgeditor, CODELOC );
        
    return ds;
}

/** Null constructor */
CGEditor::CGEditor();

/** Construct to edit a copy of the CutGroup 'cutgroup' */
CGEditor::CGEditor(const CutGroup &cutgroup);

/** Copy constructor */
CGEditor::CGEditor(const CGEditor &other);

/** Destructor */
CGEditor::~CGEditor();

/** Assign this editor so that it edits a copy of the CutGroup 'cutgroup' */
CGEditor& CGEditor::operator=(const CutGroup &cutgroup);

/** Copy assignment operator */
CGEditor& CGEditor::operator=(const CGEditor &other);

/** Return an editor for the molecule that contains this CutGroup */
MolEditor CGEditor::molecule() const;

/** Return an editor for the ith atom of this CutGroup

    \throw SireError::invalid_index
*/
AtomEditor CGEditor::atom(int i) const;

/** Return an editor for the atom that matches the ID 'atomid' in
    this CutGroup
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
AtomEditor CGEditor::atom(const AtomID &atomid) const;

/** Return an editor for the ith atom of this CutGroup

    \throw SireError::invalid_index
*/
AtomEditor CGEditor::select(const AtomID &atomid) const;

/** Return an editor for the atom that matches the ID 'atomid' in
    this CutGroup
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
AtomEditor CGEditor::select(int i) const;

/** Rename this CutGroup to 'newname' */
CGEditor& CGEditor::rename(const CGName &newname);

/** Renumber this CutGroup to 'newnum' */
CGEditor& CGEditor::renumber(CGNum newnum);

/** Move this CutGroup to index 'newidx' - this will move it
    to the start or end if this index is out of range */
CGStructureEditor CGEditor::reindex(CGIdx newidx) const;

/** Complete remove this CutGroup, and return an editor
    for the molecule that contained it */
MolStructureEditor CGEditor::remove() const;

/** Add an atom called 'atomname' to this CutGroup and return
    an editor for that atom */
AtomStructureEditor CGEditor::add(const AtomName &atomname) const;

/** Add an atom with number 'atomnum' to this CutGroup and return
    an editor for that atom */
AtomStructureEditor CGEditor::add(AtomNum atomnum) const;

/** Completely remove all atoms that match the ID 'atomid' from 
    this CutGroup 
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
CGStructureEditor CGEditor::remove(const AtomID &atomid) const;

/** Remove the ith atom from this CutGroup

    \throw SireError::invalid_index
*/
CGStructureEditor CGEditor::remove(int i) const;

/** Transfer all atoms that match the ID 'atomid' in this CutGroup 
    to the CutGroup that matches the ID 'cgid'
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
    \throw SireError::invalid_index
*/
CGStructureEditor CGEditor::transfer(const AtomID &atomid, 
                                     const CGID &cgid) const
                                     
/** Transfer the ith atom of this CutGroup into the CutGroup identified
    by the ID 'cgid'
    
    \throw SireError::invalid_index
*/
CGStructureEditor CGEditor::transfer(int i, const CGID &cgid) const;

/** Completely transfer all of the atoms in this CutGroup to 
    the CutGroup that matches the ID 'cgid'
    
    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
    \throw SireError::invalid_index
*/
CGStructureEditor CGEditor::transferAll(const CGID &cgid) const;

/** Commit the changes made by this editor and return the 
    updated CutGroup */
CutGroup CGEditor::commit() const;

////////
//////// Implementation of CGStructureEditor
////////

static const RegisterMetaType<CGStructureEditor> r_cgstructeditor;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const CGStructureEditor &cgeditor)
{
    writeHeader(ds, r_cgstructeditor, 1);
    
    ds << cgeditor.uid
       << static_cast<const StructureEditor&>(cgeditor);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       CGStructureEditor &cgeditor)
{
    VersionID v = readHeader(ds, r_cgstructeditor);
    
    if (v == 1)
    {
        ds >> cgeditor.uid
           >> static_cast<StructureEditor&>(cgeditor);
    }
    else
        throw version_error( v, "1", r_cgstructeditor, CODELOC );
        
    return ds;
}

/** Null constructor */
CGStructureEditor::CGStructureEditor();

/** Construct to edit a copy of the CutGroup 'cutgroup' */
CGStructureEditor::CGStructureEditor(const CutGroup &cutgroup);

/** Construct to edit the CutGroup at index 'cgidx' in the molecule 
    also being edited in 'data'
    
    \throw SireError::invalid_index
*/
CGStructureEditor::CGStructureEditor(const StructureEditor &data, CGIdx cgidx);

/** Copy constructor */
CGStructureEditor::CGStructureEditor(const CGStructureEditor &other);

/** Destructor */
CGStructureEditor::~CGStructureEditor();

/** Assign this editor so that it edits a copy of the CutGroup 'cutgroup' */
CGStructureEditor& CGStructureEditor::operator=(const CutGroup &cutgroup);

/** Copy assignment operator */
CGStructureEditor& CGStructureEditor::operator=(const CGStructureEditor &other);

/** Return the name of this CutGroup */
const CGName& CGStructureEditor::name() const;

/** Return the number of this CutGroup */
CGNum CGStructureEditor::number() const;

/** Return the index of this CutGroup in the molecule */
CGIdx CGStructureEditor::index() const;

/** Return the number of atoms in this CutGroup (could be zero!) */
int CGStructureEditor::nAtoms() const;

/** Return an editor for the molecule that contains this CutGroup */
MolStructureEditor CGStructureEditor::molecule();

/** Return an editor for the ith atom of this CutGroup

    \throw SireError::invalid_index
*/
AtomStructureEditor CGStructureEditor::atom(int i);

/** Return an editor for the atom that matches the ID 'atomid' in
    this CutGroup
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
AtomStructureEditor CGStructureEditor::atom(const AtomID &atomid);

/** Return an editor for the ith atom of this CutGroup

    \throw SireError::invalid_index
*/
AtomStructureEditor CGStructureEditor::select(int i);

/** Return an editor for the atom that matches the ID 'atomid' in
    this CutGroup
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
AtomStructureEditor CGStructureEditor::select(const AtomID &atomid);

/** Rename this CutGroup to 'newname' */
CGStructureEditor& CGStructureEditor::rename(const CGName &name);

/** Renumber this CutGroup to 'newnum' */
CGStructureEditor& CGStructureEditor::renumber(CGNum number);

/** Move this CutGroup to index 'newidx' - this will move it
    to the start or end if this index is out of range */
CGStructureEditor& CGStructureEditor::reindex(CGIdx index);

/** Complete remove this CutGroup, and return an editor
    for the molecule that contained it */
MolStructureEditor CGStructureEditor::remove();

/** Add an atom called 'atomname' to this CutGroup and return
    an editor for that atom */
AtomStructureEditor CGStructureEditor::add(const AtomName &atomname);

/** Add an atom with number 'atomnum' to this CutGroup and return
    an editor for that atom */
AtomStructureEditor CGStructureEditor::add(AtomNum atomnum);

/** Completely remove all atoms that match the ID 'atomid' from 
    this CutGroup 
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
CGStructureEditor& CGStructureEditor::remove(const AtomID &atomid);

/** Remove the ith atom from this CutGroup

    \throw SireError::invalid_index
*/
CGStructureEditor& CGStructureEditor::remove(int i);

/** Transfer all atoms that match the ID 'atomid' in this CutGroup 
    to the CutGroup that matches the ID 'cgid'
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
    \throw SireError::invalid_index
*/
CGStructureEditor& CGStructureEditor::transfer(const AtomID &atomid, const CGID &cgid);

/** Transfer the ith atom of this CutGroup into the CutGroup identified
    by the ID 'cgid'
    
    \throw SireError::invalid_index
*/
CGStructureEditor& CGStructureEditor::transfer(int i, const CGID &cgid);

/** Completely transfer all of the atoms in this CutGroup to 
    the CutGroup that matches the ID 'cgid'
    
    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
    \throw SireError::invalid_index
*/
CGStructureEditor& CGStructureEditor::transferAll(const CGID &cgid);

/** Commit the changes made by this editor and return the 
    updated CutGroup */
CutGroup CGStructureEditor::commit() const;

/** Allow automatic casting of this editor to a CutGroup */
CGStructureEditor::operator CutGroup() const;
