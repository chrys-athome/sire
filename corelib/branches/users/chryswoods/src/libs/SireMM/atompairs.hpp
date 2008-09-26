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

#ifndef SIREMM_ATOMPAIRS_HPP
#define SIREMM_ATOMPAIRS_HPP

#include "SireBase/sparsematrix.hpp"
#include "SireBase/property.h"

#include "SireMol/cgatomidx.h"
#include "SireMol/moleculeinfodata.h"
#include "SireMol/molviewproperty.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
template<class T>
class AtomPairs;

template<class T>
class CGAtomPairs;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireMM::AtomPairs<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireMM::AtomPairs<T>&);

template<class T>
QDataStream& operator<<(QDataStream&, const SireMM::CGAtomPairs<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireMM::CGAtomPairs<T>&);

namespace SireMM
{

using SireMol::AtomID;
using SireMol::CGIdx;
using SireMol::CGID;
using SireMol::CGAtomIdx;

using SireMol::MoleculeInfoData;

/** This class is used to store objects of type 'T' associated
    with atom pairs within a CutGroup, or between a pair of
    CutGroups.

    @author Christopher Woods
*/
template<class T>
class SIREMM_EXPORT CGAtomPairs
{

friend QDataStream& ::operator<<<>(QDataStream&, const CGAtomPairs<T>&);
friend QDataStream& ::operator>><>(QDataStream&, CGAtomPairs<T>&);

template<class U> friend class CGAtomPairs;

public:
    CGAtomPairs(const T &default_value = T());

    template<class U>
    CGAtomPairs(const CGAtomPairs<U> &other);

    CGAtomPairs(const CGAtomPairs<T> &other);

    ~CGAtomPairs();

    CGAtomPairs<T>& operator=(const CGAtomPairs<T> &other);

    bool operator==(const CGAtomPairs<T> &other) const;
    bool operator!=(const CGAtomPairs<T> &other) const;

    const T& operator()(quint32 i) const;
    const T& operator()(quint32 i, quint32 j) const;

    const T& get(quint32 i) const;
    const T& get(quint32 i, quint32 j) const;

    void set(quint32 i, const T &value);
    void set(quint32 i, quint32 j, const T &value);

    bool isEmpty() const;

    const T& defaultValue() const;

private:
    /** The matrix of objects associated with each pair of atoms */
    SireBase::SparseMatrix<T> data;
};

/** This class is used to store objects of type 'T' associated
    with each pair of atoms in a molecule, and conveniently grouped
    according to CutGroup.

    @author Christopher Woods
*/
template<class T>
class SIREMM_EXPORT AtomPairs : public SireMol::MoleculeProperty
{

friend QDataStream& ::operator<<<>(QDataStream&, const AtomPairs<T>&);
friend QDataStream& ::operator>><>(QDataStream&, AtomPairs<T>&);

template<class U> friend class AtomPairs;

public:
    typedef CGAtomPairs<T> CGPairs;

    AtomPairs(const T &default_value = T());

    AtomPairs(const MoleculeInfoData &molinfo, const T &default_value = T());

    template<class U>
    AtomPairs(const AtomPairs<U> &other);

    AtomPairs(const AtomPairs<T> &other);

    ~AtomPairs();

    AtomPairs<T>& operator=(const AtomPairs<T> &other);

    bool operator==(const AtomPairs<T> &other) const;
    bool operator!=(const AtomPairs<T> &other) const;

    const T& operator()(const CGAtomIdx &atm0) const;
    const T& operator()(const CGAtomIdx &atm0, const CGAtomIdx &atm1) const;

    const CGAtomPairs<T>& operator()(CGIdx cgid0) const;
    const CGAtomPairs<T>& operator()(CGIdx cgid0, CGIdx cgid1) const;

    const T& operator()(const AtomID &atm0) const;
    const T& operator()(const AtomID &atm0, const AtomID &atm1) const;

    const CGAtomPairs<T>& operator()(const CGID &cgid0) const;
    const CGAtomPairs<T>& operator()(const CGID &cgid0, const CGID &cgid1) const;

    const T& get(const CGAtomIdx &atm0) const;
    const T& get(const CGAtomIdx &atm0, const CGAtomIdx &atm1) const;

