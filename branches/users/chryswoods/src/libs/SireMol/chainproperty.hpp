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

#ifndef SIREMOL_CHAINPROPERTY_HPP
#define SIREMOL_CHAINPROPERTY_HPP

#include "moleculeinfodata.h"
#include "molviewproperty.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
template<class T>
class ChainProperty;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireMol::ChainProperty<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireMol::ChainProperty<T>&);

namespace SireMol
{

/** Small class used to provide a common base for all ChainProperty types */
class ChainProp : public MolViewProperty
{
public:
    ChainProp() : MolViewProperty()
    {}
    
    ChainProp(const ChainProp &other) : MolViewProperty(other)
    {}
    
    virtual ~ChainProp()
    {}
};

/** This is a property that can hold one value for each
    chain in the molecule.
    
    mol.setProperty( "charge", ChainCharges( [....] ) )
    mol.setProperty( "lj", ChainLJs( [....] ) )

    chain.setProperty( "charge", 0.0 * mod_e )
    
    @author Christopher Woods
*/
template<class T>
class ChainProperty 
    : public SireBase::ConcreteProperty<ChainProperty<T>, ChainProp>
{

friend QDataStream& ::operator<<<>(QDataStream&, const ChainProperty<T>&);
friend QDataStream& ::operator>><>(QDataStream&, ChainProperty<T>&);

public:
    ChainProperty();

    ChainProperty(const MoleculeInfoData &molinfo);
    
    ChainProperty(const QVector<T> &values);
    
    ChainProperty(const ChainProperty<T> &other);
    
    ~ChainProperty();
    
    ChainProperty<T>& operator=(const ChainProperty<T> &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< ChainProperty<T> >() );
    }
    
    ChainProperty<T>* clone() const
    {
        return new ChainProperty<T>(*this);
    }
    
    bool operator==(const ChainProperty<T> &other) const;
    bool operator!=(const ChainProperty<T> &other) const;

    const T& operator[](const ChainIdx &chainidx) const;
    const T& at(const ChainIdx &chainidx) const;
    const T& get(const ChainIdx &chainidx) const;

    ChainProperty<T>& set(ChainIdx chainidx, const T &value);

    const T* data() const;
    const T* constData() const;

    int size() const;
    int count() const;
    
    int nChains() const;

private:
    /** The actual chain property values */
    QVector<T> props;
};

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ChainProperty<T>::ChainProperty()
              : SireBase::ConcreteProperty<ChainProperty<T>,ChainProp>()
{}

/** Construct space for the values of the property for all of the 
    chains in the molecule described by 'molinfo' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ChainProperty<T>::ChainProperty(const MoleculeInfoData &molinfo)
              : SireBase::ConcreteProperty<ChainProperty<T>,ChainProp>()
{
    if (molinfo.nChains() > 0)
    {
        props = QVector<T>(molinfo.nChains());
        props.squeeze();
    }
}

/** Create chain properties from the list of passed values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ChainProperty<T>::ChainProperty(const QVector<T> &values)
              : SireBase::ConcreteProperty<ChainProperty<T>,ChainProp>()
{
    props = values;
    props.squeeze();
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ChainProperty<T>::ChainProperty(const ChainProperty<T> &other)
              : SireBase::ConcreteProperty<ChainProperty<T>,ChainProp>(other),
                props(other.props)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ChainProperty<T>::~ChainProperty()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ChainProperty<T>& ChainProperty<T>::operator=(const ChainProperty<T> &other)
{
    MolViewProperty::operator=(other);
    props = other.props;
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool ChainProperty<T>::operator==(const ChainProperty<T> &other) const
{
    return props == other.props;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool ChainProperty<T>::operator!=(const ChainProperty<T> &other) const
{
    return props != other.props;
}

/** Return the property for the chain at index 'chainidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& ChainProperty<T>::operator[](const ChainIdx &chainidx) const
{
    return props.constData()[chainidx.map(props.count())];
}

/** Return the property for the chain at index 'chainidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& ChainProperty<T>::at(const ChainIdx &chainidx) const
{
    return this->operator[](chainidx);
}

/** Return the property for the chain at index 'chainidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& ChainProperty<T>::get(const ChainIdx &chainidx) const
{
    return this->operator[](chainidx);
}

/** Set the value of the property for the chain at 
    index 'chainidx' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ChainProperty<T>& ChainProperty<T>::set(ChainIdx chainidx, const T &value)
{
    props.data()[chainidx.map(props.count())] = value;
    return *this;
}

/** Return a raw pointer to the array of property values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* ChainProperty<T>::data() const
{
    return props.constData();
}

/** Return a raw pointer to the array of property values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* ChainProperty<T>::constData() const
{
    return props.constData();
}

/** Return the number of chains */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int ChainProperty<T>::size() const
{
    return props.count();
}

/** Return the number of chains */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int ChainProperty<T>::count() const
{
    return props.count();
}

/** Return the number of chains */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int ChainProperty<T>::nChains() const
{
    return props.count();
}

}

SIRE_END_HEADER

#endif

