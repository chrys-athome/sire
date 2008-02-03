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

#include "atomeditor.h"
#include "cgeditor.h"
#include "reseditor.h"
#include "chaineditor.h"
#include "segeditor.h"
#include "moleditor.h"

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

#include "tostring.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

using boost::tuple;

/////////
///////// Implementation of detail::EditMolData
/////////
namespace SireMol
{
namespace detail
{

/** This class holds the editable data of an Atom */
class EditAtomData
{
public:
    EditAtomData();
    EditAtomData(const MoleculeInfoData &molinfo, AtomIdx i);
    
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
    EditCGData(const MoleculeInfoData &molinfo, CGIdx i, 
               const EditMolData &editmol);

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
    EditResData(const MoleculeInfoData &molinfo, ResIdx residx, 
                const EditMolData &editmol);
    
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
    EditChainData(const MoleculeInfoData &molinfo, ChainIdx chainidx,
                  const EditMolData &editmol);
    
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
    EditSegData(const MoleculeInfoData &molinfo, SegIdx segidx,
                const EditMolData &editmol);
    
    EditSegData(const EditSegData &other);
    
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
    
    EditMolData(const MoleculeData &moldata);
    
    EditMolData(const EditMolData &other);
    
    ~EditMolData();
    
    const EditAtomData& atom(quint32 uid) const;
    const EditResData& residue(quint32 uid) const;
    const EditCGData& cutGroup(quint32 uid) const;
    const EditChainData& chain(quint32 uid) const;
    const EditSegData& segment(quint32 uid) const;

    EditAtomData& atom(quint32 uid);
    EditResData& residue(quint32 uid);
    EditCGData& cutGroup(quint32 uid);
    EditChainData& chain(quint32 uid);
    EditSegData& segment(quint32 uid);
    
    CGAtomIdx cgAtomIdx(quint32 atomuid, const EditAtomData &atom) const;
    ResIdx resIdx(const EditAtomData &atom) const;
    SegIdx segIdx(const EditAtomData &atom) const;
    
    ChainIdx chainIdx(const EditResData &residue) const;
    
    quint32 getNewUID();
    
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

    quint32 last_uid;
};

} // end of namespace detail
} // end of namespace SireMol

using namespace SireMol::detail;

/////////
///////// Implementation of EditAtomData
/////////

QDataStream& operator<<(QDataStream &ds, const EditAtomData &editatom)
{
    ds << editatom.name << editatom.number
       << editatom.cg_parent << editatom.res_parent
       << editatom.seg_parent << editatom.properties;
       
    return ds;
}

QDataStream& operator>>(QDataStream &ds, EditAtomData &editatom)
{
    ds >> editatom.name >> editatom.number
       >> editatom.cg_parent >> editatom.res_parent
       >> editatom.seg_parent >> editatom.properties;
       
    return ds;
}

EditAtomData::EditAtomData()
             : name( QString::null ), number( AtomNum::null() ),
               cg_parent(0), res_parent(0), seg_parent(0)
{}
   
EditAtomData::EditAtomData(const MoleculeInfoData &molinfo, AtomIdx i)
             : name(molinfo.name(i)), number(molinfo.number(i)),
               cg_parent(0), res_parent(0), seg_parent(0)
{}
   
EditAtomData::EditAtomData(const EditAtomData &other)
             : name(other.name), number(other.number),
               cg_parent(other.cg_parent), res_parent(other.res_parent),
               seg_parent(other.seg_parent), properties(other.properties)
{}
    
EditAtomData::~EditAtomData()
{}

/////////
///////// Implementation of EditCGData
/////////

QDataStream& operator<<(QDataStream &ds, const EditCGData &editcg)
{
    ds << editcg.name << editcg.atoms << editcg.properties;
    return ds;
}

QDataStream& operator>>(QDataStream &ds, EditCGData &editcg)
{
    ds >> editcg.name >> editcg.atoms >> editcg.properties;
    return ds;
}

EditCGData::EditCGData() : name( QString::null )
{}

EditCGData::EditCGData(const MoleculeInfoData &molinfo, CGIdx i,
                       const EditMolData &editmol)
           : name(molinfo.name(i))
{
    foreach (AtomIdx atomidx, molinfo.getAtomsIn(i))
    {
        quint32 uid = editmol.atoms_by_index.at(atomidx);
        atoms.append(uid);
    }
}

EditCGData::EditCGData(const EditCGData &other)
           : name(other.name), atoms(other.atoms), properties(other.properties)
{}
    
EditCGData::~EditCGData()
{}

/////////
///////// Implementation of EditResData
/////////

QDataStream& operator<<(QDataStream &ds, const EditResData &editres)
{
    ds << editres.name << editres.number << editres.chain_parent
       << editres.atoms << editres.properties;
       
    return ds;
}

QDataStream& operator>>(QDataStream &ds, EditResData &editres)
{
    ds >> editres.name >> editres.number >> editres.chain_parent
       >> editres.atoms >> editres.properties;
       
    return ds;
}

EditResData::EditResData()
            : name( QString::null ), number( ResNum::null() ),
              chain_parent(0)
{}

EditResData::EditResData(const MoleculeInfoData &molinfo, ResIdx i,
                         const EditMolData &editmol)
            : name(molinfo.name(i)), number(molinfo.number(i))
{
    foreach (AtomIdx atomidx, molinfo.getAtomsIn(i))
    {
        quint32 uid = editmol.atoms_by_index.at(atomidx);
        atoms.append(uid);
    }
}

EditResData::EditResData(const EditResData &other)
            : name(other.name), number(other.number), 
              chain_parent(other.chain_parent), atoms(other.atoms),
              properties(other.properties)
{}
    
EditResData::~EditResData()
{}

/////////
///////// Implementation of EditChainData
/////////

QDataStream& operator<<(QDataStream &ds, const EditChainData &editchain)
{
    ds << editchain.name << editchain.residues << editchain.properties;
    
    return ds;
}

