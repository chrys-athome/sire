
#include "qhash_siremol.h"

#include "cutgroup.h"
#include "atomvector.h"
#include "atomset.h"
#include "aabox.h"

#include "SireMol/errors.h"

#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"

#include "SireBase/mutablesetiterator.hpp"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include <QSharedData>
#include <QDataStream>
#include <QMutex>

using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

namespace SireMol
{
class CutGroupPvt;
}

QDataStream& operator<<(QDataStream&, const SireMol::CutGroupPvt&);
QDataStream& operator>>(QDataStream&, SireMol::CutGroupPvt&);

static const RegisterMetaType<SireMol::CutGroup> r_cutgroup("SireMol::CutGroup");

namespace SireMol
{

/**
The CutGroupPvt class holds the private implementation of the CutGroup class. This 
allows all of the implementation details of the CutGroup to be hidden from the rest 
of the program 

@author Christopher Woods
*/
class CutGroupPvt : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const CutGroupPvt&);
friend QDataStream& ::operator>>(QDataStream&, CutGroupPvt&);

public:
    CutGroupPvt();
    CutGroupPvt(const AtomSet &atms);
    
    CutGroupPvt(const CutGroupPvt &other);
    
    ~CutGroupPvt();


   //////// Operators //////////////////////////////////////////////
    const Atom& operator[](AtomID i) const;
    
    bool operator==(const CutGroupPvt &other) const;
    bool operator!=(const CutGroupPvt &other) const;
   /////////////////////////////////////////////////////////////////

   
   //////// Managing the ID number /////////////////////////////////
    const MolCutGroupID& ID() const;
    void setID(const MolCutGroupID &id);
   /////////////////////////////////////////////////////////////////


   //////// Query functions ////////////////////////////////////////
    QString toString() const;

    const AtomSet& atoms() const;
    AtomSet atoms(AtomID strt, AtomID end) const;

    const Atom& at(AtomID i) const;
        
    const Atom& atom(AtomID i) const;
    const Atom& atom(const AtomIndex &atmidx) const;

    int nAtoms() const;
    int size() const;
    int count() const;

    const AABox& aaBox() const;

    bool contains(const AtomIndex &atm) const;
   /////////////////////////////////////////////////////////////////


   //////// Getting and setting the coordinates of the atoms ///////
    VectorVector coordinates() const;
    const Vector& coordinates(const AtomIndex &atm) const;
    const Vector& coordinates(AtomID i) const;
    
    void setCoordinates(const VectorVector &coords);
    void setCoordinates(const AtomIndex &atm, const Vector &coords);
    void setCoordinates(AtomID i, const Vector &coords);
   /////////////////////////////////////////////////////////////////


private:
    
    AtomSet atomsRange(AtomID strt, AtomID end, const AtomSet &atoms) const;

    /** AtomVector containing the atoms in this CutGroup */
    AtomSet atms;
    
    /** Axis-aligned bounding box of the atoms in the CutGroup */
    AABox aabox;
    
    /** The ID number of this CutGroup */
    MolCutGroupID id;
};

} //end of SireMol namespace

using namespace SireMol;

/** Serialise the CutGroupPvt class */
inline QDataStream& operator<<(QDataStream &ds, const CutGroupPvt &cgroup)
{
    writeHeader(ds, r_cutgroup, 1) << cgroup.id << cgroup.atms << cgroup.aabox;

    return ds;
}

/** Deserialise the CutGroupPvt class */
inline QDataStream& operator>>(QDataStream &ds, CutGroupPvt &cgroup)
{
    VersionID v = readHeader(ds, r_cutgroup);
    
    if (v == 1)
    {
        ds >> cgroup.id >> cgroup.atms >> cgroup.aabox;
    }
    else
        throw version_error(v, "1", r_cutgroup, CODELOC);
       
    return ds;
}

/** Construct a null, empty CutGroup */
CutGroupPvt::CutGroupPvt() : QSharedData(), id(0,0)
{}

/** Construct a new CutGroup to hold the atoms in 'atoms' */
CutGroupPvt::CutGroupPvt(const AtomSet &atoms) 
            : QSharedData(), atms(atoms), aabox(atoms), id(0,0)
{}

