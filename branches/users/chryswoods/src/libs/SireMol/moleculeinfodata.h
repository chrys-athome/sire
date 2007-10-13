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

#include <QSharedData>
#include <QSharedDataPointer>
#include <QVector>
#include <QMultiHash>

#include "cgatomidx.h"
#include "molname.h"

#include "segname.h"
#include "segidx.h"
#include "segidentifier.h"

#include "chainname.h"
#include "chainidx.h"
#include "chainidentifier.h"

#include "cgname.h"
#include "cgidx.h"
#include "cgidentifier.h"

#include "resname.h"
#include "resnum.h"
#include "residx.h"
#include "residentifier.h"

#include "atomname.h"
#include "atomnum.h"
#include "atomidx.h"
#include "atomidentifier.h"

#include "specify.hpp"
#include "atomsin.hpp"
#include "resin.hpp"

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
class MoleculeInfoData : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeInfoData&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeInfoData&);

public:
    MoleculeInfoData();
    
    MoleculeInfoData(const MoleculeInfoData &other);
    
    ~MoleculeInfoData();
    
    MoleculeInfoData& operator=(const MoleculeInfoData &other);
    
    const MolName& name() const;
    
    const ChainName& name(const ChainID &chainid) const;
    const ChainName& name(ChainIdx chainidx) const;
    
    const SegName& name(const SegID &segid) const;
    const SegName& name(SegIdx segidx) const;
    
    const ResName& name(const ResID &resid) const;
    const ResName& name(ResIdx residx) const;
    
    const CGName& name(const CGID &cgid) const;
    const CGName& name(CGIdx cgidx) const;
    
    const AtomName& name(const AtomID &atomid) const;
    const AtomName& name(AtomIdx atomidx) const;
    
    ResNum number(const ResID &resid) const;
    ResNum number(ResIdx residx) const;
    
    AtomNum number(const AtomID &atomid) const;
    AtomNum number(AtomIdx atomidx) const;
    
    CGAtomIdx cgAtomIdx(AtomIdx atomidx) const;
    CGAtomIdx cgAtomIdx(const AtomID &atomid) const;
    QList<CGAtomIdx> cgAtomIdxs(const AtomID &atomid) const;

    AtomIdx atomIdx(const AtomID &atomid) const;
    AtomIdx atomIdx(const CGAtomIdx &cgatomidx) const;

    ResIdx resIdx(const ResID &resid) const;
    ChainIdx chainIdx(const ChainID &chainid) const;
    SegIdx segIdx(const SegID &segid) const;
    CGIdx cgIdx(const CGID &cgid) const;

    QList<SegIdx> getSegments() const;
    QList<CGIdx> getCutGroups() const;
    QList<ChainIdx> getChains() const;
    QList<ResIdx> getResidues() const;
    
    const QList<ResIdx>& getResiduesIn(ChainIdx chainidx) const;
    QList<ResIdx> getResiduesIn(const ChainID &chainid) const;

    QList<AtomIdx> getAtoms() const;

    const QList<AtomIdx>& getAtomsIn(ResIdx residx) const;
    QList<AtomIdx> getAtomsIn(const ResID &resid) const;
    QList<AtomIdx> getAtomsIn(ResIdx residx, const AtomName &name) const;
    QList<AtomIdx> getAtomsIn(const ResID &resid,
                              const AtomName &atomname) const;

    QList<AtomIdx> getAtomsIn(ChainIdx chainidx) const;
    QList<AtomIdx> getAtomsIn(const ChainID &chainid) const;
    QList<AtomIdx> getAtomsIn(ChainIdx chainidx, 
                              const AtomName &atomname) const;
    QList<AtomIdx> getAtomsIn(const ChainID &chainid,
                              const AtomName &atomname) const;
                              
    const QList<AtomIdx>& getAtomsIn(CGIdx cgidx) const;
    QList<AtomIdx> getAtomsIn(const CGID &cgid) const;
    
    const QList<AtomIdx>& getAtomsIn(SegIdx segidx) const;
    QList<AtomIdx> getAtomsIn(const SegID &segid) const;

    ChainIdx parentChain(ResIdx residx) const;
    ChainIdx parentChain(const ResID &resid) const;
    
    ChainIdx parentChain(AtomIdx atomidx) const;
    ChainIdx parentChain(const AtomID &atomid) const;
    
    ResIdx parentResidue(AtomIdx atomidx) const;
    ResIdx parentResidue(const AtomID &atomid) const;
    
    SegIdx parentSegment(AtomIdx atomidx) const;
    SegIdx parentSegment(const AtomID &atomid) const;
    
    CGIdx parentCutGroup(AtomIdx atomidx) const;
    CGIdx parentCutGroup(const AtomID &atomid) const;
    
    bool contains(ResIdx residx, AtomIdx atomidx) const;
    bool contains(ChainIdx chainidx, AtomIdx atomidx) const;
    bool contains(SegIdx segidx, AtomIdx atomidx) const;
    bool contains(CGIdx cgidx, AtomIdx atomidx) const;
    bool contains(ChainIdx chainidx, ResIdx residx) const;
    
    bool containsAll(ResIdx residx, const AtomID &atomid) const;
    bool containsAll(ChainIdx chainidx, const AtomID &atomid) const;
    bool containsAll(SegIdx segidx, const AtomID &atomid) const;
    bool containsAll(CGIdx cgidx, const AtomID &atomid) const;
    bool containsAll(ChainIdx chainidx, const ResID &resid) const;

    bool containsSome(ResIdx residx, const AtomID &atomid) const;
    bool containsSome(ChainIdx chainidx, const AtomID &atomid) const;
    bool containsSome(SegIdx segidx, const AtomID &atomid) const;
    bool containsSome(CGIdx cgidx, const AtomID &atomid) const;
    bool containsSome(ChainIdx chainidx, const ResID &resid) const;
    
    int nAtoms() const;
    
    int nAtoms(const ChainID &chainid) const;
    int nAtoms(ChainIdx chainidx) const;
    
    int nAtoms(const ResID &resid) const;
    int nAtoms(ResIdx residx) const;
    
    int nAtoms(const SegID &segid) const;
    int nAtoms(SegIdx segidx) const;
    
    int nAtoms(const CGID &cgid) const;
    int nAtoms(CGIdx cgidx) const;
    
    int nResidues() const;
    int nResidues(const ChainID &chainid) const;
    int nResidues(ChainIdx chainidx) const;
    
    int nChains() const;
    int nCutGroups() const;
    int nSegments() const;
    
    QList<ResIdx> map(const ResName &name) const;
    QList<ResIdx> map(const ResNum &num) const;
    QList<ResIdx> map(const ResIdx &idx) const;
    QList<ResIdx> map(const ResID &resid) const;
    
    QList<ChainIdx> map(const ChainName &name) const;
    QList<ChainIdx> map(const ChainIdx &idx) const;
    QList<ChainIdx> map(const ChainID &chainid) const;

    QList<SegIdx> map(const SegName &name) const;
    QList<SegIdx> map(const SegIdx &idx) const;
    QList<SegIdx> map(const SegID &segid) const;
   
    QList<CGIdx> map(const CGName &name) const;
    QList<CGIdx> map(const CGIdx &idx) const;
    QList<CGIdx> map(const CGID &cgid) const;
    
    QList<AtomIdx> map(const AtomName &name) const;
    QList<AtomIdx> map(const AtomNum &num) const;
    QList<AtomIdx> map(const AtomIdx &idx) const;
    QList<AtomIdx> map(const AtomID &atomid) const;

    void assertSingleAtom(const QList<AtomIdx> &atomidxs) const;
    void assertSingleResidue(const QList<ResIdx> &residxs) const;
    void assertSingleChain(const QList<ChainIdx> &chainidxs) const;
    void assertSingleCutGroup(const QList<CGIdx> &cgidxs) const;
    void assertSingleSegment(const QList<SegIdx> &segidxs) const;

    template<class T>
    static QList<T> intersection(const QList<T> &list0, const QList<T> &list1);
    
    static QSharedDataPointer<MoleculeInfoData> null();
    
