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

#ifndef SIREMOL_MOLEDITOR_H
#define SIREMOL_MOLEDITOR_H

#include "structureeditor.h"
#include "editor.hpp"
#include "molecule.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MolEditor;
class MolStructureEditor;
}

QDataStream& operator<<(QDataStream&, const SireMol::MolEditor&);
QDataStream& operator>>(QDataStream&, SireMol::MolEditor&);

QDataStream& operator<<(QDataStream&, const SireMol::MolStructureEditor&);
QDataStream& operator>>(QDataStream&, SireMol::MolStructureEditor&);

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

/** This class is used to edit non-structural parts of a molecule 
    (e.g. its name, number, properties etc.). To add, move or remove
    atoms, residues, CutGroups, chains or segments, you need to 
    use a MolStructureEditor (which is created for you automatically
    by member functions that perform such editing)
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MolEditor : public Editor<Molecule>
{

friend QDataStream& ::operator<<(QDataStream&, const MolEditor&);
friend QDataStream& ::operator>>(QDataStream&, MolEditor&);

public:
    MolEditor();
    
    MolEditor(const Molecule &molecule);
    
    MolEditor(const MolEditor &other);
    
    ~MolEditor();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MolEditor>() );
    }
    
    const char* what() const
    {
        return MolEditor::typeName();
    }
    
    MolEditor* clone() const
    {
        return new MolEditor(*this);
    }
    
    MolEditor& operator=(const Molecule &molecule);
    MolEditor& operator=(const MolEditor &other);

    const MolName& name() const;
    MolNum number() const;
    
    MolEditor& rename(const MolName &name);
    MolEditor& renumber();
    
    AtomStructureEditor add(const AtomName &atom);
    AtomStructureEditor add(const AtomNum &atom);
    
    ResStructureEditor add(const ResName &residue);
    ResStructureEditor add(const ResNum &residue);
    
    CGStructureEditor add(const CGName &cutgroup);
    CGStructureEditor add(const CGNum &cutgroup);
    
    ChainStructureEditor add(const ChainName &chain);
    ChainStructureEditor add(const ChainNum &chain);
    
    SegStructureEditor add(const SegName &segment);
    SegStructureEditor add(const SegNum &segment);
    
    MolStructureEditor remove(const AtomID &atomid);
    MolStructureEditor remove(const CGID &cgid);
    MolStructureEditor remove(const ResID &resid);
    MolStructureEditor remove(const ChainID &chainid);
    MolStructureEditor remove(const SegID &segid);

};

/** This class is used to edit structural parts of the molecule,
    i.e. adding, moving or removing atoms, residues etc.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MolStructureEditor : public StructureEditor
{

friend QDataStream& ::operator<<(QDataStream&, const MolStructureEditor&);
friend QDataStream& ::operator>>(QDataStream&, MolStructureEditor&);

public:
    MolStructureEditor();
    MolStructureEditor(const MoleculeView &molview);
    
    MolStructureEditor(const StructureEditor &other);
    MolStructureEditor(const MolStructureEditor &other);
    
    ~MolStructureEditor();
    
    MolStructureEditor& operator=(const MoleculeView &molview);
    MolStructureEditor& operator=(const StructureEditor &other);
    MolStructureEditor& operator=(const MolStructureEditor &other);
    
    AtomStructureEditor select(const AtomID &atomid);
    CGStructureEditor select(const CGID &cgid);
    ResStructureEditor select(const ResID &resid);
    ChainStructureEditor select(const ChainID &chainid);
    SegStructureEditor select(const SegID &segid);

    AtomStructureEditor atom(const AtomID &atomid);
    CGStructureEditor cutGroup(const CGID &cgid);
    ResStructureEditor residue(const ResID &resid);
    ChainStructureEditor chain(const ChainID &chainid);
    SegStructureEditor segment(const SegID &segid);

    const MolName& name() const;
    
    MolEditor& rename(const MolName &name);
    
    AtomStructureEditor add(const AtomName &atom);
    AtomStructureEditor add(const AtomNum &atom);
    
    ResStructureEditor add(const ResName &residue);
    ResStructureEditor add(const ResNum &residue);
    
    CGStructureEditor add(const CGName &cutgroup);
    CGStructureEditor add(const CGNum &cutgroup);
    
    ChainStructureEditor add(const ChainName &chain);
    ChainStructureEditor add(const ChainNum &chain);
    
    SegStructureEditor add(const SegName &segment);
    SegStructureEditor add(const SegNum &segment);
    
    MolStructureEditor remove(const AtomID &atomid);
    MolStructureEditor remove(const CGID &cgid);
    MolStructureEditor remove(const ResID &resid);
    MolStructureEditor remove(const ChainID &chainid);
    MolStructureEditor remove(const SegID &segid);
};

}

Q_DECLARE_METATYPE( SireMol::MolEditor );
Q_DECLARE_METATYPE( SireMol::MolStructureEditor );

SIRE_END_HEADER

#endif