QDataStream& operator>>(QDataStream &ds, EditChainData &editchain)
{
    ds >> editchain.name >> editchain.residues >> editchain.properties;
    
    return ds;
}

EditChainData::EditChainData() : name( QString::null )
{}

EditChainData::EditChainData(const MoleculeInfoData &molinfo, ChainIdx i,
                             const EditMolData &editmol)
              : name(molinfo.name(i))
{
    foreach (ResIdx residx, molinfo.getResiduesIn(i))
    {
        quint32 uid = editmol.res_by_index.at(residx);
        residues.append(uid);
    }
}

EditChainData::EditChainData(const EditChainData &other)
              : name(other.name), residues(other.residues),
                properties(other.properties)
{}
    
EditChainData::~EditChainData()
{}
    
/////////
///////// Implementation of EditSegData
/////////

QDataStream& operator<<(QDataStream &ds, const EditSegData &editseg)
{
    ds << editseg.name << editseg.atoms << editseg.properties;
    
    return ds;
}

QDataStream& operator>>(QDataStream &ds, EditSegData &editseg)
{
    ds >> editseg.name >> editseg.atoms >> editseg.properties;
    
    return ds;
}

EditSegData::EditSegData() : name( QString::null )
{}

EditSegData::EditSegData(const MoleculeInfoData &molinfo, SegIdx i,
                         const EditMolData &editmol)
            : name(molinfo.name(i))
{
    foreach (AtomIdx atomidx, molinfo.getAtomsIn(i))
    {
        quint32 uid = editmol.atoms_by_index.at(atomidx);
        atoms.append(uid);
    }
}

EditSegData::EditSegData(const EditSegData &other)
            : name(other.name), atoms(other.atoms),
              properties(other.properties)
{}
    
EditSegData::~EditSegData()
{}

/////////
///////// Implementation of EditMolData
/////////

/** Serialise to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const EditMolData &editmol)
{
    ds << editmol.molname << editmol.molnum
       << editmol.atoms << editmol.cutgroups << editmol.residues
       << editmol.chains << editmol.segments
       << editmol.atoms_by_index << editmol.cg_by_index
       << editmol.res_by_index << editmol.chains_by_index
       << editmol.seg_by_index
       << editmol.properties
       << editmol.last_uid;
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream& operator>>(QDataStream &ds, EditMolData &editmol)
{
    ds >> editmol.molname >> editmol.molnum
       >> editmol.atoms >> editmol.cutgroups >> editmol.residues
       >> editmol.chains >> editmol.segments
       >> editmol.atoms_by_index >> editmol.cg_by_index
       >> editmol.res_by_index >> editmol.chains_by_index
       >> editmol.seg_by_index
       >> editmol.properties
       >> editmol.last_uid;
       
    return ds;
}

/** Constructor */
EditMolData::EditMolData() : molname( QString::null ), 
                             molnum( MolNum::null() ), last_uid(0)
{}

/** Construct from a MoleculeData */
EditMolData::EditMolData(const MoleculeData &moldata)
            : molname(moldata.name()), molnum(moldata.number())
{
    const MoleculeInfoData &molinfo = moldata.info();
    
    int nats = molinfo.nAtoms();
    
    for (AtomIdx i(0); i<nats; ++i)
    {
        EditAtomData atom(molinfo, i);
    
        quint32 uid = getNewUID();
        atoms.insert(uid, atom);
        atoms_by_index.append(uid);
    }
    
    int nres = molinfo.nResidues();
    
    for (ResIdx i(0); i<nres; ++i)
    {
        EditResData residue(molinfo, i, *this);
        
        quint32 uid = getNewUID();
        residues.insert(uid, residue);
        res_by_index.append(uid);
        
        foreach (quint32 atomuid, residue.atoms)
        {
            this->atom(atomuid).res_parent = uid;
        }
    }
    
    int ncg = molinfo.nCutGroups();
    
    for (CGIdx i(0); i<ncg; ++i)
    {
        EditCGData cgroup(molinfo, i, *this);
        
        quint32 uid = getNewUID();
        cutgroups.insert(uid, cgroup);
        cg_by_index.append(uid);
        
        foreach (quint32 atomuid, cgroup.atoms)
        {
            this->atom(atomuid).cg_parent = uid;
        }
    }
    
    int nchains = molinfo.nChains();
    
    for (ChainIdx i(0); i<nchains; ++i)
    {
        EditChainData chain(molinfo, i, *this);
        
        quint32 uid = getNewUID();
        chains.insert(uid, chain);
        chains_by_index.append(uid);
        
        foreach (quint32 resuid, chain.residues)
        {
            this->residue(resuid).chain_parent = uid;
        }
    }
    
    int nseg = molinfo.nSegments();
    
    for (SegIdx i(0); i<nseg; ++i)
    {
        EditSegData segment(molinfo, i, *this);
        
        quint32 uid = getNewUID();
        segments.insert(uid, segment);
        seg_by_index.append(uid);
        
        foreach (quint32 atomuid, segment.atoms)
        {
            this->atom(atomuid).seg_parent = uid;
        }
    }
    
    //now extract all of the properties...
    properties = moldata.properties();
    
    //need to convert them...
    qFatal("Need to convert properties!!!");
}

/** Copy constructor */
EditMolData::EditMolData(const EditMolData &other)
            : molname(other.molname), molnum(other.molnum),
              atoms(other.atoms), cutgroups(other.cutgroups),
              residues(other.residues), chains(other.chains),
              segments(other.segments),
              atoms_by_index(other.atoms_by_index),
              cg_by_index(other.cg_by_index),
              res_by_index(other.res_by_index),
              chains_by_index(other.chains_by_index),
              seg_by_index(other.seg_by_index),
              properties(other.properties),
              last_uid(other.last_uid)
{}

/** Destructor */
EditMolData::~EditMolData()
{}

