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

public:
    CGAtomPairs(const T &default_value = T());

    CGAtomPairs(const CGAtomPairs<T> &other);

    ~CGAtomPairs();

    CGAtomPairs& operator=(const CGAtomPairs<T> &other);

    const T& operator()(AtomID atm0) const;
    const T& operator()(AtomID atm0, AtomID atm1) const;

    const T& get(AtomID atm0) const;
    const T& get(AtomID atm0, AtomID atm1) const;

    void set(AtomID atm0, const T &value);
    void set(AtomID atm0, AtomID atm1, const T &value);

    bool isEmpty() const;

    const T& defaultValue() const;

private:
    /** The matrix of objects associated with each pair of atoms */
    SireBase::SparseMatrix<T> data;
};

/** Construct, using the passed default value for all atom pairs */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGAtomPairs<T>::CGAtomPairs(const T &default_value)
               : data(default_value)
{}

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
CGAtomPairs& CGAtomPairs<T>::operator=(const CGAtomPairs<T> &other)
{
    data = other.data;
    return *this;
}

/** Return the object associated with the atom pair between atoms
    with indicies 'atm0' and 'atm1' - this returns the default
    value if there is no specified value associated with this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& CGAtomPairs<T>::operator()(AtomID atm0, AtomID atm1) const
{
    return data(atm0,atm1);
}

/** Return the object associated with the atom 'atm0' - this
    returns the default value if none has been provided */
template<class T>
SIRE_INLINE_TEMPLATE
const T& CGAtomPairs<T>::operator()(AtomID atm0) const
{
    return data(atm0,atm0);
}

/** Return the object associated with the atom 'atm0' - this
    returns the default value if none has been provided */
template<class T>
SIRE_INLINE_TEMPLATE
const T& CGAtomPairs<T>::get(AtomID atm0) const
{
    return data(atm0,atm0);
}

/** Return the object associated with the atom pair between atoms
    with indicies 'atm0' and 'atm1' - this returns the default
    value if there is no specified value associated with this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& CGAtomPairs<T>::get(AtomID atm0, AtomID atm1) const
{
    return data(atm0,atm1);
}

/** Set the value associated with the atom 'atm0' equal to 'value' */
template<class T>
SIRE_INLINE_TEMPLATE
void CGAtomPairs<T>::set(AtomID atm0, const T &value)
{
    data.set(atm0,atm0,value);
}

/** Set the value associated with the atom pair atm0-atm1 to 'value' */
template<class T>
SIRE_INLINE_TEMPLATE
void CGAtomPairs<T>::set(AtomID atm0, AtomID atm1, const T &value)
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
    return def;
}

/** This class is used to store objects of type 'T' associated
    with each pair of atoms in a molecule, and convieniently grouped
    according to CutGroup.

    @author Christopher Woods
*/
template<class T>
class SIREMM_EXPORT AtomPairs : public SireBase::PropertyBase
{

friend QDataStream& ::operator<<<>(QDataStream&, const AtomPairs<T>&);
friend QDataStream& ::operator>><>(QDataStream&, AtomPairs<T>&);

public:
    AtomPairs(const T &default_value = T());

    AtomPairs(const MoleculeInfo &molinfo, const T &default_value = T());

    AtomPairs(const AtomPairs &other);

    ~AtomPairs();

    AtomPairs& operator=(const AtomPairs &other);

    const T& operator()(const CGAtomID &atm0) const;
    const T& operator()(const CGAtomID &atm0, const CGAtomID &atm1) const;

    const CGAtomPairs<T>& operator()(CutGroupID cgid0) const;
    const CGAtomPairs<T>& operator()(CutGroupID cgid0, CutGroupID cgid1) const;

    const T& operator()(const IDMolAtom &atm0) const;
    const T& operator()(const IDMolAtom &atm0, const IDMolAtom &atm1) const;