/** Copy constructor */
CutGroupPvt::CutGroupPvt(const CutGroupPvt &other) 
            : QSharedData(),
              atms(other.atms), aabox(other.aabox), id(other.id)
{}

/** Destructor */
CutGroupPvt::~CutGroupPvt()
{}

/** Comparison operator */
bool CutGroupPvt::operator==(const CutGroupPvt &other) const
{
    return this == &other or
        ( id == other.id and aabox == other.aabox and atms == other.atms );
}

/** Comparison operator */
bool CutGroupPvt::operator!=(const CutGroupPvt &other) const
{
    return this != &other and
        ( id != other.id or aabox != other.aabox or atms != other.atms );
}

/** Return the current ID number of the CutGroup */
inline const MolCutGroupID& CutGroupPvt::ID() const
{
    return id;
}

/** Give this CutGroup a new ID */
void CutGroupPvt::setID(const MolCutGroupID &newid)
{
    id = newid;
}

/** Return the number of atoms in this CutGroup */
inline int CutGroupPvt::nAtoms() const
{
    return atms.count();
}

/** Return a string representation of this CutGroup */
inline QString CutGroupPvt::toString() const
{
    return QObject::tr("CutGroup(%1-%2): nAtoms() == %3")
                    .arg(ID().moleculeID().toString(),ID().cutGroupID().toString())
                    .arg(nAtoms());
}
    
/** Function that does the work of extracting the range strt-end from the AtomVector 'atoms'.
    Both strt and end must refer to a valid index in 'atoms', and if strt > end, then the 
    returned array will have the atoms in the reverse order.
    
    \throw SireError::invalid_index
*/
AtomSet CutGroupPvt::atomsRange(AtomID strt, AtomID end, const AtomSet &atoms) const
{
    int nats = atoms.count();

    if (strt >= nats or end >= nats)
        throw SireError::invalid_index(QObject::tr(
            "Requested atoms from range %1 to %2, but there are only %3 atoms in the CutGroup")
                .arg(strt.toString(),end.toString()).arg(nats), CODELOC);
                
    if (strt <= end)
    {
        if (strt == 0 and end == nats-1)
            //we are copying the whole vector - this is quick!
            return atoms;
    
        //reserve sufficient space
        AtomSet atomrange;
        atomrange.reserve( end.index() - strt.index() + 1 );
        
        for (uint i=strt.index(); i<=end.index(); i++)
            atomrange.insert(atoms[i]);
        
        return atomrange;
    }
    else
    {
        //reserve sufficient space
        AtomSet atomrange;
        
        atomrange.reserve( strt.index() - end.index() + 1 );
        
        for (uint i=strt.index(); i>=end.index(); --i)
            atomrange.insert(atoms[i]);
        
        return atomrange;
    }
}

/** Return a const reference to the AtomSet contained in this CutGroup */
inline const AtomSet& CutGroupPvt::atoms() const
{
    return atms;
}

/** Return an AtomSet of the atoms from index strt to index end inclusive.
    If strt > end then the vector is reversed. If strt or end are < 0 then
    they count backwards from the last index. */
inline AtomSet CutGroupPvt::atoms( AtomID strt, AtomID end) const
{
    return atomsRange(strt, end, atoms());
}   

/** Return a const reference to the Atom with the AtomIndex 'atmidx' */
inline const Atom& CutGroupPvt::atom(const AtomIndex &atmidx) const
{
    return atms.at_key(atmidx);
}

/** Return the AABox that encloses the atoms */
inline const AABox& CutGroupPvt::aaBox() const
{
    return aabox;
}

/** Return whether or not this CutGroup contains the atom with index 'atm' */
inline bool CutGroupPvt::contains(const AtomIndex &atm) const
{
    return atms.has_key(atm);
}

/** Return the atom at index 'i' in the CutGroup */
inline const Atom& CutGroupPvt::at( AtomID i ) const
{
    return atms.at(i.index());
}

/** Synonym for 'at' */
inline const Atom& CutGroupPvt::atom( AtomID i ) const
{
    return atms.at(i.index());
}

