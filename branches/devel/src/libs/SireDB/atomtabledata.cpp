
#include "SireMol/qhash_siremol.h"

#include <QSharedData>

#include "SireMol/molecule.h"

#include "atomtabledata.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

QDataStream& operator<<(QDataStream&, const SireDB::AtomTableDataPvt&);
QDataStream& operator>>(QDataStream&, SireDB::AtomTableDataPvt&);

namespace SireDB
{

/** The private class used to store the implicitly shared metadata for the
    AtomTable classes
    
    @author Christopher Woods
*/
class AtomTableDataPvt : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const AtomTableDataPvt&);
friend QDataStream& ::operator>>(QDataStream&, AtomTableDataPvt&);

public:
    AtomTableDataPvt();
    AtomTableDataPvt(const Molecule &molecule);
    
    AtomTableDataPvt(const AtomTableDataPvt &other);
    
    ~AtomTableDataPvt();

    const MoleculeCGInfo& info() const;
    QSet<CutGroupID> cutGroupIDs() const;

    const QVector<CutGroupID> cutGroups() const;

    bool contains(CutGroupID id) const;
    int nAtoms(CutGroupID id) const;

    const AtomIndex& atom(const CGAtomID &id) const;

    AtomIndexVector atoms() const;
    const AtomIndexVector& atoms(CutGroupID id) const;
    AtomIndexVector atoms(ResNum resnum) const;

private:
    AtomIndexVector atoms(const ResidueCGInfo &resinfo) const;

    /** The meta-information about the molecule - this allows us to go
        from residue and atom information to a CGAtomID index */
    MoleculeCGInfo minfo;

    /** The atom indexes of all of the atoms in the molecule,
        broken down into CutGroups */
    QHash<CutGroupID,AtomIndexVector> atmids;

    /** The list of CutGroupIDs in the order that they appear in the molecule */
    QVector<CutGroupID> cutgroupids;
};

}

using namespace SireDB;

////////////////////
//////////////////// Implementation of AtomTableDataPvt
////////////////////

