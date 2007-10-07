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

#ifndef SIREMOL_SELECTOR_HPP
#define SIREMOL_SELECTOR_HPP

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This template class provides a way to manipulate the selection
    of parts of a a molecule, e.g. Selector<Atom> provides a 
    MoleculeView that has a user-configurable view of an 
    arbitrary set of atoms. This class is used by the code
    whenever multiple groups from within the same molecule
    are returned, e.g. when returning all residues that 
    have a particular name
    
    @author Christopher Woods
*/
template<class T>
class Selector : public MoleculeView
{
public:
    Selector();
    
    Selector(const T &view);
    Selector(const MoleculeData &moldata);
    Selector(const MoleculeData &moldata, const typename T::ID &viewid);

    Selector(const Selector<T> &other);
    
    ~Selector();
    
    Selector<T>& operator=(const Selector<T> &other);
    Selector<T>& operator=(const T &view);

    bool operator==(const Selector<T> &other) const;
    bool operator!=(const Selector<T> &other) const;
    
    Selector<T> operator+(const Selector<T> &other) const;
    Selector<T> operator-(const Selector<T> &other) const;
    
    Selector<T> operator+(const typename T::ID &id) const;
    Selector<T> operator-(const typename T::ID &id) const;
    
    Selector<T> operator+(const T &view) const;
    Selector<T> operator-(const T &view) const;
    
    T operator[](int i) const;
    T operator()(int i) const;
    Selector<T> operator()(int i, int j) const;
    
    T at(int i) const;
    Selector<T> at(int i, int j) const;

    Selector<T> add(const Selector<T> &other) const;
    Selector<T> add(const T &view) const;
    Selector<T> add(const typename T::ID &id) const;
    
    Selector<T> subtract(const Selector<T> &other) const;
    Selector<T> subtract(const T &view) const;
    Selector<T> subtract(const typename T::ID &id) const;

    Selector<T> intersection(const Selector<T> &other) const;
    Selector<T> intersection(const T &view) const;
    Selector<T> intersection(const typename T::ID &id) const;
    
    Selector<T> invert() const;
    
    bool intersects(const Selector<T> &other) const;
    bool intersects(const T &view) const;
    bool intersects(const typename T::ID &id) const;

    bool contains(const Selector<T> &other) const;
    bool contains(const T &view) const;
    bool contains(const typename T::ID &id) const;
    
    bool containsAll(const typename T::ID &id) const;
    bool containsSome(const typename T::ID &id) const;

    AtomSelection selectedAtoms() const;
    
    Mover< Selector<T> > move() const;
    Mover< Selector<T> > move(int i) const;
    Mover< Selector<T> > move(int i, int j) const;
    
    Evaluator evaluate() const;
    Evaluator evaluate(int i) const;
    Evaluator evaluate(int i, int j) const;
    
    Editor< Selector<T> > edit() const;
    Editor< Selector<T> > edit(int i) const;
    Editor< Selector<T> > edit(int i, int j) const;

private:
    /** The list of indicies of the selected parts
        of the molecule */
    QList<typename T::Index> idxs;
    
    /** The set of indicies of the selected parts - used
        for rapid searching */
    QSet<typename T::Index> idxs_set;
};

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T>::Selector() : MoleculeView()
{}

