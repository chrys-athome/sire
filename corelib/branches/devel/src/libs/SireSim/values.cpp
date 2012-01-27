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

#include <QDomElement>
#include <QDomDocument>

#include "SireSim/values.h"

#include "SireError/errors.h"

using namespace SireSim;

/////////
///////// Implementation of StringValue
/////////

/** Constructor */
StringValue::StringValue() : Value()
{}

/** Construct, passing in a value */
StringValue::StringValue(const QString &value) : Value(), val(value)
{}

QDomElement StringValue::toDomElement(QDomDocument doc) const
{
    QDomElement elem = doc.createElement("string");
    elem.appendChild( doc.createTextNode(val) );
    return elem;
}

/** Construct from a QDomElement */
StringValue::StringValue(QDomElement elem) : Value()
{
    if (elem.tagName() != "string")
        throw SireError::file_error( QObject::tr(
                "Can only create a StringValue object from an <string>...</string> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
                    
    val = elem.text();
}

/** Copy constructor */
StringValue::StringValue(const StringValue &other)
            : Value(other), val(other.val)
{}

/** Destructor */
StringValue::~StringValue()
{}

StringValue* StringValue::ptr_clone() const
{
    return new StringValue(*this);
}

/** Copy assignment operator */
StringValue& StringValue::operator=(const StringValue &other)
{
    val = other.val;
    return *this;
}

/** Comparison operator */
bool StringValue::operator==(const StringValue &other) const
{
    return val == other.val;
}

/** Comparison operator */
bool StringValue::operator!=(const StringValue &other) const
{
    return not StringValue::operator==(other);
}

const char* StringValue::typeName()
{
    return "SireSim::StringValue";
}

const char* StringValue::what() const
{
    return StringValue::typeName();
}

/** Return the actual string value */
QString StringValue::value() const
{
    return val;
}

/** Get the value. This passed key should be null, as this is a value */
ValuePtr StringValue::getValue(QString key) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to get a StringValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );

    return self();
}

/** Set the value of this string to 'value'. This passed key should be null,
    and the passed value must have type "StringValue" */
ValuePtr StringValue::setValue(QString key, const Value &value) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a StringValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );
    
    else if (not value.isA<StringValue>())
        throw SireError::invalid_arg( QObject::tr(
                "You cannot set the value of a StringValue using a value "
                "of type \"%1\".").arg(value.what()), CODELOC );
    
    return value;
}

/** Return this value as given in a configure file */
QStringList StringValue::toConfigLines(bool) const
{
    QStringList lines;
    
    lines.append(val);
    
    return lines;
}

/** Set the value from the passed line */
ValuePtr StringValue::fromConfig(detail::ParsedLine &line) const
{
    if (not line.key.key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a StringValue value with "
                "a non-empty key! \"%1\"").arg(line.key.key), CODELOC );

    if (line.been_read)
        return self();
    
    else
    {
        line.been_read = true;
        return StringValue(line.value).clone();
    }
}

/////////
///////// Implementation of DirValue
/////////

/** Constructor */
DirValue::DirValue() : Value()
{}

/** Construct, passing in a value */
DirValue::DirValue(const QString &dir) : Value(), val(dir)
{
    //should validate that this looks like a directory...
}

QDomElement DirValue::toDomElement(QDomDocument doc) const
{
    QDomElement elem = doc.createElement("directory");
    elem.appendChild( doc.createTextNode(val) );
    return elem;
}