static const RegisterMetaType<AtomTableData> r_atomtabledata(
                                              "SireDB::AtomTableData", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream& operator<<(QDataStream &ds, const AtomTableDataPvt &data)
{
    writeHeader(ds, r_atomtabledata, 1) << data.minfo << data.atmids 
                                        << data.cutgroupids;
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream& operator>>(QDataStream &ds, AtomTableDataPvt &data)
{
    VersionID v = readHeader(ds, r_atomtabledata);
    
    if (v == 1)
    {
        ds >> data.minfo >> data.atmids >> data.cutgroupids;
    }
    else
        throw version_error(v, "1", r_atomtabledata, CODELOC);
    
    return ds;
}

/** Null constructor */
AtomTableDataPvt::AtomTableDataPvt() : QSharedData()
{}

/** Construct the table data from the passed molecule object */
AtomTableDataPvt::AtomTableDataPvt(const Molecule &molecule)
                 : QSharedData(), minfo(molecule.info())
{
    //run through the atoms in the molecule and extract information
    //that allows CutGroupID numbers to be converted back into 
    //AtomIndex objects.
    uint ncg = molecule.nCutGroups();
    atmids.reserve(ncg);
    cutgroupids.reserve(ncg);
    
    for (uint i=0; i<ncg; ++i)
    {
        //the CutGroupID is merely the index into the array of CutGroups
        cutgroupids.append(i);
    
        const CutGroup &cgroup = molecule.cutGroup(i);
        int nats = cgroup.nAtoms();
        
        AtomIndexVector idvec;
        idvec.reserve(nats);
        
        for (int j=0; j<nats; ++j)
        {
            idvec.append( cgroup.at(j) );
        }
        
        atmids.insert(i,idvec);
    }
}

/** Copy constructor */
AtomTableDataPvt::AtomTableDataPvt(const AtomTableDataPvt &other) 
                 : QSharedData(), minfo(other.minfo), atmids(other.atmids),
                   cutgroupids(other.cutgroupids)
{}

/** Destructor */
AtomTableDataPvt::~AtomTableDataPvt()
{}

/** Return the meta-information about the molecule */
inline const MoleculeCGInfo& AtomTableDataPvt::info() const
{
    return minfo;
}

/** Return the list of CutGroupIDs of the CutGroups in the molecule */
inline QSet<CutGroupID> AtomTableDataPvt::cutGroupIDs() const
{
    QSet<CutGroupID> idset;
    idset.reserve(cutgroupids.count());
    
    int ncg = cutgroupids.count();
    const CutGroupID *array = cutgroupids.constData();
    
    for (int i=0; i<ncg; ++i)
        idset.insert( array[i] );
        
    return idset;
}

const QVector<CutGroupID> AtomTableDataPvt::cutGroups() const
{
    return cutgroupids;
}

/** Return whether or not the molecule contains the CutGroup with ID = 'id' */
inline bool AtomTableDataPvt::contains(CutGroupID id) const
{
    return atmids.contains(id);
}

/** Return the number of atoms in the CutGroup with ID = id 

    \throw SireMol::missing_cutgroup
*/
inline int AtomTableDataPvt::nAtoms(CutGroupID id) const
{
    QHash<CutGroupID,AtomIndexVector>::const_iterator it = atmids.find(id);
    
    if ( it == atmids.end() )
        throw SireMol::missing_cutgroup( QObject::tr(
                    "Could not find the CutGroup with CutGroupID %1")
                          .arg(id.index()), CODELOC );
        
    return it.value().count();
}

/** Return the AtomIndex of the atom with CGAtomID 'id' 

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
inline const AtomIndex& AtomTableDataPvt::atom(const CGAtomID &id) const
{
    QHash<CutGroupID,AtomIndexVector>::const_iterator it = atmids.find(id.cutGroupID());
    
    if ( it == atmids.end() )
        throw SireMol::missing_cutgroup( QObject::tr(
                    "Could not find the CutGroup with CutGroupID %1")
                          .arg(id.cutGroupID().index()), CODELOC );
    
    const AtomIndexVector &atms = it.value();
    if ( id.atomID() >= AtomID(atms.count()) )
        throw SireError::invalid_index( QObject::tr(
                  "Invalid index (%1) in CutGroup %2 - nAtoms = %3")
                      .arg(id.atomID().toString(),id.cutGroupID().toString())
                      .arg(atms.count()), CODELOC );
            
    return atms.at(id.atomID().index());
}

/** Return a vector of all of the AtomIndexes of the atoms in the CutGroup
    with ID = id, in the same order as the atoms in the CutGroup
    
    \throw SireMol::missing_cutgroup
*/
inline const AtomIndexVector& AtomTableDataPvt::atoms(CutGroupID id) const
{
    QHash<CutGroupID,AtomIndexVector>::const_iterator it = atmids.find(id);
    
    if ( it == atmids.end() )
        throw SireMol::missing_cutgroup( QObject::tr(
                    "Could not find the CutGroup with CutGroupID %1")
                          .arg(id.index()), CODELOC );
    
    return it.value();
}

/** Return a vector of the AtomIndexes of all of the atoms in the molecule,
    in the same order as the atoms in the molecule, and the same order 
    as the parameters */
inline AtomIndexVector AtomTableDataPvt::atoms() const
{
    if ( cutgroupids.count() == 0 )
        return AtomIndexVector();
    else if ( cutgroupids.count() == 1 )
        return atoms( cutgroupids.at(0) );
    else
    {
        //reserve sufficient space for the atoms
        AtomIndexVector allatms;
        allatms.reserve( info().nAtoms() );
        
        int ncg = cutgroupids.count();
        const CutGroupID *id_array = cutgroupids.constData();
        
        QHash<CutGroupID,AtomIndexVector>::const_iterator it;
        
        for (int i=0; i<ncg; ++i)
        {
            it = atmids.find( id_array[i] );
            BOOST_ASSERT( it != atmids.end() );
            
            const AtomIndexVector &atms = it.value();
            int nats = atms.count();
            const AtomIndex *atms_array = atms.constData();
            
            for (int j=0; j<nats; ++j)
                allatms.append( atms_array[j] );
        }
        
        return allatms;
    }
}

/** Return a copy of the AtomIndex objects for the residue described by 'resinfo',
    returning them in the same order as the atoms in the residue */
AtomIndexVector AtomTableDataPvt::atoms(const ResidueCGInfo &resinfo) const
{
    int nats = resinfo.nAtoms();
    
    AtomIndexVector atms;
    atms.reserve(nats);
    
    for (int i=0; i<nats; ++i)
    {
        const CGAtomID &id = resinfo.index(i);
        
        atms.append( atmids.find(id.cutGroupID())
                          .value()
                            .constData()[id.atomID().index()] );
    }
    
    return atms;
}

/** Return a vector of all of the AtomIndexes of the atoms in the residue
    with number 'resnum', in the same order as the atoms in the residue.
    
    \throw SireMol::missing_residue
*/
inline AtomIndexVector AtomTableDataPvt::atoms(ResNum resnum) const
{
    //get the info object for this residue
    ResidueCGInfo resinfo = info().residue(resnum);
    int nats = resinfo.nAtoms();
    
    if (nats == 0)
        return AtomIndexVector();
    
    //if the residue has all of its atoms in a single CutGroup, then 
    //we can just return the atoms for that CutGroup
    CutGroupID cgid = resinfo.index(0).cutGroupID();
    
    if (nAtoms(cgid) != nats)
        return atoms(resinfo);
    else
    {
        for (int i=0; i<nats; ++i)
        {
            const CGAtomID &id = resinfo.index(i);
            if (id.cutGroupID() != cgid or id.atomID() != AtomID(i))
                return atoms(resinfo);
        }
        
        //the QVector only contains AtomTypes for this Residue, and 
        //they are all in the right order. Return an implicit copy
        //of the QVector (saves memory)
        return atoms(cgid);
    }
}

////////////////////
//////////////////// Implementation of AtomTableData
////////////////////

/** Serialise the AtomTableData to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const AtomTableData &data)
{
    ds << *(data.d);
    return ds;
}

/** Deserialise the AtomTableData from a binary datastream */
QDataStream& operator>>(QDataStream &ds, AtomTableData &data)
{
    ds >> *(data.d);
    
    return ds;
}

/** Shared-null object */
static QSharedDataPointer<AtomTableDataPvt> shared_null( new AtomTableDataPvt() );

/** Null constructor */
AtomTableData::AtomTableData() : d( shared_null )
{}

/** Construct an AtomTable to hold the atom-parameters for the passed molecule */
AtomTableData::AtomTableData(const Molecule &molecule) 
              : d( new AtomTableDataPvt(molecule) )
{}

/** Copy constructor */
AtomTableData::AtomTableData(const AtomTableData &other) : d(other.d)
{}

/** Destructor */
AtomTableData::~AtomTableData()
{}

/** Assignment operator */
AtomTableData& AtomTableData::operator=(const AtomTableData &other)
{
    d = other.d;
    return *this;
}

/** Return a reference to the MoleculeCGInfo that is used to get the index 
    from atom and residue names etc. */
const MoleculeCGInfo& AtomTableData::info() const
{
    return d->info();
}
    
/** Return the list of CutGroupIDs of the CutGroups in the molecule */
QSet<CutGroupID> AtomTableData::cutGroupIDs() const
{
    return d->cutGroupIDs();
}

/** Return a vector of all of the IDs of the CutGroups, in the order of 
    the CutGroups in the molecule. */
QVector<CutGroupID> AtomTableData::orderedCutGroupIDs() const
{
    return d->cutGroups();
}

/** Return whether or not the molecule contains the CutGroup with ID = 'id' */
bool AtomTableData::contains(CutGroupID id) const
{
    return d->contains(id);
}

/** Return whether or not the molecule contains the atom with index 'idx' */
bool AtomTableData::contains(const CGAtomID &idx) const
{
    return d->contains( idx.cutGroupID() ) and
           idx.atomID() < d->nAtoms(idx.cutGroupID());
}

/** Is this empty (has no CutGroups?) */
bool AtomTableData::isEmpty() const
{
    return (d == shared_null) or d->cutGroups().isEmpty();
}

/** Return the number of atoms in the CutGroup with ID = id 

    \throw SireMol::missing_cutgroup
*/
int AtomTableData::nAtoms(CutGroupID id) const
{
    return d->nAtoms(id);
}

/** Return the number of CutGroups */
int AtomTableData::nCutGroups() const
{
    return d->cutGroups().count();
}

/** Return the AtomIndex of the atom with CGAtomID 'id' 

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
const AtomIndex& AtomTableData::atom(const CGAtomID &id) const
{
    return d->atom(id);
}

/** Return a vector of the AtomIndexes of all of the atoms in the molecule,
    in the same order as the atoms in the molecule, and the same order 
    as the parameters */
AtomIndexVector AtomTableData::atoms() const
{
    return d->atoms();
}

/** Return a vector of all of the AtomIndexes of the atoms in the residue
    with number 'resnum', in the same order as the atoms in the residue.
    
    \throw SireMol::missing_residue
*/
AtomIndexVector AtomTableData::atoms(ResNum resnum) const
{
    return d->atoms(resnum);
}

/** Return a vector of all of the AtomIndexes of the atoms in the CutGroup
    with ID = id, in the same order as the atoms in the CutGroup
    
    \throw SireMol::missing_cutgroup
*/
AtomIndexVector AtomTableData::atoms(CutGroupID id) const
{
    return d->atoms(id);
}
