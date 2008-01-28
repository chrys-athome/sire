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

#ifndef SIREMOL_CUTGROUP_H
#define SIREMOL_CUTGROUP_H

#include <QVector>

#include "moleculeview.h"
#include "cgproperty.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{
class CutGroup;
}

QDataStream& operator<<(QDataStream&, const SireMol::CutGroup&);
QDataStream& operator>>(QDataStream&, SireMol::CutGroup&);

namespace SireMol
{

class CGID;
class CGIdx;
class CGName;

class Evaluator;

template<class T>
class Editor;

template<class T>
class Mover;

template<class T>
class Selector;

class Atom;
class Molecule;

/** A CutGroup is a logical grouping of Atoms into a single group that is
    considered for intermolecular non-bonded cutting, and for periodic boundaries.

    @author Christopher Woods
*/
class SIREMOL_EXPORT CutGroup : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const CutGroup&);
friend QDataStream& ::operator>>(QDataStream&, CutGroup&);

public:
    typedef CGID ID;
    typedef CGIdx Index;
    typedef CGName Name;

    CutGroup();

    CutGroup(const MoleculeData &moldata, const CGID &cgid);

    CutGroup(const CutGroup &other);

    ~CutGroup();

    CutGroup& operator=(const CutGroup &other);
    
    bool operator==(const CutGroup &other) const;
    bool operator!=(const CutGroup &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<CutGroup>() );
    }
    
    const char* what() const
    {
        return CutGroup::typeName();
    }
    
    CutGroup* clone() const
    {
        return new CutGroup(*this);
    }
    
    AtomSelection selection() const;
    
    void update(const MoleculeData &moldata);
    
    const CGName& name() const;
    CGIdx index() const;
    
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
    
    Mover<CutGroup> move() const;
    Evaluator evaluate() const;
    Editor<CutGroup> edit() const;
    Selector<CutGroup> selector() const;
    
    int nAtoms() const;

    const QList<AtomIdx>& atomIdxs() const;

    bool contains(const AtomID &atomid) const;
    bool contains(AtomIdx atomidx) const;
    bool intersects(const AtomID &atomid) const;

    Atom select(const AtomID &atomid) const;
    Selector<Atom> selectAll(const AtomID &atomid) const;
    Selector<Atom> selectAll() const;
    
    Atom atom(const AtomID &atomid) const;
    Selector<Atom> atoms(const AtomID &atomid) const;
    Selector<Atom> atoms() const;
    
    Molecule molecule() const;

protected:
    template<class T>
    void setProperty(const QString &key, const T &value);

    template<class T>
    void setMetadata(const QString &key, const QString &metakey,
                     const T &value);
                     
    template<class T>
    void setMetadata(const QString &metakey, const T &value);

private:
    /** The index of the CutGroup */
    CGIdx cgidx;
};