/** Construct from a QDomElement */
DirValue::DirValue(QDomElement elem) : Value()
{
    if (elem.tagName() != "directory")
        throw SireError::file_error( QObject::tr(
                "Can only create a DirValue object from an <directory>...</directory> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
                    
    val = elem.text();
    
    //should validate that this looks like a directory...
}

/** Copy constructor */
DirValue::DirValue(const DirValue &other)
         : Value(other), val(other.val)
{}

/** Destructor */
DirValue::~DirValue()
{}

DirValue* DirValue::ptr_clone() const
{
    return new DirValue(*this);
}

/** Copy assignment operator */
DirValue& DirValue::operator=(const DirValue &other)
{
    val = other.val;
    return *this;
}

/** Comparison operator */
bool DirValue::operator==(const DirValue &other) const
{
    return val == other.val;
}

/** Comparison operator */
bool DirValue::operator!=(const DirValue &other) const
{
    return not DirValue::operator==(other);
}

const char* DirValue::typeName()
{
    return "SireSim::DirValue";
}

const char* DirValue::what() const
{
    return DirValue::typeName();
}

/** Return the actual string value */
QString DirValue::value() const
{
    return val;
}

/** Get the value. This passed key should be null, as this is a value */
ValuePtr DirValue::getValue(QString key) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to get a DirValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );

    return self();
}

/** Set the value of this string to 'value'. This passed key should be null,
    and the passed value must have type "DirValue" */
ValuePtr DirValue::setValue(QString key, const Value &value) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a DirValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );
    
    else if (not value.isA<DirValue>())
        throw SireError::invalid_arg( QObject::tr(
                "You cannot set the value of a DirValue using a value "
                "of type \"%1\".").arg(value.what()), CODELOC );
    
    return value;
}

/** Return this value as given in a configure file */
QStringList DirValue::toConfigLines(bool) const
{
    QStringList lines;
    
    lines.append(val);
    
    return lines;
}

/** Set the value from the passed line */
ValuePtr DirValue::fromConfig(detail::ParsedLine &line) const
{
    if (not line.key.key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a DirValue value with "
                "a non-empty key! \"%1\"").arg(line.key.key), CODELOC );

    if (line.been_read)
        return self();
    
    else
    {
        line.been_read = true;
        return DirValue(line.value).clone();
    }
}

/////////
///////// Implementation of FileValue
/////////

/** Constructor */
FileValue::FileValue() : Value()
{}

/** Construct, passing in a value */
FileValue::FileValue(const QString &file) : Value(), val(file)
{
    //should validate that this looks like a file...
}

QDomElement FileValue::toDomElement(QDomDocument doc) const
{
    QDomElement elem = doc.createElement("file");
    elem.appendChild( doc.createTextNode(val) );
    return elem;
}

/** Construct from a QDomElement */
FileValue::FileValue(QDomElement elem) : Value()
{
    if (elem.tagName() != "file")
        throw SireError::file_error( QObject::tr(
                "Can only create a FileValue object from an <file>...</file> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
                    
    val = elem.text();
    
    //should validate that this looks like a file
}

/** Copy constructor */
FileValue::FileValue(const FileValue &other)
          : Value(other), val(other.val)
{}

/** Destructor */
FileValue::~FileValue()
{}

FileValue* FileValue::ptr_clone() const
{
    return new FileValue(*this);
}

/** Copy assignment operator */
FileValue& FileValue::operator=(const FileValue &other)
{
    val = other.val;
    return *this;
}

/** Comparison operator */
bool FileValue::operator==(const FileValue &other) const
{
    return val == other.val;
}

/** Comparison operator */
bool FileValue::operator!=(const FileValue &other) const
{
    return not FileValue::operator==(other);
}

const char* FileValue::typeName()
{
    return "SireSim::FileValue";
}

const char* FileValue::what() const
{
    return FileValue::typeName();
}

/** Return the actual string value */
QString FileValue::value() const
{
    return val;
}

/** Get the value. This passed key should be null, as this is a value */
ValuePtr FileValue::getValue(QString key) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to get a FileValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );

    return self();
}

/** Set the value of this string to 'value'. This passed key should be null,
    and the passed value must have type "FileValue" */
ValuePtr FileValue::setValue(QString key, const Value &value) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a FileValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );
    
    else if (not value.isA<FileValue>())
        throw SireError::invalid_arg( QObject::tr(
                "You cannot set the value of a FileValue using a value "
                "of type \"%1\".").arg(value.what()), CODELOC );
    
    return value;
}