    const CGAtomPairs<T>& get(CGIdx cgid0) const;
    const CGAtomPairs<T>& get(CGIdx cgid0, CGIdx cgid1) const;

    const T& get(const AtomID &atm0) const;
    const T& get(const AtomID &atm0, const AtomID &atm1) const;

    const CGAtomPairs<T>& get(const CGID &cgid0) const;
    const CGAtomPairs<T>& get(const CGID &cgid0, const CGID &cgid1) const;

    void set(const CGAtomIdx &atm0, const T &value);
    void set(const CGAtomIdx &atm0, const CGAtomIdx &atm1, const T &value);

    void set(const AtomID &atm0, const T &value);
    void set(const AtomID &atm0, const AtomID &atm1, const T &value);

    void setAll(const T &value);

    void setAll(CGIdx cgid0, const T &value);
    void setAll(CGIdx cgid0, CGIdx cgid1, const T &value);

    void setAll(const CGID &cgid0, const T &value);
    void setAll(const CGID &cgid0, const CGID &cgid1, const T &value);

    bool isEmpty() const;

    const MoleculeInfoData& info() const;

    int nGroups() const;
    int nAtoms() const;

    bool isCompatibleWith(const MoleculeInfoData &molinfo) const;

private:
    /** Info about the molecule that contains these atom pairs */
    SireBase::SharedDataPointer<MoleculeInfoData> molinfo;

