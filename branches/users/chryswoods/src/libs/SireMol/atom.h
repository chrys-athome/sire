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

#ifndef SIREMOL_ATOM_H
#define SIREMOL_ATOM_H

#include <QVariant>

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Atom;
}

QDataStream& operator<<(QDataStream&, const SireMol::Atom&);
QDataStream& operator>>(QDataStream&, SireMol::Atom&);

namespace SireMol
{

/** This is a single-atom view into a molecule.

    @author Christopher Woods
*/
class SIREMOL_EXPORT Atom : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const Atom&);
friend QDataStream& ::operator>>(QDataStream&, Atom&);

public:
    typedef ID AtomID;
    typedef Index AtomIdx;
    typedef Name AtomName;
    typedef Number AtomNum;
    typedef Info AtomInfo;

    Atom();
    
    Atom(const MoleculeView &molview, const AtomID &atomid);
    Atom(const MoleculeData &moldata, const AtomID &atomid);

    Atom(const Atom &other);

    ~Atom();

    Atom& operator=(const Atom &other);

    bool operator==(const Atom &other) const;
    bool operator!=(const Atom &other) const;
    
    AtomSelection selectedAtoms() const;
    
    void update(const MoleculeData &other) const;

    AtomName name() const;
    AtomNum number() const;
    AtomIdx index() const;
    const CGAtomIdx& cgAtomIdx() const;

    AtomInfo info() const;
    
    template<class T>
    T property(const PropertyName &key) const;
    
    template<class T>
    T metadata(const PropertyName &metakey) const;
    
    template<class T>
    T metadata(const PropertyName &key,
               const PropertyName &metakey) const;
    
    Mover<Atom> move() const;
    Evaluator evaluate() const;
    Editor<Atom> edit() const;
    Selector<Atom> selection() const;
    
    Residue residue() const;
    Chain chain() const;
    Segment segment() const;
    CutGroup cutGroup() const;
    Molecule molecule() const;

    void assertContains(AtomIdx atomidx) const;

protected:
    template<class T>
    void setProperty(const QString &key, const T &value);

    template<class T>
    void setMetadata(const QString &metakey, const T &value);
    
    template<class T>
    void setMetadata(const QString &key, const QString &metakey,
                     const T &value);

private:
    /** The index of this atom in the molecule */
    AtomIdx atomidx;
};

/** Return the property (of type T) at key 'key' that is 
    specifically assigned to this atom. This will only work
    if the property at this key is an Atomic property (i.e.
    has one value for every atom) and that it can be
    cast to type T
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T Atom::property(const PropertyName &key) const
{
    AtomicProperty<T> atom_props = d->property(key);
    return atom_props.at(this->cgAtomIdx());
}

/** Return the metadata for this atom at metakey 'metakey'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T Atom::metadata(const PropertyName &metakey) const
{
    AtomicProperty<T> atom_props = d->metadata(metakey);
    return atom_props.at(this->cgAtomIdx());
}

/** Return the metadata for this atom at metakey 'metakey'
    for the property at key 'key'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T Atom::metadata(const PropertyName &key, const PropertyName &metakey) const
{
    AtomicProperty<T> atom_props = d->metadata(key, metakey);
    return atom_props.at(this->cgAtomIdx());
}

/** Set the property (of type T) at key 'key' for this
    atom to be equal to 'value'. This works by creating
    an AtomicProperty<T> for this molecule, and assigning
    the value for this atom to 'value'. If there is already
    a property at key 'key', then it must be of type 
    AtomicProperty<T> for this to work
    
    \throw SireMol::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Atom::setProperty(const QString &key, const T &value)
{
    MoleculeView::setProperty<Atom,AtomicProperty<T>,T>(this->cgAtomIdx(), *d,
                                                        key, value);
}

/** Set the metadata at metakey 'metakey' for this atom */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Atom::setMetadata(const QString &metakey, const T &value)
{
    MoleculeView::setMetadata<Atom,AtomicProperty<T>,T>(this->cgAtomIdx(), *d,
                                                        metakey, value);
}

/** Set the metadata at metakey 'metakey' for the property at 
    key 'key' for this atom */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Atom::setMetadata(const QString &key, const QString &metakey, const T &value)
{
    MoleculeView::setMetadata<Atom,AtomicProperty<T>,T>(this->cgAtomIdx(), *d,
                                                        key, metakey, value);
}

/** We have different behaviour for an atoms Vector property */
template<>
Vector Atom::property<Vector>(const PropertyName &key) const;

template<>
Vector Atom::metadata<Vector>(const PropertyName &metakey) const;

template<>
Vector Atom::metadata<Vector>(const PropertyName &key, 
                              const PropertyName &metakey) const;

template<>
void Atom::setProperty<Vector>(const QString &key, const Vector &coords);

template<>
void Atom::setMetadata<Vector>(const QString &metakey, const Vector &coords);

template<>
void Atom::setMetadata<Vector>(const QString &key, const QString &metakey, 
                               const Vector &coords); 

}

Q_DECLARE_METATYPE(SireMol::Atom);

SIRE_END_HEADER

#endif