    const T& get(const CGAtomID &atm0) const;
    const T& get(const CGAtomID &atm0, const CGAtomID &atm1) const;

    const CGAtomPairs<T>& get(CutGroupID cgid0) const;
    const CGAtomPairs<T>& get(CutGroupID cgid0, CutGroupID cgid1) const;

    const T& get(const IDMolAtom &atm0) const;
    const T& get(const IDMolAtom &atm0, const IDMolAtom &atm1) const;

    void set(const CGAtomID &atm0, const T &value);
    void set(const CGAtomID &atm0, const CGAtomID &atm1, const T &value);

    void set(const IDMolAtom &atm0, const T &value);
    void set(const IDMolAtom &atm0, const IDMolAtom &atm1, const T &value);

    void setAll(const T &value);
    void setAll(CutGroupID cgid0, const T &value);
    void setAll(CutGroupID cgid0, CutGroupID cgid1, const T &value);

    bool isEmpty() const;

    const MoleculeInfo& info() const;

private:
    /** Info about the molecule that contains these atom pairs */
    MoleculeInfo molinfo;

    /** The matrix of all CutGroup-CutGroup pairs */
    SireBase::SparseMatrix< CGAtomPairs<T> > cgpairs;
};

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
AtomPairs<T>::AtomPairs(const MoleculeInfo &info, const T &default_value)
             : molinfo(info)
{}

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
AtomPairs& AtomPairs<T>::operator=(const AtomPairs &other)
{
    if (&other != this)
    {
        molinfo = other.molinfo;
        cgpairs = other.cgpairs;
    }

    return *this;
}

/** Return the objects associated with all of the atom pairs between
    the two CutGroups with IDs (cgid0,cgid1) */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::operator()(CutGroupID cgid0, CutGroupID cgid1) const
{
    return cgpairs(cgid0, cgid1);
}

/** Return the objects associated with all of the atom pairs within the
    CutGroup with ID == cgid */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::operator()(CutGroupID cgid0) const
{
    return cgpairs(cgid0,cgid0);
}

/** Return the objects associated with all of the atom pairs within the
    CutGroup with ID == cgid */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::get(CutGroupID cgid0) const
{
    return this->operator()(cgid0);
}

/** Return the objects associated with all of the atom pairs between
    the two CutGroups with IDs (cgid0,cgid1) */
template<class T>
SIRE_INLINE_TEMPLATE
const CGAtomPairs<T>& AtomPairs<T>::get(CutGroupID cgid0, CutGroupID cgid1) const
{
    return this->operator()(cgid0, cgid1);
}

/** Return the object associated with the atom pair (atm0,atm1). This returns the
    default object if nothing has been set for this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::operator()(const CGAtomID &atm0, const CGAtomID &atm1) const
{
    return this->get(atm0.cutGroupID(), atm1.cutGroupID())
                .get(atm0.atomID(), atm1.atomID());
}

/** Return the object associated with the atom 'atm0'. This returns the
    default object if nothing has been set for this atom */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::operator()(const CGAtomID &atm0) const
{
    return cgpairs(atm0.cutGroupID(), atm0.cutGroupID())
              .get(atm0.atomID(), atm0.atomID());
}

/** Return the object associated with the atom 'atm0'. This returns the
    default object if nothing has been set for this atom */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::get(const CGAtomID &atm0) const
{
    return this->operator()(atm0);
}

/** Return the object associated with the atom pair (atm0,atm1). This returns the
    default object if nothing has been set for this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::get(const CGAtomID &atm0, const CGAtomID &atm1) const
{
    return this->operator()(atm0,atm1);
}

/** Return the object associated with the atom 'atm0'. This returns the
    default object if nothing has been set for this atom */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::operator()(const IDMolAtom &atm0) const
{
    return this->get( atm0.index(molinfo) );
}

