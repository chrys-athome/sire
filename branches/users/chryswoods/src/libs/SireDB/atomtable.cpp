
#include "SireMol/qhash_siremol.h"

#include <QDebug>

#include "atomtable.h"
#include "atomtabledata.h"

#include "SireMol/molecule.h"

#include <boost/assert.hpp>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;

static const RegisterMetaType<AtomTable> r_atomtable("SireDB::AtomTable", MAGIC_ONLY);

/** Serialise an AtomTable to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const AtomTable &table)
{
    writeHeader(ds, r_atomtable, 1) 
            << table.metadata << static_cast<const TableBase&>(table);
    
    return ds;
}

/** Deserialise an AtomTable from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, AtomTable &table)
{
    VersionID v = readHeader(ds, r_atomtable);
    
    if (v == 1)
    {
        ds >> table.metadata >> static_cast<TableBase&>(table);
    }
    else
        throw version_error( v, "1", r_atomtable, CODELOC );
    
    return ds;
}

/** Constructor */
AtomTable::AtomTable() : TableBase()
{}
    
/** Construct an AtomTable to hold the parameters for the passed molecule */
AtomTable::AtomTable(const Molecule &molecule) : TableBase()
{
    this->setMolecule(molecule);
}
    
/** Copy constructor - this is fast as the data of this class is 
    implicitly shared */
AtomTable::AtomTable(const AtomTable &other) 
          : TableBase(other), metadata(other.metadata)
{}

/** Destructor */
AtomTable::~AtomTable()
{}

/** Return the metainfo object that contains the metadata for 
    the molecule whose atom-parameters are contained in this table */
const MoleculeCGInfo& AtomTable::info() const
{
    return metadata.info();
}

/** Set the molecule for whom this table stores the parameter data */
void AtomTable::setMolecule(const Molecule &molecule)
{
    if ( not metadata.isEmpty() )
        throw SireError::program_bug(QObject::tr(
                "Cannot change the molecule for an already-active AtomTable!"), CODELOC );
                
    metadata = AtomTableData(molecule);
}

/** Return whether or not this is an empty table (contains no atoms) */
bool AtomTable::isEmpty() const
{
    return metadata.isEmpty();
}
    
/** Return the number of atoms in the table */
int AtomTable::nAtoms() const
{
    return info().nAtoms();
}

/** Return the number of atoms in each CutGroup */
QHash<CutGroupID,uint> AtomTable::nAtomsPerCutGroup() const
{
    QSet<CutGroupID> cgroupids = metadata.cutGroupIDs();
    
    if (not cgroupids.isEmpty())
    {
        QHash<CutGroupID,uint> natoms;
        natoms.reserve(cgroupids.count());
        
        for (QSet<CutGroupID>::const_iterator it = cgroupids.constBegin();
             it != cgroupids.constEnd();
             ++it)
        {
            natoms.insert( *it, metadata.nAtoms(*it) );
        }
        
        return natoms;
    }
    else
        return QHash<CutGroupID,uint>();
}

/** Return the number of atoms in the table */
int AtomTable::size() const
{
    return nAtoms();
}

/** Return the number of atoms in the table */
int AtomTable::count() const
{
    return nAtoms();
}

/** Return the number of atoms in the residue with number 'resnum' */
int AtomTable::nAtoms(ResNum resnum) const
{
    return info().nAtoms(resnum);
}

/** Return the number of atoms in the CutGroup with number 'cgid' */
int AtomTable::nAtoms(CutGroupID cgid) const
{
    return metadata.nAtoms(cgid);
}

/** Return the number of atoms in this table in the residues with 
    residue numbers in 'resnums' */
int AtomTable::nAtoms(const QSet<ResNum> &resnums) const
{
    int nats = 0;
    for (QSet<ResNum>::const_iterator it = resnums.begin();
         it != resnums.end();
         ++it)
    {
        nats += nAtoms(*it);
    }
    
    return nats;
}

/** Return the number of atoms in this table in the CutGroups 
    with CutGroupIDs in 'cgids' */
int AtomTable::nAtoms(const QSet<CutGroupID> &cgids) const
{
    int nats = 0;
    
    for (QSet<CutGroupID>::const_iterator it = cgids.begin();
         it != cgids.end();
         ++it)
    {
        nats += nAtoms(*it);
    }
    
    return nats;
}

/** Return the number of CutGroups in the molecule */
int AtomTable::nCutGroups() const
{
    return metadata.nCutGroups();
}

/** Return the number of residues in the molecule */
int AtomTable::nResidues() const
{
    return info().nResidues();
}
    
/** Return whether or not this table is compatible with the molecule 
    'molecule'. This returns true if molecule.info() == this->info(),
    else it returns false.
*/
bool AtomTable::isCompatibleWith(const Molecule &molecule) const
{
    return info() == molecule.info();
}
    
/** Return a vector of all of the IDs of the CutGroups, in the order of 
    the CutGroups in the molecule. */
QVector<CutGroupID> AtomTable::orderedCutGroupIDs() const
{
    return metadata.orderedCutGroupIDs();
}

/** Return whether or not this table contains the atom 'atom' */
bool AtomTable::contains(const AtomIndex &atom) const
{
    return info().contains(atom);
}

/** Return whether or not this table contains the CutGroup cgid */
bool AtomTable::contains(CutGroupID cgid) const
{
    return metadata.contains(cgid);
}

/** Return whether or not this table contains the residue 'resnum' */
bool AtomTable::contains(ResNum resnum) const
{
    return info().contains(resnum);
}

