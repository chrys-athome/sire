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

#include "structureeditor.h"

#include "atom.h"
#include "cutgroup.h"
#include "residue.h"
#include "chain.h"
#include "segment.h"
#include "molecule.h"
#include "mover.hpp"
#include "selector.hpp"

#include "atomname.h"
#include "atomidx.h"
#include "atomnum.h"

#include "cgname.h"
#include "cgidx.h"

#include "resname.h"
#include "resnum.h"
#include "residx.h"

#include "chainname.h"
#include "chainidx.h"

#include "segname.h"
#include "segidx.h"

#include "molname.h"
#include "molnum.h"

#include "SireBase/properties.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of detail::EditMolData
/////////
namespace detail
{

/** This class holds the editable data of an Atom */
class EditAtomData
{
public:
    EditAtomData();
    EditAtomData(const EditAtomData &other);
    
    ~EditAtomData();
    
    AtomName name;
    AtomNum number;
    
    quint32 cg_parent;
    quint32 res_parent;
    quint32 seg_parent;
    
    QHash<QString,QVariant> properties;
};

/** This class holds the editable data of a CutGroup */
class EditCGData
{
public:
    EditCGData();
    EditCGData(const EditCGData &other);
    
    ~EditCGData();
    
    CGName name;
    
    QList<quint32> atoms;
    
    QHash<QString,QVariant> properties;
};

/** This class holds the editable data of a Residue */
class EditResData
{
public:
    EditResData();
    EditResData(const EditResData &other);
    
    ~EditResData();
    
    ResName name;
    ResNum number;
    
    quint32 chain_parent;
    
    QList<quint32> atoms;
    
    QHash<QString,QVariant> properties;
};

/** This class holds the editable data of a Chain */
class EditChainData
{
public:
    EditChainData();
    EditChainData(const EditChainData &other);
    
    ~EditChainData();
    
    ChainName name;
    
    QList<quint32> residues;
    
    QHash<QString,QVariant> properties;
};

/** This class holds the editable data of a Segment */
class EditSegData
{
public:
    EditSegData();
    EditSetData(const EditSetData &other);
    
    ~EditSegData();
    
    SegName name;
    
    QList<quint32> atoms;
    
    QHash<QString,QVariant> properties;
};

/** This private class is used to hold the explicitly shared
    data of the StructureEditor. */
class EditMolData
{
public:
    EditMolData();
    EditMolData(const EditMolData &other);
    
    ~EditMolData();
    
    MolName molname;
    MolNum molnum;
    
    QHash<quint32,EditAtomData> atoms;
    QHash<quint32,EditCGData> cutgroups;
    QHash<quint32,EditResData> residues;
    QHash<quint32,EditChainData> chains;
    QHash<quint32,EditSegData> segments;
    
    QList<quint32> atoms_by_index;
    QList<quint32> cg_by_index;
    QList<quint32> res_by_index;
    QList<quint32> chains_by_index;
    QList<quint32> seg_by_index; 
    
    Properties properties;
};

} // end of namespace detail

using namespace SireMol::detail;

/////////
///////// Implementation of StructureEditor
/////////

static const RegisterMetaType<StructureEditor> r_editor(MAGIC_ONLY,
                                                        "SireMol::StructureEditor");
                                                  
/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const StructureEditor &editor)
{
    writeHeader(ds, r_editor, 1);
    
    SharedDataStream sds(ds);
    
    sds << editor.d;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       StructureEditor &editor)
{
    VersionID v = readHeader(ds, r_editor);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> editor.d;
    }
    else
        throw version_error( v, "1", r_editor, CODELOC );
        
    return ds;
}

StructureEditor::StructureEditor();
StructureEditor::StructureEditor(const MoleculeData &moldata);

StructureEditor::StructureEditor(const StructureEditor &other);

StructureEditor::~StructureEditor();

StructureEditor& StructureEditor::operator=(const MoleculeData &moldata);
StructureEditor& StructureEditor::operator=(const StructureEditor &other);

MoleculeData StructureEditor::commitChanges() const;

/** Return the number of atoms in the residue identified by 'uid'

    \throw SireMol::missing_residue
*/
int StructureEditor::nAtomsInResidue(quint32 uid) const
{
    return d->residue(uid).atoms.count();
}

