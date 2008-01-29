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
#include "atomproperty.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Atom;
}

QDataStream& operator<<(QDataStream&, const SireMol::Atom&);
QDataStream& operator>>(QDataStream&, SireMol::Atom&);

namespace SireMol
{

class AtomID;
class AtomIdx;
class AtomName;
class AtomNum;

class Evaluator;

class AtomEditor;

template<class T>
class Mover;

template<class T>
class Selector;

class Residue;
class Chain;
class CutGroup;
class Segment;
class Molecule;

/** This is a single-atom view into a molecule.

    @author Christopher Woods
*/
class SIREMOL_EXPORT Atom : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const Atom&);
friend QDataStream& ::operator>>(QDataStream&, Atom&);

public:
    typedef AtomID ID;
    typedef AtomIdx Index;
    typedef AtomName Name;
    typedef AtomNum Number;

    Atom();
    
    Atom(const MoleculeView &molview, const AtomID &atomid);
    Atom(const MoleculeData &moldata, const AtomID &atomid);

    Atom(const Atom &other);

    ~Atom();

    Atom& operator=(const Atom &other);

    bool operator==(const Atom &other) const;
    bool operator!=(const Atom &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Atom>() );
    }
    
    const char* what() const
    {
        return Atom::typeName();
    }
    
    Atom* clone() const
    {
        return new Atom(*this);
    }
    
    AtomSelection selection() const;
    
    void update(const MoleculeData &other) const;

    AtomName name() const;
    AtomNum number() const;
    AtomIdx index() const;
    const CGAtomIdx& cgAtomIdx() const;

    bool hasProperty(const PropertyName &key) const;
    bool hasMetadata(const PropertyName &metakey) const;
    bool hasMetadata(const PropertyName &key,
                     const PropertyName &metakey) const;
    
    QStringList propertyKeys() const;
    QStringList metadataKeys() const;
    QStringList metadataKeys(const PropertyName &key) const;
                
    template<class T>
    const T& property(const PropertyName &key) const;
    
    template<class T>
    const T& metadata(const PropertyName &metakey) const;
    
    template<class T>
    const T& metadata(const PropertyName &key,
                      const PropertyName &metakey) const;
    
    Mover<Atom> move() const;
    Evaluator evaluate() const;
    AtomEditor edit() const;
    Selector<Atom> selector() const;
    
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
const T& Atom::property(const PropertyName &key) const
{
    const Property &property = d->property(key);
    const AtomProperty<T> &atom_props = property->asA< AtomProperty<T> >();

    return atom_props.at(this->cgAtomIdx());
}

/** Return the metadata for this atom at metakey 'metakey'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& Atom::metadata(const PropertyName &metakey) const
{
    const Property &property = d->metadata(metakey);
    const AtomProperty<T> &atom_props = property->asA< AtomProperty<T> >();

    return atom_props.at(this->cgAtomIdx());
}

/** Return the metadata for this atom at metakey 'metakey'
    for the property at key 'key'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& Atom::metadata(const PropertyName &key, const PropertyName &metakey) const
{
    const Property &property = d->metadata(key, metakey);
    const AtomProperty<T> &atom_props = property->asA< AtomProperty<T> >();
                        
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
    MoleculeView::setProperty<Atom,AtomProperty<T>,T>(this->cgAtomIdx(), *d,
                                                      key, value);
}

/** Set the metadata at metakey 'metakey' for this atom */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Atom::setMetadata(const QString &metakey, const T &value)
{
    MoleculeView::setMetadata<Atom,AtomProperty<T>,T>(this->cgAtomIdx(), *d,
                                                      metakey, value);
}

/** Set the metadata at metakey 'metakey' for the property at 
    key 'key' for this atom */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Atom::setMetadata(const QString &key, const QString &metakey, const T &value)
{
    MoleculeView::setMetadata<Atom,AtomProperty<T>,T>(this->cgAtomIdx(), *d,
                                                      key, metakey, value);
}

