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

#ifndef SIREMOL_STRUCTUREEDITOR_H
#define SIREMOL_STRUCTUREEDITOR_H

#include "sireglobal.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class StructureEditor;
}

QDataStream& operator<<(QDataStream&, const SireMol::StructureEditor&);
QDataStream& operator>>(QDataStream&, SireMol::StructureEditor&);

namespace SireMol
{

class AtomIdx;
class AtomNum;
class AtomName;
class AtomID;

class CGIdx;
class CGName;
class CGID;

class ResIdx;
class ResNum;
class ResName;
class ResID;

class ChainIdx;
class ChainName;
class ChainID;

class SegIdx;
class SegName;
class SegID;

class MolName;
class MolNum;

class MoleculeData;

class MolStructureEditor;
class SegStructureEditor;
class ChainStructureEditor;
class ResStructureEditor;
class CGStructureEditor;
class AtomStructureEditor;

namespace detail
{
class EditMolData;
}

/** This is the base class of all of the molecule structure editors. 
    The structure editors are classes that can actually add, move
    and remove parts of a molecule (e.g. add, move or remove residues).
    As such, they are more powerful than the standard 'editor'
    classes (like AtomEditor), but they require a different underlying
    data structure, which doesn't rely on the structure of the molecule
    being constant.
    
    This class is explicitly shared and is *definitely not* thread safe!
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT StructureEditor
{

friend QDataStream& ::operator<<(QDataStream&, const StructureEditor&);
friend QDataStream& ::operator>>(QDataStream&, StructureEditor&);

public:
    ~StructureEditor();

protected:
    StructureEditor();
    StructureEditor(const MoleculeData &moldata);
    
    StructureEditor(const StructureEditor &other);
    
    StructureEditor& operator=(const MoleculeData &moldata);
    StructureEditor& operator=(const StructureEditor &other);

    MoleculeData commitChanges() const;

    quint32 getUID(AtomIdx atomidx) const;
    quint32 getUID(CGIdx cgidx) const;
    quint32 getUID(ResIdx residx) const;
    quint32 getUID(ChainIdx chainidx) const;
    quint32 getUID(SegIdx segidx) const;
    
    quint32 getUID(const AtomID &atomid) const;
    quint32 getUID(const CGID &cgid) const;
    quint32 getUID(const ResID &resid) const;
    quint32 getUID(const ChainID &chainid) const;
    quint32 getUID(const SegID &segid) const;
    
    int nAtomsInResidue(quint32 uid) const;
    int nAtomsInCutGroup(quint32 uid) const;
    int nAtomsInChain(quint32 uid) const;
    int nAtomsInSegment(quint32 uid) const;
    int nAtomsInMolecule() const;

    int nResiduesInChain(quint32 uid) const;

    int nResiduesInMolecule() const;
    int nChainsInMolecule() const;
    int nSegmentsInMolecule() const;
    int nCutGroupsInMolecule() const;

    quint32 atomInCutGroup(quint32 uid, int i) const;
    quint32 atomInResidue(quint32 uid, int i) const;
    quint32 atomInSegment(quint32 uid, int i) const;
    
    quint32 residueInChain(quint32 uid, int i) const;

    quint32 cutGroupParentOfAtom(quint32 uid) const;
    quint32 residueParentOfAtom(quint32 uid) const;
    quint32 chainParentOfAtom(quint32 uid) const;
    quint32 segmentParentOfAtom(quint32 uid) const;
    
    quint32 chainParentOfResidue(quint32 uid) const;
    
    const MolName& molName() const;
    MolNum molNum() const;
    
    const AtomName& atomName(quint32 uid) const;
    AtomNum atomNum(quint32 uid) const;
    AtomIdx atomIdx(quint32 uid) const;
    
    const CGName& cgName(quint32 uid) const;
    CGIdx cgIdx(quint32 uid) const;
    
    const ResName& resName(quint32 uid) const;
    ResNum resNum(quint32 uid) const;
    ResIdx resIdx(quint32 uid) const;
    
    const ChainName& chainName(quint32 uid) const;
    ChainIdx chainIdx(quint32 uid) const;
    
    const SegName& segName(quint32 uid) const;
    SegIdx segIdx(quint32 uid) const;
    
    AtomIdx atomIdx(const AtomID &atomid) const;
    CGIdx cgIdx(const CGID &cgid) const;
    ResIdx resIdx(const ResID &resid) const;
    ChainIdx chainIdx(const ChainID &chainid) const;
    SegIdx segIdx(const SegID &segid) const;
    
    void renameMolecule(const MolName &name);
    
    void renumberMolecule();
    void renumberMolecule(MolNum newnum);
    
    void renameAtom(quint32 uid, const AtomName &name);
    void renumberAtom(quint32 uid, AtomNum number);
    void reindexAtom(quint32 uid, AtomIdx index);
    
    void renameCutGroup(quint32 uid, const CGName &name);
    void reindexCutGroup(quint32 uid, CGIdx index);
    
    void renameResidue(quint32 uid, const ResName &name);
    void renumberResidue(quint32 uid, ResNum number);
    void reindexResidue(quint32 uid, ResIdx index);
    
    void renameChain(quint32 uid, const ChainName &name);
    void reindexChain(quint32 uid, ChainIdx index);
    
    void renameSegment(quint32 uid, const SegName &name);
    void reindexSegment(quint32 uid, SegIdx index);
    
    void removeAtom(quint32 uid);
    void removeCutGroup(quint32 uid);
    void removeResidue(quint32 uid);
    void removeChain(quint32 uid);
    void removeSegment(quint32 uid);
    
    void removeAtoms(const AtomID &atomid);
    void removeCutGroups(const CGID &cgid);
    void removeResidues(const ResID &resid);
    void removeChains(const ChainID &chainid);
    void removeSegments(const SegID &segid);
    
    void reparentAtom(quint32 uid, CGIdx cgidx);
    void reparentAtom(quint32 uid, ResIdx residx);
    void reparentAtom(quint32 uid, SegIdx segidx);
    
    void reparentResidue(quint32 uid, ChainIdx chainidx);
    
    AtomStructureEditor addAtom();
    CGStructureEditor addCutGroup();
    ResStructureEditor addResidue();
    ChainStructureEditor addChain();
    SegStructureEditor addSegment();

    void assertValidAtom(quint32 uid) const;
    void assertValidCutGroup(quint32 uid) const;
    void assertValidResidue(quint32 uid) const;
    void assertValidChain(quint32 uid) const;
    void assertValidSegment(quint32 uid) const;

private:
    /** This class is explicitly shared - this means that each copy
        edits the *same* underlying data - this is to prevent excess
        copying as small incremental changes are made to the same
        molecule */
    boost::shared_ptr<detail::EditMolData> d;
};

}

SIRE_END_HEADER

#endif