/** Return this value as given in a configure file */
QStringList FileValue::toConfigLines(bool) const
{
    QStringList lines;
    
    lines.append(val);
    
    return lines;
}

/** Set the value from the passed line */
ValuePtr FileValue::fromConfig(detail::ParsedLine &line) const
{
    if (not line.key.key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a FileValue value with "
                "a non-empty key! \"%1\"").arg(line.key.key), CODELOC );

    if (line.been_read)
        return self();
    
    else
    {
        line.been_read = true;
        return FileValue(line.value).clone();
    }
}

/////////
///////// Implementation of IntegerValue
/////////

/** Constructor */
IntegerValue::IntegerValue() 
             : Value(), val(0), minval( std::numeric_limits<qint64>::min() ),
                                maxval( std::numeric_limits<qint64>::max() )
{}

/** Construct, passing in a value */
IntegerValue::IntegerValue(qint64 value) 
             : Value(), val(value), minval( std::numeric_limits<qint64>::min() ),
                                    maxval( std::numeric_limits<qint64>::max() )
{}

/** Construct an IntegerValue with passed value, and specified minimum */
IntegerValue IntegerValue::valueWithMinimum(qint64 value, qint64 minimum)
{
    IntegerValue v;
    v.val = value;
    v.minval = minimum;
    
    if (v.val < v.minval)
        v.val = v.minval;
        
    return v;
}

/** Construct an IntegerValue with passed value, and specified maximum */
IntegerValue IntegerValue::valueWithMaximum(qint64 value, qint64 maximum)
{
    IntegerValue v;
    v.val = value;
    v.maxval = maximum;
    
    if (v.val > v.maxval)
        v.val = v.maxval;
        
    return v;
}

/** Construct an IntegerValue with passed value and specified range */
IntegerValue IntegerValue::valueWithRange(qint64 value, qint64 minimum, qint64 maximum)
{
    IntegerValue v;
    
    v.val = value;
    
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
    
    if (v.val < v.minval)
        v.val = v.minval;
        
    if (v.val > v.maxval)
        v.val = v.maxval;

    return v;
}

QDomElement IntegerValue::toDomElement(QDomDocument doc) const
{
    QDomElement elem = doc.createElement("integer");
    elem.appendChild( doc.createTextNode( QString::number(val) ) );
    
    if (minval != std::numeric_limits<qint64>::min())
        elem.setAttribute("minimum", QString::number(minval));
        
    if (maxval != std::numeric_limits<qint64>::max())
        elem.setAttribute("maximum", QString::number(maxval));
    
    return elem;
}

qint64 readInt(QString text)
{
    bool ok;
    qint64 val = text.toLongLong(&ok,0);
    
    if (not ok)
        throw SireError::file_error( QObject::tr(
                "Cannot read an integer from the text \"%1\".")
                    .arg(text), CODELOC );

    return val;
}

/** Construct from a QDomElement */
IntegerValue::IntegerValue(QDomElement elem) 
             : Value(), val(0),
                        minval(std::numeric_limits<qint64>::min()),
                        maxval(std::numeric_limits<qint64>::max())
{
    if (elem.tagName() != "integer")
        throw SireError::file_error( QObject::tr(
                "Can only create a IntegerValue object from an <integer>...</integer> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );

    if (not elem.text().isEmpty())
    {
        val = readInt(elem.text());
    }

    if (not elem.attribute("minimum").isEmpty())
    {
        minval = readInt(elem.attribute("minimum"));
    }

    if (not elem.attribute("maximum").isEmpty())
    {
        maxval = readInt(elem.attribute("maximum"));
    }

    if (minval > maxval)
    {
        qSwap(minval, maxval);
    }
    
    if (val < minval)
        val = minval;
        
    if (val > maxval)
        val = maxval;
}

/** Copy constructor */
IntegerValue::IntegerValue(const IntegerValue &other)
          : Value(other), val(other.val), 
            minval(other.minval), maxval(other.maxval)
{}

/** Destructor */
IntegerValue::~IntegerValue()
{}

IntegerValue* IntegerValue::ptr_clone() const
{
    return new IntegerValue(*this);
}

/** Copy assignment operator */
IntegerValue& IntegerValue::operator=(const IntegerValue &other)
{
    if (this != &other)
    {
        val = other.val;
        minval = other.minval;
        maxval = other.maxval;
    }
    
    return *this;
}

/** Comparison operator */
bool IntegerValue::operator==(const IntegerValue &other) const
{
    return val == other.val and minval == other.minval and
           maxval == other.maxval;
}

/** Comparison operator */
bool IntegerValue::operator!=(const IntegerValue &other) const
{
    return not IntegerValue::operator==(other);
}

const char* IntegerValue::typeName()
{
    return "SireSim::IntegerValue";
}

const char* IntegerValue::what() const
{
    return IntegerValue::typeName();
}

/** Return the actual integer value */
qint64 IntegerValue::value() const
{
    return val;
}

/** Get the value. This passed key should be null, as this is a value */
ValuePtr IntegerValue::getValue(QString key) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to get a IntegerValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );

    return self();
}

