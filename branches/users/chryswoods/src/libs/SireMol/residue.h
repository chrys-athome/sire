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

#ifndef SIREMOL_RESIDUE_H
#define SIREMOL_RESIDUE_H

#include "moleculeview.h"
#include "resproperty.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Residue;
}

QDataStream& operator<<(QDataStream&, const SireMol::Residue&);
QDataStream& operator>>(QDataStream&, SireMol::Residue&);

namespace SireMol
{

/**
This class represents a Residue in a Molecule.

@author Christopher Woods
*/
class SIREMOL_EXPORT Residue : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const Residue&);
friend QDataStream& ::operator>>(QDataStream&, Residue&);

public:
    typedef ResID ID;
    typedef ResIdx Index;
    typedef ResName Name;
    typedef ResNum Number;

    Residue();
    
    Residue(const MoleculeData &moldata, const ResID &resid);

    Residue(const Residue &other);

    ~Residue();

    Residue& operator=(const Residue &other);
    
    bool operator==(const Residue &other) const;
    bool operator!=(const Residue &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Residue>() );
    }
    
    const char* what() const
    {
        return Residue::typeName();
    }
    
    Residue* clone() const
    {
        return new Residue(*this);
    }
    
    AtomSelection selectedAtoms() const;

    ResName name() const;
    ResNum number() const;
    ResIdx index() const;
    
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
    
    Mover<Residue> move() const;
    Evaluator evaluate() const;
    Editor<Residue> edit() const;
    Selector<Residue> selection() const;
    
    int nAtoms() const;
    
    const QList<AtomIdx>& atomIdxs() const;
    
    bool contains(AtomIdx atomidx) const;
    bool contains(const AtomID &atomid) const;
    
    bool containsAll(const AtomID &atomid) const;
    bool containsSome(const AtomID &atomid) const;
    
    Molecule molecule() const;
    Chain chain() const;
    
    Atom select(const AtomID &atomid) const;
    
    Selector<Atom> selectAll(const AtomID &atomid) const;
    Selector<Atom> selectAll() const;
    
    Atom atom(const AtomID &atomid) const;
    
    Selector<Atom> atoms(const AtomID &atomid) const;
    Selector<Atom> atoms() const;

protected:
    template<class T>
    void setProperty(const QString &key, const T &value);

    template<class T>
    void setMetadata(const QString &metakey, const T &value);
    
    template<class T>
    void setMetadata(const QString &key, const QString &metakey,
                     const T &value);

private:
    /** The index of the residue in the molecule */
    ResIdx residx;
    
    /** The atoms that are selected as part of this residue */
    AtomSelection selected_atoms;
};

/** Return the property (of type T) at key 'key' that is 
    specifically assigned to this residue. This will only work
    if the property at this key is a residue property (i.e.
    has one value for every residue) and that it can be
    cast to type T
    
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& Residue::property(const PropertyName &key) const
{
    const Property &property = d->property(key);
    const ResProperty<T> &res_props = property.asA< ResProperty<T> >();
    return res_props.at(this->index());
}

/** Return the metadata at metakey 'metakey' for this residue

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& Residue::metadata(const PropertyName &metakey) const
{
    const Property &property = d->metadata(metakey);
    const ResProperty<T> &res_props = property.asA< ResProperty<T> >();
    return res_props.at(this->index());
}

/** Return the metadata at metakey 'metakey' for the property
    at key 'key'
    
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& Residue::metadata(const PropertyName &key, 
                           const PropertyName &metakey) const
{
    const Property &property = d->metadata(key, metakey);
    const ResProperty<T> &res_props = property.asA< ResProperty<T> >();
    return res_props.at(this->index());
}

/** Set the property (of type T) at key 'key' for this
    residue to be equal to 'value'. This works by creating
    a ResProperty<T> for this molecule, and assigning
    the value for this residue to 'value'. If there is already
    a property at key 'key', then it must be of type 
    ResProperty<T> for this to work
    
    \throw SireMol::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Residue::setProperty(const QString &key, const T &value)
{
    MoleculeView::setProperty<Residue,ResProperty<T>,T>(this->index(), *d,
                                                        key, value);
}

/** Set the metadata at metakey 'metakey' to the value 'value' 
    for this residue
    
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Residue::setMetadata(const QString &metakey, const T &value)
{
    MoleculeView::setMetadata<Residue,ResProperty<T>,T>(this->index(), *d,
                                                        metakey, value);
}

/** Set the metadata at metakey 'metakey' for the property at key
    'key' to the value 'value'
    
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void Residue::setMetadata(const QString &key, const QString &metakey,
                          const T &value)
{
    MoleculeView::setMetadata<Residue,ResProperty<T>,T>(this->index(), *d, 
                                                        key, metakey, value);
}

namespace detail
{

/** Specialisation of SelectorHelper for Residue */
template<>
struct SelectorHelper<Residue>
{
    static void assertSameSize(int nres, int nprops);
    
