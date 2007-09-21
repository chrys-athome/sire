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
        ResName name;
        
        /** The number of this residue */
        ResNum number;

        /** The index of the chain this residue is in */
        ChainIdx chainidx;

        /** The indicies of all atoms that are in this residue */
        QSet<AtomIdx> atom_indicies;
        
        /** Hash mapping the name of each atom in this residue
            to the indicies of the atoms in the molecule
            (there may be more than one atom with the same name) */
        QMultiHash<QString,AtomIdx> atoms_by_name;
    };

    class ChainInfo
    {
    public:
        ChainInfo();
        ~ChainInfo();
        
        /** The name of this chain */
        ChainName name;
        
        /** The indicies of the residues that are contained in this chain */
        QSet<ResIdx> res_indicies;
    };

    class SegInfo
    {
    public:
        SegInfo();
        ~SegInfo();

        /** The name of this segment */
        SegName name;
        
        /** The indicies of all of the atoms that are in this residue */
        QSet<AtomIdx> atom_indexes;
    };
    
    class CGInfo
    {
    public:
        CGInfo();
        ~CGInfo();
        
        /** The name of this CutGroup */
        CGName name;
        
        /** The indicies of all of the atoms that are in this CutGroup */
        QSet<AtomIdx> atom_indexes;
    };
    
    class AtmInfo
    {
    public:
        AtmInfo();
        ~AtmInfo();
        
        /** The name of this atom */
        QString name;

        /** Index of the residue this atom is in */
        ResIdx residx;

        /** Index of the chain this atom is in */
        ChainIdx chainidx;
        
        /** Index of the segment this atom is in */
        SegIdx segidx;
        
        /** The CGAtomIdx index of the atom in the molecule
             - this says which CutGroup the atom is in,
               which atom in that CutGroup this is */
        CGAtomIdx cgatomidx;
    };
    
    /** All of the atoms in the molecule, in the order they were
        added to the molecule */
    QVector<AtomInfo> atoms_by_index;
    
    /** All of the residues in this molecule, arranged in the
        order that they appear in this molecule */
    QVector<ResInfo> res_by_index;
    
    /** Hash mapping residue names to residue indicies */
    QMultiHash<ResName,ResIdx> res_by_name;
    
    /** Hash mapping residue numbers to residue indicies */
    QMultiHash<ResNum,ResIdx> res_by_num;
    
    /** All of the chains in this molecule, arranged in the 
        order that they appear in this molecule */
    QVector<ChainInfo> chains_by_index;
    
    /** Hash mapping chain names to chain indicies */
    QMultiHash<ChainName,ChainIdx> chain_by_name;
    
    /** All of the segments in this molecule, arranged in the
        order that they appear in this molecule */
    QVector<SegInfo> seg_by_index;
    
    /** Hash mapping segment names to segment indicies */
    QMultiHash<SegName,SegIdx> seg_by_name;
    
    /** All of the CutGroups in this molecule, arranged in the
        order that they appear in this molecule */
    QVector<CGInfo> cg_by_index;

    /** Hash mapping CutGroup name to CutGroup indicies */
    QMultiHash<CGName,CGIdx> cg_by_name;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeInfoData);

SIRE_END_HEADER

#endif
