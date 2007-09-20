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

#ifndef SIREMOL_MOLECULEINFODATA_H
#define SIREMOL_MOLECULEINFODATA_H

#include "cgatomidx.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeInfoData;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeInfoData&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeInfoData&);

namespace SireMol
{

/** This is the implicitly shared class that is used by MoleculeInfo,
    ResidueInfo, SegmentInfo and CutGroupInfo to provide information
    about the arrangement of atoms in a molecule, specifically
    how one method of indexing the atoms can be mapped to another method.
    In so doing, a MoleculeInfoData object contains all of the information
    about which atoms are in a molecule, what they are called, how they
    are arranged into residues, cutgroups and segments, and how all
    of the different parts of the molecule are identified. However,
    importantly, this object does not contain any information about
    how the atoms in the molecule are connected together, where
    the atoms are in space, or any additional properties that are
    associated with the molecule.
    
    Each Molecule is given an ID number, which is unique within a
    single invocation of Sire, and uniquely identifies a MoleculeInfo
    object within the program (thus allowing for a quick and simple
    test to ensure that molecules have the same layout of data).
    
    This class is private to SireMol, and should not be used by
    any other classes other than MoleculeInfo, ResidueInfo, SegmentInfo
    AtomInfo or CutGroupInfo
    
    @author Christopher Woods
*/
class MoleculeInfoData
{

friend QDataStream& operator<<(QDataStream&, const MoleculeInfoData&);
friend QDataStream& operator>>(QDataStream&, MoleculeInfoData&);

public:
    MoleculeInfoData();
    
private:
    /** The name of the molecule */
    QString nam;
    
    class ResInfo
    {
    public:
        ResInfo();
        ~ResInfo();
        
        /** The name of this residue */
        QString name;
        
        /** The number of this residue */
        ResNum number;

        /** The CGAtomIdx indicies of all of the atoms in 
            this residue, in the order that they appear
            in this residue */
        QVector<CGAtomIdx> atoms_by_idx;
        
        /** Hash mapping the name of each atom in this residue
            to the index of the atom in this residue (atom names
            within each residue must be unique) */
        QHash<QString,int> atoms_by_name;
    }

    class SegInfo
    {
    public:
        SegInfo();
        ~SegInfo();

        /** The name of this segment */
        QString name;
        
        /** The number of this segment */
        SegNum number;
        
        /** The CGAtomIdx indicies of all of the atoms in 
            this segment, in the order that they appear */
        QVector<CGAtomIdx> atoms_by_idx;
    };
    
    class AtmInfo
    {
    public:
        AtmInfo();
        ~AtmInfo();
        
        /** The name of this atom */
        QString name;
        
        /** The number of the residue in which this belongs
            (an atom *must* be in one residue) */
        ResNum resnum;
        
        /** The name of the segment in which this belongs
            (this is null if the atom is not in any segment) */
        QString segname;
    };
    
    class CGInfo
    {
    public:
        CGInfo();
        ~CGInfo();
        
        /** The name of this CutGroup */
        QString name;
        
        /** The number of this CutGroup */
        CGNum number;
        
        /** Information about all of the atoms in this CutGroup,
            arranged in the order that they are in this CutGroup */
        QVector<AtomInfo> atominfos;
    };
    
    /** All of the residues in this molecule, arranged in the
        order that they appear in this molecule */
    QVector<ResInfo> resinfos;
    
    /** All of the segments in this molecule, arranged in the
        order that they appear in this molecule */
    QVector<SegInfo> seginfos;
    
    /** All of the CutGroups in this molecule, arranged in the
        order that they appear in this molecule */
    QVector<CGInfo> cginfos;

    /** The CGAtomIdx of all atoms in this molecule, arranged in 
        the order that they appear in this molecule */
    QVector<CGAtomIdx> atom_by_index;

    /** Hash allowing rapid indexing of residues by name */
    QHash<QString,int> res_by_name;
    
    /** Hash allowing rapid indexing of residues by number */
    QHash<ResNum,int> res_by_number;
    
    /** Hash allowing rapid indexing of segments by name */
    QHash<QString,int> seg_by_name;
    
    /** Hash allowing rapid indexing of segments by number */
    QHash<SegNum,int> seg_by_number;
    
    /** Hash allowing rapid indexing of CutGroups by name */
    QHash<QString,int> cg_by_name;
    
    /** Hash allowing rapid indexing of CutGroups by number */
    QHash<CGNum,int> cg_by_num;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeInfoData);

SIRE_END_HEADER

#endif
