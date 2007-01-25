/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMM_MOLINTERNALINFO_HPP
#define SIREMM_MOLINTERNALINFO_HPP

#include "detail/internalinfo.hpp"

#include "resinternalinfo.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{

/** This is the base class of all classes that hold the metainformation
    about internals within a molecule (e.g. MolBondInfo, MolAngleInfo etc.)
    
    @author Christopher Woods
*/
template<class T>
class SIREMM_EXPORT MolInternalInfo : public detail::InternalInfo<T>
{
public:
    MolInternalInfo();
    MolInternalInfo(const MoleculeInfo &molinfo);
    
    MolInternalInfo(const MolInternalInfo<T> &other);
    
    ~MolInternalInfo();

    MolInternalInfo& operator=(const MolInternalInfo &other);

    ResInternalInfo<T> residue(ResNum resnum) const;

    //generic editing interface - this should be specialised
    //by derived classes, e.g. MolBondInfo should provide
    //addBond(), removeBond(), addBonds() and removeBonds()
    GroupIndexID addInternal(const T &internal);
    
    void removeInternal(const T &internal);
    void removeInternal(const GroupIndexID &id);
    void removeInternal(GroupID id);
    
    void removeIntraInternals(ResNum resnum);
    void removeInterInternals(ResNum resnum);

    template<class C>
    void addInternals(const C &internals);
    template<class C>
    void removeInternals(const C &internals);

    void removeInternals();
    void removeInternals(GroupID id);
    void removeInternals(ResNum resnum);

protected:
    ResInternalInfo<T> extractResidue(ResNum resnum) const;
};

/** Empty constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MolInternalInfo<T>::MolInternalInfo() : detail::InternalInfo<T>()
{}

/** Construct a MolInternalInfo to hold the internals for the molecule 'mol' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MolInternalInfo<T>::MolInternalInfo(const MoleculeInfo &molinfo) 
                   : detail::InternalInfo<T>(molinfo)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MolInternalInfo<T>::MolInternalInfo(const MolInternalInfo<T> &other)
                   : detail::InternalInfo<T>(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MolInternalInfo<T>::~MolInternalInfo()
{}

/** Return the information about the residue 'resnum'. This will 
    return an empty ResInternalInfo if there are no internals that 
    involve this residue. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResInternalInfo<T> MolInternalInfo<T>::extractResidue(ResNum resnum) const
{
    return detail::InternalInfo<T>::extractResidue(resnum);
}

/** Construct from residue 'resnum' in the passed molecule info - defined here so
    that the 'residue' function of MolInternalInfo has already been defined */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResInternalInfo<T>::ResInternalInfo(const MolInternalInfo<T> &molinfo, ResNum resnum)
{
    *this = molinfo.residue(resnum);
}

/** Publically expose the generic 'addInternal' editing function */
template<class T>
SIRE_INLINE_TEMPLATE
GroupIndexID MolInternalInfo<T>::addInternal(const T &internal)
{
    return detail::InternalInfo<T>::addInternal(internal);
}

/** Publically expose the generic 'removeInternal' editing function */
template<class T>
SIRE_INLINE_TEMPLATE
void MolInternalInfo<T>::removeInternal(const T &internal)
{
    detail::InternalInfo<T>::removeInternal(internal);
}

/** Publically expose the generic 'removeInternal' editing function */
template<class T>
SIRE_INLINE_TEMPLATE
void MolInternalInfo<T>::removeInternal(const GroupIndexID &id)
{
    detail::InternalInfo<T>::removeInternal(id);
}

/** Publically expose the generic 'removeInternals' editing function */
template<class T>
SIRE_INLINE_TEMPLATE
void MolInternalInfo<T>::removeInternals(GroupID id)
{
    detail::InternalInfo<T>::removeInternals(id);
}

/** Publically expose the generic 'removeInternals' editing function */
template<class T>
SIRE_INLINE_TEMPLATE
void MolInternalInfo<T>::removeInternal(GroupID id)
{
    detail::InternalInfo<T>::removeInternals(id);
}

/** Publically expose the generic 'removeInternals' editing function */
template<class T>
SIRE_INLINE_TEMPLATE
void MolInternalInfo<T>::removeInternals()
{
    detail::InternalInfo<T>::removeInternals();
}

/** Generic function to add a whole load of internals to the molecule */
template<class T>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void MolInternalInfo<T>::addInternals(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        detail::InternalInfo<T>::addInternal(*it);
    }
}

/** Generic function to remove a whole load of internals from the molecule */
template<class T>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void MolInternalInfo<T>::removeInternals(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        detail::InternalInfo<T>::removeInternal(*it);
    }
}

}

SIRE_END_HEADER

#endif
