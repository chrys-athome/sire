/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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

#ifndef SIRESIM_DIMENSIONS_HPP
#define SIRESIM_DIMENSIONS_HPP

#include <QDomDocument>

#include "dimensions.h"

#include "SireError/errors.h"

namespace SireSim
{

/** Constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T>::DimensionValue() 
                  : Value(), sclfac(1), val(0), minval(0), maxval(0),
                    has_minval(false), has_maxval(false)
{}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
double DimensionValue<T>::scaleFactor(QString &units)
{
    units = units.trimmed();
    
    if (units.isEmpty())
        return 1;
    
    QHash<QString,double> sclfacs = T::units();

    double scl = sclfacs.value(units,0);
    
    if (scl == 0)
    {
        throw SireError::invalid_arg( QObject::tr(
                "Cannot interpret the unit \"%1\". Available units "
                "are [ %2 ].")
                    .arg(units, QStringList(sclfacs.keys()).join("\n")),
                        CODELOC );
    }
    
    return scl;
}

/** Construct, passing in a value in the specified units. If no
    units are supplied, then this is read in base units */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T>::DimensionValue(double value, QString units) 
                  : Value(), val(value), minval(0), maxval(0),
                    has_minval(false), has_maxval(false)
{
    sclfac = DimensionValue<T>::scaleFactor(units);
    unitname = units;
}

/** Construct an DimensionValue with passed value, and specified minimum */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T> DimensionValue<T>::valueWithMinimum(double value, 
                                                      double minimum, QString units)
{
    DimensionValue<T> v(value, units);
    v.minval = minimum;
    v.has_minval = true;
    
    if (v.val < v.minval)
        v.val = v.minval;
        
    return v;
}

/** Construct an DimensionValue with passed value, and specified maximum */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T> DimensionValue<T>::valueWithMaximum(double value, 
                                                      double maximum, QString units)
{
    DimensionValue<T> v(value, units);
    v.maxval = maximum;
    v.has_maxval = true;
    
    if (v.val > v.maxval)
        v.val = v.maxval;
        
    return v;
}

/** Construct an DimensionValue with passed value and specified range */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T> DimensionValue<T>::valueWithRange(double value, double minimum, 
                                                    double maximum, QString units)
{
    DimensionValue<T> v(value, units);
    
    if (minimum <= maximum)
    {
        v.minval = minimum;
        v.maxval = maximum;
    }
    else
    {
        v.minval = maximum;
        v.maxval = minimum;
    }
    
    v.has_minval = true;
    v.has_maxval = true;
    
    if (v.val < v.minval)
        v.val = v.minval;
        
    if (v.val > v.maxval)
        v.val = v.maxval;

    return v;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDomElement DimensionValue<T>::toDomElement(QDomDocument doc) const
{
    QDomElement elem = doc.createElement(T::tagName());
    elem.appendChild( doc.createTextNode( QString::number(val) ) );
    
    if (not unitname.isEmpty())
        elem.setAttribute("units", unitname);
    
    if (has_minval)
        elem.setAttribute("minimum", QString::number(minval));
        
    if (has_maxval)
        elem.setAttribute("maximum", QString::number(maxval));
    
    return elem;
}

namespace detail
{
    QPair<double,QString> readDimension(QString value);
    double readFloat(QString value);
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T>::DimensionValue(QString value)
                  : Value(), minval(0), maxval(0),
                    has_minval(false), has_maxval(false)
{
    QPair<double,QString> parsed_value = detail::readDimension(value);
    this->operator=( DimensionValue<T>(parsed_value.first, parsed_value.second) );
}

/** Construct from a QDomElement */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T>::DimensionValue(QDomElement elem) 
                  : Value(), sclfac(1), val(0), minval(0), maxval(0),
                    has_minval(false), has_maxval(false)
{
    if (elem.tagName() != T::tagName())
        throw SireError::file_error( QObject::tr(
                "Can only create a %1 object from a <%2>...</%2> "
                "XML DOM element. Cannot use a <%3>...</%3> element!")
                    .arg(T::typeName()).arg(T::tagName())
                    .arg(elem.tagName()), CODELOC );

    if (not elem.text().isEmpty())
    {
        val = detail::readFloat(elem.text());
    }

    if (not elem.attribute("minimum").isEmpty())
    {
        minval = detail::readFloat(elem.attribute("minimum"));
        has_minval = true;
    }

    if (not elem.attribute("maximum").isEmpty())
    {
        maxval = detail::readFloat(elem.attribute("maximum"));
        has_maxval = true;
    }
    
    if (not elem.attribute("units").isEmpty())
    {
        unitname = elem.attribute("units");
        sclfac = scaleFactor(unitname);
    }
    
    if (has_minval and has_maxval)
    {
        if (minval > maxval)
        {
            qSwap(minval, maxval);
        }
    }
    
    if (has_minval and val < minval)
        val = minval;
        
    if (has_maxval and val > maxval)
        val = maxval;
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T>::DimensionValue(const DimensionValue<T> &other)
                  : Value(other), 
                    unitname(other.unitname), 
                    sclfac(other.sclfac), val(other.val), 
                    minval(other.minval), maxval(other.maxval),
                    has_minval(other.has_minval),
                    has_maxval(other.has_maxval)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T>::~DimensionValue()
{}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T>* DimensionValue<T>::ptr_clone() const
{
    return new DimensionValue<T>(*this);
}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
DimensionValue<T>& DimensionValue<T>::operator=(const DimensionValue<T> &other)
{
    if (this != &other)
    {
        unitname = other.unitname;
        sclfac = other.sclfac;
        val = other.val;
        minval = other.minval;
        maxval = other.maxval;
        has_minval = other.has_minval;
        has_maxval = other.has_maxval;
    }
    
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool DimensionValue<T>::operator==(const DimensionValue<T> &other) const
{
    return sclfac == other.sclfac and 
           val == other.val and minval == other.minval and
           maxval == other.maxval and has_minval == other.has_minval and
           has_maxval == other.has_maxval;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool DimensionValue<T>::operator!=(const DimensionValue<T> &other) const
{
    return not DimensionValue<T>::operator==(other);
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const char* DimensionValue<T>::typeName()
{
    return T::typeName();
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const char* DimensionValue<T>::what() const
{
    return T::typeName();
}

/** Return the actual value, with no conversion from the units
    in which this value was supplied */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
double DimensionValue<T>::value() const
{
    return val;
}

/** Return the actual value, converted to units 'units' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
double DimensionValue<T>::value(QString units) const
{
    double sclto = DimensionValue<T>::scaleFactor(units);
    
    return val * (sclfac/sclto);
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
QString DimensionValue<T>::toValueString() const
{
    if (unitname.isEmpty())
        return QString::number(val, 'g', 12);
    else
        return QString("%1 %2").arg( QString::number(val, 'g', 12), unitname );
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
ValuePtr DimensionValue<T>::fromValueString(QString value) const
{
    QPair<double,QString> parsed_value = detail::readDimension(value);
    
    double sclfrom = DimensionValue<T>::scaleFactor(parsed_value.second);
    
    double v = parsed_value.first * (sclfrom/sclfac);
    
    if (has_minval and v < minval)
        v = minval;
        
    if (has_maxval and v > maxval)
        v = maxval;
        
    if (v == val)
        return self();
    else
    {
        DimensionValue<T> ret(*this);
        ret.val = v;
        return ret;
    }
}

}

#endif
