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

#ifndef SIREMOL_RESPROPERTY_HPP
#define SIREMOL_RESPROPERTY_HPP

#include <QVector>
#include <QVariant>

#include "moleculeinfodata.h"
#include "molviewproperty.h"

#include "SireError/errors.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResProp;

template<class T>
class ResProperty;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResProp&);
QDataStream& operator>>(QDataStream&, SireMol::ResProp&);

template<class T>
QDataStream& operator<<(QDataStream&, const SireMol::ResProperty<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireMol::ResProperty<T>&);

namespace SireMol
{

/** Small class used to provide a common base for all ResProperty types */
class ResProp : public MolViewProperty
{
public:
    ResProp();
    ResProp(const ResProp &other);
   
    virtual ~ResProp();
    
    virtual bool canConvert(const QVariant &value) const=0;
    
    virtual void assignFrom(const QVector<QVariant> &values)=0;
    
    virtual QVector<QVariant> toVariant() const=0;
    
    virtual void assertCanConvert(const QVariant &value) const=0;
};

/** This is a property that can hold one value for each
    residue in the molecule.
    
    mol.setProperty( "charge", ResCharges( [....] ) )
    mol.setProperty( "lj", ResLJs( [....] ) )

    res.setProperty( "charge", 0.0 * mod_e )
    
    @author Christopher Woods
*/
template<class T>
class ResProperty 
    : public SireBase::ConcreteProperty<ResProperty<T>, ResProp>
{

friend QDataStream& ::operator<<<>(QDataStream&, const ResProperty<T>&);
friend QDataStream& ::operator>><>(QDataStream&, ResProperty<T>&);

public:
    ResProperty();

    ResProperty(const MoleculeInfoData &molinfo);
    
    ResProperty(const QVector<T> &values);
    ResProperty(const QVector<QVariant> &values);
    
    ResProperty(const ResProperty<T> &other);
    
    ~ResProperty();
    
    ResProperty<T>& operator=(const ResProperty<T> &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< ResProperty<T> >() );
    }
    
    ResProperty<T>* clone() const
    {
        return new ResProperty<T>(*this);
    }
    
    bool operator==(const ResProperty<T> &other) const;
    bool operator!=(const ResProperty<T> &other) const;

    const T& operator[](const ResIdx &residx) const;
    const T& at(const ResIdx &residx) const;
    const T& get(const ResIdx &residx) const;

    ResProperty<T>& set(ResIdx residx, const T &value);

    const T* data() const;
    const T* constData() const;

    int size() const;
    int count() const;
    
    int nResidues() const;

    void assignFrom(const QVector<QVariant> &values);
    
    QVector<QVariant> toVariant() const;
    
    bool canConvert(const QVariant &value) const;
    
    void assertCanConvert(const QVariant &value) const;

private:
    /** The actual residue property values */
    QVector<T> props;
};

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResProperty<T>::ResProperty()
              : SireBase::ConcreteProperty<ResProperty<T>,ResProp>()
{}

/** Construct space for the values of the property for all of the 
    residues in the molecule described by 'molinfo' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResProperty<T>::ResProperty(const MoleculeInfoData &molinfo)
              : SireBase::ConcreteProperty<ResProperty<T>,ResProp>()
{
    if (molinfo.nResidues() > 0)
    {
        props = QVector<T>(molinfo.nResidues());
        props.squeeze();
    }
}

/** Create residue properties from the list of passed values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResProperty<T>::ResProperty(const QVector<T> &values)
              : SireBase::ConcreteProperty<ResProperty<T>,ResProp>()
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
void ResProperty<T>::assertCanConvert(const QVariant &value) const
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
ResProperty<T>::ResProperty(const QVector<QVariant> &values)
               : SireBase::ConcreteProperty<ResProperty<T>,ResProp>()
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
        ResProperty<T>::assertCanConvert(value);
        
        props_array[i] = value.value<T>();
    }
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResProperty<T>::ResProperty(const ResProperty<T> &other)
              : SireBase::ConcreteProperty<ResProperty<T>,ResProp>(other),
                props(other.props)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResProperty<T>::~ResProperty()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResProperty<T>& ResProperty<T>::operator=(const ResProperty<T> &other)
{
    MolViewProperty::operator=(other);
    props = other.props;
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool ResProperty<T>::operator==(const ResProperty<T> &other) const
{
    return props == other.props;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool ResProperty<T>::operator!=(const ResProperty<T> &other) const
{
    return props != other.props;
}

/** Return the property for the residue at index 'residx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& ResProperty<T>::operator[](const ResIdx &residx) const
{
    return props.constData()[residx.map(props.count())];
}

/** Return whether or not it is possible to convert the variant
    'value' so that it can be part of this property */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool ResProperty<T>::canConvert(const QVariant &value) const
{
    return value.canConvert<T>();
}

/** Assign the values of this property from the array of variants
    in 'values'
    
    \throw SireError::invalid_cast
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ResProperty<T>::assignFrom(const QVector<QVariant> &values)
{
    this->operator=( ResProperty<T>(values) );
}

/** Convert the properties into an array of QVariants */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QVector<QVariant> ResProperty<T>::toVariant() const
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

/** Return the property for the residue at index 'residx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& ResProperty<T>::at(const ResIdx &residx) const
{
    return this->operator[](residx);
}

/** Return the property for the residue at index 'residx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& ResProperty<T>::get(const ResIdx &residx) const
{
    return this->operator[](residx);
}

/** Set the value of the property for the residue at 
    index 'residx' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResProperty<T>& ResProperty<T>::set(ResIdx residx, const T &value)
{
    props.data()[residx.map(props.count())] = value;
    return *this;
}

/** Return a raw pointer to the array of property values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* ResProperty<T>::data() const
{
    return props.constData();
}

/** Return a raw pointer to the array of property values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* ResProperty<T>::constData() const
{
    return props.constData();
}

/** Return the number of residues */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int ResProperty<T>::size() const
{
    return props.count();
}

/** Return the number of residues */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int ResProperty<T>::count() const
{
    return props.count();
}

/** Return the number of residues */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int ResProperty<T>::nResidues() const
{
    return props.count();
}

}

/** Serialise this property to a binary datastream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireMol::ResProperty<T> &prop)
{
    //serialise the base class - this writes the header and version!
    ds << static_cast<const SireMol::ResProp&>(prop);
    ds << prop.props;
    
    return ds;
}

/** Extract from an binary datastream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireMol::ResProperty<T> &prop)
{
    ds >> static_cast<SireMol::ResProp&>(prop);
    ds >> prop.props;
        
    return ds;
}

SIRE_END_HEADER

#endif