/** Indexing operator - return the i'th atom */
inline const Atom& CutGroupPvt::operator[]( AtomID i ) const
{
    return this->at(i);
}

/** Synonym for natoms() */
inline int CutGroupPvt::size() const
{
    return this->nAtoms();
}

/** Synonym for natoms() */
inline int CutGroupPvt::count() const
{
    return this->nAtoms();
}
    
/** Return a vector of the coordinates of all of the atoms, in the same
    order as the atoms are arranged in this cutgroup (i.e. the coordinates
    for atom 'i' in this CutGroup will be at index 'i' in the returned array) */
inline VectorVector CutGroupPvt::coordinates() const
{
    VectorVector coords;
    
    int nats = atms.count();
    coords.reserve(nats);
    const Atom *atmarray = atms.constData();
    
    for (int i=0; i<nats; ++i)
        coords.append( atmarray[i] );
        
    return coords;
}

/** Return the coordinates of atom 'atm'. 

    \throw SireError::invalid_key
*/
inline const Vector& CutGroupPvt::coordinates(const AtomIndex &atm) const
{
    return atom(atm);
}

/** Return the coordinates of atom 'i'

    \throw SireError::invalid_index
*/
inline const Vector& CutGroupPvt::coordinates(AtomID i) const
{
    return atom(i);
}
    
/** Set the coordinates of all of the atoms to be the same as those in 'coords'. 
    Note that atom 'i' in the CutGroup will obtain it coordinates from the ith
    Vector in 'coords'. An exception will be thrown in coords.count() is not
    equal to nAtoms().
    
    \throw SireError::invalid_arg
*/
inline void CutGroupPvt::setCoordinates(const VectorVector &coords)
{
    int nats = atms.count();
    if (coords.count() != nats)
        throw SireError::invalid_arg(QObject::tr(
            "Cannot set the coordinates as coords.count() == %1, while nAtoms() == %2")
                .arg(coords.count()).arg(nats), CODELOC);
                
    const Vector *coordarray = coords.constData();
    
    MutableSetIterator<AtomIndex,Atom> it(atms);
    int i = 0;
    
    while( it.hasNext() )
    {
        it.next();
        
        it.value() = coordarray[i];
        i++;
    }
}

/** Set the coordinates of atom with index 'atm' to 'coords' 

    \throw SireMol::missing_atom
*/
inline void CutGroupPvt::setCoordinates(const AtomIndex &atm, const Vector &coords)
{
    MutableSetIterator<AtomIndex,Atom> it(atms);
    
    it.findKey(atm);
    if (not it.isValid())
        throw SireMol::missing_atom(QObject::tr(
            "No atom \"%1\" in CutGroup %2")
                .arg(atm.toString(),toString()), CODELOC);
                
    it.value() = coords;
}

/** Set the coordinates of atom 'i' in the CutGroup 

    \throw SireMol::missing_atom
*/
inline void CutGroupPvt::setCoordinates(AtomID i, const Vector &coords)
{
    if (i < 0 or i >= atms.count())
        throw SireMol::missing_atom(QObject::tr(
            "No atom with index %i in CutGroup %2")
                .arg(i.toString(),this->toString()), CODELOC);

    MutableSetIterator<AtomIndex,Atom> it(atms);
    
    it.jumpTo(i.index());
    it.value() = coords;
}
    
//////////////////
////////////////// Implementation of CutGroup
//////////////////

/** Global pointer to the null CutGroupPvt - this is used when creating null CutGroups */
static QSharedDataPointer<CutGroupPvt> shared_null( new CutGroupPvt() );

/** Serialise a CutGroup */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, CutGroup &cgroup)
{
    ds >> *(cgroup.d);
    return ds;
}

/** Deserialise a CutGroup */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const CutGroup &cgroup)
{
    ds << *(cgroup.d);
    return ds;
}

/** Construct a null CutGroup */
CutGroup::CutGroup() : d( shared_null )
{}

/** Construct a new CutGroup that contains the atoms in 'atoms' */
CutGroup::CutGroup(const AtomSet &atoms) : d( new CutGroupPvt(atoms) )
{}

/** Copy constructor. CutGroups are implicitly shared, so this is fast. */
CutGroup::CutGroup(const CutGroup &other) : d( other.d )
{}
    
