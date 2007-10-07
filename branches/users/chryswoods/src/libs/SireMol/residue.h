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
    typedef ID ResID;
    typedef Index ResIdx;
    typedef Name ResName;
    typedef Number ResNum;
    typedef Info ResInfo;

    Residue();
    
    Residue(const MoleculeData &moldata, const ResID &resid);

    Residue(const Residue &other);

    ~Residue();

    Residue& operator=(const Residue &other);
    
    bool operator==(const Residue &other) const;
    bool operator!=(const Residue &other) const;
    
    AtomSelection selectedAtoms() const;

    ResName name() const;
    ResNum number() const;
    ResIdx index() const;
    
    ResInfo info() const;
    
    template<class T>
    T property(const PropertyName &key) const;
    
    template<class T>
    T metadata(const PropertyName &metakey) const;
    
    template<class T>
    T metadata(const PropertyName &key,
               const PropertyName &metakey) const;
    
    Mover<Residue> move() const;
    Evaluator evaluate() const;
    Editor<Residue> edit() const;
    Selector<Residue> selection() const;
    
    Molecule molecule() const;
    Chain chain() const;
    
    Atom select(const AtomID &atomid) const;
    
    AtomsInMol selectAll(const AtomID &atomid) const;
    AtomsInMol selectAll() const;
    
    Atom atom(const AtomID &atomid) const;
    
    AtomsInMol atoms(const AtomID &atomid) const;
    AtomsInMol atoms() const;

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
T Residue::property(const PropertyName &key) const
{
    ResProperty<T> res_props = d->property(key);
    return res_props.at(this->index());
}

/** Return the metadata at metakey 'metakey' for this residue

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T Residue::metadata(const PropertyName &key) const
{
    ResProperty<T> res_props = d->metadata(key);
    return res_props.at(this->index());
}

/** Return the metadata at metakey 'metakey' for the property
    at key 'key'
    
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T Residue::metadata(const PropertyName &key, const PropertyName &metakey) const
{
    ResProperty<T> res_props = d->metadata(key, metakey);
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

}

Q_DECLARE_METATYPE(SireMol::Residue)

SIRE_END_HEADER

#endif
