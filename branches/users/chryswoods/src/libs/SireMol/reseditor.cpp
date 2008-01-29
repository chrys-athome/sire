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

#include "reseditor.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

//instantiate Editor<Residue> fully
template class Editor<Residue>;
//template class Editor< Selector<Residue> >;

//////////
////////// Implementation of ResEditor
//////////

static const RegisterMetaType<ResEditor> r_reseditor;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const ResEditor &reseditor)
{
    writeHeader(ds, r_reseditor, 1);
    
    ds << static_cast<const Editor<Residue>&>(reseditor);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       ResEditor &reseditor)
{
    VersionID v = readHeader(ds, r_reseditor);
    
    if (v == 1)
    {
        ds >> static_const<Editor<Residue>&>(reseditor);
    }
    else
        throw version_error( v, "1", r_reseditor, CODELOC );
        
    return ds;
}

/** Null constructor */
ResEditor::ResEditor() : Editor<Residue>(), uid(0)
{}

/** Construct an editor that edits a copy of the residue 'residue' */
ResEditor::ResEditor(const Residue &residue) : Editor<Residue>(residue)
{
    uid = this->getUID(residue.index());
}

/** Copy constructor */
ResEditor::ResEditor(const ResEditor &other) 
          : Editor<Residue>(other), uid(other.uid)
{}

/** Destructor */
ResEditor::~ResEditor()
{}

/** Copy assignment so that this will edit a copy of 'residue */
ResEditor& ResEditor::operator=(const Residue &residue)
{
    Editor<Residue>::operator=(residue);
    uid = this->getUID(residue.index());
    
    return *this;
}

/** Copy assignment operator */
ResEditor& ResEditor::operator=(const ResEditor &other)
{
    Editor<Residue>::operator=(other);
    uid = other.uid;
    
    return *this;
}

ChainEditor ResEditor::chain() const;
MolEditor ResEditor::molecule() const;

AtomEditor ResEditor::atom(int i) const;
AtomEditor ResEditor::atom(const AtomID &atomid) const;

AtomEditor ResEditor::select(const AtomID &atomid) const;
AtomEditor ResEditor::select(int i) const;

ResEditor& ResEditor::rename(const ResName &name);
ResEditor& ResEditor::renumber(ResNum number);

ResStructureEditor ResEditor::reindex(ResIdx index) const;

MolStructureEditor ResEditor::remove() const;

ResStructureEditor ResEditor::reparent(ChainIdx chainidx) const;
ResStructureEditor ResEditor::reparent(const ChainID &chainid) const;

AtomStructureEditor ResEditor::add(const AtomName &atomname) const;
AtomStructureEditor ResEditor::add(AtomNum atomnum) const;

ResStructureEditor ResEditor::remove(const AtomID &atomid) const;

ResStructureEditor ResEditor::remove(int i) const;
ResStructureEditor ResEditor::remove(const std::slice &s) const;

ResStructureEditor ResEditor::transfer(const AtomID &atomid, const ResID &resid) const;
ResStructureEditor ResEditor::transfer(int i, const ResID &resid) const;
ResStructureEditor ResEditor::transfer(const std::slice &s, const ResID &resid) const;

ResStructureEditor ResEditor::transferAll(const ResID &resid) const;

//////////
////////// Implementation of ResStructureEditor
//////////

static const RegisterMetaType<ResStructureEditor> r_restructeditor;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const ResStructureEditor &reseditor)
{
    writeHeader(ds, r_resstructeditor, 1);
    
    ds << reseditor.uid
       << static_cast<const StructureEditor&>(reseditor);
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       ResStructureEditor &reseditor)
{
    VersionID v = readHeader(ds, r_resstructeditor);
    
    if (v == 1)
    {
        ds >> reseditor.uid
           >> static_cast<StructureEditor&>(reseditor);
    }
    else
        throw version_error( v, "1", r_resstructeditor, CODELOC );
        
    return ds;
}

ResStructureEditor::ResStructureEditor();
ResStructureEditor::ResStructureEditor(const Residue &residue);
ResStructureEditor::ResStructureEditor(const StructureEditor &data, ResIdx residx);

ResStructureEditor::ResStructureEditor(const ResStructureEditor &other);

ResStructureEditor::~ResStructureEditor();

ResStructureEditor& ResStructureEditor::operator=(const Residue &residue);
ResStructureEditor& ResStructureEditor::operator=(const ResStructureEditor &other);

ChainStructureEditor ResStructureEditor::chain();
MolStructureEditor ResStructureEditor::molecule();

AtomStructureEditor ResStructureEditor::atom(int i);
AtomStructureEditor ResStructureEditor::atom(const AtomID &atomid);

AtomStructureEditor ResStructureEditor::select(int i);
AtomStructureEditor ResStructureEditor::select(const AtomID &atomid);

ResStructureEditor& ResStructureEditor::rename(const ResName &name);
ResStructureEditor& ResStructureEditor::renumber(ResNum number);

ResStructureEditor& ResStructureEditor::reindex(ResIdx index);

MolStructureEditor ResStructureEditor::remove();

ResStructureEditor& ResStructureEditor::reparent(ChainIdx chainidx);
ResStructureEditor& ResStructureEditor::reparent(const ChainID &chainid);

AtomStructureEditor ResStructureEditor::add(const AtomName &atomname);
AtomStructureEditor ResStructureEditor::add(AtomNum atomnum);

ResStructureEditor& ResStructureEditor::remove(const AtomID &atomid);

ResStructureEditor& ResStructureEditor::remove(int i);
ResStructureEditor& ResStructureEditor::remove(const std::slice &s);

ResStructureEditor& ResStructureEditor::transfer(const AtomID &atomid, const ResID &resid);
ResStructureEditor& ResStructureEditor::transfer(int i, const ResID &resid);
ResStructureEditor& ResStructureEditor::transfer(const std::slice &s, const ResID &resid);

ResStructureEditor& ResStructureEditor::transferAll(const ResID &resid);
