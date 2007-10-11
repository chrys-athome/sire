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

#ifndef SIREMOL_ATOMPROPERTY_HPP
#define SIREMOL_ATOMPROPERTY_HPP

#include "moleculeproperty.h"

namespace SireMol
{
template<class T>
class AtomProperty;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireMol::AtomProperty<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireMol::AtomProperty<T>&);

namespace SireMol
{

/** This is a property that can hold one value for each
    atom in the molecule. The properties are held in 
    arrays, with each array corresponding to a CutGroup,
    and the order of the properties stored according 
    to CutGroup
    
    mol.setProperty( "charge", AtomCharges( [....] ) )
    mol.setProperty( "lj", AtomLJs( [ [...], [...] ] ) )

    atom.setProperty( "charge", 0.0 * mod_electron )
    
    @author Christopher Woods
*/
template<class T>
class AtomProperty 
    : public SireBase::ConcreteProperty<AtomProperty<T>, MolViewProperty>
{

friend QDataStream& ::operator<<<>(QDataStream&, const AtomProperty<T>&);
friend QDataStream& ::operator>><>(QDataStream&, AtomProperty<T>&);

public:
    AtomProperty();

    AtomProperty(const MoleculeInfoData &molinfo);
    
    AtomProperty(const QVector<T> &values);
    AtomProperty(const QVector< QVector<T> > &values);
    
    AtomProperty(const AtomProperty<T> &other);
    
    ~AtomProperty();
    
    AtomProperty<T>& operator=(const AtomProperty<T> &other);
    
    static const char* typeName()
    {
        QMetaType::typeName( qMetaTypeId< AtomProperty<T> >() );
    }
    
    AtomProperty<T>* clone() const
    {
        return new AtomProperty<T>(*this);
    }
    
    bool operator==(const AtomProperty<T> &other) const;
    bool operator!=(const AtomProperty<T> &other) const;

    const QVector<T>& operator[](CGIdx cgidx) const;
    const QVector<T>& at(CGIdx cgidx) const;
    const QVector<T>& get(CGIdx cgidx) const;

    const T& operator[](const CGAtomIdx &cgatomidx) const;
    const T& at(const CGAtomIdx &cgatomidx) const;
    const T& get(const CGAtomIdx &cgatomidx) const;

    AtomProperty<T>& set(const CGAtomIdx &cgatomidx, const T &value);

    AtomProperty<T>& set(CGIdx cgidx, const QVector<T> &values);

    const QVector<T>* data() const;
    const QVector<T>* constData() const;

    const T* data(CGIdx cgidx) const;
    const T* constData(CGIdx cgidx) const;

    int size() const;
    int count() const;
    
    int nCutGroups() const;
    int nAtoms(CGIdx cgidx) const;

private:
    /** The actual atomic property values */
    QVector< QVector<T> > props;
};

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomProperty<T>::AtomProperty()
{}

/** Create an AtomProperty that holds one value for each 
    atom described in 'molinfo'. Each atom starts with
    a default-constructed value of the property */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomProperty<T>::AtomProperty(const MoleculeInfoData &molinfo)
{   
    int ncg = molinfo.nCutGroups();

    if (ncg > 0)
    {
        //create space for each CutGroup
        props = QVector< QVector<T> >(ncg);
        
        QVector<T> *props_array = props.data();
        
        for (CGIdx i(0); i<ncg; ++i)
        {
            //now create space for all of the atoms
            props_array[i] = QVector<T>(molinfo.nAtoms(i));
            props_array[i].squeeze();
        }
        
        props.squeeze();
    }
}

/** Construct the Atom property from the array of values
    (this would be for a molecule with just a single CutGroup) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomProperty<T>::AtomProperty(const QVector<T> &values)
{
    if (not values.isEmpty())
    {
        props = QVector< QVector<T> >(1);
        props[0] = values;
        props[0].squeeze();
        props.squeeze();
    }
}

/** Create the Atom property from the array of array of values
     - each array contains the properties for a single CutGroup */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomProperty<T>::AtomProperty(const QVector< QVector<T> > &values)
                : props(values)
{
    if (not props.isEmpty())
    {
        //squeeze everything to save space
        int ncg = props.count();
        QVector<T> *props_array = props.data();
        
        for (int i=0; i<ncg; ++i)
        {
            props_array[i].squeeze();
        }
        
        props.squeeze();
    }
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomProperty<T>::AtomProperty(const AtomProperty<T> &other)
                : MoleculeProperty(), props(other.props)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomProperty<T>::~AtomProperty()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomProperty<T>& AtomProperty<T>::operator=(const AtomProperty<T> &other)
{
    props = other.props;
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool AtomProperty<T>::operator==(const AtomProperty<T> &other) const
{
    return props == other.props;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool AtomProperty<T>::operator!=(const AtomProperty<T> &other) const
{
    return props != other.props;
}

/** Return the array of properties for the atoms in the CutGroup
    identified by index 'cgidx'
    
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const QVector<T>& AtomProperty<T>::operator[](CGIdx cgidx) const
{
    return props.constData()[ cgidx.map(props.count()) ];
}

/** Return the array of properties for the atoms in the CutGroup
    identified by index 'cgidx'
    
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const QVector<T>& AtomProperty<T>::at(CGIdx cgidx) const
{
    return this->operator[](cgidx);
}

/** Return the array of properties for the atoms in the CutGroup
    identified by index 'cgidx'
    
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const QVector<T>& AtomProperty<T>::get(CGIdx cgidx) const
{
    return this->operator[](cgidx);
}

/** Return the property for the atom at index 'cgatomidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& AtomProperty<T>::operator[](const CGAtomIdx &cgatomidx) const
{
    const QVector<T> &group_props = this->operator[](cgatomidx.cutGroup());

    return group_props.constData()[ cgatomidx.atom().map(group_props.count()) ];
}

/** Return the property for the atom at index 'cgatomidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& AtomProperty<T>::at(const CGAtomIdx &cgatomidx) const
{
    return this->operator[](cgatomidx);
}

/** Return the property for the atom at index 'cgatomidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& AtomProperty<T>::get(const CGAtomIdx &cgatomidx) const
{
    return this->operator[](cgatomidx);
}

/** Set the value of the property for the atom at index 'cgatomidx'

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomProperty<T>& AtomProperty<T>::set(const CGAtomIdx &cgatomidx, const T &value)
{
    QVector<T> &group_props = props.data()[cgatomidx.cutGroup().map(props.count())];
    
    group_props.data()[cgatomidx.atom().map(group_props.count())] = value;
    
    return *this;
}

/** Set the values for all atoms in the CutGroup at index 'cgidx'

    \throw SireError::incompatible_error
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
AtomProperty<T>& AtomProperty<T>::set(CGIdx cgidx, const QVector<T> &values)
{
    QVector<T> &group_props = props.data()[cgidx.map(props.count())];
    
    if (values.count() != group_props.count())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot set the atomic properties of a %1 for the CutGroup "
            "at index %2, as the number of atoms in this CutGroup is "
            "%3, but the number of supplied properties is %4.")
                .arg( AtomProperty<T>::typeName() )
                .arg(cgidx).arg(group_props.count()).arg(values.count()),
                    CODELOC );
                    
    group_props = values;
    group_props.squeeze();
    
    return *this;
}

/** Return a raw pointer to the array of arrays */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const QVector<T>* AtomProperty<T>::data() const
{
    return props.constData();
}

/** Return a raw pointer to the array of arrays */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const QVector<T>* AtomProperty<T>::constData() const
{
    return props.constData();
}

/** Return a raw pointer to the array of properties for 
    the atoms in the CutGroup at index 'cgidx'
    
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* AtomProperty<T>::data(CGIdx cgidx) const
{
    return this->at(cgidx).constData();
}

/** Return a raw pointer to the array of properties for 
    the atoms in the CutGroup at index 'cgidx'
    
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* AtomProperty<T>::constData(CGIdx cgidx) const
{
    return this->at(cgidx).constData();
}

/** Return the number of CutGroups in the molecule */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int AtomProperty<T>::size() const
{
    return props.count();
}

/** Return the number of CutGroups in the molecule */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int AtomProperty<T>::count() const
{
    return props.count();
}

/** Return the number of CutGroups in the molecule */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int AtomProperty<T>::nCutGroups() const
{
    return props.count();
}

/** Return the number of atoms in the CutGroup at index 'cgidx'

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int AtomProperty<T>::nAtoms(CGIdx cgidx) const
{
    return this->at(cgidx).count();
}

}

#endif