/** Get a new unique ID number */
quint32 EditMolData::getNewUID()
{
    if (last_uid == std::numeric_limits<quint32>::max())
        throw SireError::program_bug( QObject::tr(
            "An EditMolData can only identify %1 unique objects!")
                .arg(std::numeric_limits<quint32>::max()), CODELOC );
                
    ++last_uid;
    
    return last_uid;
}
    
const EditAtomData& EditMolData::atom(quint32 uid) const
{
    QHash<quint32,EditAtomData>::const_iterator it = atoms.find(uid);
    
    if (it == atoms.end())
        throw SireMol::missing_atom( QObject::tr(
            "There is no atom in this molecule that is identified by "
            "the UID %1.").arg(uid), CODELOC );
            
    return it.value();
}

const EditResData& EditMolData::residue(quint32 uid) const
{
    QHash<quint32,EditResData>::const_iterator it = residues.find(uid);
    
    if (it == residues.end())
        throw SireMol::missing_residue( QObject::tr(
            "There is no residue in this molecule that is identified by "
            "the UID %1.").arg(uid), CODELOC );
            
    return it.value();
}

const EditCGData& EditMolData::cutGroup(quint32 uid) const
{
    QHash<quint32,EditCGData>::const_iterator it = cutgroups.find(uid);
    
    if (it == cutgroups.end())
        throw SireMol::missing_cutgroup( QObject::tr(
            "There is no CutGroup in this molecule that is identified by "
            "the UID %1.").arg(uid), CODELOC );
            
    return it.value();
}

const EditChainData& EditMolData::chain(quint32 uid) const
{
    QHash<quint32,EditChainData>::const_iterator it = chains.find(uid);
    
    if (it == chains.end())
        throw SireMol::missing_chain( QObject::tr(
            "There is no chain in this molecule that is identified by "
            "the UID %1.").arg(uid), CODELOC );
            
    return it.value();
}

const EditSegData& EditMolData::segment(quint32 uid) const
{
    QHash<quint32,EditSegData>::const_iterator it = segments.find(uid);
    
    if (it == segments.end())
        throw SireMol::missing_segment( QObject::tr(
            "There is no segment in this molecule that is identified by "
            "the UID %1.").arg(uid), CODELOC );
            
    return it.value();
}

EditAtomData& EditMolData::atom(quint32 uid)
{
    QHash<quint32,EditAtomData>::iterator it = atoms.find(uid);
    
    if (it == atoms.end())
        throw SireMol::missing_atom( QObject::tr(
            "There is no atom in this molecule that is identified by "
            "the UID %1.").arg(uid), CODELOC );
            
    return it.value();
}

EditResData& EditMolData::residue(quint32 uid)
{
    QHash<quint32,EditResData>::iterator it = residues.find(uid);
    
    if (it == residues.end())
        throw SireMol::missing_residue( QObject::tr(
            "There is no residue in this molecule that is identified by "
            "the UID %1.").arg(uid), CODELOC );
            
    return it.value();
}

EditCGData& EditMolData::cutGroup(quint32 uid)
{
    QHash<quint32,EditCGData>::iterator it = cutgroups.find(uid);
    
    if (it == cutgroups.end())
        throw SireMol::missing_cutgroup( QObject::tr(
            "There is no CutGroup in this molecule that is identified by "
            "the UID %1.").arg(uid), CODELOC );
            
    return it.value();
}

EditChainData& EditMolData::chain(quint32 uid)
{
    QHash<quint32,EditChainData>::iterator it = chains.find(uid);
    
    if (it == chains.end())
        throw SireMol::missing_chain( QObject::tr(
            "There is no chain in this molecule that is identified by "
            "the UID %1.").arg(uid), CODELOC );
            
    return it.value();
}

EditSegData& EditMolData::segment(quint32 uid)
{
    QHash<quint32,EditSegData>::iterator it = segments.find(uid);
    
    if (it == segments.end())
        throw SireMol::missing_segment( QObject::tr(
            "There is no segment in this molecule that is identified by "
            "the UID %1.").arg(uid), CODELOC );
            
    return it.value();
}

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

/** Null constructor */
StructureEditor::StructureEditor()
{}

/** Assign so that this will edit a copy of 'moldata' */
StructureEditor& StructureEditor::operator=(const MoleculeData &moldata)
{
    d.reset( new EditMolData(moldata) );
    
    return *this;
}

/** Construct an editor that edits a copy of 'moldata' */
StructureEditor::StructureEditor(const MoleculeData &moldata)
{
    this->operator=(moldata);
}

/** Copy constructor - this is fast as this class is explicitly
    shared */
StructureEditor::StructureEditor(const StructureEditor &other)
                : d(other.d)
{}

/** Destructor */
StructureEditor::~StructureEditor()
{}

/** Copy assignment operator - this is fast as this class is 
    explicitly shared */
StructureEditor& StructureEditor::operator=(const StructureEditor &other)
{
    d = other.d;
    return *this;
}

CGAtomIdx EditMolData::cgAtomIdx(quint32 atomuid, const EditAtomData &atom) const
{
    if (atom.cg_parent == 0)
        return CGAtomIdx::null();
        
    else
        return CGAtomIdx( CGIdx(cg_by_index.indexOf(atom.cg_parent)), 
                          Index(cutGroup(atom.cg_parent).atoms.indexOf(atomuid) ) );
}

ResIdx EditMolData::resIdx(const EditAtomData &atom) const
{
    if (atom.res_parent == 0)
        return ResIdx::null();
    else
        return ResIdx( res_by_index.indexOf(atom.res_parent) );
}

SegIdx EditMolData::segIdx(const EditAtomData &atom) const
{
    if (atom.seg_parent == 0)
        return SegIdx::null();
    else
        return SegIdx( seg_by_index.indexOf(atom.seg_parent) );
}