/** Return whether or not this table contains the atom with index 'idx' */
bool AtomTable::contains(const CGAtomID &idx) const
{
    return metadata.contains(idx);
}

/** Return whether or not this table contains the atom with index 'idx' */
bool AtomTable::contains(const ResNumAtomID &idx) const
{
    return info().contains(idx);
}

/** Return the Atom at index 'cgid'. Throws an exception 
    if there is no such atom.
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomIndex AtomTable::atom(const CGAtomID &cgid) const
{
    return metadata.atom(cgid);
}

/** Return the Atom at index 'rsid'. Throws an exception
    if there is no such atom. 
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
AtomIndex AtomTable::atom(const ResNumAtomID &rsid) const
{
    return info().atom(rsid.resNum(),rsid.atomID());
}

/** Return the set of all atoms for the set of indexes in 'cgids'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QSet<AtomIndex> AtomTable::atoms(const QSet<CGAtomID> &cgids) const
{
    if ( not cgids.isEmpty() )
    {
        QSet<AtomIndex> atms;
        atms.reserve( cgids.count() );
        
        for (QSet<CGAtomID>::const_iterator it = cgids.begin();
             it != cgids.end();
             ++it)
        {
            atms.insert( metadata.atom(*it) );
        }
        
        return atms;
    }
    else
        return QSet<AtomIndex>();
    
}

/** Return the set of all atoms for the set of indexes in 'rsids'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QSet<AtomIndex> AtomTable::atoms(const QSet<ResNumAtomID> &rsids) const
{
    if ( not rsids.isEmpty() )
    {
        QSet<AtomIndex> atms;
        atms.reserve( rsids.count() );
        
        for (QSet<ResNumAtomID>::const_iterator it = rsids.begin();
             it != rsids.end();
             ++it)
        {
            atms.insert( info().atom(*it) );
        }
        
        return atms;
    }
    else
        return QSet<AtomIndex>();
    
}

/** Return a vector of the AtomIndex objects of all of the atoms 
    in the table. This returns the atom indexes in the same order
    as any parameters would be returned. */
QVector<AtomIndex> AtomTable::atoms() const
{
    return metadata.atoms();
}

/** Return a vector of the AtomIndex objects of all of the atoms
    in the table in residue 'resnum'. This returns the atom indexes
    in the same order as any parameters for this residue would be
    returned. */ 
QVector<AtomIndex> AtomTable::atoms(ResNum resnum) const
{
    return metadata.atoms(resnum);
}

/** Return a vector of the AtomIndex objects of all of the atoms
    in the table in CutGroup 'cgid'. This returns teh atom indexes
    in the same order as any parameters for this CutGroup would be
    returned. */
QVector<AtomIndex> AtomTable::atoms(CutGroupID cgid) const
{
    return metadata.atoms(cgid);
}

/** Return the set of all CutGroupID numbers of all of the CutGroups
    in this table */
QSet<CutGroupID> AtomTable::cutGroupIDs() const
{
    return metadata.cutGroupIDs();
}

/** Return the set of all residue numbers of all of the residues
    in this table */
QSet<ResNum> AtomTable::residueNumbers() const
{
    return info().residueNumbers().toSet();
}

/** Return a GroupedVector of all of the AtomIndex objects of 
    the atoms in this table indexed by CGAtomID of the atoms
    in CutGroups with IDs in 'cgids' */
GroupedVector<CGAtomID,AtomIndex> 
AtomTable::atomsByCutGroup(const QSet<CutGroupID> &cgids) const
{
    if (not cgids.isEmpty())
    {
        GroupedVector<CGAtomID,AtomIndex> allatoms;
        allatoms.reserve( cgids.count() );
        
        for (QSet<CutGroupID>::const_iterator it = cgids.begin();
             it != cgids.end();
             ++it)
        {
            CutGroupID cgid = *it;
            
            if (metadata.contains(cgid))
                allatoms.insert( cgid, atoms(cgid) );
        }
        
        if (not allatoms.isEmpty())
            return allatoms;
    }
        
    return GroupedVector<CGAtomID,AtomIndex>();
}

/** Return a GroupedVector of all of the AtomIndex objects of
    the atoms in this table indexed by ResNumAtomID of the atoms
    in the residues with residue numbers in 'resnums' */
GroupedVector<ResNumAtomID,AtomIndex> 
AtomTable::atomsByResidue(const QSet<ResNum> &resnums) const
{
    if (not resnums.isEmpty())
    {
        GroupedVector<ResNumAtomID,AtomIndex> allatoms;
        allatoms.reserve( resnums.count() );
        
        for (QSet<ResNum>::const_iterator it = resnums.begin();
             it != resnums.end();
             ++it)
        {
            ResNum resnum = *it;
            
            if (info().contains(resnum))
                allatoms.insert( resnum, atoms(resnum) );
        }
        
        if (not allatoms.isEmpty())
            return allatoms;
    }
    
    return GroupedVector<ResNumAtomID,AtomIndex>();
    
}
    
/** Return a GroupedVector of all of the AtomIndex objects of the 
    atoms in this table indexed by CGAtomID */
GroupedVector<CGAtomID,AtomIndex> AtomTable::atomsByCutGroup() const
{
    return atomsByCutGroup( cutGroupIDs() );
}

/** Return a GroupedVector of all of the AtomIndex objects of the 
    atoms in this table indexed by ResNumAtomID */
GroupedVector<ResNumAtomID,AtomIndex> AtomTable::atomsByResidue() const
{
    return atomsByResidue( residueNumbers() );
}