/** Return the number of atoms in the CutGroup identified by 'uid'

    \throw SireMol::missing_cutgroup
*/
int StructureEditor::nAtomsInCutGroup(quint32 uid) const
{
    return d->cutgroup(uid).atoms.count();
}

/** Return the number of residues in the chain identified by 'uid'

    \throw SireMol::missing_chain
*/
int StructureEditor::nResiduesInChain(quint32 uid) const
{
    return d->chain(uid).residues.count();
}

/** Return the number of atoms in the chain identified by 'uid'

    \throw SireMol::missing_chain
*/
int StructureEditor::nAtomsInChain(quint32 uid) const
{
    int nats = 0;
    
    foreach (quint32 resuid, d->chain(uid).residues)
    {
        nats += d->residue(uid).atoms.count();
    }
    
    return nats;
}

/** Return the number of atoms in the segment identified by 'uid'

    \throw SireMol::missing_segment
*/
int StructureEditor::nAtomsInSegment(quint32 uid) const
{
    return d->segment(uid).count();
}

/** Return the number of atoms this molecule */
int StructureEditor::nAtomsInMolecule() const
{
    return d->atoms.count();
}

/** Return the number of residues in this molecule */
int StructureEditor::nResiduesInMolecule() const
{
    return d->residues.count();
}

/** Return the number of chains in this molecule */
int StructureEditor::nChainsInMolecule() const
{
    return d->chains.count();
}

/** Return the number of segments in this molecule */
int StructureEditor::nSegmentsInMolecule() const
{
    return d->segments.count();
}

/** Return the number of CutGroups in this molecule */
int StructureEditor::nCutGroupsInMolecule() const
{
    return d->cutgroups.count();
}

/** Return the UID of the atom at index 'atomidx'

    \throw SireError::invalid_index
*/
quint32 StructureEditor::getUID(AtomIdx atomidx) const
{
    return d->atoms_by_index.at( atomidx.map(nAtomsInMolecule()) );
}

/** Return the UID of the CutGroup at index 'cgidx'

    \throw SireError::invalid_index
*/
quint32 StructureEditor::getUID(CGIdx cgidx) const
{
    return d->cg_by_index.at( cgidx.map(nCutGroupsInMolecule()) );
}

/** Return the UID of the residue at index 'residx'

    \throw SireError::invalid_index
*/
quint32 StructureEditor::getUID(ResIdx residx) const
{
    return d->res_by_index.at( residx.map(nResiduesInMolecule()) );
}

/** Return the UID of the chain at index 'chainidx'

    \throw SireError::invalid_index
*/
quint32 StructureEditor::getUID(ChainIdx chainidx) const
{
    return d->chains_by_index.at( chainidx.map(nChainsInMolecule()) );
}

/** Return the UID of the segment at index 'segidx'

    \throw SireError::invalid_index
*/
quint32 StructureEditor::getUID(SegIdx segidx) const
{
    return d->seg_by_index.at( segidx.map(nSegmentsInMolecule()) );
}

/** Return the UID of the atom that matches the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
quint32 StructureEditor::getUID(const AtomID &atomid) const
{
    return this->getUID( atomIdx(atomid) );
}

/** Return the UID of the CutGroup that matches the ID 'cgid'

    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
    \throw SireError::invalid_index
*/
quint32 StructureEditor::getUID(const CGID &cgid) const
{
    return this->getUID( cgIdx(cgid) );
}

/** Return the UID of the residue that matches the ID 'resid'

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
quint32 StructureEditor::getUID(const ResID &resid) const
{
    return this->getUID( resIdx(resid) );
}

/** Return the UID of the chain that matches the ID 'chainid'

    \throw SireMol::missing_chain
    \throw SireMol::duplicate_chain
    \throw SireError::invalid_index
*/
quint32 StructureEditor::getUID(const ChainID &chainid) const
{
    return this->getUID( chainIdx(chainid) );
}

/** Return the UID of the segment that matches the ID 'segid'

    \throw SireMol::missing_segment
    \throw SireMol::duplicate_segment
    \throw SireError::invalid_index
*/
quint32 StructureEditor::getUID(const SegID &segid) const
{
    return this->getUID( segIdx(segid) );
}

quint32 StructureEditor::atomInCutGroup(quint32 uid, int i) const;
quint32 StructureEditor::atomInResidue(quint32 uid, int i) const;
quint32 StructureEditor::atomInSegment(quint32 uid, int i) const;

