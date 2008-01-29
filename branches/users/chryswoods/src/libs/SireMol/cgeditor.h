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

#ifndef SIREMOL_CGEDITOR_H
#define SIREMOL_CGEDITOR_H

#include "structureeditor.h"
#include "cutgroup.h"
#include "editor.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{
class CGEditor;
class CGStructureEditor;
}

QDataStream& operator<<(QDataStream&, const SireMol::CGEditor&);
QDataStream& operator>>(QDataStream&, SireMol::CGEditor&);

QDataStream& operator<<(QDataStream&, const SireMol::CGStructureEditor&);
QDataStream& operator>>(QDataStream&, SireMol::CGStructureEditor&);

namespace std
{
class slice;
}

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

/** This class is used to edit the non-structural parts of a CutGroup

    @author Christopher Woods
*/
class SIREMOL_EXPORT CGEditor : public Editor<CutGroup>
{

friend QDataStream& ::operator<<(QDataStream&, const CGEditor&);
friend QDataStream& ::operator>>(QDataStream&, CGEditor&);

public:
    CGEditor();
    
    CGEditor(const CutGroup &residue);
    
    CGEditor(const CGEditor &other);
    
    ~CGEditor();
    
    CGEditor& operator=(const CutGroup &residue);
    CGEditor& operator=(const CGEditor &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<CGEditor>() );
    }
    
    const char *what() const
    {
        return CGEditor::typeName();
    }
    
    CGEditor* clone() const
    {
        return new CGEditor(*this);
    }
    
    MolEditor molecule() const;
    
    AtomEditor atom(int i) const;
    AtomEditor atom(const AtomID &atomid) const;
    
    AtomEditor select(const AtomID &atomid) const;
    AtomEditor select(int i) const;
    
    CGEditor& rename(const CGName &name);
    CGEditor& renumber(CGNum number);
    
    CGStructureEditor reindex(CGIdx index) const;
    
    MolStructureEditor remove() const;

    AtomStructureEditor add(const AtomName &atomname) const;
    AtomStructureEditor add(AtomNum atomnum) const;
    
    CGStructureEditor remove(const AtomID &atomid) const;

    CGStructureEditor remove(int i) const;
    CGStructureEditor remove(const std::slice &s) const;
    
    CGStructureEditor transfer(const AtomID &atomid, const CGID &cgid) const;
    CGStructureEditor transfer(int i, const CGID &cgid) const;
    CGStructureEditor transfer(const std::slice &s, const CGID &cgid) const;
    
    CGStructureEditor transferAll(const CGID &cgid) const;
};

/** This is the class used to edit a CutGroup's structure 

    @author Christopher Woods
*/
class SIREMOL_EXPORT CGStructureEditor : public StructureEditor
{

friend QDataStream& ::operator<<(QDataStream&, const CGStructureEditor&);
friend QDataStream& ::operator>>(QDataStream&, CGStructureEditor&);

public:
    CGStructureEditor();
    CGStructureEditor(const CutGroup &cutgroup);
    CGStructureEditor(const StructureEditor &data, CGIdx cgidx);
    
    CGStructureEditor(const CGStructureEditor &other);
    
    ~CGStructureEditor();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<CGStructureEditor>() );
    }
    
    const char* what() const
    {
        return CGStructureEditor::typeName();
    }
    
    CGStructureEditor* clone() const
    {
        return new CGStructureEditor(*this);
    }
    
    CGStructureEditor& operator=(const CutGroup &cutgroup);
    CGStructureEditor& operator=(const CGStructureEditor &other);
    
    MolStructureEditor molecule();
    
    AtomStructureEditor atom(int i);
    AtomStructureEditor atom(const AtomID &atomid);

    AtomStructureEditor select(int i);
    AtomStructureEditor select(const AtomID &atomid);
    
    CGStructureEditor& rename(const CGName &name);
    CGStructureEditor& renumber(CGNum number);
    
    CGStructureEditor& reindex(CGIdx index);
    
    MolStructureEditor remove();

    AtomStructureEditor add(const AtomName &atomname);
    AtomStructureEditor add(AtomNum atomnum);
    
    CGStructureEditor& remove(const AtomID &atomid);

    CGStructureEditor& remove(int i);
    CGStructureEditor& remove(const std::slice &s);
    
    CGStructureEditor& transfer(const AtomID &atomid, const CGID &cgid);
    CGStructureEditor& transfer(int i, const CGID &cgid);
    CGStructureEditor& transfer(const std::slice &s, const CGID &cgid);
    
    CGStructureEditor& transferAll(const CGID &resid);
};

}

Q_DECLARE_METATYPE( SireMol::CGEditor );
Q_DECLARE_METATYPE( SireMol::CGStructureEditor );

SIRE_END_HEADER

#endif