/** Construct an empty set of groups for the molecule whose
    data is in 'moldata' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T>::Selector(const MoleculeData &moldata) : MoleculeView(moldata)
{}

/** Construct the set that contains only the view 'view' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T>::Selector(const T &view) : MoleculeView(view)
{
    idxs.append(view.index());
    idxs_set.insert(view.index());
}

/** Construct the set of parts that match the ID 'id' from the 
    molecule whose data is in 'moldata'
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::missing_chain
    \throw SireMol::missing_segment
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T>::Selector(const MoleculeData &moldata, 
                      const typename T::ID &viewid)
            : MoleculeView(moldata)
{
    idxs = viewid.map(moldata.info());
    idxs_set = idxs.toSet();
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T>::Selector(const Selector<T> &other)
            : MoleculeView(other),
              idxs(other.idxs), idxs_set(other.idxs_set)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T>::~Selector()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T>& Selector<T>::operator=(const Selector<T> &other)
{
    MoleculeView::operator=(other);
    idxs = other.idxs;
    idxs_set = other.idxs_set;
    
    return *this;
}

/** Copy so that this contains just 'view' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T>& Selector<T>::operator=(const T &view)
{
    MoleculeView::operator=(view);
    
    idxs.clear()
    idxs.append(view.index());
    
    idxs_set.clear();
    idxs_set.insert(view.index());
    
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::operator==(const Selector<T> &other) const
{
    return idxs == other.idxs and MoleculeView::operator==(other);
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::operator!=(const Selector<T> &other) const
{
    return idxs != other.idxs or MoleculeView::operator!=(other);
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Selector<T>::_pvt_add(typename T::Index idx)
{
    if (not idxs_set.contains(idx))
    {
        idxs.append(idx);
        idxs_set.insert(idx);
    }
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Selector<T>::_pvt_sub(typename T::Index idx)
{
    if (idxs_set.contains(idx))
    {
        idxs.removeAll(idx);
        idxs_set.remove(idx);
    }
}

/** Return the sum of this selection with 'other' - items from
    'other' are appended to this set only if they are not already
    in this set 
    
    \throw SireError::incompatible_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::add(const Selector<T> &other) const
{
    MoleculeView::assertSameMolecule(other);
    
    Selector<T> ret(*this);
    
    foreach (typename T::Index idx, other.idxs)
    {
        ret._pvt_add(idx);
    }
    
    return ret;
}

/** Return the view 'view' to this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::add(const T &view) const
{
    MoleculeView::assertSameMolecule(view);

    Selector<T> ret(*this);
    ret._pvt_add(view.index());
    return ret;
}

/** Return the set that has all views that match the ID 'id'
    added 
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::missing_chain
    \throw SireMol::missing_segment
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::add(const typename T::ID &id) const
{
    Selector<T> ret(*this);
    QList<typename T::Index> idxs = id.map(d->info());
    
    foreach (typename T::Index idx, idxs)
    {
        ret._pvt_add(idx);
    }
    
    return ret;
}

/** Subtract all of the views in 'other' from this set 

    \throw SireError::incompatible_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::subtract(const Selector<T> &other) const
{
    MoleculeView::assertSameMolecule(other);
    
    Selector<T> ret(*this);
    
    foreach (typename T::Index idx, other.idxs)
    {
        ret._pvt_sub(idx);
    }
    
    return ret;
}

/** Remove the view 'view' from this set 

    \throw SireError::incompatible_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::subtract(const T &view) const
{
    MoleculeView::assertSameMolecule(view);
    
    Selector<T> ret(*this);
    ret._pvt_sub(view.index());
    
    return ret;
}

/** Subtract all of the views that match the ID 'id' from
    this set
      
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::missing_chain
    \throw SireMol::missing_segment
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::subtract(const typename T::ID &id) const
{
    QList<typename ID::Index> idxs = id.map(d->info());
    
    Selector<T> ret(*this);
    
    foreach (typename ID::Index idx, idxs)
    {
        ret._pvt_sub(idx);
    }
    
    return ret;
}

/** Syntactic sugar for add() */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::operator+(const Selector<T> &other) const
{
    return this->add(other);
}

/** Syntactic sugar for subtract() */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::operator-(const Selector<T> &other) const
{
    return this->subtract(other);
}

/** Syntactic sugar for add() */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::operator+(const typename T::ID &id) const
{
    return this->add(id);
}

/** Syntactic sugar for subtract() */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::operator-(const typename T::ID &id) const
{
    return this->subtract(id);
}

/** Syntactic sugar for add() */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::operator+(const T &view) const
{
    return this->add(view);
}

/** Syntactic sugar for subtract() */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::operator-(const T &view) const
{
    return this->subtract(view);
}

/** Return the ith view in this set (this supports negative indexing!)

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T Selector<T>::operator[](int i) const
{
    return T( this->data(), idxs.at( SireID::Index(i).map(idxs.count()) ) );
}

/** Return the ith view in this set (this supports negative indexing!)

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T Selector<T>::operator()(int i) const
{
    return this->operator[](i);
}

/** Return the range of views from index i to j in this set. This
    supports negative indexing, and also, if j is less then i, then
    the order of views in the returned set is reversed
    
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::operator()(int i, int j) const
{
    i = SireID::Index(i).map(idxs.count());
    j = SireID::Index(j).map(idxs.count());
    
    Selector<T> ret(*this);
    ret.idxs.clear();
    ret.idxs_set.clear();
        
    if (i <= j)
    {
        for ( ; i<=j; ++i)
        {
            ret._pvt_add( typename T::Index(i) );
        }
    }
    else
    {
        for ( ; i >= j; --i)
        {
            ret._pvt_add( typename T::Index(i) );
        }
    }
    
    return ret;
}

/** Return the ith view in this set

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T Selector<T>::at(int i) const
{
    return this->operator[](i);
}

/** Return the views in this set from i to j */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::at(int i, int j) const
{
    return this->operator()(i,j);
}