/** Return the property (of type T) at key 'key' that is 
    specifically assigned to this CutGroup. This will only work
    if the property at this key is a CutGroup property (i.e.
    has one value for every CutGroup) and that it can be
    cast to type T
    
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& CutGroup::property(const PropertyName &key) const
{
    const Property &property = d->property(key);
    const CGProperty<T> &cg_props = property->asA< CGProperty<T> >();
    return cg_props.at(this->index());
}

/** Return the metadata at metakey 'metakey' for this residue

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& CutGroup::metadata(const PropertyName &key) const
{
    const Property &property = d->metadata(key);
    const CGProperty<T> &cg_props = property->asA< CGProperty<T> >();
    return cg_props.at(this->index());
}

/** Return the metadata at metakey 'metakey' for the property
    at key 'key'
    
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& CutGroup::metadata(const PropertyName &key, 
                            const PropertyName &metakey) const
{
    const Property &property = d->metadata(key, metakey);
    const CGProperty<T> &cg_props = property->asA< CGProperty<T> >();
    return cg_props.at(this->index());
}

/** Set the property (of type T) at key 'key' for this
    CutGroup to be equal to 'value'. This works by creating
    a CGProperty<T> for this molecule, and assigning
    the value for this CutGroup to 'value'. If there is already
    a property at key 'key', then it must be of type 
    CGProperty<T> for this to work
    
    \throw SireMol::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void CutGroup::setProperty(const QString &key, const T &value)
{
    MoleculeView::setProperty<CutGroup,CGProperty<T>,T>(this->index(), *d,
                                                        key, value);
}

/** Set the metadata at metakey 'metakey' to the value 'value' 
    for this residue
    
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void CutGroup::setMetadata(const QString &metakey, const T &value)
{
    MoleculeView::setMetadata<CutGroup,CGProperty<T>,T>(this->index(), *d,
                                                        metakey, value);
}

/** Set the metadata at metakey 'metakey' for the property at key
    'key' to the value 'value'
    
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void CutGroup::setMetadata(const QString &key, const QString &metakey,
                           const T &value)
{
    MoleculeView::setMetadata<CutGroup,CGProperty<T>,T>(this->index(), *d, 
                                                        key, metakey, value);
}

namespace detail
{

void assertSameSize(CutGroup*, int nres, int nprops);
    
template<>
inline QList<CGIdx> getAll<CutGroup>(const MoleculeInfoData &molinfo)
{
    return molinfo.getCutGroups();
}
    
template<class V>
SIRE_OUTOFLINE_TEMPLATE
QList<V> get_property(CutGroup*, const MoleculeData &moldata,
                      const QList<CutGroup::Index> &idxs,
                      const PropertyName &key)
{
    return get_property<CGProperty<V>,CutGroup::Index,V>(moldata,idxs,key);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
QList<V> get_metadata(CutGroup*, const MoleculeData &moldata,
                      const QList<CutGroup::Index> &idxs,
                      const PropertyName &metakey)
{
    return get_metadata<CGProperty<V>,CutGroup::Index,V>(moldata,idxs,metakey);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
QList<V> get_metadata(CutGroup*, const MoleculeData &moldata,
                      const QList<CutGroup::Index> &idxs,
                      const PropertyName &key,
                      const PropertyName &metakey)
{
    return get_metadata<CGProperty<V>,CutGroup::Index,V>(moldata,idxs,key,metakey);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_property(CutGroup *ptr, MoleculeData &moldata,
                  const QList<CutGroup::Index> &idxs,
                  const QString &key,
                  const QList<V> &values)
{
    assertSameSize(ptr, idxs.count(), values.count());
    set_property<CGProperty<V>,CutGroup::Index,V>(moldata,idxs,key,values);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_metadata(CutGroup *ptr, MoleculeData &moldata,
                  const QList<CutGroup::Index> &idxs,
                  const QString &metakey,
                  const QList<V> &values)
{
    assertSameSize(ptr, idxs.count(), values.count());
    set_metadata<CGProperty<V>,CutGroup::Index,V>(moldata,idxs,metakey,values);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_metadata(CutGroup *ptr, MoleculeData &moldata,
                  const QList<CutGroup::Index> &idxs,
                  const QString &key, const QString &metakey,
                  const QList<V> &values)
{
    assertSameSize(ptr, idxs.count(), values.count());
    
    set_metadata<CGProperty<V>,CutGroup::Index,V>(moldata,idxs,key,metakey,values);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_property(CutGroup*, MoleculeData &moldata,
                  const QList<CutGroup::Index> &idxs,
                  const QString &key,
                  const V &value)
{
    set_property<CGProperty<V>,CutGroup::Index,V>(moldata,idxs,key,value);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_metadata(CutGroup*, MoleculeData &moldata,
                  const QList<CutGroup::Index> &idxs,
                  const QString &metakey,
                  const V &value)
{
    set_metadata<CGProperty<V>,CutGroup::Index,V>(moldata,idxs,metakey,value);
}

template<class V>
SIRE_OUTOFLINE_TEMPLATE
void set_metadata(CutGroup*, MoleculeData &moldata,
                  const QList<CutGroup::Index> &idxs,
                  const QString &key, const QString &metakey,
                  const V &value)
{
    set_metadata<CGProperty<V>,CutGroup::Index,V>(moldata,idxs,key,metakey,value);
}

bool has_property(const CutGroup*, const MoleculeData &moldata,
                  const PropertyName &key);
                  
bool has_metadata(const CutGroup*, const MoleculeData &moldata,
                  const PropertyName &metakey);
                  
bool has_metadata(const CutGroup*, const MoleculeData &moldata,
                  const PropertyName &key, const PropertyName &metakey);                 

} //end of namespace detail

}

Q_DECLARE_METATYPE(SireMol::CutGroup);
Q_DECLARE_METATYPE(SireMol::Editor<SireMol::CutGroup>);
Q_DECLARE_METATYPE(SireMol::Mover<SireMol::CutGroup>);
Q_DECLARE_METATYPE(SireMol::Selector<SireMol::CutGroup>);

Q_DECLARE_METATYPE(SireMol::Editor< SireMol::Selector<SireMol::CutGroup> >);
Q_DECLARE_METATYPE(SireMol::Mover< SireMol::Selector<SireMol::CutGroup> >);

SIRE_END_HEADER

#endif