quint32 StructureEditor::residueInChain(quint32 uid, int i) const;

quint32 StructureEditor::cutGroupParentOfAtom(quint32 uid) const;
quint32 StructureEditor::residueParentOfAtom(quint32 uid) const;
quint32 StructureEditor::chainParentOfAtom(quint32 uid) const;
quint32 StructureEditor::segmentParentOfAtom(quint32 uid) const;

ChainIdx StructureEditor::chainParentOfResidue(quint32 uid) const;

const MolName& StructureEditor::molName() const;
MolNum StructureEditor::molNum() const;

const AtomName& StructureEditor::atomName(quint32 uid) const;
AtomNum StructureEditor::atomNum(quint32 uid) const;
AtomIdx StructureEditor::atomIdx(quint32 uid) const;

const CGName& StructureEditor::cgName(quint32 uid) const;
CGIdx StructureEditor::cgIdx(quint32 uid) const;

const ResName& StructureEditor::resName(quint32 uid) const;
ResNum StructureEditor::resNum(quint32 uid) const;
ResIdx StructureEditor::resIdx(quint32 uid) const;

const ChainName& StructureEditor::chainName(quint32 uid) const;
ChainIdx StructureEditor::chainIdx(quint32 uid) const;

const SegName& StructureEditor::segName(quint32 uid) const;
SegIdx StructureEditor::segIdx(quint32 uid) const;

AtomIdx StructureEditor::atomIdx(const AtomID &atomid) const;
CGIdx StructureEditor::cgIdx(const CGID &cgid) const;
ResIdx StructureEditor::resIdx(const ResID &resid) const;
ChainIdx StructureEditor::chainIdx(const ChainID &chainid) const;
SegIdx StructureEditor::segIdx(const SegID &segid) const;

void StructureEditor::renameMolecule(const MolName &name);

void StructureEditor::renumberMolecule();
void StructureEditor::renumberMolecule(MolNum newnum);

void StructureEditor::renameAtom(quint32 uid, const AtomName &name);
void StructureEditor::renumberAtom(quint32 uid, AtomNum number);
void StructureEditor::reindexAtom(quint32 uid, AtomIdx index);

void StructureEditor::renameCutGroup(quint32 uid, const CGName &name);
void StructureEditor::reindexCutGroup(quint32 uid, CGIdx index);

void StructureEditor::renameResidue(quint32 uid, const ResName &name);
void StructureEditor::renumberResidue(quint32 uid, ResNum number);
void StructureEditor::reindexResidue(quint32 uid, ResIdx index);

void StructureEditor::renameChain(quint32 uid, const ChainName &name);
void StructureEditor::reindexChain(quint32 uid, ChainIdx index);

void StructureEditor::renameSegment(quint32 uid, const SegName &name);
void StructureEditor::reindexSegment(quint32 uid, SegIdx index);

void StructureEditor::removeAtom(quint32 uid);
void StructureEditor::removeCutGroup(quint32 uid);
void StructureEditor::removeResidue(quint32 uid);
void StructureEditor::removeChain(quint32 uid);
void StructureEditor::removeSegment(quint32 uid);

void StructureEditor::removeAtoms(const AtomID &atomid);
void StructureEditor::removeCutGroups(const CGID &cgid);
void StructureEditor::removeResidues(const ResID &resid);
void StructureEditor::removeChains(const ChainID &chainid);
void StructureEditor::removeSegments(const SegID &segid);

void StructureEditor::reparentAtom(quint32 uid, CGIdx cgidx);
void StructureEditor::reparentAtom(quint32 uid, ResIdx residx);
void StructureEditor::reparentAtom(quint32 uid, SegIdx segidx);

void StructureEditor::reparentResidue(quint32 uid, ChainIdx chainidx);

AtomStructureEditor StructureEditor::addAtom();
CGStructureEditor StructureEditor::addCutGroup();
ResStructureEditor StructureEditor::addResidue();
ChainStructureEditor StructureEditor::addChain();
SegStructureEditor StructureEditor::addSegment();

void StructureEditor::assertValidAtom(quint32 uid) const;
void StructureEditor::assertValidCutGroup(quint32 uid) const;
void StructureEditor::assertValidResidue(quint32 uid) const;
void StructureEditor::assertValidChain(quint32 uid) const;
void StructureEditor::assertValidSegment(quint32 uid) const;