/** Return the intersection of this set with 'other' - the
    views in both sets are returned, in the order that they
    appear in this set 
    
    \throw SireError::incompatible_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::intersection(const Selector<T> &other) const
{
    MoleculeView::assertSameMolecule(other);
    
    Selector<T> ret(*this);
    
    if (idxs.count() <= other.idxs.count())
    {
        foreach (typename T::Index idx, idxs)
        {
            if (not other.idxs_set.contains(idx))
                ret._pvt_sub(idx);
        }
    }
    else
    {
        foreach (typename T::Index idx, other.idxs)
        {
            if (not idxs_set.contains(idx))
                ret._pvt_sub(idx);
        }
    }
    
    return ret;
}

/** Return the intersection of this set with 'view' 
    
    \throw SireError::incompatible_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::intersection(const T &view) const
{
    MoleculeView::assertSameMolecule(view);

    if (idxs_set.contains(view.index()))
    {
        Selector<T> ret(*this);
        ret.idxs.clear();
        ret.idxs_set.clear();
        ret._pvt_add(view.index());
        return ret;
    }
    else
        return Selector<T>(this->data());
}

/** Return the intersection of this set with the views identified
    by the ID 'id' 

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::missing_chain
    \throw SireMol::missing_segment
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::intersection(const typename T::ID &id) const
{
    QList<typename T::Index> other_idxs = id.map(d->info());
    
    Selector<T> ret(*this);
    
    if (idxs.count() <= other_idxs.count())
    {
        QSet<typename T::Index> other_idxs_set = other_idxs.toSet();
        
        foreach (typename T::Index idx, idxs)
        {
            if (not other_idxs_set.contains(idx))
                ret._pvt_sub(idx);
        }
    }
    else
    {
        foreach (typename T::Index idx, other_idxs)
        {
            if (not idxs_set.contains(idx))
                ret._pvt_sub(idx);
        }
    }
    
    return ret;
}

/** Return the set that has a completely inverted selection */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Selector<T> Selector<T>::invert() const
{
    Selector<T> ret(*this);
    
    QList<typename T::Index> all_idxs = typename T::Index::null().map(d->info());
    
    Selector<T> ret(*this);
    ret.idxs.clear();
    ret.idxs_set.clear();
    
    foreach (typename T::Index idx, all_idxs)
    {
        if (not idxs_set.contains(idx))
            ret._pvt_add(idx);
    }
    
    return ret;
}