/** Return all of the metadata about the atom at index 'atomidx'

    \throw SireError::invalid_index
*/
tuple<AtomName,AtomNum,CGAtomIdx,ResIdx,SegIdx> 
StructureEditor::getAtomData(AtomIdx atomidx) const
{
    quint32 atomuid = getUID(atomidx);
    const EditAtomData &atom = d->atom(atomuid);
    
    return tuple<AtomName,AtomNum,CGAtomIdx,ResIdx,SegIdx>(
                 atom.name, atom.number, d->cgAtomIdx(atomuid, atom),
                 d->resIdx(atom), d->segIdx(atom) );
}

/** Return all of the metadata about the CutGroup at index 'cgidx' 

    \throw SireError::invalid_index
*/
boost::tuple< CGName,QList<AtomIdx> >
StructureEditor::getCGData(CGIdx cgidx) const
{
    const EditCGData &cgroup = d->cutGroup( getUID(cgidx) );
    
    if (cgroup.atoms.isEmpty())
        return tuple< CGName,QList<AtomIdx> >(cgroup.name, QList<AtomIdx>());
    else
    {
        QList<AtomIdx> atomidxs;
        
        foreach (quint32 atom, cgroup.atoms)
        {
            atomidxs.append( AtomIdx(d->atoms_by_index.indexOf(atom)) );
        }
        
        return tuple< CGName,QList<AtomIdx> >(cgroup.name, atomidxs);
    }
}

ChainIdx EditMolData::chainIdx(const EditResData &residue) const
{
    if (residue.chain_parent == 0)
        return ChainIdx::null();
    else
        return ChainIdx( chains_by_index.indexOf(residue.chain_parent) );
}

/** Return the metadata for the residue at index 'residx' 

    \throw SireError::invalid_index
*/
boost::tuple< ResName,ResNum,ChainIdx,QList<AtomIdx> >
StructureEditor::getResData(ResIdx residx) const
{
    const EditResData &residue = d->residue( getUID(residx) );
    
    QList<AtomIdx> atomidxs;
    
    foreach (quint32 atom, residue.atoms)
    {
        atomidxs.append( AtomIdx(d->atoms_by_index.indexOf(atom) ) );
    }
    
    return tuple< ResName,ResNum,ChainIdx,QList<AtomIdx> >(
                    residue.name, residue.number, d->chainIdx(residue), atomidxs );
}

/** Return the metadata for the chain at index 'chainidx'

    \throw SireError::invalid_index
*/
boost::tuple< ChainName,QList<ResIdx> >
StructureEditor::getChainData(ChainIdx chainidx) const
{
    const EditChainData &chain = d->chain( getUID(chainidx) );
    
    QList<ResIdx> residxs;
    
    foreach (quint32 residue, chain.residues)
    {
        residxs.append( ResIdx(d->res_by_index.indexOf(residue)) );
    }
    
    return tuple< ChainName,QList<ResIdx> >(chain.name, residxs);
}

/** Return the metadata for the segment at index 'segidx' 

    \throw SireError::invalid_index
*/
boost::tuple< SegName,QList<AtomIdx> >
StructureEditor::getSegData(SegIdx segidx) const
{
    const EditSegData &segment = d->segment( getUID(segidx) );
    
    QList<AtomIdx> atomidxs;
    
    foreach (quint32 atom, segment.atoms)
    {
        atomidxs.append( AtomIdx(d->atoms_by_index.indexOf(atom)) );
    }
    
    return tuple< SegName,QList<AtomIdx> >(segment.name, atomidxs);
}

/** Commit the changes - this creates a MoleculeData object that contains
    all of the data in this editor */
MoleculeData StructureEditor::commitChanges() const
{
    return MoleculeData(*this);
}

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
    return d->cutGroup(uid).atoms.count();
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
        nats += d->residue(resuid).atoms.count();
    }
    
    return nats;
}

