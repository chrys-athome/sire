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

#include "editor.h"
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
    
    MolStructureEditor remove(AtomIdx atomidx) const;
    MolStructureEditor remove(const AtomID &atomid) const;
    
    MolStructureEditor remove(CGIdx cgidx) const;
    MolStructureEditor remove(const CGID &cgid) const;
    
    MolStructureEditor remove(ResIdx residx) const;
    MolStructureEditor remove(const ResID &resid) const;
    
    MolStructureEditor remove(ChainIdx Chainidx) const;
    MolStructureEditor remove(const ChainID &chainid) const;
    
    MolStructureEditor remove(SegIdx segidx) const;
    MolStructureEditor remove(const SegID &segid) const;

};

}

Q_DECLARE_METATYPE( SireMol::MolEditor );
Q_DECLARE_METATYPE( SireMol::MolStructureEditor );

SIRE_END_HEADER

#endif
