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

#ifndef SIREMOL_CGPROPERTY_HPP
#define SIREMOL_CGPROPERTY_HPP

#include "moleculeinfodata.h"
#include "molviewproperty.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
template<class T>
class CGProperty;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireMol::CGProperty<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireMol::CGProperty<T>&);

namespace SireMol
{

/** This is a property that can hold one value for each
    CutGroup in the molecule.
    
    mol.setProperty( "charge", CGCharges( [....] ) )
    mol.setProperty( "lj", CGLJs( [....] ) )

    cg.setProperty( "charge", 0.0 * mod_e )
    
    @author Christopher Woods
*/
template<class T>
class CGProperty 
    : public SireBase::ConcreteProperty<CGProperty<T>, MolViewProperty>
{

friend QDataStream& ::operator<<<>(QDataStream&, const CGProperty<T>&);
friend QDataStream& ::operator>><>(QDataStream&, CGProperty<T>&);

public:
    CGProperty();

    CGProperty(const MoleculeInfoData &molinfo);
    
    CGProperty(const QVector<T> &values);
    
    CGProperty(const CGProperty<T> &other);
    
    ~CGProperty();
    
    CGProperty<T>& operator=(const CGProperty<T> &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< CGProperty<T> >() );
    }
    
    CGProperty<T>* clone() const
    {
        return new CGProperty<T>(*this);
    }
    
    bool operator==(const CGProperty<T> &other) const;
    bool operator!=(const CGProperty<T> &other) const;

    const T& operator[](const CGIdx &cgidx) const;
    const T& at(const CGIdx &cgidx) const;
    const T& get(const CGIdx &cgidx) const;

    CGProperty<T>& set(CGIdx cgidx, const T &value);

    const T* data() const;
    const T* constData() const;

    int size() const;
    int count() const;
    
    int nCutGroups() const;

private:
    /** The actual CutGroup property values */
    QVector<T> props;
};

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGProperty<T>::CGProperty()
              : SireBase::ConcreteProperty<CGProperty<T>,MolViewProperty>()
{}

/** Construct space for the values of the property for all of the 
    CutGroups in the molecule described by 'molinfo' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGProperty<T>::CGProperty(const MoleculeInfoData &molinfo)
              : SireBase::ConcreteProperty<CGProperty<T>,MolViewProperty>()
{
    if (molinfo.nCutGroups() > 0)
    {
        props = QVector<T>(molinfo.nCutGroups());
        props.squeeze();
    }
}

/** Create CutGroup properties from the list of passed values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGProperty<T>::CGProperty(const QVector<T> &values)
              : SireBase::ConcreteProperty<CGProperty<T>,MolViewProperty>()
{
    props = values;
    props.squeeze();
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGProperty<T>::CGProperty(const CGProperty<T> &other)
              : SireBase::ConcreteProperty<CGProperty<T>,MolViewProperty>(other),
                props(other.props)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGProperty<T>::~CGProperty()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGProperty<T>& CGProperty<T>::operator=(const CGProperty<T> &other)
{
    MolViewProperty::operator=(other);
    props = other.props;
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool CGProperty<T>::operator==(const CGProperty<T> &other) const
{
    return props == other.props;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool CGProperty<T>::operator!=(const CGProperty<T> &other) const
{
    return props != other.props;
}

/** Return the property for the CutGroup at index 'cgidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& CGProperty<T>::operator[](const CGIdx &cgidx) const
{
    return props.constData()[cgidx.map(props.count())];
}

/** Return the property for the CutGroup at index 'cgidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& CGProperty<T>::at(const CGIdx &cgidx) const
{
    return this->operator[](cgidx);
}

/** Return the property for the CutGroup at index 'cgidx' 

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& CGProperty<T>::get(const CGIdx &cgidx) const
{
    return this->operator[](cgidx);
}

/** Set the value of the property for the CutGroup at 
    index 'cgidx' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
CGProperty<T>& CGProperty<T>::set(CGIdx cgidx, const T &value)
{
    props.data()[cgidx.map(props.count())] = value;
    return *this;
}

/** Return a raw pointer to the array of property values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* CGProperty<T>::data() const
{
    return props.constData();
}

/** Return a raw pointer to the array of property values */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T* CGProperty<T>::constData() const
{
    return props.constData();
}

/** Return the number of CutGroups */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int CGProperty<T>::size() const
{
    return props.count();
}

/** Return the number of CutGroups */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int CGProperty<T>::count() const
{
    return props.count();
}

/** Return the number of CutGroups */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int CGProperty<T>::nCutGroups() const
{
    return props.count();
}

}

SIRE_END_HEADER

#endif
