#ifndef CONSPIRE_DIMENSIONS_HPP
#define CONSPIRE_DIMENSIONS_HPP
/********************************************\
  *
  *  Conspire
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

#include "Conspire/dimensions.h"
#include "Conspire/exceptions.h"
#include "Conspire/conspire.hpp"

CONSPIRE_BEGIN_HEADER

#ifdef WIN32
#define CONSPIRE_IMPORT CONSPIRE_EXPORT
#else
#define CONSPIRE_IMPORT
#endif

namespace Conspire
{
    /** Constructor */
    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    DimensionValue<T>::DimensionValue() 
                      : Value(), sclfac(1), val(0), minval(0), maxval(0),
                        has_minval(false), has_maxval(false)
    {}

    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    double DimensionValue<T>::scaleFactor(Qt::String &units)
    {
        units = units.trimmed();
        
        if (units.isEmpty())
            return 1;
        
        Qt::Hash<Qt::String,double>::Type sclfacs = T::units();

        double scl = sclfacs.value(units,0);
        
        if (scl == 0)
        {
            throw Conspire::invalid_arg( QObject::tr(
                    "Cannot interpret the unit \"%1\". Available units "
                    "are [ %2 ].")
                        .arg(units, Qt::StringList(sclfacs.keys()).join("\n")),
                            CODELOC );
        }
        
        return scl;
    }

    /** Construct, passing in a value in the specified units. If no
        units are supplied, then this is read in base units */
    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    DimensionValue<T>::DimensionValue(double value, Qt::String units) 
                      : Value(), val(value), minval(0), maxval(0),
                        has_minval(false), has_maxval(false)
    {
        sclfac = DimensionValue<T>::scaleFactor(units);
        unitname = units;
    }

    /** Construct an DimensionValue with passed value, and specified minimum */
    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    DimensionValue<T> DimensionValue<T>::valueWithMinimum(double value, 
                                                          double minimum, 
                                                          Qt::String units)
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
    CONSPIRE_OUTOFLINE_TEMPLATE
    DimensionValue<T> DimensionValue<T>::valueWithMaximum(double value, 
                                                          double maximum, 
                                                          Qt::String units)
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
    CONSPIRE_OUTOFLINE_TEMPLATE
    DimensionValue<T> DimensionValue<T>::valueWithRange(double value, double minimum, 
                                                        double maximum, 
                                                        Qt::String units)
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
    CONSPIRE_OUTOFLINE_TEMPLATE
    Qt::DomElement DimensionValue<T>::toDomElement(Qt::DomDocument doc) const
    {
        Qt::DomElement elem = doc.createElement(T::tagName());
        elem.appendChild( doc.createTextNode( Qt::String::number(val) ) );
        
        if (not unitname.isEmpty())
            elem.setAttribute("units", unitname);
        
        if (has_minval)
            elem.setAttribute("minimum", Qt::String::number(minval));
            
        if (has_maxval)
            elem.setAttribute("maximum", Qt::String::number(maxval));
        
        return elem;
    }

    namespace detail
    {
        Qt::Pair<double,Qt::String>::Type 
        readDimension(Qt::String value, Qt::StringList units);
        
        Qt::Pair<Qt::String,qint64>::Type 
        CONSPIRE_IMPORT getPostFactor(Qt::String value);
        
        double readFloat(Qt::String value);
        Qt::int64 CONSPIRE_IMPORT readInt(Qt::String value);
        bool readBool(Qt::String value);
    }

    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    DimensionValue<T>::DimensionValue(Qt::String value)
                      : Value(), minval(0), maxval(0),
                        has_minval(false), has_maxval(false)
    {
        Qt::Pair<double,Qt::String>::Type 
        parsed_value = detail::readDimension(value, DimensionValue<T>::supportedUnits());
        this->operator=( DimensionValue<T>(parsed_value.first, parsed_value.second) );
    }

    /** Construct from a DomElement */
    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    DimensionValue<T>::DimensionValue(Qt::DomElement elem) 
                      : Value(), sclfac(1), val(0), minval(0), maxval(0),
                        has_minval(false), has_maxval(false)
    {
        if (elem.tagName() != T::tagName())
            throw Conspire::parse_error( QObject::tr(
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
    CONSPIRE_OUTOFLINE_TEMPLATE
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
    CONSPIRE_OUTOFLINE_TEMPLATE
    DimensionValue<T>::~DimensionValue()
    {}

    /** Copy assignment operator */
    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    void DimensionValue<T>::copy_object(const DimensionValue<T> &other)
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
        
        super::copy_object(other);
    }

    /** Comparison operator */
    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    bool DimensionValue<T>::compare_object(const DimensionValue<T> &other) const
    {
        return sclfac == other.sclfac and 
               val == other.val and minval == other.minval and
               maxval == other.maxval and has_minval == other.has_minval and
               has_maxval == other.has_maxval and
               super::compare_object(other);
    }

    /** Return the list of units that are supported for this dimension */
    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    Qt::StringList DimensionValue<T>::supportedUnits()
    {
        return Qt::StringList(T::units().keys());
    }

    /** Return the actual value, with no conversion from the units
        in which this value was supplied */
    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    double DimensionValue<T>::value() const
    {
        return val;
    }

    /** Return the actual value, converted to units 'units' */
    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    double DimensionValue<T>::value(Qt::String units) const
    {
        double sclto = DimensionValue<T>::scaleFactor(units);
        
        return val * (sclfac/sclto);
    }

    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    Qt::String DimensionValue<T>::toValueString() const
    {
        if (unitname.isEmpty())
            return Qt::String::number(val, 'g', 12);
        else
            return Qt::String("%1 %2").arg( 
                        Qt::String::number(val, 'g', 12), unitname );
    }

    template<class T>
    CONSPIRE_OUTOFLINE_TEMPLATE
    Obj DimensionValue<T>::fromValueString(Qt::String value) const
    {
        Qt::Pair<double,Qt::String>::Type 
        parsed_value = detail::readDimension(value, DimensionValue<T>::supportedUnits());
        
        double sclfrom = DimensionValue<T>::scaleFactor(parsed_value.second);
        
        double v = parsed_value.first * (sclfrom/sclfac);
        
        if (has_minval and v < minval)
            v = minval;
            
        if (has_maxval and v > maxval)
            v = maxval;
            
        if (v == val)
            return *this;
        else
        {
            DimensionValue<T> ret(*this);
            ret.val = v;
            return ret;
        }
    }
}

CONSPIRE_END_HEADER

#endif
