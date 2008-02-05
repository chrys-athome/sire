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

#include <QVector>
#include <QVariant>

#include "moleculeinfodata.h"
#include "molviewproperty.h"

#include "SireError/errors.h"

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

/** Small class used to provide a common base for all SegProperty types */
class SegProp : public MolViewProperty
{
public:
    SegProp() : MolViewProperty()
    {}
    
    SegProp(const SegProp &other) : MolViewProperty(other)
    {}
    
    virtual ~SegProp()
    {}
    
    virtual bool canConvert(const QVariant &value) const=0;
    
    virtual void assignFrom(const QVector<QVariant> &values)=0;
    
    virtual QVector<QVariant> toVariant() const=0;
    
    virtual void assertCanConvert(const QVariant &value) const=0;
};

/** This is a property that can hold one value for each
    segment in the molecule.
    
    mol.setProperty( "charge", SegCharges( [....] ) )
    mol.setProperty( "lj", SegLJs( [....] ) )

    seg.setProperty( "charge", 0.0 * mod_e )
    
    @author Christopher Woods
*/
template<class T>
class SegProperty 
    : public SireBase::ConcreteProperty<SegProperty<T>, SegProp>
{

friend QDataStream& ::operator<<<>(QDataStream&, const SegProperty<T>&);
friend QDataStream& ::operator>><>(QDataStream&, SegProperty<T>&);

public:
    SegProperty();

    SegProperty(const MoleculeInfoData &molinfo);
    
    SegProperty(const QVector<T> &values);
    SegProperty(const QVector<QVariant> &values);
    
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

    void assignFrom(const QVector<QVariant> &values);
    
    QVector<QVariant> toVariant() const;
    
    bool canConvert(const QVariant &value) const;
    
    void assertCanConvert(const QVariant &value) const;

private:
    /** The actual segment property values */
    QVector<T> props;
};

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>::SegProperty()
              : SireBase::ConcreteProperty<SegProperty<T>,SegProp>()
{}

/** Construct space for the values of the property for all of the 
    segments in the molecule described by 'molinfo' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>::SegProperty(const MoleculeInfoData &molinfo)
              : SireBase::ConcreteProperty<SegProperty<T>,SegProp>()
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
              : SireBase::ConcreteProperty<SegProperty<T>,SegProp>()
{
    props = values;
    props.squeeze();
}

/** Assert that the variant can be converted to a value that can
    be held in this list of properties
    
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void SegProperty<T>::assertCanConvert(const QVariant &value) const
{
    if (not value.canConvert<T>())
    {
        throw SireError::invalid_cast( QObject::tr(
            "Cannot convert an object of type %1 to an object "
            "of type %2, as required by a %3.")
                .arg(value.typeName()).arg(T::typeName())
                .arg(this->what()), CODELOC );
    }
}

/** Construct from an array of variants

    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>::SegProperty(const QVector<QVariant> &values)
               : SireBase::ConcreteProperty<SegProperty<T>,SegProp>()
{
    if (values.isEmpty())
        return;
        
    int nvals = values.count();
    const QVariant *values_array = values.constData();
    
    props = QVector<T>(nvals);
    props.squeeze();
    T *props_array = props.data();
    
    for (int i=0; i<nvals; ++i)
    {
        const QVariant &value = values_array[i];
        SegProperty<T>::assertCanConvert(value);
        
        props_array[i] = value.value<T>();
    }
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SegProperty<T>::SegProperty(const SegProperty<T> &other)
              : SireBase::ConcreteProperty<SegProperty<T>,SegProp>(other),
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

/** Return whether or not it is possible to convert the variant
    'value' so that it can be part of this property */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool SegProperty<T>::canConvert(const QVariant &value) const
{
    return value.canConvert<T>();
}

/** Assign the values of this property from the array of variants
    in 'values'
    
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void SegProperty<T>::assignFrom(const QVector<QVariant> &values)
{
    this->operator=( SegProperty<T>(values) );
}

/** Convert the properties into an array of QVariants */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QVector<QVariant> SegProperty<T>::toVariant() const
{
    if (props.isEmpty())
        return QVector<QVariant>();
        
    int nvals = props.count();
    const T *props_array = props.constData();
    
    QVector<QVariant> converted_vals(nvals);
    converted_vals.squeeze();
    QVariant *converted_vals_array = converted_vals.data();

    for (int i=0; i<nvals; ++i)
    {
        converted_vals_array[i].setValue<T>(props_array[i]);
    }
    
    return converted_vals;
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

