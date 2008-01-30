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

#ifndef SIREMOL_SEGEDITOR_H
#define SIREMOL_SEGEDITOR_H

#include "structureeditor.h"
#include "segment.h"
#include "editor.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{
class SegEditor;
class SegStructureEditor;
}

QDataStream& operator<<(QDataStream&, const SireMol::SegEditor&);
QDataStream& operator>>(QDataStream&, SireMol::SegEditor&);

QDataStream& operator<<(QDataStream&, const SireMol::SegStructureEditor&);
QDataStream& operator>>(QDataStream&, SireMol::SegStructureEditor&);

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

/** This class is used to edit the non-structural parts of a segment

    @author Christopher Woods
*/
class SIREMOL_EXPORT SegEditor : public Editor<Segment>
{

friend QDataStream& ::operator<<(QDataStream&, const SegEditor&);
friend QDataStream& ::operator>>(QDataStream&, SegEditor&);

public:
    SegEditor();
    
    SegEditor(const Segment &residue);
    
    SegEditor(const SegEditor &other);
    
    ~SegEditor();
    
    SegEditor& operator=(const Segment &residue);
    SegEditor& operator=(const SegEditor &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<SegEditor>() );
    }
    
    const char *what() const
    {
        return SegEditor::typeName();
    }
    
    SegEditor* clone() const
    {
        return new SegEditor(*this);
    }
    
    MolEditor molecule() const;
    
    AtomEditor atom(int i) const;
    AtomEditor atom(const AtomID &atomid) const;
    
    AtomEditor select(const AtomID &atomid) const;
    AtomEditor select(int i) const;
    
    SegEditor& rename(const SegName &name);
    SegStructureEditor reindex(SegIdx index) const;
    
    MolStructureEditor remove() const;

    AtomStructureEditor add(const AtomName &atomname) const;
    AtomStructureEditor add(AtomNum atomnum) const;
    
    SegStructureEditor remove(const AtomID &atomid) const;

    SegStructureEditor remove(int i) const;
    
    SegStructureEditor transfer(const AtomID &atomid, const SegID &segid) const;
    SegStructureEditor transfer(int i, const SegID &segid) const;
    
    SegStructureEditor transferAll(const SegID &segid) const;
    
    Segment commit() const;
};

/** This is the class used to edit a segment's structure 

    @author Christopher Woods
*/
class SIREMOL_EXPORT SegStructureEditor : public StructureEditor
{

friend QDataStream& ::operator<<(QDataStream&, const SegStructureEditor&);
friend QDataStream& ::operator>>(QDataStream&, SegStructureEditor&);

public:
    SegStructureEditor();
    SegStructureEditor(const Segment &residue);
    SegStructureEditor(const StructureEditor &data, SegIdx residx);
    
    SegStructureEditor(const SegStructureEditor &other);
    
    ~SegStructureEditor();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<SegStructureEditor>() );
    }
    
    const char* what() const
    {
        return SegStructureEditor::typeName();
    }
    
    SegStructureEditor* clone() const
    {
        return new SegStructureEditor(*this);
    }
    
    SegStructureEditor& operator=(const Segment &residue);
    SegStructureEditor& operator=(const SegStructureEditor &other);
    
    const SegName &name() const;
    SegIdx index() const;
    
    int nAtoms() const;
    
    MolStructureEditor molecule();
    
    AtomStructureEditor atom(int i);
    AtomStructureEditor atom(const AtomID &atomid);

    AtomStructureEditor select(int i);
    AtomStructureEditor select(const AtomID &atomid);
    
    SegStructureEditor& rename(const SegName &name);
    SegStructureEditor& reindex(SegIdx index);
    
    MolStructureEditor remove();

    AtomStructureEditor add(const AtomName &atomname);
    AtomStructureEditor add(AtomNum atomnum);
    
    SegStructureEditor& remove(const AtomID &atomid);

    SegStructureEditor& remove(int i);
    
    SegStructureEditor& transfer(const AtomID &atomid, const SegID &segid);
    SegStructureEditor& transfer(int i, const SegID &segid);
    
    SegStructureEditor& transferAll(const SegID &segid);
    
    Segment commit() const;
    
    operator Segment() const;

private:
    /** The unique ID for this segment in the molecule editor */
    quint32 uid;
};

}

Q_DECLARE_METATYPE( SireMol::SegEditor );
Q_DECLARE_METATYPE( SireMol::SegStructureEditor );

SIRE_END_HEADER

#endif