namespace detail
{

void assertSameSize(Atom*, int nats, int nprops);

template<>
inline QList<AtomIdx> getAll<Atom>(const MoleculeInfoData &molinfo)
{
    return molinfo.getAtoms();
}

template<>
inline QList<AtomIdx> getAll<Atom>(const MoleculeData &moldata,
                            const AtomSelection &selected_atoms)
{
    selected_atoms.assertCompatibleWith(moldata);
    return selected_atoms.selectedAtoms().toList();
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
QList<V> get_property(Atom*, const MoleculeData &moldata,
                      const QList<Atom::Index> &idxs,
                      const PropertyName &key)
{
    QList<V> props;
        
    const Property &property = moldata.property(key);
        
    const AtomProperty<V> &atom_prop = property->asA< AtomProperty<V> >();

    const MoleculeInfoData &molinfo = moldata.info();

    foreach (Atom::Index idx, QList<Atom::Index>(idxs))
    {
        props.append( atom_prop.at( molinfo.cgAtomIdx(idx) ) );
    }
        
    return props;
}
    
template<class V>
SIRE_OUTOFLINE_TEMPLATE
QList<V> get_metadata(Atom*, const MoleculeData &moldata,
                      const QList<Atom::Index> &idxs,
                      const PropertyName &metakey)
{
    QList<V> mdata;
    
    const Property &property = moldata.metadata(metakey);
    
    const AtomProperty<V> &atom_prop = property->asA< AtomProperty<V> >();

    const MoleculeInfoData &molinfo = moldata.info();

    foreach (Atom::Index idx, QList<Atom::Index>(idxs))
    {
        mdata.append( atom_prop.at( molinfo.cgAtomIdx(idx) ) );
    }
    
    return mdata;
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
QList<V> get_metadata(Atom*, const MoleculeData &moldata,
                      const QList<Atom::Index> &idxs,
                      const PropertyName &key,
                      const PropertyName &metakey)
{
    QList<V> mdata;
    
    const Property &property = moldata.metadata(key, metakey);
    
    const AtomProperty<V> &atom_prop = property->asA< AtomProperty<V> >();
                                              
    const MoleculeInfoData &molinfo = moldata.info();
    
    foreach (Atom::Index idx, QList<Atom::Index>(idxs))
    {
        mdata.append( atom_prop.at( molinfo.cgAtomIdx(idx) ) );
    }
    
    return mdata;
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_property(Atom *ptr, MoleculeData &moldata,
                  const QList<Atom::Index> &idxs,
                  const QString &key,
                  const QList<V> &values)
{
    assertSameSize(ptr, idxs.count(), values.count());
    
    const MoleculeInfoData &molinfo = moldata.info();
    
    AtomProperty<V> atom_prop;
    
    if ( moldata.hasProperty(key) )
        atom_prop = moldata.property(key);
    else
        atom_prop = AtomProperty<V>(molinfo);
        
    for (int i=0; i<idxs.count(); ++i)
    {
        atom_prop.set( molinfo.cgAtomIdx(idxs[i]), values[i] );
    }
    
    moldata.setProperty(key, atom_prop);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_metadata(Atom *ptr, MoleculeData &moldata,
                  const QList<Atom::Index> &idxs,
                  const QString &metakey,
                  const QList<V> &values)
{
    assertSameSize(ptr, idxs.count(), values.count());
    
    const MoleculeInfoData &molinfo = moldata.info();
    
    AtomProperty<V> atom_prop;
    
    if (moldata.hasMetadata(metakey))
        atom_prop = moldata.metadata(metakey);
    else
        atom_prop = AtomProperty<V>(molinfo);
        
    for (int i=0; i<idxs.count(); ++i)
    {
        atom_prop.set(molinfo.cgAtomIdx(idxs[i]), values[i]); 
    }
    
    moldata.setMetadata(metakey, atom_prop);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_metadata(Atom *ptr, MoleculeData &moldata,
                  const QList<Atom::Index> &idxs,
                  const QString &key, const QString &metakey,
                  const QList<V> &values)
{
    assertSameSize(ptr, idxs.count(), values.count());
    
    const MoleculeInfoData &molinfo = moldata.info();
    
    AtomProperty<V> atom_prop;
    
    if (moldata.hasMetadata(key, metakey))
        atom_prop = moldata.metadata(key, metakey);
    else
        atom_prop = AtomProperty<V>(molinfo);
        
    for (int i=0; i<idxs.count(); ++i)
    {
        atom_prop.set(molinfo.cgAtomIdx(idxs[i]), values[i]);
    }
    
    moldata.setMetadata(key, metakey, atom_prop);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_property(Atom*, MoleculeData &moldata,
                  const QList<Atom::Index> &idxs,
                  const QString &key,
                  const V &value)
{
    const MoleculeInfoData &molinfo = moldata.info();
    
    AtomProperty<V> atom_prop;
    
    if (moldata.hasProperty(key))
        atom_prop = moldata.property(key);
    else
        atom_prop = AtomProperty<V>(molinfo);
        
    foreach (Atom::Index idx, QList<Atom::Index>(idxs))
    {
        atom_prop.set(molinfo.cgAtomIdx(idx), value);
    }
    
    moldata.setProperty(key, atom_prop);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_metadata(Atom*, MoleculeData &moldata,
                  const QList<Atom::Index> &idxs,
                  const QString &metakey,
                  const V &value)
{
    const MoleculeInfoData &molinfo = moldata.info();
    
    AtomProperty<V> atom_prop;
    
    if (moldata.hasMetadata(metakey))
        atom_prop = moldata.metadata(metakey);
    else
        atom_prop = AtomProperty<V>(molinfo);
        
    foreach (Atom::Index idx, QList<Atom::Index>(idxs))
    {
        atom_prop.set(molinfo.cgAtomIdx(idx), value);
    }
    
    moldata.setMetadata(metakey, atom_prop);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_metadata(Atom*, MoleculeData &moldata,
                  const QList<Atom::Index> &idxs,
                  const QString &key, const QString &metakey,
                  const V &value)
{
    const MoleculeInfoData &molinfo = moldata.info();
    
    AtomProperty<V> atom_prop;
    
    if (moldata.hasMetadata(key,metakey))
        atom_prop = moldata.metadata(key,metakey);
    else
        atom_prop = AtomProperty<V>(molinfo);
        
    foreach (Atom::Index idx, QList<Atom::Index>(idxs))
    {
        atom_prop.set(molinfo.cgAtomIdx(idx), value);
    }
    
    moldata.setMetadata(key, metakey, atom_prop);
}

bool has_property(const Atom*, const MoleculeData &moldata,
                  const PropertyName &key);
                  
bool has_metadata(const Atom*, const MoleculeData &moldata,
                  const PropertyName &metakey);
                  
bool has_metadata(const Atom*, const MoleculeData &moldata,
                  const PropertyName &key, const PropertyName &metakey);                 

} // end of namespace detail

} // end of namespace SireMol

Q_DECLARE_METATYPE(SireMol::Atom);
Q_DECLARE_METATYPE(SireMol::Mover<SireMol::Atom>);
Q_DECLARE_METATYPE(SireMol::Selector<SireMol::Atom>);

Q_DECLARE_METATYPE(SireMol::Mover< SireMol::Selector<SireMol::Atom> >);

SIRE_END_HEADER

#endif
