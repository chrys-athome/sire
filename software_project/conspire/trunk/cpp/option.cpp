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

#include "Conspire/option.h"
#include "Conspire/conspire.hpp"

#include "Conspire/exceptions.h"

using namespace Conspire;
using namespace Conspire::Qt;
using namespace Conspire::detail;

///////////
/////////// Implementation of Option
///////////

REGISTER_SIREN_CLASS(Conspire::Option)

/** Constructor */
Option::Option() : Value(), default_idx(1)
{}

/** Construct an option with the specified key, user-readable description,
    default (suggested) value, and flags to say whether or not multiple
    values of this option are supported, and whether or not this option is
    optional */
Option::Option(const String &key, const String &description,
               const Value &value,
               bool is_opt, bool allow_mult)
       : Value(), k(key.trimmed().toLower()), desc(description), val(value.clone()),
         default_idx(1), is_optional(is_opt), allow_multiple(allow_mult)
{
    //the key may not contain any spaces or square brackets
    if (key.indexOf(' ') != -1 or key.indexOf('[') != -1 or key.indexOf(']') != -1)
    {
        throw Conspire::invalid_arg( Conspire::tr(
                "You cannot set a key (%1) that contains spaces or square brakets.")
                    .arg(k), CODELOC );
    }
    
    if (not is_optional)
    {
        //copy the default value into the value for the first index
        user_vals.append(val);
    }
}

bool getBool(DomElement elem, String option, bool default_val)
{
    String str = elem.attribute(option).trimmed().toLower();
    
    if (str.isEmpty())
        return default_val;
        
    if (str == "true")
        return true;
    else if (str == "false")
        return false;
    else
        throw Conspire::parse_error( Conspire::tr(
                "Cannot interpret the boolean option \"%1\" from XML element "
                "\"%2\". The option should read 'true' or 'false'.")
                    .arg(option, elem.tagName()), CODELOC );
                    
    return false;
}

DomElement Option::toDomElement(DomDocument doc) const
{
    assertNotNull();
    
    DomElement elem = doc.createElement("option");
    
    if (is_optional)
        elem.setAttribute("optional", "true");
    
    if (allow_multiple)
        elem.setAttribute("allow_multiple", "true");
    
    // describe the key
    {
        DomElement key_node = doc.createElement("key");
        key_node.appendChild( doc.createTextNode(key()) );
        elem.appendChild(key_node);
    }
    
    //describe the description
    {
        DomElement desc_node = doc.createElement("description");
        desc_node.appendChild( doc.createTextNode(description()) );
        elem.appendChild(desc_node);
    }
    
    //now describe the value
    {
        DomElement value_node = doc.createElement("value");
        value_node.appendChild( val.asA<Value>().toDomElement(doc) );
        elem.appendChild(value_node);
    }
    
    return elem;
}
  