private:
    
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

        /** The sorted list of the indicis of all atoms that are in this residue */
        QList<AtomIdx> atom_indicies;
        
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
        
        /** The sorted list of all indicies of the residues that 
            are contained in this chain */
        QList<ResIdx> res_indicies;
    };

    class SegInfo
    {
    public:
        SegInfo();
        ~SegInfo();

        /** The name of this segment */
        SegName name;
        
        /** The sorted list of all indicies of the atoms that are in this residue */
        QList<AtomIdx> atom_indicies;
    };
    
    class CGInfo
    {
    public:
        CGInfo();
        ~CGInfo();
        
        /** The name of this CutGroup */
        CGName name;
        
        /** The sorted list of all indicies of all of the atoms 
            that are in this CutGroup */
        QList<AtomIdx> atom_indicies;
    };
    
    class AtomInfo
    {
    public:
        AtomInfo();
        ~AtomInfo();
        
        /** The name of this atom */
        AtomName name;

        /** The number of this atom */
        AtomNum number;

        /** Index of the residue this atom is in
            (this also tells us the index of the chain
             this atom is in) */
        ResIdx residx;
        
        /** Index of the segment this atom is in */
        SegIdx segidx;
        
        /** The CGAtomIdx index of the atom in the molecule
             - this says which CutGroup the atom is in,
               which atom in that CutGroup this is */
        CGAtomIdx cgatomidx;
    };
    
    QList<AtomIdx> _pvt_getAtomsIn(const QList<ResIdx> &residxs) const;
    QList<AtomIdx> _pvt_getAtomsIn(const QList<ResIdx> &residxs,
                                   const AtomName &name) const;
                                   
    int _pvt_nAtoms(const QVector<ResIdx> &residxs) const;
    int _pvt_nAtoms(const QList<ResIdx> &residxs) const;
    int _pvt_nAtoms(ChainIdx chainidx) const;
    
    /** The name of the molecule */
    MolName molname;

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
    QMultiHash<ChainName,ChainIdx> chains_by_name;
    
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

