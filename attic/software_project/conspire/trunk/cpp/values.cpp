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

#include "Conspire/values.h"
#include "Conspire/dimensions.hpp"
#include "Conspire/conspire.hpp"

#include "Conspire/exceptions.h"

using namespace Conspire;
using namespace Conspire::Qt;
using namespace Conspire::detail;

using namespace Siren;

/////////
///////// Implementation of StringValue
/////////

REGISTER_SIREN_CLASS(Conspire::StringValue)

/** Constructor */
StringValue::StringValue() : Value()
{}

/** Construct, passing in a value */
StringValue::StringValue(String value) : Value(), val(value)
{}

DomElement StringValue::toDomElement(DomDocument doc) const
{
    DomElement elem = doc.createElement("string");
    elem.appendChild( doc.createTextNode(val) );
    return elem;
}

/** Construct from a QDomElement */
StringValue::StringValue(DomElement elem) : Value()
{
    if (elem.tagName() != "string")
        throw Conspire::parse_error( Conspire::tr(
                "Can only create a StringValue object from an <string>...</string> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
    
    this->operator=( StringValue(elem.text()) );
}

/** Copy constructor */
StringValue::StringValue(const StringValue &other)
            : Value(other), val(other.val)
{}

/** Destructor */
StringValue::~StringValue()
{}

/** Copy assignment operator */
void StringValue::copy_object(const StringValue &other)
{
    val = other.val;
    super::copy_object(other);
}

/** Comparison operator */
bool StringValue::compare_object(const StringValue &other) const
{
    return val == other.val and super::compare_object(other);
}

/** Return the actual string value */
String StringValue::value() const
{
    return val;
}

/** Set the value of this string to 'value' */
Obj StringValue::fromValueString(String value) const
{
    if (val == value)
        return *this;
    else
        return StringValue(value);
}

/** Return this value as a string */
String StringValue::toValueString() const
{
    return val;
}

/////////
///////// Implementation of DirValue
/////////

REGISTER_SIREN_CLASS(Conspire::DirValue)

/** Constructor */
DirValue::DirValue() : Value()
{}

/** Construct, passing in a value */
DirValue::DirValue(String dir) : Value(), val(dir)
{
    //should validate that this looks like a directory...
}

DomElement DirValue::toDomElement(DomDocument doc) const
{
    DomElement elem = doc.createElement("directory");
    elem.appendChild( doc.createTextNode(val) );
    return elem;
}

/** Construct from a QDomElement */
DirValue::DirValue(DomElement elem) : Value()
{
    if (elem.tagName() != "directory")
        throw Conspire::parse_error( Conspire::tr(
                "Can only create a DirValue object from an <directory>...</directory> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
                    
    this->operator=( DirValue(elem.text()) );
}

/** Copy constructor */
DirValue::DirValue(const DirValue &other)
         : Value(other), val(other.val)
{}

/** Destructor */
DirValue::~DirValue()
{}

/** Copy assignment operator */
void DirValue::copy_object(const DirValue &other)
{
    val = other.val;
    super::copy_object(other);
}

/** Comparison operator */
bool DirValue::compare_object(const DirValue &other) const
{
    return val == other.val and super::compare_object(other);
}

/** Return the actual string value */
String DirValue::value() const
{
    return val;
}

/** Return this directory encoded as a string */
String DirValue::toValueString() const
{
    return val;
}

/** Return this value as read from the passed string */
Obj DirValue::fromValueString(String value) const
{
    if (val == value)
        return *this;
    else
        return DirValue(value);
}

/////////
///////// Implementation of FileValue
/////////

REGISTER_SIREN_CLASS(Conspire::FileValue)

/** Constructor */
FileValue::FileValue() : Value()
{}

/** Construct, passing in a value */
FileValue::FileValue(String file) : Value(), val(file)
{
    //should validate that this looks like a file...
}

DomElement FileValue::toDomElement(DomDocument doc) const
{
    DomElement elem = doc.createElement("file");
    elem.appendChild( doc.createTextNode(val) );
    return elem;
}

/** Construct from a QDomElement */
FileValue::FileValue(DomElement elem) : Value()
{
    if (elem.tagName() != "file")
        throw Conspire::parse_error( Conspire::tr(
                "Can only create a FileValue object from an <file>...</file> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
                    
    this->operator=( FileValue(elem.text()) );
}

/** Copy constructor */
FileValue::FileValue(const FileValue &other)
          : Value(other), val(other.val)
{}

/** Destructor */
FileValue::~FileValue()
{}

/** Copy assignment operator */
void FileValue::copy_object(const FileValue &other)
{
    val = other.val;
    super::copy_object(other);
}

/** Comparison operator */
bool FileValue::compare_object(const FileValue &other) const
{
    return val == other.val and super::compare_object(other);
}

/** Return the actual string value */
String FileValue::value() const
{
    return val;
}

String FileValue::toValueString() const
{
    return val;
}

Obj FileValue::fromValueString(String value) const
{
    if (val == value)
        return *this;
    else
        return FileValue(value);
}

/////////
///////// Implementation of IntegerValue
/////////

REGISTER_SIREN_CLASS(Conspire::IntegerValue)

/** Constructor */
IntegerValue::IntegerValue() 
             : Value(), val(0), minval( std::numeric_limits<Conspire::Qt::int64>::min() ),
                                maxval( std::numeric_limits<Conspire::Qt::int64>::max() )
{}

/** Construct, passing in a value */
IntegerValue::IntegerValue(Conspire::Qt::int64 value) 
             : Value(), val(value), minval( std::numeric_limits<Conspire::Qt::int64>::min() ),
                                    maxval( std::numeric_limits<Conspire::Qt::int64>::max() )
{}

/** Construct an IntegerValue with passed value, and specified minimum */
IntegerValue IntegerValue::valueWithMinimum(Conspire::Qt::int64 value, Conspire::Qt::int64 minimum)
{
    IntegerValue v;
    v.val = value;
    v.minval = minimum;
    
    if (v.val < v.minval)
        v.val = v.minval;
        
    return v;
}

/** Construct an IntegerValue with passed value, and specified maximum */
IntegerValue IntegerValue::valueWithMaximum(Conspire::Qt::int64 value, Conspire::Qt::int64 maximum)
{
    IntegerValue v;
    v.val = value;
    v.maxval = maximum;
    
    if (v.val > v.maxval)
        v.val = v.maxval;
        
    return v;
}

/** Construct an IntegerValue with passed value and specified range */
IntegerValue IntegerValue::valueWithRange(Conspire::Qt::int64 value, Conspire::Qt::int64 minimum, 
                                          Conspire::Qt::int64 maximum)
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

DomElement IntegerValue::toDomElement(DomDocument doc) const
{
    DomElement elem = doc.createElement("integer");
    elem.appendChild( doc.createTextNode( String::number(val) ) );
    
    if (minval != std::numeric_limits<Conspire::Qt::int64>::min())
        elem.setAttribute("minimum", String::number(minval));
        
    if (maxval != std::numeric_limits<Conspire::Qt::int64>::max())
        elem.setAttribute("maximum", String::number(maxval));
    
    return elem;
}

/** Construct from a QDomElement */
IntegerValue::IntegerValue(DomElement elem) 
             : Value(), val(0),
                        minval(std::numeric_limits<Conspire::Qt::int64>::min()),
                        maxval(std::numeric_limits<Conspire::Qt::int64>::max())
{
    if (elem.tagName() != "integer")
        throw Conspire::parse_error( Conspire::tr(
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

/** Construct from a string */
IntegerValue::IntegerValue(QString value)
             : Value(), val(readInt(value)),
                        minval(std::numeric_limits<Conspire::Qt::int64>::min()),
                        maxval(std::numeric_limits<Conspire::Qt::int64>::max())
{}             

/** Copy constructor */
IntegerValue::IntegerValue(const IntegerValue &other)
          : Value(other), val(other.val), 
            minval(other.minval), maxval(other.maxval)
{}

/** Destructor */
IntegerValue::~IntegerValue()
{}

/** Copy assignment operator */
void IntegerValue::copy_object(const IntegerValue &other)
{
    if (this != &other)
    {
        val = other.val;
        minval = other.minval;
        maxval = other.maxval;
        super::copy_object(other);
    }
}

/** Comparison operator */
bool IntegerValue::compare_object(const IntegerValue &other) const
{
    return val == other.val and minval == other.minval and
           maxval == other.maxval and super::compare_object(other);
}

/** Return the actual integer value */
Conspire::Qt::int64 IntegerValue::value() const
{
    return val;
}

String IntegerValue::toValueString() const
{
    return String::number(val);
}

Obj IntegerValue::fromValueString(String value) const
{
    Conspire::Qt::int64 v = readInt(value);
    
    if (v > maxval)
        v = maxval;
    
    if (v < minval)
        v = minval;
    
    if (val == v)
        return *this;
    else
    {
        IntegerValue ret(*this);
        ret.val = v;
        return ret;
    }
}

/////////
///////// Implementation of FloatValue
/////////

REGISTER_SIREN_CLASS(Conspire::FloatValue)

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

DomElement FloatValue::toDomElement(DomDocument doc) const
{
    DomElement elem = doc.createElement("float");
    elem.appendChild( doc.createTextNode( String::number(val) ) );
    
    if (has_minval)
        elem.setAttribute("minimum", String::number(minval));
        
    if (has_maxval)
        elem.setAttribute("maximum", String::number(maxval));
    
    return elem;
}

FloatValue::FloatValue(String value)
           : Value(), val(readFloat(value)), minval(0), maxval(0),
             has_minval(false), has_maxval(false)
{}

/** Construct from a QDomElement */
FloatValue::FloatValue(DomElement elem) 
           : Value(), val(0), minval(0), maxval(0),
             has_minval(false), has_maxval(false)
{
    if (elem.tagName() != "float")
        throw Conspire::parse_error( Conspire::tr(
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

/** Copy assignment operator */
void FloatValue::copy_object(const FloatValue &other)
{
    if (this != &other)
    {
        val = other.val;
        minval = other.minval;
        maxval = other.maxval;
        has_minval = other.has_minval;
        has_maxval = other.has_maxval;
        super::copy_object(other);
    }
}

/** Comparison operator */
bool FloatValue::compare_object(const FloatValue &other) const
{
    return val == other.val and minval == other.minval and
           maxval == other.maxval and has_minval == other.has_minval and
           has_maxval == other.has_maxval and super::compare_object(other);
}

/** Return the actual integer value */
double FloatValue::value() const
{
    return val;
}

String FloatValue::toValueString() const
{
    return String::number(val, 'g', 12);
}

Obj FloatValue::fromValueString(String value) const
{
    double v = readFloat(value);
    
    if (has_minval and v < minval)
        v = minval;
        
    if (has_maxval and v > maxval)
        v = maxval;
        
    if (v == val)
        return *this;
    else
    {
        FloatValue ret(*this);
        ret.val = v;
        return ret;
    }
}

/////////
///////// Implementation of BoolValue
/////////

REGISTER_SIREN_CLASS(Conspire::BoolValue)

/** Constructor */
BoolValue::BoolValue() : Value(), val(false)
{}

/** Construct, passing in a value */
BoolValue::BoolValue(bool value) : Value(), val(value)
{}

DomElement BoolValue::toDomElement(DomDocument doc) const
{
    DomElement elem = doc.createElement("bool");
    
    if (val)
        elem.appendChild( doc.createTextNode("true") );
    else
        elem.appendChild( doc.createTextNode("false") );

    return elem;
}

BoolValue::BoolValue(String value) : Value(), val(readBool(value))
{}

/** Construct from a QDomElement */
BoolValue::BoolValue(DomElement elem) : Value()
{
    if (elem.tagName() != "bool")
        throw Conspire::parse_error( Conspire::tr(
                "Can only create a BoolValue object from an <bool>...</bool> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
                    
    val = readBool(elem.text());
}

/** Copy constructor */
BoolValue::BoolValue(const BoolValue &other)
          : Value(other), val(other.val)
{}

/** Destructor */
BoolValue::~BoolValue()
{}

/** Copy assignment operator */
void BoolValue::copy_object(const BoolValue &other)
{
    val = other.val;
    super::copy_object(other);
}

/** Comparison operator */
bool BoolValue::compare_object(const BoolValue &other) const
{
    return val == other.val and super::compare_object(other);
}

/** Return the actual string value */
bool BoolValue::value() const
{
    return val;
}

String BoolValue::toValueString() const
{
    static String true_string("true");
    static String false_string("false");

    if (val)
        return true_string;
    else
        return false_string;
}

Obj BoolValue::fromValueString(String value) const
{
    bool v = readBool(value);
    
    if (v == val)
        return *this;
    else
        return BoolValue(v);
}

/////////
///////// Implementation of EnumValue
/////////

REGISTER_SIREN_CLASS(Conspire::EnumValue)

/** Constructor */
EnumValue::EnumValue() : Value(), idx(0)
{}

void removeDuplicates(StringList &options)
{
    Set<String>::Type seen_strings;
    seen_strings.reserve(options.count());
    
    List<String>::MutableIterator it(options);
    
    while (it.hasNext())
    {
        //toLower as enum matching is case-insensitive
        String s = it.next().trimmed().toLower();
        
        if (s.isEmpty() or seen_strings.contains(s))
            it.remove();
        else
            seen_strings.insert(s);
    }
}

void removeDuplicates(StringList &options, StringList &docs)
{
    if (docs.count() != options.count())
        throw Conspire::invalid_arg( Conspire::tr(
            "The list of options must be attached to a list of documentation "
            "that has the same length. %1 vs. %2.")
                .arg(options.count()).arg(docs.count()), CODELOC );

    Set<String>::Type seen_strings;
    seen_strings.reserve(options.count());
    
    List<String>::MutableIterator it(options);
    List<String>::MutableIterator it2(docs);
    
    while (it.hasNext())
    {
        //toLower as enum matching is case-insensitive
        String s = it.next().trimmed().toLower();
        it2.next();
        
        if (s.isEmpty() or seen_strings.contains(s))
        {
            it.remove();
            it2.remove();
        }
        else
            seen_strings.insert(s);
    }
}

void cleanUp(StringList &options)
{
    List<String>::MutableIterator it(options);
    
    while (it.hasNext())
    {
        String &s = it.next();
        s = s.trimmed().toLower();
    }
}

/** Construct an enum from the list of passed options. By default, 
    the first option in the list is selected */
EnumValue::EnumValue(StringList options)
          : Value(), opts(options), idx(0)
{
    removeDuplicates(opts);
    cleanUp(opts);
}

/** Construct an enum from the list of passed options, with the 
    documentation for each option in the attached 'docs' list. 
    By default, the first option in the list is selected */
EnumValue::EnumValue(StringList options, StringList documentation)
          : Value(), opts(options), docs(documentation), idx(0)
{
    removeDuplicates(opts, docs);
    cleanUp(opts);
}

/** Construct an enum from the list of passed options, with the
    option at index 'index' initially selected */
EnumValue::EnumValue(StringList options, int index)
          : Value(), opts(options)
{
    if (index < 0 or index >= opts.count())
        throw Conspire::invalid_index( Conspire::tr(
            "Cannot select enum option %1 as the number of options is %2.")
                .arg(index).arg(opts.count()), CODELOC );
    
    String s = opts.at(index);
    
    removeDuplicates(opts);
    
    idx = opts.indexOf(s);
    
    if (idx == -1)
        idx = 0;

    cleanUp(opts);
}

/** Construct an enum from the list of passed options and associated
    documentation. The option at index 'index' is initially selected */
EnumValue::EnumValue(StringList options, StringList documents, int index)
          : Value(), opts(options), docs(documents)
{
    if (index < 0 or index >= opts.count())
        throw Conspire::invalid_index( Conspire::tr(
            "Cannot select enum option %1 as the number of options is %2.")
                .arg(index).arg(opts.count()), CODELOC );
    
    String s = opts.at(index);
    
    removeDuplicates(opts,docs);
    
    idx = opts.indexOf(s);
    
    if (idx == -1)
        idx = 0;

    cleanUp(opts);
}

/** Construct an enum with just a single value */
EnumValue::EnumValue(String value) : Value(), idx(0)
{
    opts.append(value);
    cleanUp(opts);
    
    if (opts.at(0).isEmpty())
        opts = QStringList();
}

/** Construct an enum with just a single value, with accompanying documentation */
EnumValue::EnumValue(String value, String doc)
          : Value(), idx(0)
{
    opts.append(value);
    cleanUp(opts);
    
    if (opts.at(0).isEmpty())
    {
        opts = StringList();
    }
    else
    {
        docs.append(doc);
    }
}

/** Convert this enum into XML */
DomElement EnumValue::toDomElement(DomDocument doc) const
{
    DomElement elem = doc.createElement("enum");
    
    if (opts.count() == docs.count())
    {
        for (int i=0; i<opts.count(); ++i)
        {
            DomElement item = doc.createElement("item");
            item.setAttribute("name",opts[i]);
            
            DomElement documentation = doc.createElement("description");
            documentation.appendChild( doc.createTextNode(docs[i]) );
            
            item.appendChild(documentation);
            elem.appendChild(item);
        }
    }
    else
    {
        for (int i=0; i<opts.count(); ++i)
        {
            DomElement item = doc.createElement("item");
            item.setAttribute("name",opts[i]);
            elem.appendChild(item);
        }
    }

    if (not opts.isEmpty())
    {
        DomElement val = doc.createElement("value");
        val.appendChild( doc.createTextNode(opts[idx]) );
        elem.appendChild(val);
    }

    return elem;
}

/** Construct from a QDomElement */
EnumValue::EnumValue(DomElement elem) : Value(), idx(0)
{
    if (elem.tagName() != "enum")
        throw Conspire::parse_error( Conspire::tr(
                "Can only create a EnumValue object from an <enum>...</enum> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
                    
    //get the items...
    DomNode n = elem.firstChild();
    
    String val;
    
    while(not n.isNull()) 
    {
        elem = n.toElement(); // try to convert the node to an element.
        
        if (not elem.isNull()) 
        {
            if (elem.tagName() == "item")
            {
                String name = elem.attribute("name", String::null);
                String doc;
                
                DomNode n2 = elem.firstChild();
                
                while (not n2.isNull())
                {
                    elem = n2.toElement();
                    
                    if (not elem.isNull())
                    {
                        if (elem.tagName() == "description")
                        {
                            doc = elem.text();
                        }
                        else
                            sirenDebug() << "<enum><item>... cannot recognise"
                                         << elem.tagName() << "...</item></enum>"; 
                    }
                    
                    n2 = n2.nextSibling();
                }
                
                opts.append(name);
                docs.append(doc);
            }
            else if (elem.tagName() == "value")
            {
                val = elem.text().trimmed();
            }
        }
        
        n = n.nextSibling();
    }
    
    removeDuplicates(opts, docs);
    
    idx = opts.indexOf(val);
    
    if (idx == -1)
        idx = 0;
        
    cleanUp(opts);
    
    if (opts.isEmpty())
    {
        opts = StringList();
        docs = StringList();
    }
}

/** Copy constructor */
EnumValue::EnumValue(const EnumValue &other)
          : Value(other), opts(other.opts), docs(other.docs), idx(other.idx)
{}

/** Destructor */
EnumValue::~EnumValue()
{}

/** Copy assignment operator */
void EnumValue::copy_object(const EnumValue &other)
{
    if (this != &other)
    {
        opts = other.opts;
        docs = other.docs;
        idx = other.idx;
        super::copy_object(other);
    }
}

/** Comparison operator */
bool EnumValue::compare_object(const EnumValue &other) const
{
    return opts == other.opts and docs == other.docs and idx == other.idx 
               and super::compare_object(other);
}

/** Return the set of possible options */
StringList EnumValue::options() const
{
    return opts;
}

/** Return the documentation associated with each option.
    If this is empty, then no documentation is associated with any option */
StringList EnumValue::documentation() const
{
    return docs;
}

/** Return the actual string value */
String EnumValue::value() const
{
    if (opts.isEmpty())
        return String::null;
    else
        return opts[idx];
}

String EnumValue::toValueString() const
{
    if (opts.isEmpty())
        return String::null;
    else
        return opts[idx];
}

Obj EnumValue::fromValueString(String value) const
{
    int index = opts.indexOf(value.trimmed().toLower());

    if (index == -1)
        index = 0;

    if (index == idx)
        return *this;
    else
    {
        EnumValue ret(*this);
        ret.idx = index;
        return ret;
    }
}
