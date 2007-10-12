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

#ifndef SIREMOL_SEGPROPERTY_HPP
#define SIREMOL_SEGPROPERTY_HPP

#include "moleculeinfodata.h"
#include "molviewproperty.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
template<class T>
class SegProperty;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireMol::SegProperty<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireMol::SegProperty<T>&);

namespace SireMol
{

/** This is a property that can hold one value for each
    segment in the molecule.
    
    mol.setProperty( "charge", SegCharges( [....] ) )
    mol.setProperty( "lj", SegLJs( [....] ) )

    seg.setProperty( "charge", 0.0 * mod_e )
    
    @author Christopher Woods
*/
template<class T>
class SegProperty 
    : public SireBase::ConcreteProperty<SegProperty<T>, MolViewProperty>
{

friend QDataStream& ::operator<<<>(QDataStream&, const SegProperty<T>&);
friend QDataStream& ::operator>><>(QDataStream&, SegProperty<T>&);

public:
    SegProperty();

    SegProperty(const MoleculeInfoData &molinfo);
    
    SegProperty(const QVector<T> &values);
    
    SegProperty(const SegProperty<T> &other);
    
    ~SegProperty();
    
    SegProperty<T>& operator=(const SegProperty<T> &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< SegProperty<T> >() );
    }
    
    SegProperty<T>* clone() const
    {
        return new SegProperty<T>(*this);
    }
    
    bool operator==(const SegProperty<T> &other) const;
    bool operator!=(const SegProperty<T> &other) const;

    const T& operator[](const SegIdx &segidx) const;
    const T& at(const SegIdx &segidx) const;
    const T& get(const SegIdx &segidx) const;

    SegProperty<T>& set(SegIdx segidx, const T &value);

    const T* data() const;
    const T* constData() const;

    int size() const;
    int count() const;
    
    int nSegments() const;

private:
    /** The actual segment property values */
    QVector<T> props;
};

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>::SegProperty()
              : SireBase::ConcreteProperty<SegProperty<T>,MolViewProperty>()
{}

/** Construct space for the values of the property for all of the 
    segments in the molecule described by 'molinfo' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>::SegProperty(const MoleculeInfoData &molinfo)
              : SireBase::ConcreteProperty<SegProperty<T>,MolViewProperty>()
{
    if (molinfo.nSegments() > 0)
    {
        props = QVector<T>(molinfo.nSegments());
        props.squeeze();
    }
}

/** Create segment properties from the list of passed values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>::SegProperty(const QVector<T> &values)
              : SireBase::ConcreteProperty<SegProperty<T>,MolViewProperty>()
{
    props = values;
    props.squeeze();
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>::SegProperty(const SegProperty<T> &other)
              : SireBase::ConcreteProperty<SegProperty<T>,MolViewProperty>(other),
                props(other.props)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>::~SegProperty()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>& SegProperty<T>::operator=(const SegProperty<T> &other)
{
    MolViewProperty::operator=(other);
    props = other.props;
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool SegProperty<T>::operator==(const SegProperty<T> &other) const
{
    return props == other.props;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool SegProperty<T>::operator!=(const SegProperty<T> &other) const
{
    return props != other.props;
}

/** Return the property for the segment at index 'segidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& SegProperty<T>::operator[](const SegIdx &segidx) const
{
    return props.constData()[segidx.map(props.count())];
}

/** Return the property for the segment at index 'segidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& SegProperty<T>::at(const SegIdx &segidx) const
{
    return this->operator[](segidx);
}

/** Return the property for the segment at index 'segidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& SegProperty<T>::get(const SegIdx &segidx) const
{
    return this->operator[](segidx);
}

/** Set the value of the property for the segment at 
    index 'segidx' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>& SegProperty<T>::set(SegIdx segidx, const T &value)
{
    props.data()[segidx.map(props.count())] = value;
    return *this;
}

/** Return a raw pointer to the array of property values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* SegProperty<T>::data() const
{
    return props.constData();
}

/** Return a raw pointer to the array of property values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* SegProperty<T>::constData() const
{
    return props.constData();
}

/** Return the number of segments */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int SegProperty<T>::size() const
{
    return props.count();
}

/** Return the number of segments */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int SegProperty<T>::count() const
{
    return props.count();
}

/** Return the number of segments */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int SegProperty<T>::nSegments() const
{
    return props.count();
}

}

SIRE_END_HEADER

#endif