/** Set the value of this string to 'value'. This passed key should be null,
    and the passed value must have type "IntegerValue" */
ValuePtr IntegerValue::setValue(QString key, const Value &value) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a IntegerValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );
    
    else if (not value.isA<IntegerValue>())
        throw SireError::invalid_arg( QObject::tr(
                "You cannot set the value of a IntegerValue using a value "
                "of type \"%1\".").arg(value.what()), CODELOC );
    
    qint64 newval = value.asA<IntegerValue>().val;
    
    if (newval == val)
        return self();
    else
    {
        IntegerValue ret(*this);
        
        ret.val = newval;
        
        if (ret.val < ret.minval)
            ret.val = ret.minval;
            
        if (ret.val > ret.maxval)
            ret.val = ret.maxval;
        
        return ret;
    }
}

/** Return this value as given in a configure file */
QStringList IntegerValue::toConfigLines(bool) const
{
    QStringList lines;
    
    lines.append( QString::number(val) );
    
    return lines;
}

/** Set the value from the passed line */
ValuePtr IntegerValue::fromConfig(detail::ParsedLine &line) const
{
    if (not line.key.key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a IntegerValue value with "
                "a non-empty key! \"%1\"").arg(line.key.key), CODELOC );

    if (line.been_read)
        return self();
    
    else
    {
        line.been_read = true;
        
        qint64 new_val = readInt(line.value);
        
        if (val == new_val)
            return self();
        else
        {
            IntegerValue ret(*this);
            
            ret.val = new_val;
            
            if (ret.val < minval)
                ret.val = minval;
                
            if (ret.val > maxval)
                ret.val = maxval;
                
            return ret;
        }
    }
}

/////////
///////// Implementation of FloatValue
/////////

/** Constructor */
FloatValue::FloatValue() 
           : Value(), val(0), minval(0), maxval(0),
             has_minval(false), has_maxval(false)
{}

/** Construct, passing in a value */
FloatValue::FloatValue(double value) 
             : Value(), val(value), minval(0), maxval(0),
               has_minval(false), has_maxval(false)
{}

/** Construct an FloatValue with passed value, and specified minimum */
FloatValue FloatValue::valueWithMinimum(double value, double minimum)
{
    FloatValue v;
    v.val = value;
    v.minval = minimum;
    v.has_minval = true;
    
    if (v.val < v.minval)
        v.val = v.minval;
        
    return v;
}

/** Construct an FloatValue with passed value, and specified maximum */
FloatValue FloatValue::valueWithMaximum(double value, double maximum)
{
    FloatValue v;
    v.val = value;
    v.maxval = maximum;
    v.has_maxval = true;
    
    if (v.val > v.maxval)
        v.val = v.maxval;
        
    return v;
}

