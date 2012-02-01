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

#include <QSet>
#include <QDomDocument>

#include "SireSim/tempvalue.h"
#include "SireSim/dimensions.h"
#include "SireSim/dimensions.hpp"

#include "SireError/errors.h"

using namespace SireSim;
using namespace SireSim::detail;

/** Constructor */
TempValue::TempValue() 
          : Value(), val(0), minval(0), maxval(0),
            has_minval(false), has_maxval(false)
{}

QSet<QString> *celsius_units = 0;
QSet<QString> *fahrenheit_units = 0;
QSet<QString> *kelvin_units = 0;

void makeUnits()
{
    while (celsius_units == 0)
    {
        QSet<QString> units;
        units.insert( "celsius" );
        units.insert( "C" );
        units.insert( "centigrade" );
        
        if (celsius_units == 0)
            celsius_units = new QSet<QString>(units);
    }
    
    while (fahrenheit_units == 0)
    {
        QSet<QString> units;
        units.insert("fahrenheit");
        units.insert("F");
        
        if (fahrenheit_units == 0)
            fahrenheit_units = new QSet<QString>(units);
    }
    
    while (kelvin_units == 0)
    {
        QSet<QString> units;
        units.insert("kelvin");
        units.insert("K");
        
        if (kelvin_units == 0)
            kelvin_units = new QSet<QString>(units);
    }
}

double TempValue::fromKelvin(double temperature, QString units)
{
    makeUnits();
    
    units = units.trimmed();
    
    if (celsius_units->contains(units))
    {
        return temperature - 273.15;
    }
    else if (fahrenheit_units->contains(units))
    {
        return ( (9.0/5.0) * temperature ) - 459.67;
    }
    else if (kelvin_units->contains(units))
    {
        return temperature;
    }
    else
    {
        throw SireError::invalid_arg( QObject::tr(
                "Unrecognised temperature unit \"%1\". Recognised units "
                "are [ %2 ].")
                    .arg(units, TempValue::supportedUnits().join(", ")), 
                        CODELOC );
                        
        return 0;
    }
}

double TempValue::toKelvin(double temperature, QString &units)
{
    makeUnits();

    units = units.trimmed();
    
    if (units.isEmpty())
        return temperature;
    
    if (celsius_units->contains(units))
    {
        double kelvin = temperature + 273.15;
        
        if (kelvin < 0)
            throw SireError::invalid_arg( QObject::tr(
                    "You cannot specify a temperature that is below absolute zero! "
                    "%1 C is %2 C below absolute zero!")
                        .arg(temperature).arg(-kelvin), CODELOC );
                        
        return kelvin;
    }
    else if (fahrenheit_units->contains(units))
    {
        double kelvin = (temperature + 459.67) * (5.0/9.0);
        
        if (kelvin < 0)
            throw SireError::invalid_arg( QObject::tr(
                    "You cannot specify a temperature that is below absolute zero! "
                    "%1 F is %2 F below absolute zero!")
                        .arg(temperature)
                        .arg( (9.0/5.0) * (-kelvin) ), CODELOC );
                        
        return kelvin;
    }
    else if (kelvin_units->contains(units))
    {
        double kelvin = temperature;
        
        if (kelvin < 0)
            throw SireError::invalid_arg( QObject::tr(
                    "You cannot specify a temperature that is below absolute zero! "
                    "%1 K is %2 K below absolute zero!")
                        .arg(temperature)
                        .arg(-kelvin), CODELOC );
                        
        return kelvin;
    }
    else
    {
        throw SireError::invalid_arg( QObject::tr(
                "Could not recognise the temperature unit \"%1\". Supported "
                "units are [ %2 ].")
                    .arg(units, TempValue::supportedUnits().join(", ")), 
                        CODELOC );
                        
        return 0;
    }
}

/** Construct, passing in a value in the specified units. If no
    units are supplied, then this is read in base units */
TempValue::TempValue(double value, QString units) 
          : Value(), val(0), minval(0), maxval(0),
            has_minval(false), has_maxval(false)
{
    val = toKelvin(value, units);
    unitname = units;
}

/** Construct an TempValue with passed value, and specified minimum */
TempValue TempValue::valueWithMinimum(double value, 
                                      double minimum, QString units)
{
    TempValue v(value, units);
    v.minval = toKelvin(minimum, units);
    v.has_minval = true;
    
    if (v.val < v.minval)
        v.val = v.minval;
        
    return v;
}

/** Construct an TempValue with passed value, and specified maximum */
TempValue TempValue::valueWithMaximum(double value, 
                                      double maximum, QString units)
{
    TempValue v(value, units);
    v.maxval = toKelvin(maximum, units);
    v.has_maxval = true;
    
    if (v.val > v.maxval)
        v.val = v.maxval;
        
    return v;
}