/** Construct from a QDomElement */
Option::Option(DomElement elem, StringList path) 
       : Value(), default_idx(1), is_optional(false), allow_multiple(false)
{
    if (elem.tagName() != "option")
        throw Conspire::parse_error( Conspire::tr(
                "Can only create an Option object from an <option>...</option> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
    
    is_optional = getBool(elem, "optional", false);
    allow_multiple = getBool(elem, "allow_multiple", false);
                                                    
    DomNode n = elem.firstChild();
    
    while (not n.isNull()) 
    {
        elem = n.toElement(); // try to convert the node to an element.
        
        if (not elem.isNull()) 
        {
            if (elem.tagName() == "key")
            {
                //get the key
                k = elem.text();
            }
            else if (elem.tagName() == "description")
            {
                //get the description of the option
                desc = elem.text();
            }
            else if (elem.tagName() == "value")
            {
                //now read the value
                DomNode n1 = elem.firstChild();
                
                if (not n.isNull())
                {
                    elem = n1.toElement();
                    
                    if (not elem.isNull())
                    {
                        if (elem.tagName() == "options")
                        {
                            val = Options(elem,path);
                        }
                        else if (elem.tagName() == "option")
                        {
                            val = Option(elem,path);
                        }
                        else if (elem.tagName() == "include")
                        {
                            //read the options from the include file
                            val = Options::fromXMLFile(elem.attribute("file"),path);
                        }
                        else
                        {
                            val = Value::createFrom(elem);
                        }
                    }
                }
            }
            else
            {
                conspireDebug() << "Option(DomElement elem) cannot recognise tag" 
                                << elem.tagName();
            }
        }
        
        n = n.nextSibling();
    }
    
    if (not is_optional)
    {
        //copy the default value to the first user value
        user_vals.append(val);
    }
}
  
/** Copy constructor */
Option::Option(const Option &other) 
       : Value(other), k(other.k), desc(other.desc), 
         val(other.val), user_vals(other.user_vals),
         default_idx(other.default_idx),
         is_optional(other.is_optional), allow_multiple(other.allow_multiple)
{}

/** Destructor */
Option::~Option()
{}

/** Copy assignment operator */
void Option::copy_object(const Option &other)
{
    if (this != &other)
    {
        k = other.k;
        desc = other.desc;
        val = other.val;
        user_vals = other.user_vals;
        default_idx = other.default_idx;
        is_optional = other.is_optional;
        allow_multiple = other.allow_multiple;
        super::copy_object(other);
    }
}

/** Comparison operator */
bool Option::compare_object(const Option &other) const
{
    return this == &other or
           (k == other.k and desc == other.desc and val == other.val and
            user_vals == other.user_vals and
            default_idx == other.default_idx and
            is_optional == other.is_optional and
            allow_multiple == other.allow_multiple and
            super::compare_object(other));
}

bool Option::isContainer() const
{
    return true;
}

void Option::assertNotNull() const
{
    if (val.isNone())
    {
        throw Conspire::invalid_state( Conspire::tr(
                "You can not perform any operations on a null Option object!"),
                    CODELOC );
    }
}

bool Option::isNull() const
{
    return val.isNone();
}

namespace Conspire
{
    namespace detail
    {
        ParsedKey::ParsedKey()
        {
            key = String::null;
            tail = String::null;
            index = 1;
        }
        
        ParsedKey::ParsedKey(String k)
        {
            k = k.trimmed().toLower();
    
            int idx = k.indexOf(".");
    
            if (idx == -1)
            {
                key = k;
                tail = String::null;
            }
            else
            {   
                key = k.mid(0,idx).trimmed();
                tail = k.mid(idx+1).trimmed();
            }
    
            //now get the index of the key
            idx = key.indexOf("[");
    
            if (idx == -1)
            {
                //no index to the key
                index = 1;
            }
            else
            {
                int idx2 = key.indexOf("]");
        
                if (idx2 == -1 or idx2 < idx)
                    throw Conspire::invalid_key( Conspire::tr(
                        "Could not get the index of the key \"%1\". The index "
                        "should be an integer in square brackets, e.g. key[1].")
                            .arg(key), CODELOC );
    
                bool ok;
                String idx_string = key.mid(idx+1, idx2-idx-1);
                index = idx_string.toInt(&ok);
                
                if (not ok)
                    throw Conspire::invalid_key( Conspire::tr(
                            "Could not get the index integer from the index "
                            "string \"%1\" read from the key \"%2\". The index "
                            "should be an integer in square brackets, e.g. key[1].")
                                .arg(idx_string, key), CODELOC );
                                
                key = key.mid(0,idx).trimmed();
            }
        }
            
        String ParsedKey::unparse() const
        {
            if (index == 1)
            {
                if (tail.isEmpty())
                    return key;
                else
                    return String("%1.%2").arg(key, tail);
            }
            else
            {
                if (tail.isEmpty())
                    return String("%1[%2]").arg(key).arg(index);
                else
                    return String("%1[%2].%3").arg(key).arg(index).arg(tail);
            }
        }

        ParsedLine::ParsedLine()
        {
            key = ParsedKey();
            value = String::null;
            been_read = true;
        }
        
        ParsedLine::ParsedLine(const ParsedKey &k, const String &v)
        {
            key = k;
            value = v;
            been_read = false;
        }

        String ParsedLine::unparse() const
        {
            return String("%1 = %2").arg(key.unparse(),value);
        }

        String ParsedLine::cleanLine(String line)
        {
            int idx = line.indexOf('#');

            if (idx != -1)
            {
                //what about if the "#" sign is in a quoted string?
                if (idx == 0)
                    return String::null;
                    
                while (line[idx-1] == '\\')
                {
                    idx = line.indexOf('#', idx+1);
                }
                
                return line.mid(0,idx).trimmed();
            }
            else
            {
                return line.trimmed();
            }
        }

        ParsedLine ParsedLine::parseLine(String line)
        {
            int idx = line.indexOf('=');
            
            if (idx == -1)
                throw Conspire::invalid_state( Conspire::tr(
                    "Could not parse the config line \"%1\". It should be of the "
                    "form \"key = value\"").arg(line), CODELOC );
                        
            return ParsedLine( ParsedKey(line.mid(0,idx-1).trimmed().toLower()),
                               line.mid(idx+1).trimmed() );
        }

        List<ParsedLine>::Type ParsedLine::parse(String config)
        {
            StringList lines = config.split("\n", String::SkipEmptyParts);
        
            List<ParsedLine>::Type parsed_lines;
            String parse_line;
        
            foreach (String line, lines)
            {
                parse_line += cleanLine(line);
            
                if (not parse_line.endsWith("\\"))
                {
                    if (not parse_line.isEmpty())
                    {
                        parsed_lines.append( parseLine(parse_line) );
                        parse_line = String::null;
                    }
                }
            }
        
            if (not parse_line.isEmpty())
            {
                parsed_lines.append( parseLine(parse_line) );
            }
            
            return parsed_lines;
        }
    }
}

/** Return whether or this option has a value. This uses the 
    default index of the option. You set this using the operator[](int) 
    operator, e.g. option[1].hasUserValue(), or option[2].hasUserValue() etc. */
bool Option::hasValue() const
{
    assertNotNull();

    if (not is_optional)
        return true;
    else if (user_vals.count() >= default_idx)
        return not user_vals.at(default_idx-1).isNone();
    else
        return false;
}

/** Return whether or not this option has a value for the suboption at key 'key' */
bool Option::hasValue(String key) const
{
    assertNotNull();
    
    Obj val = this->value();
    
    if (not val.isNone())
    {
        if (val.isA<Options>())
            return val.asA<Options>().hasValue(key);
        
        else if (val.isA<Option>())
            return val.asA<Option>().hasValue() and
                   val.asA<Option>().key() == key;
        else
            return false;
    }
    else
        return false;
}

/** Return whether or not this option has any suboptions */
bool Option::hasSubOptions() const
{
    assertNotNull();
    
    Obj val = this->value();
    
    if (not val.isNone())
    {
        return val.isA<Option>() or val.isA<Options>();
    }
    else
        return false;
}

/** Return whether or not any index of this option has a value */
bool Option::hasIndiciesWithValue() const
{
    assertNotNull();
    
    if (is_optional)
    {
        for (int i=0; i<user_vals.count(); ++i)
        {
            if (not user_vals.at(i).isNone())
                return true;
        }
        
        return false;
    }
    else
        return true;
}

/** Return the default value of the option */
Obj Option::defaultValue() const
{
    assertNotNull();
    return val;
}

/** Return the index of this option - this is the index that is set 
    for the option, and the index of the value returned by the
    "value()" function */
int Option::index() const
{
    return default_idx;
}

/** Return the value of this option. If no value has been set, then
    a null pointer will be returned if is_optional is true, or the 
    default value will be returned. Note that this will only look 
    at the value for the default index of this option, e.g. if we
    have set the value for index 1, then option[1].value() would be that value,
    while option[2].value() would be none, or the default value */
Obj Option::value() const
{
    assertNotNull();
    
    if (default_idx > 0 and user_vals.count() >= default_idx)
    {
        if (not user_vals.at(default_idx-1).isNone())
            return user_vals.at(default_idx-1);
    }
                    
    if (is_optional)
        return None();
    else
        return val;
}

/** Return the option with the passed key. Note that the first element
    of the key must equal the key of this option, or it must be empty.
    If it is empty, then this option is returned. If the first element
    does not match this option's key, then an invalid_key exception is raised */
Option Option::getNestedOption(String key) const
{
    assertNotNull();
    
    ParsedKey p(key);
    
    if (not p.key.isEmpty())
    {
        if (p.key != k)
        {
            //maybe we are looking for one of our subkeys?
            Obj v = value();
            
            if (v.isNone())
            {
                throw Conspire::invalid_key( Conspire::tr(
                        "Invalid key \"%1\". No value for this key (%2[%3]) has "
                        "been set.")
                            .arg(key, k).arg(default_idx), CODELOC );
            }
            
            if (v.isA<Options>())
                return v.asA<Options>().getNestedOption(key);

            else if (v.isA<Option>())
            {
                if (v.asA<Option>().key() == p.key)
                    return v.asA<Option>().getNestedOption(key);
            }

            throw Conspire::invalid_key( Conspire::tr(
                    "Invalid key. Option \"%1\" from key \"%2\" cannot be used "
                    "to search for an option with key \"%3\".")
                        .arg(p.key, key, k), CODELOC );
        }
    
        if (p.tail.isEmpty())
            return *this;
    
        Obj value = this->operator[](p.index).value();
            
        if (value.isA<Options>())
            return value.asA<Options>().getNestedOption(p.tail);
        
        else if (value.isA<Option>())
            return value.asA<Option>().getNestedOption(p.tail);
        
        else
        {
            throw Conspire::invalid_key( Conspire::tr(
                    "Invalid key. Option \"%1\" from key \"%2\" cannot be used "
                    "to search for an option as this option only has a value.")
                        .arg(p.key, key), CODELOC );
                        
            return Option();
        }
    }
    else
    {
        return *this;
    }
}

/** Return the value of the passed key. Note that the first element of 
    the key must equal the key of this option, or it must be empty. If 
    it is empty, then the value is returned. If the first element doesn't
    match this option's key, then an invalid_key exception is raised. */
Obj Option::getNestedValue(String key) const
{
    assertNotNull();

    ParsedKey p(key);

    if (not p.key.isEmpty())
    {
        if (p.key != k)
        {
            //maybe we are looking for one of our subkeys?
            Obj v = value();
            
            if (v.isNone())
                throw Conspire::invalid_key( Conspire::tr(
                        "Invalid key \"%1\". No value for this key (%2[%3]) has "
                        "been set.")
                            .arg(key, k).arg(default_idx), CODELOC );
            
            if (v.isA<Options>())
                return v.asA<Options>().getNestedValue(key);

            else if (v.isA<Option>())
            {
                if (v.asA<Option>().key() == p.key)
                    return v.asA<Option>().getNestedValue(key);
            }

            throw Conspire::invalid_key( Conspire::tr(
                    "Invalid key. Option \"%1\" from key \"%2\" cannot be used "
                    "to search for values from an option with key \"%3\".")
                        .arg(p.key, key, k), CODELOC );
        }
    }

    return this->operator[](p.index).value().asA<Value>().getNestedValue(p.tail);
}

void Option::setUserValue(int index, Obj value)
{
    conspireDebug() << "Option::setUserValue(" << index << "," << value.toString() << ")";

    if (value.isNone())
        throw Conspire::program_bug( Conspire::tr(
            "It should not be possible to set a null value here for "
            "key %1[%2].")
                .arg(key()).arg(index), CODELOC );

    else if (not value.isA<Value>())
        throw Conspire::program_bug( Conspire::tr(
            "It should not be possible to set a non-Value value here for "
            "key %1[%2] - type is %3.")
                .arg(key()).arg(index).arg(value.what()), CODELOC );

    else if (index < 1 or ((allow_multiple == false) and index > 1))
        throw Conspire::invalid_index( Conspire::tr(
            "Cannot set the user value for %1[%2] as this index is invalid. "
            "Either it is less than 1, or it is more than one for an option "
            "that does not support multiple values.")
                .arg(key()).arg(index), CODELOC );
                
    while (user_vals.count() < index)
        user_vals.append( None() );
        
    user_vals[index-1] = value;
}

/** Add the default value of this option to the key 'key' */
Obj Option::addDefaultValue(String key) const
{
    conspireDebug() << "Option::addDefaultValue(" << key << ")";

    assertNotNull();
    
    ParsedKey p(key);
    
    if (not p.key.isEmpty())
    {
        if (p.key != k)
        {
            throw Conspire::invalid_key( Conspire::tr(
                    "Invalid key. Option \"%1\" from key \"%2\" cannot be used "
                    "to set values for an option with key \"%3\".")
                        .arg(p.key, key, k), CODELOC );
        }
    }

    Obj new_val = this->operator[](p.index).value();
    
    if (new_val.isNone())
        new_val = defaultValue();
    
    conspireDebug() << "new_val ==" << new_val.toString();

    if (not p.tail.isEmpty())
    {
        conspireDebug() << "tail == " << p.tail;
        new_val = new_val.asA<Value>().addDefaultValue(p.tail);
    }
    
    Option ret(*this);
    ret.setUserValue(p.index, new_val);
    return ret;
}

/** Remove the value associated with the key 'key' */
Obj Option::removeValue(String key) const
{
    assertNotNull();
    
    ParsedKey p(key);
    
    if (not p.key.isEmpty())
    {
        if (p.key != k)
        {
            throw Conspire::invalid_key( Conspire::tr(
                    "Invalid key. Option \"%1\" from key \"%2\" cannot be used "
                    "to set values for an option with key \"%3\".")
                        .arg(p.key, key, k), CODELOC );
        }
    }

    Obj new_val = this->operator[](p.index).value();
    
    if (new_val.isNone())
        // there is no value
        return *this;

    if (p.tail.isEmpty())
    {
        conspireDebug() << "MUST REMOVE THE VALUE";
    }
    else
    {
        new_val = new_val.asA<Value>().removeValue(p.tail);
    }
    
    Option ret(*this);
    // THIS IS WRONG
    ret.setUserValue(p.index, new_val);
    return ret;
}

/** Set the value of the specified key to 'value'. Note that the first element of 
    the key must equal the key of this option, or it must be empty. If 
    it is empty, then the value is returned. If the first element doesn't
    match this option's key, then an invalid_key exception is raised. */
Obj Option::fromValueString(String key, String value) const
{
    assertNotNull();

    ParsedKey p(key);
    
    if (not p.key.isEmpty())
    {
        if (p.key != k)
        {
            throw Conspire::invalid_key( Conspire::tr(
                    "Invalid key. Option \"%1\" from key \"%2\" cannot be used "
                    "to set values for an option with key \"%3\".")
                        .arg(p.key, key, k), CODELOC );
        }
    }
    
    Obj old_val = this->operator[](p.index).value();
    
    if (old_val.isNone())
        old_val = defaultValue();
    
    Obj new_val = old_val.asA<Value>().setNestedValue(p.tail, value);
   
    if (new_val == defaultValue() or old_val != new_val)
    {
        Option ret(*this);
        ret.setUserValue(p.index, new_val);
        return ret;
    }
    else
    {
        return *this;
    }
}

/** Return a copy of this option which has all user-set data cleared */
Obj Option::clear() const
{
    if (user_vals.isEmpty())
        return *this;
    else
    {
        Option ret(*this);
        ret.user_vals.clear();
        
        if (not is_optional)
            ret.user_vals.append(defaultValue());
        
        return ret;
    }
}

/** Return a copy of this option, where the default value index is 
    set to 'i'. Note that user options are 1-indexed */
Option Option::operator[](int i) const
{
    if (not allow_multiple)
        return *this;

    if (i < 1)
        i = 1;

    Option ret(*this);
    
    ret.default_idx = i;
    
    return ret;
}

/** Return the option(s) associated with the passed key. Note that this will 
    only work if this option contains a set of sub-options, and no attempt
    is made to break this key down into sub-keys. Note also that the option
    associated with the current user value is returned, e.g. if you want
    to find the name of the molecule set with index 2, then you would need
    to use "molecule[2]["name"]" */
Option Option::operator[](String key) const
{
    assertNotNull();

    Obj val = this->value();

    if (val.isNone())
        throw Conspire::invalid_key( Conspire::tr(
                "Invalid key \"%1\". No value for this key (%2[%3]) has "
                "been set.")
                    .arg(key, k).arg(default_idx), CODELOC );
    
    else if (val.isA<Options>())
        return val.asA<Options>().operator[](key);
    
    else if (val.isA<Option>())
    {
        Option opt = val.asA<Option>();
        
        if (opt.key() != key)
            throw Conspire::invalid_key( Conspire::tr(
                    "There is no sub-option with key \"%1\" available in option \"%2\". "
                    "This is because the only sub-option has key \"%3\".")
                        .arg(key).arg(k).arg(opt.key()), CODELOC );
        
        return opt;
    }
    else
    {
        throw Conspire::invalid_key( Conspire::tr(
                "There is no sub-option with key \"%1\" available in option \"%2\". "
                "This is because this option contains a value of type %3.")
                    .arg(key).arg(k).arg(val.what()), CODELOC );
    
        return Option();
    }
}

/** Return the set of indicies for options - note that this will return
    indicies for options that have not been set, and that have a default value */
List<int>::Type Option::indicies() const
{
    List<int>::Type idxs;

    idxs.append(1);

    for (int i=1; i<user_vals.count(); ++i)
    {
        idxs.append(i+1);
    }
    
    return idxs;
}

/** Return the set of indicies of options that have been explicitly set 
    by the user */
List<int>::Type Option::indiciesWithValue() const
{
    List<int>::Type idxs;
    
    if (is_optional)
    {
        for (int i=0; i<user_vals.count(); ++i)
        {
            if (not user_vals.at(i).isNone())
                idxs.append(i+1);
        }
    }
    else
    {
        return indicies();
    }
    
    return idxs;
}

/** Return the set of keys of sub-options of this option */
StringList Option::keys() const
{
    Obj val = this->value();
    
    if (val.isNone())
        return StringList();
    
    else if (val.isA<Options>())
        return val.asA<Options>().keys();
    
    else if (val.isA<Option>())
    {
        StringList kys;
        kys.append( val.asA<Option>().key() );
        return kys;
    }
    else
        return StringList();
}

/** Return the set of keys of sub-options that have been explicitly
    set by the user */
StringList Option::keysWithValue() const
{
    Obj val = this->value();

    if (val.isNone())
    {
        if (is_optional)
            return StringList();
        else
            val = defaultValue();
    }

    if (val.isA<Options>())
        return val.asA<Options>().keysWithValue();
        
    else if (val.isA<Option>())
    {
        StringList kys;
        
        if (val.asA<Option>().hasIndiciesWithValue())
        {
            kys.append( val.asA<Option>().key() );
        }
        
        return kys;
    }
    
    return StringList();
}

/** Return the total number of indicies available to this option */
int Option::count() const
{
    if (is_optional)
        return user_vals.count();
    else
        return qMax(1, user_vals.count());
}

/** Return the key for this option */
String Option::key() const
{
    return k;
}

/** Return the description of this option */
String Option::description() const
{
    return desc;
}

/** Return whether or not this option is optional */
bool Option::isOptional() const
{
    return is_optional;
}

/** Return whether or not multiple values for this option are supported */
bool Option::allowMultiple() const
{
    return allow_multiple;
}

/** Internal function used to set the user values from the passed,
    parsed configure file */
Obj Option::fromConfig(ParsedLine &line) const
{
    assertNotNull();

    if (line.been_read)
        //this line has been read already - skip it
        return *this;

    if (not line.key.key.isEmpty())
    {
        if (line.key.key != key())
            //this option is unaffected
            return *this;
    }
    
    Obj old_val = this->operator[](line.key.index).value();
    
    if (old_val.isNone())
        old_val = defaultValue();
    
    ParsedLine new_line(ParsedKey(line.key.tail), line.value);
    
    Obj new_val = old_val.asA<Value>().fromConfig(new_line);
    
    if (new_line.been_read)
    {
        //the line was read :-)
        line.been_read = true;
    
        if (new_val == defaultValue() or new_val != old_val)
        {
            Option ret(*this);
            ret.setUserValue(line.key.index,new_val);
            return ret;
        }
    }

    return *this;
}

StringList prep(String p, StringList lines)
{
    if (p.isEmpty())
        return lines;
        
    else
    {
        MutableStringListIterator it(lines);
        
        while (it.hasNext())
        {
            String &line = it.next();
            
            if (not (line.startsWith("#") or line.isEmpty()))
                line.prepend(p);
        }
        
        return lines;
    }
}

/** Return the lines needed in the text configure file to be able to 
    set all of the user-supplied values held in this option */
StringList Option::toConfigLines(bool include_help) const
{
    assertNotNull();

    StringList lines;

    if (user_vals.count() == 1)
    {
        if (not user_vals[0].isNone())
        {
            if (user_vals[0].asA<Value>().isContainer())
                lines = prep( String("%1.").arg(key()), 
                             user_vals[0].asA<Value>().toConfigLines(include_help) );
            else
                lines = prep( String("%1 = ").arg(key()), 
                             user_vals[0].asA<Value>().toConfigLines(include_help) );
        }
    }
    else
    {
        for (int i=0; i<user_vals.count(); ++i)
        {
            if (not user_vals[i].isNone())
            {
                if (user_vals[i].asA<Value>().isContainer())
                    lines += prep(String("%1[%2].").arg(key()).arg(i+1),
                                user_vals[i].asA<Value>().toConfigLines(include_help));
                else
                    lines += prep(String("%1[%2] = ").arg(key()).arg(i+1),
                                user_vals[i].asA<Value>().toConfigLines(include_help));
            }
        }
    }

    if (lines.isEmpty())
    {
        if (is_optional)
            return lines;
        else
        {
            if (defaultValue().asA<Value>().isContainer())
                lines = prep( String("%1.").arg(key()), 
                             defaultValue().asA<Value>().toConfigLines(include_help) );
            else
                lines = prep( String("%1 = ").arg(key()), 
                             defaultValue().asA<Value>().toConfigLines(include_help) );
        }
    }

    if (include_help)
    {
        if (not description().isEmpty())
        {
            StringList desclines = description().split("\n", String::SkipEmptyParts);
            
            for (int i=desclines.count()-1; i>=0; --i)
            {
                lines.prepend( String("# %1").arg(desclines.at(i).trimmed()) );
            }
        }
        
        lines.prepend("");
    }

    return lines;
}

///////////
/////////// Implementation of Options
///////////

REGISTER_SIREN_CLASS(Conspire::Options);

/** Constructor */
Options::Options() : Value()
{}

/** Return the options object represented by the passed XML data.
    Note that this is the structure of the options, and does not 
    contain any user-supplied data. To add user data, you must
    combine the XML with the config file, e.g. by calling
    Options::fromXMLConfig(xml, config) */
Options Options::fromXML(String xml, StringList path)
{
    return Options(xml, path);
}

String readLines(File &file)
{
    TextStream ts(&file);
    
    //only read in 1 MB of the file at a time, to avoid
    //accidentally reading in way too much...
    static Conspire::Qt::int64 chunk_size = 1048576;
    
    String lines = ts.read(chunk_size);
    
    if (not ts.atEnd())
        conspireDebug() << "WARNING: Input file is greater than 1 MB in size!";
        
    return lines;
}

/** Read the passed file, prepending the directory that contains the file
    to the path. This searches for the file in the path, unless the file is
    in the current directory, or if an absolute path is specified */
String readFile(String file, StringList &path)
{
    String lines;
    
    //try to open the file in the current directory
    {
        File f(file);
        
        if (f.open(IODevice::ReadOnly))
        {
            lines = readLines(f);
            
            String fullpath = FileInfo(file).canonicalPath();
            
            path.prepend(fullpath);
            
            StringList add_paths;
            
            foreach (String dir, path)
            {
                FileInfo dirinfo(dir);
                
                if (dirinfo.isRelative())
                {
                    add_paths.append( FileInfo(Dir(fullpath),dir).absoluteFilePath() );
                }
            }
            
            path += add_paths;
            
            return lines;
        }
        else if (FileInfo(file).isAbsolute())
        {
            throw Conspire::file_error( file, Conspire::tr(
                    "Could not open the file \"%1\".")
                        .arg(file), CODELOC );
        }
    }
    
    //now try each directory in the path...
    foreach (String dir, path)
    {
        FileInfo info( Dir(dir), file );
        
        File f(info.absoluteFilePath());
        
        if (f.open(IODevice::ReadOnly))
        {
            lines = readLines(f);
            
            path.prepend(info.canonicalPath());
            
            return lines;
        }
    }
    
    throw Conspire::file_error( Conspire::tr(
                "Could not find the file \"%1\" in any directory in the path [ %2 ].")
                    .arg(file, path.join(", ")), CODELOC );
                    
    return String::null;
}

/** Return the options object represented by the XML data contained
    in the passed file. This will use the (optionally supplied) path
    to find the file, unless the file is in the current directory, or
    an absolute path is supplied */
Options Options::fromXMLFile(QString xmlfile, QStringList path)
{
    String xml = readFile(xmlfile, path);
    return Options::fromXML(xml,path);
}

bool hasDifferentKeys(const MultiHash<int,int>::Type &hash)
{
    if (hash.count() <= 1)
        return false;
    else
    {
        MultiHash<int,int>::const_iterator it = hash.constBegin();
        
        int key = it.key();
        
        for (++it; it != hash.constEnd(); ++it)
        {
            if (it.key() != key)
                return true;
        }
        
        return false;
    }
}

/** Convert this set of Options to a QDomElement */
DomElement Options::toDomElement(DomDocument doc) const
{
    DomElement elem = doc.createElement("options");
    
    if ( hasDifferentKeys(color_to_option) or
        (color_to_option.count() > 0 and option_to_color.count() != opts.count()) )
    {
        //we have multiple colors, or one color but some not colored. This
        //means that we have to write this Options block as a series of 
        //options blocks, one for each color
        
        //first add all non-exclusive options...
        {
            List<Option>::Type non_excl_opts;
        
            for (int i=0; i<opts.count(); ++i)
            {
                if (not option_to_color.contains(i))
                {
                    non_excl_opts.append(opts.at(i).asA<Option>());
                }
            }
        
            elem.appendChild( Options(non_excl_opts).toDomElement(doc) );
        }
        
        //now add the options for each color in turn
        List<int>::Type colors = color_to_option.keys();
        qSort(colors);
        
        foreach (int color, colors)
        {
            List<Option>::Type color_opts;
            
            foreach (int idx, color_to_option.values(color))
            {
                color_opts.append(opts.at(idx).asA<Option>());
            }
            
            elem.appendChild( Options(color_opts,true).toDomElement(doc) );
        }
        
        return elem;
    }
    else
    {
        if (color_to_option.count() == 1)
            elem.setAttribute("mutually_exclusive", "true");
            
        for (int i=0; i<opts.count(); ++i)
        {
            elem.appendChild( opts.at(i).asA<Value>().toDomElement(doc) );
        }
        
        return elem;
    }
}

/** Construct from the passed QDomElement */
Options::Options(DomElement elem, StringList path) : Value()
{
    if (elem.tagName() != "options")
        throw Conspire::parse_error( Conspire::tr(
                "Can only create an Options object from an <options>...</options> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );

                    
    DomNode n = elem.firstChild();
    
    List<Option>::Type options;
    List<Options>::Type child_options;
    bool mutually_exclusive = getBool(elem, "mutually_exclusive", false);
    
    while(not n.isNull()) 
    {
        elem = n.toElement(); // try to convert the node to an element.
        
        if (not elem.isNull()) 
        {
            if (elem.tagName() == "option")
            {
                //this is an Option :-)
                options.append( Option(elem,path) );
            }
            else if (elem.tagName() == "options")
            {
                child_options.append( Options(elem,path) );
            }
            else if (elem.tagName() == "include")
            {
                child_options.append( Options::fromXMLFile(elem.attribute("file"),
                                                           path) );
            }
            else
                conspireDebug() << "Options(QDomElement elem) cannot recognise tag" 
                                << elem.tagName();
        }
        
        n = n.nextSibling();
    }
    
    this->operator=( Options(options,mutually_exclusive) );
    
    foreach (Options child, child_options)
    {
        this->operator=( this->add(child) );
    }
}

/** Construct the set of options by reading the passed XML string */
Options::Options(String xml, StringList path) : Value()
{
    DomDocument doc;
    {
        String errmsg;
        int errline,errcolumn;
     
        if (not doc.setContent(xml,false,&errmsg,&errline,&errcolumn)) 
        {
            throw Conspire::parse_error(Conspire::tr(
                    "Unable to parse the XML data.\n"
                    "Error = %1\n"
                    "Error on line %2, column %3\n"
                    "XML =\n%4")
                        .arg(errmsg).arg(errline).arg(errcolumn).arg(xml), CODELOC );
        }
    }

    //get the root of the XML document
    DomElement docElem = doc.documentElement();

    //now loop over all children, and create Options objects for each
    // <options>...</options> block
    List<Options>::Type all_options;
    
    DomNode n = docElem.firstChild();
     
    while(not n.isNull()) 
    {
        DomElement e = n.toElement(); // try to convert the node to an element.
        
        if (not e.isNull()) 
        {
            if (e.tagName() == "options")
            {
                //this is an Options option :-)
                all_options.append( Options(e,path) );
            }
            else if (e.tagName() == "option")
            {
                all_options.append( Options(Option(e,path)) );
            }
            else if (e.tagName() == "include")
            {
                //include the contents of the passed XML file
                all_options.append( Options::fromXMLFile(e.attribute("file"), 
                                                         path) );
            }
            else
                conspireDebug() << "Options(QString xmlfile) cannot recognise tag" 
                                << e.tagName();
        }
        
        n = n.nextSibling();
    }
    
    if (all_options.isEmpty())
    {
        throw Conspire::parse_error( Conspire::tr(
                "Could not find any <options>...</options> blocks in the "
                "XML data\n%1").arg(xml), CODELOC );
    }
    
    this->operator=(all_options.at(0));
    
    for (int i=1; i<all_options.count(); ++i)
    {
        this->operator=( this->add(all_options.at(i) ) );
    }
}

/** Construct from a single option */
Options::Options(const Option &option) : Value()
{
    if (not option.isNull())
    {
        String key = option.key();
        opts.append(option.clone());
        kys.insert(key,0);
    }
}

/** Construct the set of options from the passed list of options.
    If mutually_exclusive is true, then only one of these options can
    be selected at a time. */
Options::Options(const List<Option>::Type &options, bool mutually_exclusive) : Value()
{
    foreach (const Option &option, options)
    {
        if (not option.isNull())
        {
            String key = option.key();
            
            if (key.isEmpty())
            {
                if (not opts.isEmpty())
                    throw Conspire::invalid_arg( Conspire::tr(
                        "You can only add an unnamed option as the first option "
                        "in a group of options, otherwise the unnamed option can "
                        "never be selected! Option at index %1 is unnamed!\n"
                        "%2\n%3")
                            .arg(opts.count())
                            .arg(this->toXML())
                            .arg(Options(option).toXML()), CODELOC );
            }
            else if (kys.contains(key))
            {
                StringList keynames;
                
                foreach (const Option &option2, options)
                {
                    if (not option2.isNull())
                        keynames.append(option.key());
                }
            
                throw Conspire::invalid_key( Conspire::tr(
                        "You cannot create a series of options if two have a duplicated "
                        "keys! Options at indexes %1 and %2 have the same key (%3). "
                        "[ %4 ]")
                            .arg(kys[key]).arg(opts.count())
                            .arg(key).arg(keynames.join(", ")),
                                CODELOC );
            }
            else
                kys.insert(key, opts.count());
            
            opts.append( option.clone() );
        }
    }
    
    //if the options are mutually exclusive, then add all of the keys
    //to a new color group
    if (mutually_exclusive)
    {
        for (int i=opts.count()-1; i>=0; --i)
        {
            option_to_color.insert(i, 1);
            color_to_option.insertMulti(1, i);
        }
    }
}

/** Copy constructor */
Options::Options(const Options &other) 
        : Value(other), opts(other.opts), kys(other.kys),
          option_to_color(other.option_to_color),
          color_to_option(other.color_to_option),
          color_option(other.color_option)
{}

/** Destructor */
Options::~Options()
{}

/** Copy assignment operator */
void Options::copy_object(const Options &other)
{
    if (this != &other)
    {
        opts = other.opts;
        kys = other.kys;
        option_to_color = other.option_to_color;
        color_to_option = other.color_to_option;
        color_option = other.color_option;
        super::copy_object(other);
    }
}

/** Comparison operator */
bool Options::compare_object(const Options &other) const
{
    return this == &other or 
           (opts == other.opts and option_to_color == other.option_to_color
                and super::compare_object(other));
}

bool Options::isContainer() const
{
    return true;
}

/** Return the list of keys for all options that have been given 
    a value. All other options have no value. */
StringList Options::keysWithValue() const
{
    StringList k;

    for (int i=0; i<opts.count(); ++i)
    {
        int color = option_to_color.value(i, 0);
        
        if (color != 0)
        {
            //is this option excluded by other options?
            if (color_option.value(color,String::null) != opts[i].asA<Option>().key())
                //this option is not selected from the set
                continue;
        }
    
        if (opts.at(i).asA<Option>().hasIndiciesWithValue())
            k += opts.at(i).asA<Option>().key();
    }
    
    return k;
}
    
/** Return the list of keys for all active options. These are the options
    that have either been set by the user, or are the options which are not
    excluded by other options (e.g. only one option from each color group is set) */
StringList Options::keys() const
{
    StringList k;
    
    for (int i=0; i<opts.count(); ++i)
    {
        int color = option_to_color.value(i,0);
        String key = opts.at(i).asA<Option>().key();
    
        if (not key.isEmpty())
        {
            if (color == 0)
            {
                k += key;
            }
            else
            {
                if (color_option.value(color,String::null) == key)
                {
                    k += key;
                }
            }
        }
    }
    
    return k;
}

int getIndex(String key, const Hash<String,int>::Type &keys)
{
    if (key.isEmpty())
    {
        if (keys.isEmpty())
            throw Conspire::invalid_key( Conspire::tr(
                    "Cannot get the value of the option as there are no options "
                    "in this group!"), CODELOC );
                    
        return 0;
    }
    else
    {
        int idx = keys.value(key, -1);
        
        if (idx == -1)
            throw Conspire::invalid_key( Conspire::tr(
                    "Cannot find the option with key \"%1\". Available options are "
                    "[ %2 ].")
                        .arg(key, StringList(keys.keys()).join(", ")),
                            CODELOC );
    
        return idx;
    }
}

/** Return the option with the passed key. Note that this will only return 
    top-level options, and will not try to break the key into suboption types.
    Note also that this will not try to work out if the option has been set,
    or if it is excluded by any other options. */
Option Options::operator[](String key) const
{
    return opts.at( getIndex(key,kys) ).asA<Option>();
}

/** Return the options as they have been read from the passed parsed line */
Obj Options::fromConfig(detail::ParsedLine &line) const
{
    if (line.been_read)
        //this line has been read already - skip it
        return *this;
        
    if (opts.isEmpty())
        return *this;
    
    String key;
    int idx = -1;
    
    if (line.key.key.isEmpty())
    {
        idx = 0;
        key = opts.at(0).asA<Option>().key();
    }
    else
    {
        idx = kys.value(line.key.key, -1);
        key = line.key.key;
    }

    //see if changing this option would affect the selected color. This is
    //strictly forbidden when parsing a config file
    int color = option_to_color.value(idx, 0);
    
    if (color != 0)
    {
        if (color_option.contains(color))
        {
            if (color_option.value(color) != key)
                throw Conspire::invalid_state( Conspire::tr(
                        "You cannot set the value of the option \"%1\" as the option "
                        "\"%2\" has been set already, and these two options are "
                        "mutually exclusive!")
                            .arg(key, color_option.value(color)), CODELOC );
        }
    }

    Obj old_option = opts.at(idx);
    Obj new_option = old_option.asA<Value>().fromConfig(line);
    
    if (old_option == new_option)
    {
        //no change in the option :-). Has this set the color though?
        if (color != 0)
        {
            if (not color_option.contains(color))
            {
                Options ret(*this);
                ret.color_option.insert(color,key);
                return ret;
            }
        }
        
        return *this;
    }
    else
    {
        Options ret(*this);
        
        ret.opts[idx] = new_option;
        
        if (color != 0)
        {
            if (not color_option.contains(color))
            {
                ret.color_option.insert(color,key);
            }
        }
        
        return ret;
    }
}

/** Return the option associated with the passed key */
Option Options::getNestedOption(String key) const
{
    return opts.at( getIndex(ParsedKey(key).key, kys) )
                                    .asA<Option>().getNestedOption(key);
}

/** Return the value of the option with the passed key */
Obj Options::getNestedValue(String key) const
{
    return opts.at( getIndex(ParsedKey(key).key, kys) ).asA<Value>().getNestedValue(key);
}

Obj Options::addDefaultValue(String key) const
{
    conspireDebug() << "Options::addDefaultValue(" << key << ")";

    int idx = getIndex(ParsedKey(key).key, kys);
    
    Obj old_option = opts.at(idx);
    Obj new_option = old_option.asA<Value>().addDefaultValue(key);
    
    //SORT OUT THE COLOR
    Options ret(*this);
    ret.opts[idx] = new_option;
    return ret;
}

Obj Options::removeValue(String key) const
{
    int idx = getIndex(ParsedKey(key).key, kys);
    
    Obj old_option = opts.at(idx);
    Obj new_option = new_option.asA<Value>().removeValue(key);
    
    //SORT OUT THE COLOR
    Options ret(*this);
    ret.opts[idx] = new_option;
    return ret;
}

/** Set the of the key 'key' to 'value'. Note that setting the value of 
    one mutually exclusive option will clear all values of the other
    mutually exclusive options in the same group */
Obj Options::fromValueString(String key, String value) const
{
    int idx = getIndex(ParsedKey(key).key, kys);
    
    Obj old_option = opts.at(idx);
    Obj new_option = old_option.asA<Value>().setNestedValue(key, value);

    int color = option_to_color.value(idx, 0);
    
    if (old_option == new_option)
    {
        if (color == 0)
        {
            //nothing has changed
            return *this;
        }
        else if (color_option.contains(color))
        {
            String old_color_key = color_option.value(color);
            String new_color_key = new_option.asA<Option>().key();
            
            if (old_color_key == new_color_key)
                //the choice of color key has not changed
                return *this;
                
            Options ret(*this);
            
            int old_color_index = getIndex(old_color_key, kys);
            
            ret.opts[old_color_index] = opts.at(old_color_index).asA<Option>().clear();
            ret.color_option[color] = new_color_key;
            
            return ret;
        }
        else
        {
            //we need to record that this option has been selected
            Options ret(*this);
            ret.color_option[color] = new_option.asA<Option>().key();
            return ret;
        }
    }
    else
    {
        //the option has been changed
        Options ret(*this);
        
        ret.opts[idx] = new_option;
        
        if (color == 0)
            return ret;
            
        else if (color_option.contains(color))
        {
            String old_color_key = color_option.value(color);
            String new_color_key = new_option.asA<Option>().key();
            
            if (old_color_key == new_color_key)
                //the choice of color key has not changed
                return ret;
                
            int old_color_index = getIndex(old_color_key, kys);
            
            ret.opts[old_color_index] = opts.at(old_color_index).asA<Option>().clear();
            ret.color_option[color] = new_color_key;
            
            return ret;
        }
        else
        {
            //we need to record that this option has been selected
            ret.color_option[color] = new_option.asA<Option>().key();
            return ret;
        }
    }
}

/** Return the list of all options */
List<Option>::Type Options::options() const
{
    List<Option>::Type ret;
    
    foreach (Obj option, opts)
    {
        ret.append( option.asA<Option>() );
    }
    
    return ret;
}

/** Update the passed option so that its new value can be recorded */
Options Options::update(const Option &option) const
{
    int idx = kys.value(option.key(), -1);
    
    if (idx == -1)
        throw Conspire::invalid_key( Conspire::tr(
                "Cannot update the Option \"%1\" as this option cannot be "
                "found in the list of options ( %2 )")
                    .arg(option.toString())
                    .arg(this->keys().join(", ")), CODELOC );
                    
    int color = option_to_color.value(idx, 0);
    
    if (color != 0)
    {
        //we may have changed color
        throw Conspire::incomplete_code( Conspire::tr(
                "Cannot change the option as it may change color!"), CODELOC );
    }
    
    Options ret(*this);
    ret.opts[idx] = option;

    return ret;
}

/** Add this set of options to 'other', returning the result. The options
    in this set are listed before the options in 'other' */
Options Options::add(const Options &other) const
{
    if (opts.isEmpty())
        return other;
    else if (other.opts.isEmpty())
        return *this;

    //make sure that there are no duplicated keys...
    {
        bool overlaps = false;
        
        if (kys.count() <= other.kys.count())
        {
            foreach (const String &k, kys.keys())
            {
                if (other.kys.contains(k))
                {
                    overlaps = true;
                    break;
                }
            }
        }
        else
        {
            foreach (const String &k, other.kys.keys())
            {
                if (kys.contains(k))
                {
                    overlaps = true;
                    break;
                }
            }
        }
        
        if (overlaps)
            throw Conspire::invalid_key( Conspire::tr(
                    "Cannot add the options [ %1 ] to the options [ %2 ] as "
                    "some of the option names are duplicated!")
                        .arg( StringList(other.kys.keys()).join(", "), 
                              StringList(kys.keys()).join(", ") ), CODELOC );
    }

    //the first option of the second group cannot be unnamed
    if (other.opts.at(0).asA<Option>().key().isEmpty())
        throw Conspire::invalid_key( Conspire::tr(
                "It is not possible to add a set of options where the first key "
                "in the second set is empty. This is because the unnamed key will "
                "be lost."), CODELOC );

    Options ret(*this);
    
    ret.opts += other.opts;
    
    for (int i=opts.count(); i<ret.opts.count(); ++i)
    {
        ret.kys.insert(ret.opts.at(i).asA<Option>().key(), i);
    }
    
    //now sort out the color groups
    if (not other.option_to_color.isEmpty())
    {
        //we just need to copy across the color groups from other to ret
        //(updating the indicies by increasing them by opts.count()
        // and updating the color by max_color
        int max_color = 0;

        if (not color_to_option.isEmpty() )
        {
            List<int>::Type colors = color_to_option.keys();
            qSort(colors);
            max_color = colors.at( colors.count() - 1 );
        }
        
        for (Hash<int,int>::const_iterator it = other.option_to_color.constBegin();
             it != option_to_color.constEnd();
             ++it)
        {
            int color = it.value() + max_color;
            int idx = it.key() + opts.count();
            
            ret.option_to_color.insert(idx, color);
            ret.color_to_option.insertMulti(color, idx);
        }
        
        if (max_color == 0)
        {
            ret.color_option = other.color_option;
        }
        else
        {
            for (Hash<int,QString>::const_iterator it = other.color_option.constBegin();
                 it != other.color_option.constEnd();
                 ++it)
            {
                ret.color_option.insert( it.key() + max_color, it.value() );
            }
        }
    }

    return ret;
}

/** Clear this set of options of all user-supplied data */
Obj Options::clear() const
{
    Options ret(*this);
    
    for (int i=0; i<ret.opts.count(); ++i)
    {
        ret.opts[i] = ret.opts[i].asA<Option>().clear();
    }
    
    ret.color_option.clear();
    
    return ret;
}

/** Convenenient shorthand for Options::add */
Options Options::operator+(const Options &other) const
{
    return Options::add(other);
}

/** Return the lines of a configure file that would be needed to set
    all of the options in this group */
StringList Options::toConfigLines(bool include_help) const
{
    StringList lines;
    
    for (int i=0; i<opts.count(); ++i)
    {
        //get the color of the option
        int color = option_to_color.value(i, 0);
        
        if (color == 0)
            lines += opts[i].asA<Value>().toConfigLines(include_help);
        
        else
        {
            if (color_option.value(color, String::null) == opts[i].asA<Option>().key())
            {
                lines += opts[i].asA<Value>().toConfigLines(include_help);
            }
        }
    }
    
    return lines;
}

/** Return whether or not there is a value set for the option at key 'key' */
bool Options::hasValue(String key) const
{
    return this->keysWithValue().contains(key);
}

/** Dummy function used to provide Options with a similar interface to Option.
    This always returns false, as an Options group does not have a value */
bool Options::hasValue() const
{
    return false;
}

/** Return whether or not there are any suboptions available to this option */
bool Options::hasSubOptions() const
{
    return not opts.isEmpty();
}

/** Return the set of options that have been modified by reading the passed
    configure lines */
Options Options::fromConfig(String config) const
{
    List<ParsedLine>::Type parsed_lines = ParsedLine::parse(config);
    
    Options ret(*this);
    
    StringList lost_lines;
    
    foreach (ParsedLine parsed_line,  parsed_lines)
    {
        ret = ret.fromConfig(parsed_line).asA<Options>();
        
        if (not parsed_line.been_read)
            lost_lines.append(parsed_line.unparse());
    }
    
    if (not lost_lines.isEmpty())
        conspireDebug() << "FAILED TO READ\n" << lost_lines.join("\n") << "\n";
    
    return ret;
}

/** Return the set of options that have been modified by reading the 
    text in the passed configure file */
Options Options::fromConfigFile(String file) const
{
    StringList path;
    return this->fromConfig( readFile(file,path) );
}
