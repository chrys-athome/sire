/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREMOL_MOLECULEINFOEDITORDATA_H
#define SIREMOL_MOLECULEINFOEDITORDATA_H

#include "moleculeinfodata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeInfoDataEditor;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeInfoDataEditor);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeInfoDataEditor&);

namespace SireMol
{

/** This is the class that is used to edit a MoleculeInfoData object - 
    this is the underlying class of MoleculeInfoEditor, ResidueInfoEditor,
    CutGroupInfoEditor, SegmentInfoEditor and AtomInfoEditor
    
    Constraints:
        Atoms identified by atom name in a specified residue
        Residues identified by residue number
        CutGroups identified by CutGroup number (may shadow resnum)
        Segments identified by number
    
    @author Christopher Woods
*/
class MoleculeInfoDataEditor
{

friend QDataStream& operator<<(QDataStream&, const MoleculeInfoDataEditor&);
friend QDataStream& operator>>(QDataStream&, MoleculeInfoDataEditor&);

friend class MoleculeInfoData; // friend so that conversion to and 
                               // from MoleculeInfoData works smoothly

public:
    MoleculeInfoDataEditor();
    ~MoleculeInfoDataEditor();
    
    
private:
    
    class AtmInfo
    {
    public:
        AtmInfo();
        ~AtmInfo();
        
        /** The name of the atom - must be unique within
            the residue */
        QString name;
        /** The number of the atom - does not need to
            be unique */
        AtomNum number;
        
        /** The number of the CutGroup this atom is in */
        CutGroupNum cgnum;
        /** The number of the residue this atom is in */
        ResNum resnum;
        /** The number of the segment this atom is in */
        SegNum segnum;
    };
    
    class ResInfo
    {
    public:
        ResInfo();
        ~ResInfo();
        
        /** The name of this residue */
        QString name;
        
        /** The index of each atom in this residue mapped 
            according to index */
        QHash<QString,int> atom_index;
        
        /** The names of the atoms in this residue in the order
            that they were added to this residue */
        QList<QString> atoms_by_index;
    };
    
    class CGInfo
    {
    public:
        CGInfo();
        ~CGInfo();
        
        /** The name of this CutGroup */
        QString name;
        
        /** The list of atoms in this CutGroup in the order they
            were added to this CutGroup */
        QList<ResNumAtomName> atoms_by_index;
    };
    
    class SegInfo
    {
    public:
        SegInfo();
        ~SegInfo();
        
        /** The name of this Segment */
        QString name;
        
        /** The list of atoms in this segment in the order they
            were added to this segment */
        QList<ResNumAtomName> atoms_by_index;
    };
    
    /** The list of all atoms in this molecule in the order
        that they were added to this molecule */
    QList<AtmInfo> atoms_by_index;
    
    /** The list of all residues in this molecule, in the order
        that they were added to this molecule */
    QList<ResInfo> res_by_index;
    
    /** The list of all CutGroups in this molecule, in the order
        that they were added to this molecule */
    QList<CGInfo> cg_by_index;
    
    /** The list of all Segments in this molecule, in the order
        that they were added to this molecule */
    QList<SegInfo> seg_by_index;
    
    /** Hash mapping the number of each residue to its index */
    QHash<ResNum,int> res_index;
    
    /** Hash mapping the number of each CutGroup to its index */
    QHash<CGNum,int> cg_index;
    
    /** Hash mapping the number of each Segment ot its index */
    QHash<SegNum,int> seg_index;

};

}

Q_DECLARE_METATYPE(SireMol::MoleculeInfoDataEditor);

SIRE_END_HEADER

#endif