/** Return the intersection of the objects in 'list0' and 'list1'. This returns
    a list that contains only those items that are in both lists. The order
    of the list is the same as the input, namely if both list0 and list1 are
    sorted, then the returned list will be sorted as well. */
template<class T>
QList<T> MoleculeInfoData::intersection(const QList<T> &list0, const QList<T> &list1)
{
    QList<T> intersection_list;

    if (list0.count() <= list1.count())
    {
        QSet<T> set1 = list1.toSet();
        
        for (typename QList<T>::const_iterator it = list0.constBegin();
             it != list0.constEnd();
             ++it)
        {
            if (set1.contains(*it))
                intersection_list.append(*it);
        }
    }
    else
    {
        QSet<T> set0 = list0.toSet();
        
        for (typename QList<T>::const_iterator it = list1.constBegin();
             it != list1.constEnd();
             ++it)
        {
            if (set0.contains(*it))
                intersection_list.append(*it);
        }
    }
    
    return intersection_list;
}


/** Map this ID to the indicies of matching atoms 
    
    (This is here, rather than in atomsin.hpp, to prevent
     problems caused by circular header dependencies)

    \throw ???::missing_ID
    \throw SireError::invalid_index
*/
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
QList<AtomIdx> AtomsIn<GROUP>::map(const MoleculeInfoData &molinfo) const
{
    //first get the list of the indicies of the matching groups
    QList<typename GROUP::Index> idxs = groupid.map(molinfo);
    
    //now get a list of the indicies of all of the atoms in these groups
    QList<AtomIdx> atomidxs;
    
    foreach (typename GROUP::Index idx, idxs)
    {
        atomidxs += molinfo.getAtomsIn(idx);
    }
    
    //now map _i and _j to the indicies...
    int nats = atomidxs.count();
    
    int sane_strt = strt.map(nats);
    int sane_end = end.map(nats);
    
    if (sane_strt > sane_end)
        qSwap(sane_strt, sane_end);
    
    //now extract only the desired atom indicies
    if (sane_end - sane_strt == nats)
    {
        return atomidxs;
    }
    else
    {
        QList<AtomIdx> specified_atomidxs;
    
        for (int i=sane_strt; i<=sane_end; ++i)
        {
            specified_atomidxs.append( atomidxs[i] );
        }
    
        return specified_atomidxs;
    }
}

/** Map this ID to the indicies of matching residues
    
    (This is here, rather than in resin.hpp, to prevent
     problems caused by circular header dependencies)

    \throw ???::missing_ID
    \throw SireError::invalid_index
*/
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
QList<ResIdx> ResIn<GROUP>::map(const MoleculeInfoData &molinfo) const
{
    //first get the list of the indicies of the matching groups
    QList<typename GROUP::Index> idxs = groupid.map(molinfo);
    
    //now get a list of the indicies of all of the residues in these groups
    QList<ResIdx> residxs;
    
    foreach (typename GROUP::Index idx, idxs)
    {
        residxs += molinfo.getResiduesIn(idx);
    }
    
    //now map _i and _j to the indicies...
    int nres = residxs.count();
    
    int sane_strt = strt.map(nres);
    int sane_end = end.map(nres);
    
    if (sane_strt > sane_end)
        qSwap(sane_strt, sane_end);
    
    //now extract only the desired atom indicies
    if (sane_end - sane_strt == nres)
    {
        return residxs;
    }
    else
    {
        QList<ResIdx> specified_residxs;
    
        for (int i=sane_strt; i<=sane_end; ++i)
        {
            specified_residxs.append( residxs[i] );
        }
    
        return specified_residxs;
    }
}

}

Q_DECLARE_METATYPE(SireMol::MoleculeInfoData);

SIRE_END_HEADER

#endif