/** Construct an FloatValue with passed value and specified range */
FloatValue FloatValue::valueWithRange(double value, double minimum, double maximum)
{
    FloatValue v;
    
    v.val = value;
    
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

QDomElement FloatValue::toDomElement(QDomDocument doc) const
{
    QDomElement elem = doc.createElement("float");
    elem.appendChild( doc.createTextNode( QString::number(val) ) );
    
    if (has_minval)
        elem.setAttribute("minimum", QString::number(minval));
        
    if (has_maxval)
        elem.setAttribute("maximum", QString::number(maxval));
    
    return elem;
}

double readFloat(QString text)
{
    bool ok;
    double val = text.toDouble(&ok);
    
    if (not ok)
        throw SireError::file_error( QObject::tr(
                "Cannot read a floating point number from the text \"%1\".")
                    .arg(text), CODELOC );

    return val;
}

/** Construct from a QDomElement */
FloatValue::FloatValue(QDomElement elem) 
           : Value(), val(0), minval(0), maxval(0),
             has_minval(false), has_maxval(false)
{
    if (elem.tagName() != "float")
        throw SireError::file_error( QObject::tr(
                "Can only create a FloatValue object from an <float>...</float> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );

    if (not elem.text().isEmpty())
    {
        val = readFloat(elem.text());
    }

    if (not elem.attribute("minimum").isEmpty())
    {
        minval = readFloat(elem.attribute("minimum"));
        has_minval = true;
    }

    if (not elem.attribute("maximum").isEmpty())
    {
        maxval = readFloat(elem.attribute("maximum"));
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
FloatValue::FloatValue(const FloatValue &other)
           : Value(other), val(other.val), 
             minval(other.minval), maxval(other.maxval),
             has_minval(other.has_minval),
             has_maxval(other.has_maxval)
{}

/** Destructor */
FloatValue::~FloatValue()
{}

FloatValue* FloatValue::ptr_clone() const
{
    return new FloatValue(*this);
}

/** Copy assignment operator */
FloatValue& FloatValue::operator=(const FloatValue &other)
{
    if (this != &other)
    {
        val = other.val;
        minval = other.minval;
        maxval = other.maxval;
        has_minval = other.has_minval;
        has_maxval = other.has_maxval;
    }
    
    return *this;
}

/** Comparison operator */
bool FloatValue::operator==(const FloatValue &other) const
{
    return val == other.val and minval == other.minval and
           maxval == other.maxval and has_minval == other.has_minval and
           has_maxval == other.has_maxval;
}

/** Comparison operator */
bool FloatValue::operator!=(const FloatValue &other) const
{
    return not FloatValue::operator==(other);
}

const char* FloatValue::typeName()
{
    return "SireSim::FloatValue";
}

const char* FloatValue::what() const
{
    return FloatValue::typeName();
}

/** Return the actual integer value */
double FloatValue::value() const
{
    return val;
}

/** Get the value. This passed key should be null, as this is a value */
ValuePtr FloatValue::getValue(QString key) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to get a FloatValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );

    return self();
}

/** Set the value of this string to 'value'. This passed key should be null,
    and the passed value must have type "FloatValue" */
ValuePtr FloatValue::setValue(QString key, const Value &value) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a FloatValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );
    
    else if (not value.isA<FloatValue>())
        throw SireError::invalid_arg( QObject::tr(
                "You cannot set the value of a FloatValue using a value "
                "of type \"%1\".").arg(value.what()), CODELOC );
    
    double newval = value.asA<FloatValue>().val;
    
    if (newval == val)
        return self();
    else
    {
        FloatValue ret(*this);
        
        ret.val = newval;
        
        if (has_minval and ret.val < ret.minval)
            ret.val = ret.minval;
            
        if (has_maxval and ret.val > ret.maxval)
            ret.val = ret.maxval;
        
        return ret;
    }
}

/** Return this value as given in a configure file */
QStringList FloatValue::toConfigLines(bool) const
{
    QStringList lines;
    
    lines.append( QString::number(val,'g',12) );
    
    return lines;
}

/** Set the value from the passed line */
ValuePtr FloatValue::fromConfig(detail::ParsedLine &line) const
{
    if (not line.key.key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a FloatValue value with "
                "a non-empty key! \"%1\"").arg(line.key.key), CODELOC );

    if (line.been_read)
        return self();
    
    else
    {
        line.been_read = true;
        
        double new_val = readInt(line.value);
        
        if (val == new_val)
            return self();
        else
        {
            FloatValue ret(*this);
            
            ret.val = new_val;
            
            if (has_minval and ret.val < minval)
                ret.val = minval;
                
            if (has_maxval and ret.val > maxval)
                ret.val = maxval;
                
            return ret;
        }
    }
}