/** Return the object associated with the atom pair (atm0,atm1). This returns the
    default object if nothing has been set for this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::operator()(const IDMolAtom &atm0,
                                  const IDMolAtom &atm1) const
{
    return this->get( atm0.index(molinfo), atm1.index(molinfo) );
}

/** Return the object associated with the atom 'atm0'. This returns the
    default object if nothing has been set for this atom */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::get(const IDMolAtom &atm0) const
{
    return this->operator()(atm0);
}

/** Return the object associated with the atom pair (atm0,atm1). This returns the
    default object if nothing has been set for this pair */
template<class T>
SIRE_INLINE_TEMPLATE
const T& AtomPairs<T>::get(const IDMolAtom &atm0, const IDMolAtom &atm1) const
{
    return this->operator()(atm0,atm1);
}

/** Set the value for the atom-pair 'atm0'-'atm1' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void AtomPairs<T>::set(const CGAtomID &atm0, const CGAtomID &atm1,
                       const T &value)
{
    molinfo.assertAtomExists(atm0);
    molinfo.assertAtomExists(atm1);

    typename CGAtomPairs<T> cgpair = cgpairs.get(atm0.cutGroupID(),
                                                 atm1.cutGroupID());

    cgpair.set(atm0.atomID(), atm1.atomID(), value);

    if (atm0.cutGroupID() == atm1.cutGroupID())
    {
        cgpair.set(atm1.atomID(), atm0.atomID(), value);

        cgpairs.set(atm0.cutGroupID(), atm1.cutGroupID(), cgpair);
    }
    else
    {
        cgpairs.set(atm0.cutGroupID(), atm1.cutGroupID(), cgpair);

        cgpair = cgpairs.get(atm1.cutGroupID(), atm0.cutGroupID());
        cgpair.set(atm1.atomID(), atm0.atomID(), value);
        cgpairs.set(atm1.cutGroupID(), atm0.cutGroupID(), cgpair);
    }
}

/** Set the value for the atom 'atm0' */
template<class T>
SIRE_INLINE_TEMPLATE
void AtomPairs<T>::set(const CGAtomID &atm0, const T &value)
{
    this->set(atm0, atm0, value);
}

/** Set the value for the atom 'atm0' */
template<class T>
SIRE_INLINE_TEMPLATE
void AtomPairs<T>::set(const IDMolAtom &atm0, const T &value)
{
    this->set( atm0.index(molinfo), value );
}

/** Set the value for the atom-pair 'atm0'-'atm1' */
template<class T>
SIRE_INLINE_TEMPLATE
void AtomPairs<T>::set(const IDMolAtom &atm0, const IDMolAtom &atm1,
                       const T &value)
{
    this->set( atm0.index(molinfo), atm1.index(molinfo), value );
}

/** Set every single atom pair in this molecule to have the value 'value' */
template<class T>
SIRE_INLINE_TEMPLATE
void AtomPairs<T>::setAll(const T &value)
{
    cgpairs = SparseMatrix< CGAtomPairs<T> >( CGAtomPairs<T>(value) );
}

/** Set every single atom pair in the CutGroup with ID == cgid0 to have
    the value 'value' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void AtomPairs<T>::setAll(CutGroupID cgid0, const T &value)
{
    molinfo.assertCutGroupExists(cgid0);

    cgpairs.set(cgid0, CGAtomPairs<T>(value));
}

/** Set every single atom pair between the CutGroups with IDs (cgid0,cgid1)
    to have the value 'value' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void AtomPairs<T>::setAll(CutGroupID cgid0, CutGroupID cgid1, const T &value)
{
    molinfo.assertCutGroupExists(cgid0);
    molinfo.assertCutGroupExists(cgid1);

    cgpairs.set(cgid0, cgid1, CGAtomPairs<T>(value));

    if (cgid0 != cgid1)
        cgpairs.set(cgid1, cgid0, CGAtomPairs<T>(value));
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
const MoleculeInfo& AtomPairs<T>::info() const
{
    return molinfo;
}

}

SIRE_END_HEADER

#endif
