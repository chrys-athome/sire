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

#ifndef SIREMOL_ATOMEDITOR_H
#define SIREMOL_ATOMEDITOR_H

#include "editor.hpp"
#include "atom.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomEditor;
class AtomStructureEditor;
}

QDataStream &operator<<(QDataStream&, const SireMol::AtomEditor&);
QDataStream &operator>>(QDataStream&, SireMol::AtomEditor&);

QDataStream &operator<<(QDataStream&, const SireMol::AtomStructureEditor&);
QDataStream &operator>>(QDataStream&, SireMol::AtomStructureEditor&);

namespace SireMol
{

class MolStructureEditor;

class CGIdx;
class CGID;
class ResIdx;
class ResID;

/** This class is used to edit an atom in a molecule. This
    class is able to edit everything about the molecule
    *except* for its relationship to other parts of the
    molecule. To do that, you need an AtomStructureEditor
    (which is created automatically by the 'reparent()'
    function)

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomEditor : public Editor<Atom>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomEditor&);
friend QDataStream& ::operator>>(QDataStream&, AtomEditor&);

public:
    AtomEditor();
    
    AtomEditor(const Atom &atom);
    
    AtomEditor(const AtomEditor &other);
    
    ~AtomEditor();
    
    AtomEditor& operator=(const Atom &other);
    AtomEditor& operator=(const AtomEditor &other);

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<AtomEditor>() );
    }
    
    const char* what() const
    {
        return AtomEditor::typeName();
    }

    AtomEditor* clone() const
    {
        return new AtomEditor(*this);
    }

    MolStructureEditor remove() const;
    
    AtomStructureEditor reparent(CGIdx cgidx) const;
    AtomStructureEditor reparent(const CGID &cgid) const;
    
    AtomStructureEditor reparent(ResIdx residx) const;
    AtomStructureEditor reparent(const ResID &resid) const;

    AtomStructureEditor reparent(SegIdx segidx) const;
    AtomStructureEditor reparent(const SegID &segid) const;
};

/** This class is used to edit an atom's relationship to 
    other parts of the molecule (e.g. which CutGroup it
    is in, or which Residue it is in)
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomStructureEditor
{

friend QDataStream& ::operator<<(QDataStream&, const AtomStructureEditor&);
friend QDataStream& ::operator>>(QDataStream&, AtomStructureEditor&); 

public:
    AtomStructureEditor();
    AtomStructureEditor(const Atom &atom);

    AtomStructureEditor(const AtomStructureEditor &other);
    
    ~AtomStructureEditor();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<AtomStructureEditor>() );
    }
    
    const char* what() const
    {
        return AtomStructureEditor::typeName();
    }
    
    AtomStructureEditor* clone() const
    {
        return new AtomStructureEditor(*this);
    }

    AtomStructureEditor& reparent(CGIdx cgidx);
    AtomStructureEditor& reparent(const CGID &cgid);
    
    AtomStructureEditor& reparent(ResIdx residx);
    AtomStructureEditor& reparent(const ResID &resid);
    
    AtomStructureEditor& reparent(SegIdx segidx);
    AtomStructureEditor& reparent(const SegID &segid);

    operator Atom() const;
};

}

Q_DECLARE_METATYPE( SireMol::AtomEditor );
Q_DECLARE_METATYPE( SireMol::AtomStructureEditor );

SIRE_END_HEADER

#endif