/////////
///////// Implementation of BoolValue
/////////

/** Constructor */
BoolValue::BoolValue() : Value(), val(false)
{}

/** Construct, passing in a value */
BoolValue::BoolValue(bool value) : Value(), val(value)
{}

QDomElement BoolValue::toDomElement(QDomDocument doc) const
{
    QDomElement elem = doc.createElement("bool");
    
    if (val)
        elem.appendChild( doc.createTextNode("true") );
    else
        elem.appendChild( doc.createTextNode("false") );

    return elem;
}

bool getBool(QString text)
{
    text = text.trimmed().toLower();
    
    if (text == "true" or text == "yes" or text == "on" or text == "1")
    {
        return true;
    }
    else if (text == "false" or text == "no" or text == "off" or text == "0")
    {
        return false;
    }
    else
        throw SireError::file_error( QObject::tr(
                "Could not get a true/false value from the string \"%1\".")
                    .arg(text), CODELOC );
                    
    return false;
}

/** Construct from a QDomElement */
BoolValue::BoolValue(QDomElement elem) : Value()
{
    if (elem.tagName() != "bool")
        throw SireError::file_error( QObject::tr(
                "Can only create a BoolValue object from an <bool>...</bool> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
                    
    val = getBool(elem.text());
}

/** Copy constructor */
BoolValue::BoolValue(const BoolValue &other)
          : Value(other), val(other.val)
{}

/** Destructor */
BoolValue::~BoolValue()
{}

BoolValue* BoolValue::ptr_clone() const
{
    return new BoolValue(*this);
}

/** Copy assignment operator */
BoolValue& BoolValue::operator=(const BoolValue &other)
{
    val = other.val;
    return *this;
}

/** Comparison operator */
bool BoolValue::operator==(const BoolValue &other) const
{
    return val == other.val;
}

/** Comparison operator */
bool BoolValue::operator!=(const BoolValue &other) const
{
    return not BoolValue::operator==(other);
}

const char* BoolValue::typeName()
{
    return "SireSim::BoolValue";
}

const char* BoolValue::what() const
{
    return BoolValue::typeName();
}

/** Return the actual string value */
bool BoolValue::value() const
{
    return val;
}

/** Get the value. This passed key should be null, as this is a value */
ValuePtr BoolValue::getValue(QString key) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to get a BoolValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );

    return self();
}

/** Set the value of this string to 'value'. This passed key should be null,
    and the passed value must have type "BoolValue" */
ValuePtr BoolValue::setValue(QString key, const Value &value) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a BoolValue value with "
                "a non-empty key! \"%1\"").arg(key), CODELOC );
    
    else if (not value.isA<BoolValue>())
        throw SireError::invalid_arg( QObject::tr(
                "You cannot set the value of a BoolValue using a value "
                "of type \"%1\".").arg(value.what()), CODELOC );
    
    return value;
}

/** Return this value as given in a configure file */
QStringList BoolValue::toConfigLines(bool) const
{
    QStringList lines;
    
    if (val)
        lines.append("true");
    else
        lines.append("false");
    
    return lines;
}

/** Set the value from the passed line */
ValuePtr BoolValue::fromConfig(detail::ParsedLine &line) const
{
    if (not line.key.key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a BoolValue value with "
                "a non-empty key! \"%1\"").arg(line.key.key), CODELOC );

    if (line.been_read)
        return self();
    
    else
    {
        line.been_read = true;
        bool new_val = getBool(line.value);
        
        if (val == new_val)
            return self();
        else
            return BoolValue(new_val);
    }
}