/** Destructor */
CutGroup::~CutGroup()
{}

/** Assignment operator */
CutGroup& CutGroup::operator=(const CutGroup &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool CutGroup::operator==(const CutGroup &other) const
{
    return *d == *(other.d);
}

/** Comparison operator */
bool CutGroup::operator!=(const CutGroup &other) const
{
    return *d != *(other.d);
}

/** Return the ID number of this CutGroup */
const MolCutGroupID& CutGroup::ID() const
{
    return d->ID();
}

/** Give this CutGroup a new, unique ID number */
void CutGroup::setID(const MolCutGroupID &newid)
{
    d->setID(newid);
}

/** Return a string representation of this CutGroup */
QString CutGroup::toString() const
{
    return d->toString();
}

/** Return a const reference to the AtomSet contained in this CutGroup */
const AtomSet& CutGroup::atoms() const
{
    return d->atoms();
}

/** Return an AtomSet of the atoms from index strt to index end inclusive */
AtomSet CutGroup::atoms(AtomID strt, AtomID end) const
{
    return d->atoms(strt,end);
}

/** Return the atom at index 'i' in the CutGroup */
const Atom& CutGroup::at(AtomID i) const
{
    return d->at(i);
}

/** Indexing operator - return the i'th atom */
const Atom& CutGroup::operator[](AtomID i) const
{
    return d->operator[](i);
}

/** Synonym for 'at' */
const Atom& CutGroup::atom(AtomID i) const
{
    return d->atom(i);
}

/** Return a const reference to the Atom with the AtomIndex 'atmidx' */
const Atom& CutGroup::atom(const AtomIndex &atmidx) const
{
    return d->atom(atmidx);
}

/** Return the number of atoms in this CutGroup */
int CutGroup::nAtoms() const
{
    return d->nAtoms();
}

/** Synonym for natoms() */
int CutGroup::size() const
{
    return d->size();
}

/** Synonym for natoms() */
int CutGroup::count() const
{
    return d->count();
}

/** Return the AABox that encloses the atoms */
const AABox& CutGroup::aaBox() const
{
    return d->aaBox();
}

/** Return whether this CutGroup contains the atom with index 'idx' */
bool CutGroup::contains(const AtomIndex &atm) const
{
    return d->contains(atm);
}

/** Return a vector of the coordinates of all of the atoms, in the same
    order as the atoms are arranged in this cutgroup (i.e. the coordinates
    for atom 'i' in this CutGroup will be at index 'i' in the returned array) */
VectorVector CutGroup::coordinates() const
{
    return d->coordinates();
}

/** Return the coordinates of atom 'atm'. 

    \throw SireError::invalid_key
*/
const Vector& CutGroup::coordinates(const AtomIndex &atm) const
{
    return d->coordinates(atm);
}

/** Return the coordinates of atom 'i'

    \throw SireError::invalid_index
*/
const Vector& CutGroup::coordinates(AtomID i) const
{
    return d->coordinates(i);
}
    
/** Set the coordinates of all of the atoms to be the same as those in 'coords'. 
    Note that atom 'i' in the CutGroup will obtain it coordinates from the ith
    Vector in 'coords'. An exception will be thrown in coords.count() is not
    equal to nAtoms().
    
    \throw SireError::invalid_arg
*/
void CutGroup::setCoordinates(const VectorVector &coords)
{
    d->setCoordinates(coords);
}

/** Set the coordinates of atom with index 'atm' to 'coords' 

    \throw SireMol::missing_atom
*/
void CutGroup::setCoordinates(const AtomIndex &atm, const Vector &coords)
{
    d->setCoordinates(atm,coords);
}

/** Set the coordinates of atom 'i' in the CutGroup 

    \throw SireMol::missing_atom
*/
void CutGroup::setCoordinates(AtomID i, const Vector &coords)
{
    d->setCoordinates(i,coords);
}

/** Function used to index a CutGroup in a CutGroupSet */
template<>
SireMol::MolCutGroupID SIREMOL_EXPORT set_indexer(const SireMol::CutGroup &cgroup)
{
    return cgroup.ID();
}