/** Construct an TempValue with passed value and specified range */
TempValue TempValue::valueWithRange(double value, double minimum, 
                                    double maximum, QString units)
{
    TempValue v(value, units);
    
    minimum = toKelvin(minimum, units);
    maximum = toKelvin(maximum, units);
    
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

QDomElement TempValue::toDomElement(QDomDocument doc) const
{
    QDomElement elem = doc.createElement("temperature");
    elem.appendChild( doc.createTextNode( QString::number(fromKelvin(val,unitname)) ) );
    
    if (not unitname.isEmpty())
        elem.setAttribute("units", unitname);
    
    if (has_minval)
        elem.setAttribute("minimum", QString::number(fromKelvin(minval,unitname)));
        
    if (has_maxval)
        elem.setAttribute("maximum", QString::number(fromKelvin(maxval,unitname)));
    
    return elem;
}

TempValue::TempValue(QString value)
          : Value(), minval(0), maxval(0),
            has_minval(false), has_maxval(false)
{
    QPair<double,QString> parsed_value = detail::readDimension(value);
    this->operator=( TempValue(parsed_value.first, parsed_value.second) );
}

/** Construct from a QDomElement */
TempValue::TempValue(QDomElement elem) 
          : Value(), val(0), minval(0), maxval(0),
            has_minval(false), has_maxval(false)
{
    if (elem.tagName() != "temperature")
        throw SireError::file_error( QObject::tr(
              "Can only create a TempValue object from a <temperature>...</temperature> "
              "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );

    
    if (not elem.attribute("units").isEmpty())
    {
        unitname = elem.attribute("units");
    }
    else
    {
        unitname = QString();
    }

    if (not elem.text().isEmpty())
    {
        val = toKelvin(detail::readFloat(elem.text()), unitname);
    }

    if (not elem.attribute("minimum").isEmpty())
    {
        minval = toKelvin(detail::readFloat(elem.attribute("minimum")), unitname);
        has_minval = true;
    }

    if (not elem.attribute("maximum").isEmpty())
    {
        maxval = toKelvin(detail::readFloat(elem.attribute("maximum")), unitname);
        has_maxval = true;
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
TempValue::TempValue(const TempValue &other)
          : Value(other), 
            unitname(other.unitname), 
            val(other.val), 
            minval(other.minval), maxval(other.maxval),
            has_minval(other.has_minval),
            has_maxval(other.has_maxval)
{}

/** Destructor */
TempValue::~TempValue()
{}

TempValue* TempValue::ptr_clone() const
{
    return new TempValue(*this);
}

/** Copy assignment operator */
TempValue& TempValue::operator=(const TempValue &other)
{
    if (this != &other)
    {
        unitname = other.unitname;
        val = other.val;
        minval = other.minval;
        maxval = other.maxval;
        has_minval = other.has_minval;
        has_maxval = other.has_maxval;
    }
    
    return *this;
}

/** Comparison operator */
bool TempValue::operator==(const TempValue &other) const
{
    return unitname == other.unitname and
           val == other.val and minval == other.minval and
           maxval == other.maxval and has_minval == other.has_minval and
           has_maxval == other.has_maxval;
}

/** Comparison operator */
bool TempValue::operator!=(const TempValue &other) const
{
    return not TempValue::operator==(other);
}

const char* TempValue::typeName()
{
    return "SireSim::TempValue";
}

const char* TempValue::what() const
{
    return TempValue::typeName();
}

/** Return the list of units that are supported for this dimension */
QStringList TempValue::supportedUnits()
{
    makeUnits();

    QStringList units;
    
    units += celsius_units->toList();
    units += fahrenheit_units->toList();
    units += kelvin_units->toList();
    
    return units;
}

/** Return the actual value, with no conversion from the units
    in which this value was supplied */
double TempValue::value() const
{
    return fromKelvin(val,unitname);
}

/** Return the actual value, converted to units 'units' */
double TempValue::value(QString units) const
{
    return fromKelvin(val,units);
}

QString TempValue::toValueString() const
{
    if (unitname.isEmpty())
        return QString::number(val, 'g', 12);
    else
        return QString("%1 %2").arg( QString::number(fromKelvin(val,unitname), 'g', 12), 
                                     unitname );
}

ValuePtr TempValue::fromValueString(QString value) const
{
    QPair<double,QString> parsed_value = detail::readDimension(value);
    
    QString units = parsed_value.second;
    double v = toKelvin(parsed_value.first,units);
    
    if (has_minval and v < minval)
        v = minval;
        
    if (has_maxval and v > maxval)
        v = maxval;
        
    if (v == val and units == unitname)
        return self();
    else
    {
        TempValue ret(*this);
        ret.val = v;
        ret.unitname = units;
        return ret;
    }
}