    /** The matrix of all CutGroup-CutGroup pairs */
    SireBase::SparseMatrix< CGAtomPairs<T> > cgpairs;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

////////
//////// Implementation of CGAtomPairs<T>
////////

/** Construct, using the passed default value for all atom pairs */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGAtomPairs<T>::CGAtomPairs(const T &default_value)
               : data(default_value)
{}

/** Construct by casting from a CGAtomPairs<U> */
template<class T>
template<class U>
SIRE_OUTOFLINE_TEMPLATE
CGAtomPairs<T>::CGAtomPairs(const CGAtomPairs<U> &other)
{
    data = SireBase::SparseMatrix<T>(other.data);
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGAtomPairs<T>::CGAtomPairs(const CGAtomPairs<T> &other)
               : data(other.data)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGAtomPairs<T>::~CGAtomPairs()
{}

/** Copy assignment operator */
template<class T>
SIRE_INLINE_TEMPLATE
CGAtomPairs<T>& CGAtomPairs<T>::operator=(const CGAtomPairs<T> &other)
{
    data = other.data;
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool CGAtomPairs<T>::operator==(const CGAtomPairs<T> &other) const
{
    return data == other.data;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool CGAtomPairs<T>::operator!=(const CGAtomPairs<T> &other) const
{
    return data != other.data;
}

/** Return the object associated with the atom pair between atoms
    with indicies 'atm0' and 'atm1' - this returns the default
    value if there is no specified value associated with this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& CGAtomPairs<T>::operator()(quint32 atm0, quint32 atm1) const
{
    return data(atm0,atm1);
}

/** Return the object associated with the atom 'atm0' - this
    returns the default value if none has been provided */
template<class T>
SIRE_INLINE_TEMPLATE
const T& CGAtomPairs<T>::operator()(quint32 atm0) const
{
    return data(atm0,atm0);
}

/** Return the object associated with the atom 'atm0' - this
    returns the default value if none has been provided */
template<class T>
SIRE_INLINE_TEMPLATE
const T& CGAtomPairs<T>::get(quint32 atm0) const
{
    return data(atm0,atm0);
}

/** Return the object associated with the atom pair between atoms
    with indicies 'atm0' and 'atm1' - this returns the default
    value if there is no specified value associated with this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& CGAtomPairs<T>::get(quint32 atm0, quint32 atm1) const
{
    return data(atm0,atm1);
}

/** Set the value associated with the atom 'atm0' equal to 'value' */
template<class T>
SIRE_INLINE_TEMPLATE
void CGAtomPairs<T>::set(quint32 atm0, const T &value)
{
    data.set(atm0,atm0,value);
}

/** Set the value associated with the atom pair atm0-atm1 to 'value' */
template<class T>
SIRE_INLINE_TEMPLATE
void CGAtomPairs<T>::set(quint32 atm0, quint32 atm1, const T &value)
{
    data.set(atm0,atm1,value);
}

/** Return whether all atom pairs have the default value - i.e. this
    matrix is empty */
template<class T>
SIRE_INLINE_TEMPLATE
bool CGAtomPairs<T>::isEmpty() const
{
    return data.isEmpty();
}

/** Return the default value for all pairs */
template<class T>
SIRE_INLINE_TEMPLATE
const T& CGAtomPairs<T>::defaultValue() const
{
    return data.defaultValue();
}

//////////
////////// Implementation of AtomPairs<T>
//////////

/** Construct an empty set of AtomPairs, using the provided default value of 'T' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomPairs<T>::AtomPairs(const T &default_value)
             : cgpairs( CGAtomPairs<T>(default_value) )
{}

/** Construct a set of AtomPairs for the passed molecule, using the provided
    default value of 'T' for all pairs */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomPairs<T>::AtomPairs(const MoleculeInfoData &info, const T &default_value)
             : molinfo(info), cgpairs( CGAtomPairs<T>(default_value) )
{}

/** Construct by casting from an AtomPairs<U> */
template<class T>
template<class U>
SIRE_OUTOFLINE_TEMPLATE
AtomPairs<T>::AtomPairs(const AtomPairs<U> &other)
{
    molinfo = other.molinfo;
    cgpairs = SireBase::SparseMatrix< CGAtomPairs<T> >(other.cgpairs);
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomPairs<T>::AtomPairs(const AtomPairs &other)
             : molinfo(other.molinfo), cgpairs(other.cgpairs)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomPairs<T>::~AtomPairs()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomPairs<T>& AtomPairs<T>::operator=(const AtomPairs &other)
{
    if (&other != this)
    {
        molinfo = other.molinfo;
        cgpairs = other.cgpairs;
    }

    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool AtomPairs<T>::operator==(const AtomPairs<T> &other) const
{
    return (this == &other) or
           ( (molinfo == other.molinfo or *molinfo == *(other.molinfo)) and
              cgpairs == other.cgpairs );
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool AtomPairs<T>::operator!=(const AtomPairs<T> &other) const
{
    return not this->operator==(other);
}

/** Return the objects associated with all of the atom pairs between
    the two CutGroups at indicies (cgid0,cgid1) */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::operator()(CGIdx cgid0, CGIdx cgid1) const
{
    return cgpairs(cgid0.map(molinfo->nCutGroups()), 
                   cgid1.map(molinfo->nCutGroups()));
}

/** Return the objects associated with all of the atom pairs within the
    CutGroup at index cgid0 */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::operator()(CGIdx cgid0) const
{
    return cgpairs(cgid0.map(molinfo->nCutGroups()),
                   cgid0.map(molinfo->nCutGroups()));
}

/** Return the objects associated with all of the atom pairs between
    the two CutGroups with IDs (cgid0,cgid1) */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::operator()(const CGID &cgid0, 
                                               const CGID &cgid1) const
{
    return cgpairs(molinfo->cgIdx(cgid0), molinfo->cgIdx(cgid1));
}

/** Return the objects associated with all of the atom pairs within the
    CutGroup with ID cgid0 */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::operator()(const CGID &cgid0) const
{
    quint32 idx = molinfo->cgIdx(cgid0);

    return cgpairs(idx,idx);
}

/** Return the objects associated with all of the atom pairs within the
    CutGroup at index 'cgid0' */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::get(CGIdx cgid0) const
{
    return this->operator()(cgid0);
}

/** Return the objects associated with all of the atom pairs between
    the two CutGroups at indicies (cgid0,cgid1) */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::get(CGIdx cgid0, CGIdx cgid1) const
{
    return this->operator()(cgid0, cgid1);
}

/** Return the objects associated with all of the atom pairs within the
    CutGroup with ID 'cgid0' */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::get(const CGID &cgid0) const
{
    return this->operator()(cgid0);
}

/** Return the objects associated with all of the atom pairs between
    the two CutGroups with ID (cgid0,cgid1) */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::get(const CGID &cgid0, const CGID &cgid1) const
{
    return this->operator()(cgid0, cgid1);
}

/** Return the object associated with the atom pair (atm0,atm1). This returns the
    default object if nothing has been set for this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::operator()(const CGAtomIdx &atm0, const CGAtomIdx &atm1) const
{
    const quint32 cg0 = atm0.cutGroup().map(molinfo->nCutGroups());
    const quint32 cg1 = atm1.cutGroup().map(molinfo->nCutGroups());
    
    const quint32 atom0 = atm0.atom().map(molinfo->nAtoms(atm0.cutGroup()));
    const quint32 atom1 = atm1.atom().map(molinfo->nAtoms(atm1.cutGroup())); 

    return cgpairs(cg0, cg1)(atom0, atom1);
}

/** Return the object associated with the atom 'atm0'. This returns the
    default object if nothing has been set for this atom */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::operator()(const CGAtomIdx &atm0) const
{
    const quint32 cg0 = atm0.cutGroup().map(molinfo->nCutGroups());
    const quint32 atom0 = atm0.atom().map(molinfo->nAtoms(atm0.cutGroup()));

    return cgpairs(cg0, cg0)(atom0, atom0);
}

/** Return the object associated with the atom 'atm0'. This returns the
    default object if nothing has been set for this atom */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::get(const CGAtomIdx &atm0) const
{
    return this->operator()(atm0);
}

/** Return the object associated with the atom pair (atm0,atm1). This returns the
    default object if nothing has been set for this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::get(const CGAtomIdx &atm0, const CGAtomIdx &atm1) const
{
    return this->operator()(atm0,atm1);
}

/** Return the object associated with the atom 'atm0'. This returns the
    default object if nothing has been set for this atom */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::operator()(const AtomID &atm0) const
{
    return this->operator()( molinfo->cgAtomIdx(atm0) );
}

/** Return the object associated with the atom pair (atm0,atm1). This returns the
    default object if nothing has been set for this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::operator()(const AtomID &atm0,
                                  const AtomID &atm1) const
{
    return this->operator()( molinfo->cgAtomIdx(atm0),
                             molinfo->cgAtomIdx(atm1) );
}

/** Return the object associated with the atom 'atm0'. This returns the
    default object if nothing has been set for this atom */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::get(const AtomID &atm0) const
{
    return this->operator()(atm0);
}

/** Return the object associated with the atom pair (atm0,atm1). This returns the
    default object if nothing has been set for this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::get(const AtomID &atm0, const AtomID &atm1) const
{
    return this->operator()(atm0,atm1);
}

/** Set the value for the atom-pair 'atm0'-'atm1' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void AtomPairs<T>::set(const CGAtomIdx &atm0, const CGAtomIdx &atm1,
                       const T &value)
{
    quint32 cg0 = atm0.cutGroup().map(molinfo->nCutGroups());
    quint32 cg1 = atm1.cutGroup().map(molinfo->nCutGroups());
    
    quint32 atom0 = atm0.atom().map(molinfo->nAtoms(atm0.cutGroup()));
    quint32 atom1 = atm1.atom().map(molinfo->nAtoms(atm1.cutGroup()));

    CGAtomPairs<T> cgpair = cgpairs(cg0,cg1);

    cgpair.set(atom0, atom1, value);

    if (cg0 == cg1)
    {
        cgpair.set(atom1, atom0, value);
        cgpairs.set(cg0, cg0, cgpair);
    }
    else
    {
        cgpairs.set(cg0, cg1, cgpair);

        cgpair = cgpairs.get(cg1, cg0);
        cgpair.set(atom1, atom0, value);
        cgpairs.set(cg1, cg0, cgpair);
    }
}

/** Set the value for the atom 'atm0' */
template<class T>
SIRE_INLINE_TEMPLATE
void AtomPairs<T>::set(const CGAtomIdx &atm0, const T &value)
{
    this->set(atm0, atm0, value);
}

/** Set the value for the atom 'atm0' */
template<class T>
SIRE_INLINE_TEMPLATE
void AtomPairs<T>::set(const AtomID &atm0, const T &value)
{
    this->set( molinfo->cgAtomIdx(atm0), value );
}

/** Set the value for the atom-pair 'atm0'-'atm1' */
template<class T>
SIRE_INLINE_TEMPLATE
void AtomPairs<T>::set(const AtomID &atm0, const AtomID &atm1,
                       const T &value)
{
    this->set( molinfo->cgAtomIdx(atm0), 
               molinfo->cgAtomIdx(atm1), value );
}

/** Set every single atom pair in this molecule to have the value 'value' */
template<class T>
SIRE_INLINE_TEMPLATE
void AtomPairs<T>::setAll(const T &value)
{
    cgpairs = SireBase::SparseMatrix< CGAtomPairs<T> >( CGAtomPairs<T>(value) );
}

/** Set every single atom pair in the CutGroup at index cgid0 to have
    the value 'value' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void AtomPairs<T>::setAll(CGIdx cgid0, const T &value)
{
    quint32 cg0 = cgid0.map(molinfo->nCutGroups());

    cgpairs.set(cg0, cg0, CGAtomPairs<T>(value));
}

/** Set every single atom pair between the CutGroups at indicies (cgid0,cgid1)
    to have the value 'value' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void AtomPairs<T>::setAll(CGIdx cgid0, CGIdx cgid1, const T &value)
{
    quint32 cg0 = cgid0.map( molinfo->nCutGroups() );
    quint32 cg1 = cgid1.map( molinfo->nCutGroups() );

    cgpairs.set(cg0, cg1, CGAtomPairs<T>(value));

    if (cg0 != cg1)
        cgpairs.set(cg1, cg0, CGAtomPairs<T>(value));
}

/** Set every single atom pair in the CutGroup with ID cgid0 to have
    the value 'value' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void AtomPairs<T>::setAll(const CGID &cgid0, const T &value)
{
    this->setAll( molinfo->cgIdx(cgid0), value );
}

/** Set every single atom pair between the CutGroups with IDs (cgid0,cgid1)
    to have the value 'value' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void AtomPairs<T>::setAll(const CGID &cgid0, const CGID &cgid1, const T &value)
{
    this->setAll( molinfo->cgIdx(cgid0), molinfo->cgIdx(cgid1), value );
}

/** Return whether or not this is empty - all of the atom pairs have
    the default value. */
template<class T>
SIRE_INLINE_TEMPLATE
bool AtomPairs<T>::isEmpty() const
{
    return cgpairs.isEmpty();
}

/** Return the info object describing the molecule that contains
    all of the atoms */
template<class T>
SIRE_INLINE_TEMPLATE
const MoleculeInfoData& AtomPairs<T>::info() const
{
    return *molinfo;
}

/** Return the number of atoms in the molecule (hence the size of the 
    square matrix) */
template<class T>
SIRE_INLINE_TEMPLATE
int AtomPairs<T>::nAtoms() const
{
    return molinfo->nAtoms();
}

/** Return the number of CutGroups in the molecule (hence the size
    of the square group matrix) */
template<class T>
int AtomPairs<T>::nGroups() const
{
    return molinfo->nCutGroups();
}

/** Return whether this object is compatible with a molecule with
    metadata in 'info' */
template<class T>
SIRE_INLINE_TEMPLATE
bool AtomPairs<T>::isCompatibleWith(const MoleculeInfoData &info) const
{
    return *molinfo == info;
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

} // end of namespace SireMM

/** Serialise to a binary datastream */
template<class T>
QDataStream& operator<<(QDataStream &ds,
                        const SireMM::CGAtomPairs<T> &cgpairs)
{
    ds << cgpairs.data;
    return ds;
}

/** Extract from a binary datastream */
template<class T>
QDataStream& operator>>(QDataStream &ds,
                        SireMM::CGAtomPairs<T> &cgpairs)
{
    ds >> cgpairs.data;
    return ds;
}

/** Serialise to a binary datastream */
template<class T>
QDataStream& operator<<(QDataStream &ds,
                        const SireMM::AtomPairs<T> &atompairs)
{
    SireStream::SharedDataStream sds(ds);

    sds << atompairs.molinfo << atompairs.cgpairs;
    
    return ds;
}

/** Extract from a binary datastream */
template<class T>
QDataStream& operator>>(QDataStream &ds,
                        SireMM::AtomPairs<T> &atompairs)
{
    SireStream::SharedDataStream sds(ds);

    sds >> atompairs.molinfo >> atompairs.cgpairs;

    return ds;
}

SIRE_END_HEADER

#endif