/** Return whether this set contains all of the views
    in 'other' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::contains(const Selector<T> &other) const
{
    if (idxs_set.count() < other.idxs_set.count())
        return false;
    else
    {
        foreach (typename T::Index idx, other.idxs)
        {
            if (not idxs_set.contains(idx))
                return false;
        }
    
        return true;
    }
}

/** Return whether this set contains some of the view
    in other

    \throw SireError::incompatible_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::containsSome(const Selector<T> &other) const
{
    MoleculeView::assertSameMolecule(other);
    
    if (idxs_set.count() <= other.idxs_set.count())
    {
        foreach (typename T::Index idx, idxs)
        {
            if (other.idxs_set.contains(idx))
                return true;
        }
    }
    else
    {
        foreach (typename T::Index idx, other.idxs)
        {
            if (idxs_set.contains(idx))
                return true;
        }
    }
    
    return false;
}

/** Return whether this set contains all of the views
    in 'other' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::containsAll(const Selector<T> &other) const
{
    return this->contains(other);
}

/** Return whether or not this set contains the view 'view' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::contains(const T &view) const
{
    MoleculeView::assertSameMolecule(view);
    return idxs_set.contains(view.index());
}

/** Return whether or not this set contains all of the  
    view identified by the ID 'id' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::contains(const typename T::ID &id) const
{
    QList<typename T::Index> id_idxs = id.map(d->info());
    
    if (idxs.count() < id_idxs)
        return false;
    else
    {
        foreach (typename T::Index idx, id_idxs)
        {
            if (not idxs_set.contains(idx))
                return false;
        }
        
        return true;
    }
}

/** Return whether or not this set contains all of the  
    view identified by the ID 'id' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::containsAll(const typename T::ID &id) const
{
    return this->contains(id);
}

/** Return whether this set contains some of the views
    identified by the ID 'id' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::containsSome(const typename T::ID &id) const
{
    QList<typename T::Index> id_idxs = id.map(d->info());
    
    if (idxs.count() <= id_idxs)
    {
        QSet<typename T::Index> id_idxs_set = id_idxs.toSet();
        
        foreach (typename T::Index idx, idxs)
        {
            if (id_idxs_set.contains(idx))
                return true;
        }
    }
    else
    {
        foreach (typename T::Index idx, id_idxs)
        {
            if (idxs_set.contains(idx))
                return true;
        }
    }
    
    return false;
}

/** Return whether or not this set intersects with 'other' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::intersects(const Selector<T> &other) const
{
    return this->containsSome(other);
}

/** Return whether or not this set contain the view 'view' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::intersects(const T &view) const
{
    return this->contains(view);
}

/** Return whether or not this set contains some of the
    views identified by the ID 'id' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool Selector<T>::intersects(const typename T::ID &id) const
{
    return this->containsSome(id);
}

/** Return all of the atoms selected in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomSelection Selector<T>::selectedAtoms() const
{
    AtomSelection selected_atoms(*d);
    
    foreach (typename T::Index idx, idxs)
    {
        selected_atoms.select(idx);
    }
    
    return selected_atoms;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomSelection Selector<T>::_pvt_selectedAtoms(int i) const
{
    AtomSelection selected_atoms(*d);
    
    selected_atoms.select( idxs.at( SireID::Index(i).map(idxs.count()) ) );
    
    return selected_atoms;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomSelection Selector<T>::_pvt_selectedAtoms(int i, int j) const
{
    AtomSelection selected_atoms(*d);
    
    i = SireID::Index(i).map(idxs.count());
    j = SireID::Index(j).map(idxs.count());
    
    if (i > j)
        qSwap(i,j);
        
    for ( ; i<=j; ++i)
    {
        selected_atoms.select( idxs.at(i) );
    }
    
    return selected_atoms;
}

/** Return an object that can move a copy of all of the views
    in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover< Selector<T> > Selector<T>::move() const
{
    return Mover< Selector<T> >(*this);
}

/** Return an object that can move the ith view in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover< Selector<T> > Selector<T>::move(int i) const
{
    return Mover< Selector<T> >(*this, this->_pvt_selectedAtoms(i));
}

/** Return an object that can move the ith to jth views in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover< Selector<T> > Selector<T>::move(int i, int j) const
{
    return Mover< Selector<T> >(*this, this->_pvt_selectedAtoms(i,j));
}

/** Return an evaluator that can evaluate properties over all 
    of the views in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Evaluator Selector<T>::evaluate() const
{
    return Evaluator(*this);
}

/** Return an evaluator that can evaluate properties over the 
    ith view in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Evaluator Selector<T>::evaluate(int i) const
{
    return Evaluator(*this, this->_pvt_selectedAtoms(i));
}

/** Return an evaluator that can evaluate properties over  
    the ith to jth views in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Evaluator Selector<T>::evaluate(int i, int j) const
{
    return Evaluator(*this, this->_pvt_selectedAtoms(i,j));
}

/** Return an editor that can edit all of the views in 
    this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor< Selector<T> > Selector<T>::edit() const
{
    return Editor< Selector<T> >(*this);
}

/** Return an editor that can edit the ith view in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor< Selector<T> > Selector<T>::edit(int i) const
{
    return Editor< Selector<T> >(*this, this->_pvt_selectedAtoms(i));
}

/** Return an editor that can edit the ith to jth views in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor< Selector<T> > Selector<T>::edit(int i, int j) const
{
    return Editor< Selector<T> >(*this, this->_pvt_selectedAtoms(i,j));
}

}

SIRE_END_HEADER

#endif