    template<class V>
    static QList<V> property(const MoleculeData &moldata,
                             const QList<Residue::Index> &idxs,
                             const PropertyName &key)
    {
        return GeneralSelectorHelper<ResProperty<V>,Residue::Index>::
                            property(moldata,idxs,key);
    }
    
    template<class V>
    static QList<V> metadata(const MoleculeData &moldata,
                             const QList<Residue::Index> &idxs,
                             const PropertyName &metakey)
    {
        return GeneralSelectorHelper<ResProperty<V>,Residue::Index>::
                            metadata(moldata,idxs,metakey);
    }
    
    template<class V>
    static QList<V> metadata(const MoleculeData &moldata,
                             const QList<Residue::Index> &idxs,
                             const PropertyName &key,
                             const PropertyName &metakey)
    {
        return GeneralSelectorHelper<ResProperty<V>,Residue::Index>::
                            metadata(moldata,idxs,key,metakey);
    }

    template<class V>
    static void setProperty(MoleculeData &moldata,
                            const QList<Residue::Index> &idxs,
                            const QString &key,
                            const QList<V> &values)
    {
        SelectorHelper<Residue>::assertSameSize(idxs.count(), values.count());
        
        GeneralSelectorHelper<ResProperty<V>,Residue::Index>::
                            setProperty(moldata,idxs,key,values);
    }

    template<class V>
    static void setMetadata(MoleculeData &moldata,
                            const QList<Residue::Index> &idxs,
                            const QString &metakey,
                            const QList<V> &values)
    {
        SelectorHelper<Residue>::assertSameSize(idxs.count(), values.count());
        
        GeneralSelectorHelper<ResProperty<V>,Residue::Index>::
                            setMetadata(moldata,idxs,metakey,values);
    }

    template<class V>
    static void setMetadata(MoleculeData &moldata,
                            const QList<Residue::Index> &idxs,
                            const QString &key, const QString &metakey,
                            const QList<V> &values)
    {
        SelectorHelper<Residue>::assertSameSize(idxs.count(), values.count());
        
        GeneralSelectorHelper<ResProperty<V>,Residue::Index>::
                            setProperty(moldata,idxs,key,metakey,values);
    }
    
    template<class V>
    static void setProperty(MoleculeData &moldata,
                            const QList<Residue::Index> &idxs,
                            const QString &key,
                            const V &value)
    {
        GeneralSelectorHelper<ResProperty<V>,Residue::Index>::
                            setProperty(moldata,idxs,key,value);
    }
    
    template<class V>
    static void setMetadata(MoleculeData &moldata,
                            const QList<Residue::Index> &idxs,
                            const QString &metakey,
                            const V &value)
    {
        GeneralSelectorHelper<ResProperty<V>,Residue::Index>::
                            setMetadata(moldata,idxs,metakey,value);
    }
    
    template<class V>
    static void setMetadata(MoleculeData &moldata,
                            const QList<Residue::Index> &idxs,
                            const QString &key, const QString &metakey,
                            const V &value)
    {
        GeneralSelectorHelper<ResProperty<V>,Residue::Index>::
                            setProperty(moldata,idxs,key,metakey,value);
    }
};

}

}

Q_DECLARE_METATYPE(SireMol::Residue)

SIRE_END_HEADER

#endif
