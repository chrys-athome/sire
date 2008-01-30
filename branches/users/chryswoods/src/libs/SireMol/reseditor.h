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

#ifndef SIREMOL_RESEDITOR_H
#define SIREMOL_RESEDITOR_H

#include "structureeditor.h"
#include "residue.h"
#include "editor.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResEditor;
class ResStructureEditor;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResEditor&);
QDataStream& operator>>(QDataStream&, SireMol::ResEditor&);

QDataStream& operator<<(QDataStream&, const SireMol::ResStructureEditor&);
QDataStream& operator>>(QDataStream&, SireMol::ResStructureEditor&);

namespace SireMol
{

class MolStructureEditor;
class SegStructureEditor;
class ChainStructureEditor;
class ResStructureEditor;
class CGStructureEditor;
class AtomStructureEditor;

class MolEditor;
class SegEditor;
class ChainEditor;
class ResEditor;
class CGEditor;
class AtomEditor;

class CGIdx;
class CGID;
class ResIdx;
class ResID;

/** This class is used to edit the non-structural parts of a residue

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResEditor : public Editor<Residue>
{

friend QDataStream& ::operator<<(QDataStream&, const ResEditor&);
friend QDataStream& ::operator>>(QDataStream&, ResEditor&);

public:
    ResEditor();
    
    ResEditor(const Residue &residue);
    
    ResEditor(const ResEditor &other);
    
    ~ResEditor();
    
    ResEditor& operator=(const Residue &residue);
    ResEditor& operator=(const ResEditor &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ResEditor>() );
    }
    
    const char *what() const
    {
        return ResEditor::typeName();
    }
    
    ResEditor* clone() const
    {
        return new ResEditor(*this);
    }
    
    ChainEditor chain() const;
    MolEditor molecule() const;
    
    AtomEditor atom(int i) const;
    AtomEditor atom(const AtomID &atomid) const;
    
    AtomEditor select(const AtomID &atomid) const;
    AtomEditor select(int i) const;
    
    ResEditor& rename(const ResName &name);
    ResEditor& renumber(ResNum number);
    
    ResStructureEditor reindex(ResIdx index) const;
    
    MolStructureEditor remove() const;
    
    ResStructureEditor reparent(const ChainID &chainid) const;

    AtomStructureEditor add(const AtomName &atomname) const;
    AtomStructureEditor add(AtomNum atomnum) const;
    
    ResStructureEditor remove(const AtomID &atomid) const;

    ResStructureEditor remove(int i) const;
    
    ResStructureEditor transfer(const AtomID &atomid, const ResID &resid) const;
    ResStructureEditor transfer(int i, const ResID &resid) const;
    
    ResStructureEditor transferAll(const ResID &resid) const;
    
    Residue commit() const;
};

/** This is the class used to edit a residue's structure 

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResStructureEditor : public StructureEditor
{

friend QDataStream& ::operator<<(QDataStream&, const ResStructureEditor&);
friend QDataStream& ::operator>>(QDataStream&, ResStructureEditor&);

public:
    ResStructureEditor();
    ResStructureEditor(const Residue &residue);
    ResStructureEditor(const StructureEditor &data, ResIdx residx);
    
    ResStructureEditor(const ResStructureEditor &other);
    
    ~ResStructureEditor();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ResStructureEditor>() );
    }
    
    const char* what() const
    {
        return ResStructureEditor::typeName();
    }
    
    ResStructureEditor* clone() const
    {
        return new ResStructureEditor(*this);
    }
    
    ResStructureEditor& operator=(const Residue &residue);
    ResStructureEditor& operator=(const ResStructureEditor &other);
    
    const ResName& name() const;
    ResNum number() const;
    ResIdx index() const;
    
    int nAtoms() const;
    
    ChainStructureEditor chain();
    MolStructureEditor molecule();
    
    AtomStructureEditor atom(int i);
    AtomStructureEditor atom(const AtomID &atomid);

    AtomStructureEditor select(int i);
    AtomStructureEditor select(const AtomID &atomid);
    
    ResStructureEditor& rename(const ResName &name);
    ResStructureEditor& renumber(ResNum number);
    
    ResStructureEditor& reindex(ResIdx index);
    
    MolStructureEditor remove();
    
    ResStructureEditor& reparent(const ChainID &chainid);

    AtomStructureEditor add(const AtomName &atomname);
    AtomStructureEditor add(AtomNum atomnum);
    
    ResStructureEditor& remove(const AtomID &atomid);

    ResStructureEditor& remove(int i);
    
    ResStructureEditor& transfer(const AtomID &atomid, const ResID &resid);
    ResStructureEditor& transfer(int i, const ResID &resid);
    
    ResStructureEditor& transferAll(const ResID &resid);

    Residue commit() const;
    
    operator Residue() const;

private:
    /** The unique ID number of this residue in this editor */
    quint32 uid;
};

}

Q_DECLARE_METATYPE( SireMol::ResEditor );
Q_DECLARE_METATYPE( SireMol::ResStructureEditor );

SIRE_END_HEADER

#endif