/** Return the number of atoms in the segment identified by 'uid'

    \throw SireMol::missing_segment
*/
int StructureEditor::nAtomsInSegment(quint32 uid) const
{
    return d->segment(uid).atoms.count();
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

/** Assert that the UID 'uid' refers to a valid atom 

    \throw SireMol::missing_atom
*/
void StructureEditor::assertValidAtom(quint32 uid) const
{
    d->atom(uid);
}

/** Assert that the UID 'uid' refers to a valid CutGroup

    \throw SireMol::missing_cutgroup
*/
void StructureEditor::assertValidCutGroup(quint32 uid) const
{
    d->cutGroup(uid);
}

/** Assert that the UID 'uid' refers to a valid residue

    \throw SireMol::missing_residue
*/
void StructureEditor::assertValidResidue(quint32 uid) const
{
    d->residue(uid);
}

/** Assert that the UID 'uid' refers to a valid chain

    \throw SireMol::missing_chain
*/
void StructureEditor::assertValidChain(quint32 uid) const
{
    d->chain(uid);
}

/** Assert that the UID 'uid' refers to a valid segment

    \throw SireMol::missing_segment
*/
void StructureEditor::assertValidSegment(quint32 uid) const
{
    d->segment(uid);
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

/** Return the UID of the ith atom in the CutGroup identified by 'uid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
quint32 StructureEditor::atomInCutGroup(quint32 uid, int i) const
{
    const EditCGData &cgroup = d->cutGroup(uid);
    
    return cgroup.atoms.at( Index(i).map(cgroup.atoms.count()) );
}

/** Return the UID of the ith atom in the residue identified by 'uid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
quint32 StructureEditor::atomInResidue(quint32 uid, int i) const
{
    const EditResData &residue = d->residue(uid);
    
    return residue.atoms.at( Index(i).map(residue.atoms.count()) );
}

/** Return the UID of the ith atom in the segment identified
    by 'uid'
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
quint32 StructureEditor::atomInSegment(quint32 uid, int i) const
{
    const EditSegData &segment = d->segment(uid);
    
    return segment.atoms.at( Index(i).map(segment.atoms.count()) );
}

/** Return the UID of the ith residue in the chain identified
    by 'uid'
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
quint32 StructureEditor::residueInChain(quint32 uid, int i) const
{
    const EditChainData &chain = d->chain(uid);
    
    return chain.residues.at( Index(i).map(chain.residues.count()) );
}

/** Return the UID of the CutGroup that contains the atom identified
    by 'uid'
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_cutgroup
*/
quint32 StructureEditor::cutGroupParentOfAtom(quint32 uid) const
{
    const EditAtomData &atom = d->atom(uid);
    
    if (atom.cg_parent == 0)
        throw SireMol::missing_cutgroup( QObject::tr(
            "The atom at index %1 is not part of a CutGroup.")
                .arg(this->atomIdx(uid)), CODELOC );
                
    return atom.cg_parent;
}

/** Return the UID of the residue that contains the atom identified
    by 'uid'
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
quint32 StructureEditor::residueParentOfAtom(quint32 uid) const
{
    const EditAtomData &atom = d->atom(uid);
    
    if (atom.res_parent == 0)
        throw SireMol::missing_residue( QObject::tr(
            "The atom at index %1 is not part of a residue.")
                .arg(this->atomIdx(uid)), CODELOC );
                
    return atom.res_parent;
}

/** Return the UID of the chain that contains the residue
    identified by the ID 'uid'
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_chain
*/
quint32 StructureEditor::chainParentOfResidue(quint32 uid) const
{
    const EditResData &residue = d->residue(uid);
    
    if (residue.chain_parent == 0)
        throw SireMol::missing_chain( QObject::tr(
            "The residue at index %1 is not part of a chain.")
                .arg(this->resIdx(uid)), CODELOC );
                
    return residue.chain_parent;
}

/** Return the UID of the chain that contains the atom identified
    by the UID 'uid'
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::missing_chain
*/
quint32 StructureEditor::chainParentOfAtom(quint32 uid) const
{
    return this->chainParentOfResidue( this->residueParentOfAtom(uid) );
}

/** Return the UID of the segment that contains the atom identified
    by the UID 'uid'
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_segment
*/
quint32 StructureEditor::segmentParentOfAtom(quint32 uid) const
{
    const EditAtomData &atom = d->atom(uid);
    
    if (atom.seg_parent == 0)
        throw SireMol::missing_segment( QObject::tr(
            "The atom at index %1 is not part of a segment.")
                .arg(this->atomIdx(uid)), CODELOC );
                
    return atom.seg_parent;
}

/** Return the name of this molecule */
const MolName& StructureEditor::molName() const
{
    return d->molname;
}

/** Return the number of this molecule */
MolNum StructureEditor::molNum() const
{
    return d->molnum;
}

/** Return the name of the atom identified by 'uid'

    \throw SireMol::missing_atom
*/
const AtomName& StructureEditor::atomName(quint32 uid) const
{
    return d->atom(uid).name;
}

/** Return the number of the atom identified by 'uid' 
    
    \throw SireMol::missing_atom
*/
AtomNum StructureEditor::atomNum(quint32 uid) const
{
    return d->atom(uid).number;
}

/** Return the index of the atom identified by 'uid'

    \throw SireMol::missing_atom
*/
AtomIdx StructureEditor::atomIdx(quint32 uid) const
{
    int i = d->atoms_by_index.indexOf(uid);
    
    if (i == -1)
        throw SireMol::missing_atom( QObject::tr(
            "There is no atom identified by the UID %1 in this molecule.")
                .arg(uid), CODELOC );
                
    return AtomIdx(i);
}

/** Return the name of the CutGroup identified by 'uid'

    \throw SireMol::missing_cutgroup
*/
const CGName& StructureEditor::cgName(quint32 uid) const
{
    return d->cutGroup(uid).name;
}

/** Return the index of the CutGroup identified by 'uid'

    \throw SireMol::missing_cutgroup
*/
CGIdx StructureEditor::cgIdx(quint32 uid) const
{
    int i = d->cg_by_index.indexOf(uid);
    
    if (i == -1)
        throw SireMol::missing_cutgroup( QObject::tr(
            "There is no CutGroup identified by the UID %1 in this molecule.")
                .arg(uid), CODELOC );
                
    return CGIdx(i);
}

/** Return the name of the residue identified by 'uid'

    \throw SireMol::missing_residue
*/
const ResName& StructureEditor::resName(quint32 uid) const
{
    return d->residue(uid).name;
}

/** Return the number of the residue identified by 'uid'

    \throw SireMol::missing_residue
*/
ResNum StructureEditor::resNum(quint32 uid) const
{
    return d->residue(uid).number;
}

/** Return the index of the residue identified by 'uid'

    \throw SireMol::missing_residue
*/
ResIdx StructureEditor::resIdx(quint32 uid) const
{
    int i = d->res_by_index.indexOf(uid);
    
    if (i == -1)
        throw SireMol::missing_residue( QObject::tr(
            "There is no residue identified by the UID %1 in this molecule.")
                .arg(uid), CODELOC );
                
    return ResIdx(i);
}

/** Return the name of the chain identified by 'uid'

    \throw SireMol::missing_chain
*/
const ChainName& StructureEditor::chainName(quint32 uid) const
{
    return d->chain(uid).name;
}

/** Return the index of the chain identified by 'uid'

    \throw SireMol::missing_chain
*/
ChainIdx StructureEditor::chainIdx(quint32 uid) const
{
    int i = d->chains_by_index.indexOf(uid);
    
    if (i == -1)
        throw SireMol::missing_chain( QObject::tr(
            "There is no chain identified by the UID %1 in this molecule.")
                .arg(uid), CODELOC );
                
    return ChainIdx(i);
}

/** Return the name of the segment identified by 'uid'

    \throw SireMol::missing_segment
*/
const SegName& StructureEditor::segName(quint32 uid) const
{
    return d->segment(uid).name;
}

/** Return the index of the segment identified by 'uid'

    \throw SireMol::missing_segment
*/
SegIdx StructureEditor::segIdx(quint32 uid) const
{
    int i = d->seg_by_index.indexOf(uid);
    
    if (i == -1)
        throw SireMol::missing_segment( QObject::tr(
            "There is no segment identified by the UID %1 in this molecule.")
                .arg(uid), CODELOC );
                
    return SegIdx(i);
}

/** Return the index of the atom that matches the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
AtomIdx StructureEditor::atomIdx(const AtomID &atomid) const
{
    QList<AtomIdx> atomidxs = atomid.map( EditMolInfo(*this) );
    
    if (atomidxs.count() > 1)
        throw SireMol::duplicate_atom( QObject::tr(
            "More than one atom in the molecule matches the ID \"%1\" %2")
                .arg(atomid.toString(), Sire::toString(atomidxs)), CODELOC );
                
    return atomidxs.first();
}

/** Return the index of the CutGroup that matches the ID 'cgid'

    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
    \throw SireError::invalid_index
*/
CGIdx StructureEditor::cgIdx(const CGID &cgid) const
{
    QList<CGIdx> cgidxs = cgid.map( EditMolInfo(*this) );
    
    if (cgidxs.count() > 1)
        throw SireMol::duplicate_cutgroup( QObject::tr(
            "More than one CutGroup in the molecule matches the ID \"%1\" %2")
                .arg(cgid.toString(), Sire::toString(cgidxs)), CODELOC );
                
    return cgidxs.first();
}

/** Return the index of the residue that matches the ID 'resid'

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
ResIdx StructureEditor::resIdx(const ResID &resid) const
{
    QList<ResIdx> residxs = resid.map( EditMolInfo(*this) );
    
    if (residxs.count() > 1)
        throw SireMol::duplicate_residue( QObject::tr(
            "More than one residue in the molecule matches the ID \"%1\" %2")
                .arg(resid.toString(), Sire::toString(residxs)), CODELOC );
                
    return residxs.first();
}

/** Return the index of the chain that matches the ID 'chainid'

    \throw SireMol::missing_chain
    \throw SireMol::duplicate_chain
    \throw SireError::invalid_index
*/
ChainIdx StructureEditor::chainIdx(const ChainID &chainid) const
{
    QList<ChainIdx> chainidxs = chainid.map( EditMolInfo(*this) );
    
    if (chainidxs.count() > 1)
        throw SireMol::duplicate_chain( QObject::tr(
            "More than one chain in the molecule matches the ID \"%1\" %2")
                .arg(chainid.toString(), Sire::toString(chainidxs)), CODELOC );
                
    return chainidxs.first();
}

/** Return the index of the segment that matches the ID 'segid'

    \throw SireMol::missing_segment
    \throw SireMol::duplicate_segment
    \throw SireError::invalid_index
*/
SegIdx StructureEditor::segIdx(const SegID &segid) const
{
    QList<SegIdx> segidxs = segid.map( EditMolInfo(*this) );
    
    if (segidxs.count() > 1)
        throw SireMol::duplicate_segment( QObject::tr(
            "More than one segment in the molecule matches the ID \"%1\" %2")
                .arg(segid.toString(), Sire::toString(segidxs)), CODELOC );
                
    return segidxs.first();
}

/** Rename this molecule to 'newname' */
void StructureEditor::renameMolecule(const MolName &newname)
{
    d->molname = newname;
}

/** Give this molecule a new, unique, number */
void StructureEditor::renumberMolecule()
{
    d->molnum = MolNum::getUniqueNumber();
}

/** Renumber this molecule to 'newnum' */
void StructureEditor::renumberMolecule(MolNum newnum)
{
    d->molnum = newnum;
}

/** Rename the atom identified by 'uid' to 'newname'

    \throw SireMol::missing_atom
*/
void StructureEditor::renameAtom(quint32 uid, const AtomName &newname)
{
    d->atom(uid).name = newname;
}

/** Renumber the atom identified by 'uid' to 'newnum'

    \throw SireMol::missing_atom
*/
void StructureEditor::renumberAtom(quint32 uid, AtomNum newnum)
{
    d->atom(uid).number = newnum;
}

static void changeIndex(QList<quint32> &uids, quint32 uid, int newidx)
{
    if (uids.removeAll(uid) == 0)
        throw SireError::program_bug( QObject::tr(
            "Couldn't remove UID %1 from UIDs %2")
                .arg(uid).arg( Sire::toString(uids) ), CODELOC );
                
    if (newidx < 0)
    {
        newidx = uids.count() + newidx;
        
        if (newidx < 0)
            newidx = 0;
    }
    else if (newidx > uids.count())
    {
        newidx = uids.count();
    }
    
    uids.insert( newidx, uid );
}

/** Change the index of the atom identified by 'uid' to 'newidx'

    \throw SireMol::missing_atom
*/
void StructureEditor::reindexAtom(quint32 uid, AtomIdx newidx)
{
    this->assertValidAtom(uid);
    
    changeIndex( d->atoms_by_index, uid, newidx );
}

/** Rename the CutGroup identified by 'uid' to 'newname'

    \throw SireMol::missing_cutgroup
*/
void StructureEditor::renameCutGroup(quint32 uid, const CGName &newname)
{
    d->cutGroup(uid).name = newname;
}

/** Change the index of the atom identified by 'uid' to 'newidx'

    \throw SireMol::missing_cutgroup
*/
void StructureEditor::reindexCutGroup(quint32 uid, CGIdx newidx)
{
    this->assertValidCutGroup(uid);
    
    changeIndex( d->cg_by_index, uid, newidx );
}

/** Rename the residue identified by 'uid' to 'newname'

    \throw SireMol::missing_residue
*/
void StructureEditor::renameResidue(quint32 uid, const ResName &newname)
{
    d->residue(uid).name = newname;
}

/** Renumber the residue identified by 'uid' to 'newnum'

    \throw SireMol::missing_residue
*/
void StructureEditor::renumberResidue(quint32 uid, ResNum newnum)
{
    d->residue(uid).number = newnum;
}

/** Change the index of the residue identified by 'uid' to 'newidx'

    \throw SireMol::missing_residue
*/
void StructureEditor::reindexResidue(quint32 uid, ResIdx newidx)
{
    this->assertValidResidue(uid);
    
    changeIndex( d->res_by_index, uid, newidx );
}

/** Rename the chain identified by 'uid' to 'newname'

    \throw SireMol::missing_chain
*/
void StructureEditor::renameChain(quint32 uid, const ChainName &newname)
{
    d->chain(uid).name = newname;
}

/** Change the index of the chain identified by 'uid' to 'newidx'

    \throw SireMol::missing_chain
*/
void StructureEditor::reindexChain(quint32 uid, ChainIdx newidx)
{
    this->assertValidChain(uid);
    
    changeIndex( d->chains_by_index, uid, newidx );
}

/** Rename the segment identified by 'uid' to 'newname'

    \throw SireMol::missing_segment
*/
void StructureEditor::renameSegment(quint32 uid, const SegName &newname)
{
    d->segment(uid).name = newname;
}

/** Change the index of the segment identified by 'uid' to 'newidx' */
void StructureEditor::reindexSegment(quint32 uid, SegIdx newidx)
{
    this->assertValidSegment(uid);
    
    changeIndex( d->seg_by_index, uid, newidx );
}

/** Remove the atom identified by 'uid'

    \throw SireMol::missing_atom
*/
void StructureEditor::removeAtom(quint32 uid)
{
    const EditAtomData &atom = d->atom(uid);
    
    //remove the atom from its parent groups...
    if (atom.cg_parent != 0)
        d->cutGroup(atom.cg_parent).atoms.removeAll(uid);
        
    if (atom.res_parent != 0)
        d->residue(atom.res_parent).atoms.removeAll(uid);
        
    if (atom.seg_parent != 0)
        d->segment(atom.seg_parent).atoms.removeAll(uid);
    
    //now remove the atom itself
    d->atoms.remove(uid);
    d->atoms_by_index.removeAll(uid);
}

/** Remove the CutGroup identified by 'uid'. This only
    removes the CutGroup - it doesn't remove the atoms
    that are contained in this CutGroup
    
    \throw SireMol::missing_cutgroup
*/
void StructureEditor::removeCutGroup(quint32 uid)
{
    const EditCGData &cgroup = d->cutGroup(uid);
    
    //tell the atoms that they don't now have a CG parent
    foreach (quint32 atom, cgroup.atoms)
    {
        d->atom(atom).cg_parent = 0;
    }
    
    d->cutgroups.remove(uid);
    d->cg_by_index.removeAll(uid);
}

/** Remove the residue identified by 'uid'

    \throw SireMol:missing_residue
*/
void StructureEditor::removeResidue(quint32 uid)
{
    const EditResData &residue = d->residue(uid);
    
    //tell the parent Chain that this residue is leaving...
    if (residue.chain_parent != 0)
        d->chain(residue.chain_parent).residues.removeAll(uid);
        
    //now tell the atoms that this residue is leaving
    foreach (quint32 atom, residue.atoms)
    {
        d->atom(atom).res_parent = 0;
    }
    
    d->residues.remove(uid);
    d->res_by_index.removeAll(uid);
}

/** Remove the chain identified by 'uid'

    \throw SireMol::missing_chain
*/
void StructureEditor::removeChain(quint32 uid)
{
    const EditChainData &chain = d->chain(uid);
    
    //tell each residue that this chain is leaving
    foreach (quint32 residue, chain.residues)
    {
        d->residue(residue).chain_parent = 0;
    }
    
    d->chains.remove(uid);
    d->chains_by_index.removeAll(uid);
}

/** Remove the segment identified by 'uid'

    \throw SireMol::missing_segment
*/
void StructureEditor::removeSegment(quint32 uid)
{
    const EditSegData &segment = d->segment(uid);
    
    //tell each atom that this segment is leaving
    foreach (quint32 atom, segment.atoms)
    {
        d->atom(atom).seg_parent = 0;
    }
    
    d->segments.remove(uid);
    d->seg_by_index.removeAll(uid);
}

/** Remove all atoms identified by 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
void StructureEditor::removeAtoms(const AtomID &atomid)
{
    QList<AtomIdx> atomidxs = atomid.map( EditMolInfo(*this) );
    
    if (atomidxs.count() == 1)
    {
        this->removeAtom( getUID(atomidxs.first()) );
        return;
    }
    
    //convert the atomidx to uid
    QList<quint32> uids;
    
    foreach (AtomIdx atomidx, atomidxs)
    {
        uids.append( getUID(atomidx) );
    }
    
    //now remove all of the atoms
    foreach (quint32 uid, uids)
    {
        this->removeAtom(uid);
    }
}

/** Remove all CutGroups identified by 'cgid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void StructureEditor::removeCutGroups(const CGID &cgid)
{
    QList<CGIdx> cgidxs = cgid.map( EditMolInfo(*this) );
    
    if (cgidxs.count() == 1)
    {
        this->removeCutGroup( getUID(cgidxs.first()) );
        return;
    }
    
    //convert the cgidx to uid
    QList<quint32> uids;
    
    foreach (CGIdx cgidx, cgidxs)
    {
        uids.append( getUID(cgidx) );
    }
    
    //now remove all of the CutGroups
    foreach (quint32 uid, uids)
    {
        this->removeCutGroup(uid);
    }
}

/** Remove all residues identified by 'resid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void StructureEditor::removeResidues(const ResID &resid)
{
    QList<ResIdx> residxs = resid.map( EditMolInfo(*this) );
    
    if (residxs.count() == 1)
    {
        this->removeResidue( getUID(residxs.first()) );
        return;
    }
    
    //convert the residx to uid
    QList<quint32> uids;
    
    foreach (ResIdx residx, residxs)
    {
        uids.append( getUID(residx) );
    }
    
    //now remove all of the residues
    foreach (quint32 uid, uids)
    {
        this->removeResidue(uid);
    }
}

/** Remove all chains identified by 'chainid'

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
void StructureEditor::removeChains(const ChainID &chainid)
{
    QList<ChainIdx> chainidxs = chainid.map( EditMolInfo(*this) );
    
    if (chainidxs.count() == 1)
    {
        this->removeChain( getUID(chainidxs.first()) );
        return;
    }
    
    //convert the chainidx to uid
    QList<quint32> uids;
    
    foreach (ChainIdx chainidx, chainidxs)
    {
        uids.append( getUID(chainidx) );
    }
    
    //now remove all of the chains
    foreach (quint32 uid, uids)
    {
        this->removeChain(uid);
    }
}

/** Remove all segments identified by 'segid'

    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
void StructureEditor::removeSegments(const SegID &segid)
{
    QList<SegIdx> segidxs = segid.map( EditMolInfo(*this) );
    
    if (segidxs.count() == 1)
    {
        this->removeSegment( getUID(segidxs.first()) );
        return;
    }
    
    //convert the segidx to uid
    QList<quint32> uids;
    
    foreach (SegIdx segidx, segidxs)
    {
        uids.append( getUID(segidx) );
    }
    
    //now remove all of the segments
    foreach (quint32 uid, uids)
    {
        this->removeSegment(uid);
    }
}

/** Move the atom identified by 'uid' into the CutGroup at index 'cgidx'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
void StructureEditor::reparentAtom(quint32 uid, CGIdx cgidx)
{
    EditAtomData &atom = d->atom(uid);
    
    quint32 cg_uid = this->getUID(cgidx);
    
    if (atom.cg_parent == cg_uid)
        return;
        
    //remove the atom from the old CutGroup
    if (atom.cg_parent != 0)    
        d->cutGroup(atom.cg_parent).atoms.removeAll(uid);
        
    //add the atom to the new CutGroup
    if (cg_uid != 0)
        d->cutGroup(cg_uid).atoms.append(uid);
        
    atom.cg_parent = cg_uid;
}

/** Move the atom identified by 'uid' into the residue at index 'residx'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
void StructureEditor::reparentAtom(quint32 uid, ResIdx residx)
{
    EditAtomData &atom = d->atom(uid);
    
    quint32 res_uid = this->getUID(residx);
    
    if (atom.res_parent == res_uid)
        return;
        
    //remove the atom from the old residue
    if (atom.res_parent != 0)    
        d->residue(atom.res_parent).atoms.removeAll(uid);
        
    //add the atom to the new residue
    if (res_uid != 0)
        d->residue(res_uid).atoms.append(uid);
        
    atom.res_parent = res_uid;
}

/** Move the atom identified by 'uid' into the segment at index 'segidx'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
void StructureEditor::reparentAtom(quint32 uid, SegIdx segidx)
{
    EditAtomData &atom = d->atom(uid);
    
    quint32 seg_uid = this->getUID(segidx);
    
    if (atom.seg_parent == seg_uid)
        return;
        
    //remove the atom from the old segment
    if (atom.seg_parent != 0)    
        d->segment(atom.seg_parent).atoms.removeAll(uid);
        
    //add the atom to the new segment
    if (seg_uid != 0)
        d->segment(seg_uid).atoms.append(uid);
        
    atom.seg_parent = seg_uid;
}

/** Move the residue identified by 'uid' into the chain at index 'chainidx'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void StructureEditor::reparentResidue(quint32 uid, ChainIdx chainidx)
{
    EditResData &residue = d->residue(uid);
    
    quint32 chain_uid = this->getUID(chainidx);
    
    if (residue.chain_parent == chain_uid)
        return;
        
    //remove the residue from the old chain
    if (residue.chain_parent != 0)    
        d->chain(residue.chain_parent).residues.removeAll(uid);
        
    //add the residue to the new chain
    if (chain_uid != 0)
        d->chain(chain_uid).residues.append(uid);
        
    residue.chain_parent = chain_uid;
}

/** Add a new atom to the molecule, returning an editor for that atom */
AtomStructureEditor StructureEditor::addAtom()
{
    quint32 uid = d->getNewUID();
    d->atoms.insert( uid, EditAtomData() );
    d->atoms_by_index.append(uid);
    
    return AtomStructureEditor( *this, AtomIdx(nAtomsInMolecule()-1) );
}

/** Add a new CutGroup to the molecule, returning an editor for that CutGroup */
CGStructureEditor StructureEditor::addCutGroup()
{
    quint32 uid = d->getNewUID();
    d->cutgroups.insert( uid, EditCGData() );
    d->cg_by_index.append(uid);
    
    return CGStructureEditor( *this, CGIdx(nCutGroupsInMolecule()-1) );
}

/** Add a new residue to the molecule, returning an editor for that residue */
ResStructureEditor StructureEditor::addResidue()
{
    quint32 uid = d->getNewUID();
    d->residues.insert( uid, EditResData() );
    d->res_by_index.append(uid);
    
    return ResStructureEditor( *this, ResIdx(nResiduesInMolecule()-1) );
}

/** Add a new chain to the molecule, returning an editor for that chain */
ChainStructureEditor StructureEditor::addChain()
{
    quint32 uid = d->getNewUID();
    d->chains.insert( uid, EditChainData() );
    d->chains_by_index.append(uid);
    
    return ChainStructureEditor( *this, ChainIdx(nChainsInMolecule()-1) );
}

/** Add a new segment to the molecule, returning an editor for that segment */
SegStructureEditor StructureEditor::addSegment()
{
    quint32 uid = d->getNewUID();
    d->segments.insert( uid, EditSegData() );
    d->seg_by_index.append(uid);
    
    return SegStructureEditor( *this, SegIdx(nSegmentsInMolecule()-1) );
}
