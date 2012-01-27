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

#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QFile>

#include "simparams.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

using namespace SireSim;
using namespace SireSim::detail;
using namespace SireBase;
using namespace SireStream;

///////////
/////////// Implementation of Sim
///////////

static const RegisterMetaType<Sim> r_sim( MAGIC_ONLY, Sim::typeName() );

QDataStream SIRESIM_EXPORT &operator<<(QDataStream &ds, const Sim &sim)
{
    writeHeader(ds, r_sim, 0);

    ds << static_cast<const Property&>(sim);
    
    return ds;
}

QDataStream SIRESIM_EXPORT &operator>>(QDataStream &ds, Sim &sim)
{
    VersionID v = readHeader(ds, r_sim);
    
    if (v == 0)
    {
        ds >> static_cast<Property&>(sim);
    }
    else
        throw version_error(v, "1", r_sim, CODELOC);
        
    return ds;
}

/** Constructor */
Sim::Sim() : Property()
{}

/** Copy constructor */
Sim::Sim(const Sim &other) : Property(other)
{}

/** Destructor */
Sim::~Sim()
{}

const char* Sim::typeName()
{
    return "SireSim::Sim";
}

///////////
/////////// Implementation of SimParams
///////////

static const RegisterMetaType<SimParams> r_simparams( MAGIC_ONLY, SimParams::typeName() );

QDataStream SIRESIM_EXPORT &operator<<(QDataStream &ds, const SimParams &params)
{
    writeHeader(ds, r_simparams, 0);

    ds << static_cast<const Property&>(params);
    
    return ds;
}

QDataStream SIRESIM_EXPORT &operator>>(QDataStream &ds, SimParams &params)
{
    VersionID v = readHeader(ds, r_simparams);
    
    if (v == 0)
    {
        ds >> static_cast<Property&>(params);
    }
    else
        throw version_error(v, "1", r_simparams, CODELOC);
        
    return ds;
}

/** Constructor */
SimParams::SimParams() : Property()
{}

/** Copy constructor */
SimParams::SimParams(const SimParams &other) : Property(other)
{}

/** Destructor */
SimParams::~SimParams()
{}

const char* SimParams::typeName()
{
    return "SireSim::SimParams";
}

///////////
/////////// Implementation of Value
///////////

/** Constructor */
Value::Value()
{}

/** Destructor */
Value::~Value()
{}

/** Return a clone of this value */
ValuePtr Value::clone() const
{
    Value *c = this->ptr_clone();
    
    ValuePtr ptr(c);
    c->self_ptr = ptr;
    
    return ptr;
}

/** Return a point to self */
ValuePtr Value::self() const
{
    ValuePtr ptr = self_ptr.lock();
    
    if (ptr)
        return ptr;
    else
        return this->clone();
}

/** Return the value of the option with the passed key */
ValuePtr Value::operator[](QString key) const
{
    return this->getValue(key);
}

void Value::throwInvalidCast(const char* this_type, const char* other_type)
{
    throw SireError::invalid_cast( QObject::tr(
            "You cannot cast from an object of type %1 to an object of type %2.")
                .arg(other_type).arg(this_type), CODELOC );
}

bool Value::isContainer() const
{
    return false;
}

Value::operator ValuePtr() const
{
    return self();
}

QString Value::toConfig() const
{
    return this->toConfigLines().join("\n");
}

///////////
/////////// Implementation of Option
///////////

/** Constructor */
Option::Option() : Value()
{}

/** Construct an option with the specified key, user-readable description,
    default (suggested) value, and flags to say whether or not multiple
    values of this option are supported, and whether or not this option is
    optional */
Option::Option(const QString &key, const QString &description,
               const Value &value,
               bool is_opt, bool allow_mult)
       : Value(), k(key.trimmed().toLower()), desc(description), val(value.clone()),
         is_optional(is_opt), allow_multiple(allow_mult)
{
    //the key may not contain any spaces or square brackets
    if (key.indexOf(' ') != -1 or key.indexOf('[') != -1 or key.indexOf(']') != -1)
    {
        throw SireError::invalid_arg( QObject::tr(
                "You cannot set a key (%1) that contains spaces or square brakets.")
                    .arg(k), CODELOC );
    }
}

bool getBool(QDomElement elem, QString option, bool default_val)
{
    QString str = elem.attribute(option).trimmed().toLower();
    
    if (str.isEmpty())
        return default_val;
        
    if (str == "true")
        return true;
    else if (str == "false")
        return false;
    else
        throw SireError::file_error( QObject::tr(
                "Cannot interpret the boolean option \"%1\" from XML element "
                "\"%2\". The option should read 'true' or 'false'.")
                    .arg(option, elem.tagName()), CODELOC );
                    
    return false;
}

QDomElement Option::toDomElement(QDomDocument doc) const
{
    assertNotNull();
    
    QDomElement elem = doc.createElement("option");
    
    if (is_optional)
        elem.setAttribute("optional", "true");
    
    if (allow_multiple)
        elem.setAttribute("allow_multiple", "true");
    
    // describe the key
    {
        QDomElement key_node = doc.createElement("key");
        key_node.appendChild( doc.createTextNode(key()) );
        elem.appendChild(key_node);
    }
    
    //describe the description
    {
        QDomElement desc_node = doc.createElement("description");
        desc_node.appendChild( doc.createTextNode(description()) );
        elem.appendChild(desc_node);
    }
    
    //now describe the value
    {
        QDomElement value_node = doc.createElement("value");
        value_node.appendChild( val->toDomElement(doc) );
        elem.appendChild(value_node);
    }
    
    return elem;
}
  
/** Construct from a QDomElement */
Option::Option(QDomElement elem) 
       : Value(), is_optional(false), allow_multiple(false)
{
    if (elem.tagName() != "option")
        throw SireError::file_error( QObject::tr(
                "Can only create an Option object from an <option>...</option> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );
    
    is_optional = getBool(elem, "optional", false);
    allow_multiple = getBool(elem, "allow_multiple", false);
                                                    
    QDomNode n = elem.firstChild();
    
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
                QDomNode n1 = elem.firstChild();
                
                if (not n.isNull())
                {
                    elem = n1.toElement();
                    
                    if (not elem.isNull())
                    {
                        if (elem.tagName() == "options")
                        {
                            val = Options(elem);
                        }
                        else if (elem.tagName() == "option")
                        {
                            val = Option(elem);
                        }
                        else if (elem.tagName() == "string")
                        {
                            val = StringValue(elem);
                        }
                        else
                            qDebug() << "Option(QDomElement elem) cannot recognise "
                                     << "the value tag" << elem.tagName();
                    }
                }
            }
            else
            {
                qDebug() << "Option(QDomElement elem) cannot recognise tag" 
                         << elem.tagName();
            }
        }
        
        n = n.nextSibling();
    }
}
  
/** Copy constructor */
Option::Option(const Option &other) 
       : Value(other), k(other.k), desc(other.desc), 
         val(other.val), user_vals(other.user_vals),
         is_optional(other.is_optional), allow_multiple(other.allow_multiple)
{}

/** Destructor */
Option::~Option()
{}

/** Copy assignment operator */
Option& Option::operator=(const Option &other)
{
    if (this != &other)
    {
        k = other.k;
        desc = other.desc;
        val = other.val;
        user_vals = other.user_vals;
        is_optional = other.is_optional;
        allow_multiple = other.allow_multiple;
    }
    
    return *this;
}

/** Comparison operator */
bool Option::operator==(const Option &other) const
{
    return this == &other or
           (k == other.k and desc == other.desc and val == other.val and
            user_vals == other.user_vals and
            is_optional == other.is_optional and
            allow_multiple == other.allow_multiple);
}

/** Comparison operator */
bool Option::operator!=(const Option &other) const
{
    return not Option::operator==(other);
}

const char* Option::what() const
{
    return Option::typeName();
}

const char* Option::typeName()
{
    return "SireSim::Option";
}

Option* Option::ptr_clone() const
{
    return new Option(*this);
}

bool Option::isContainer() const
{
    return true;
}

void Option::assertNotNull() const
{
    if (val.get() == 0)
    {
        throw SireError::invalid_state( QObject::tr(
                "You can not perform any operations on a null Option object!"),
                    CODELOC );
    }
}

bool Option::isNull() const
{
    return val.get() == 0;
}

namespace SireSim
{
    namespace detail
    {
        class ParsedKey
        {
        public:
            ParsedKey()
            {
                key = QString::null;
                tail = QString::null;
                index = 1;
            }
        
            ParsedKey(QString k)
            {
                k = k.trimmed().toLower();
        
                int idx = k.indexOf(".");
        
                if (idx == -1)
                {
                    key = k;
                    tail = QString::null;
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
                        throw SireError::invalid_key( QObject::tr(
                            "Could not get the index of the key \"%1\". The index "
                            "should be an integer in square brackets, e.g. key[1].")
                                .arg(key), CODELOC );
        
                    bool ok;
                    QString idx_string = key.mid(idx+1, idx2-idx-1);
                    index = idx_string.toInt(&ok);
                    
                    if (not ok)
                        throw SireError::invalid_key( QObject::tr(
                                "Could not get the index integer from the index "
                                "string \"%1\" read from the key \"%2\". The index "
                                "should be an integer in square brackets, e.g. key[1].")
                                    .arg(idx_string, key), CODELOC );
                                    
                    key = key.mid(0,idx).trimmed();
                }
            }
            
            QString unparse() const
            {
                if (index == 1)
                {
                    if (tail.isEmpty())
                        return key;
                    else
                        return QString("%1.%2").arg(key, tail);
                }
                else
                {
                    if (tail.isEmpty())
                        return QString("%1[%2]").arg(key).arg(index);
                    else
                        return QString("%1[%2].%3").arg(key).arg(index).arg(tail);
                }
            }
            
            QString key;
            QString tail;
            int index;
        };

        class ParsedLine
        {
        public:
            ParsedLine()
            {
                key = ParsedKey();
                value = QString::null;
                been_read = true;
            }
            
            ParsedLine(const ParsedKey &k, const QString &v)
            {
                key = k;
                value = v;
                been_read = false;
            }

            QString unparse() const
            {
                return QString("%1 = %2").arg(key.unparse(),value);
            }

            static QString cleanLine(QString line)
            {
                int idx = line.indexOf('#');

                if (idx != -1)
                {
                    //what about if the "#" sign is in a quoted string?
                    if (idx == 0)
                        return QString::null;
                        
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

            static ParsedLine parseLine(QString line)
            {
                int idx = line.indexOf('=');
                
                if (idx == -1)
                    throw SireError::invalid_state( QObject::tr(
                        "Could not parse the config line \"%1\". It should be of the "
                        "form \"key = value\"").arg(line), CODELOC );
                            
                return ParsedLine( ParsedKey(line.mid(0,idx-1).trimmed().toLower()),
                                   line.mid(idx+1).trimmed() );
            }

            static QList<ParsedLine> parse(QString config)
            {
                QStringList lines = config.split("\n");
            
                QList<ParsedLine> parsed_lines;
                QString parse_line;
            
                foreach (QString line, lines)
                {
                    parse_line += cleanLine(line);
                
                    if (not parse_line.endsWith("\\"))
                    {
                        if (not parse_line.isEmpty())
                        {
                            parsed_lines.append( parseLine(parse_line) );
                            parse_line = QString::null;
                        }
                    }
                }
            
                if (not parse_line.isEmpty())
                {
                    parsed_lines.append( parseLine(parse_line) );
                }
                
                return parsed_lines;
            }
            
            ParsedKey key;
            QString value;
            bool been_read;
        };
    }
}

/** Return whether not we have a user-supplied value for the passed index */
bool Option::hasUserValue(int index) const
{
    assertNotNull();

    if (index <= user_vals.count())
    {
        return user_vals.at(index-1).get() != 0;
    }
    else
        return false;
}

/** Return the user value at the supplied index. This raises an exception
    if there is no value set for this index */
ValuePtr Option::userValue(int index) const
{
    assertNotNull();

    if (user_vals.count() <= index)
    {
        ValuePtr ptr = user_vals.at(index-1);
        
        if (ptr)
            return ptr;
    }
    
    throw SireError::invalid_index( QObject::tr(
              "No value for option %1[%2] has been set.")
                    .arg(k).arg(index), CODELOC );

    return ValuePtr();
}

/** Return the default value of the option */
ValuePtr Option::defaultValue() const
{
    assertNotNull();
    return val;
}

/** Return the value of the passed key. Note that the first element of 
    the key must equal the key of this option, or it must be empty. If 
    it is empty, then the value is returned. If the first element doesn't
    match this option's key, then an invalid_key exception is raised. */
ValuePtr Option::getValue(QString key) const
{
    assertNotNull();

    ParsedKey p(key);

    if (not p.key.isEmpty())
    {
        if (p.key != k)
        {
            throw SireError::invalid_key( QObject::tr(
                    "Invalid key. Option \"%1\" from key \"%2\" cannot be used "
                    "to search for values from an option with key \"%3\".")
                        .arg(p.key, key, k), CODELOC );
        }
    }

    if (hasUserValue(p.index))
        return userValue(p.index)->getValue(p.tail);
    else
        return val->getValue(p.tail);
}

void Option::setUserValue(int index, ValuePtr value)
{
    if (value.get() == 0)
        throw SireError::program_bug( QObject::tr(
            "It should not be possible to set a null value here for "
            "key %1[%2].")
                .arg(key()).arg(index), CODELOC );

    else if (index < 1 or ((allow_multiple == false) and index > 1))
        throw SireError::invalid_index( QObject::tr(
            "Cannot set the user value for %1[%2] as this index is invalid. "
            "Either it is less than 1, or it is more than one for an option "
            "that does not support multiple values.")
                .arg(key()).arg(index), CODELOC );
                
    while (user_vals.count() < index)
        user_vals.append( ValuePtr() );
        
    user_vals[index-1] = value;
}

/** Set the value of the specified key to 'value'. Note that the first element of 
    the key must equal the key of this option, or it must be empty. If 
    it is empty, then the value is returned. If the first element doesn't
    match this option's key, then an invalid_key exception is raised. */
ValuePtr Option::setValue(QString key, const Value &value) const
{
    assertNotNull();

    ParsedKey p(key);
    ValuePtr v;
    
    if (not p.key.isEmpty())
    {
        if (p.key != k)
        {
            throw SireError::invalid_key( QObject::tr(
                    "Invalid key. Option \"%1\" from key \"%2\" cannot be used "
                    "to set values for an option with key \"%3\".")
                        .arg(p.key, key, k), CODELOC );
        }
    }
    
    if (hasUserValue(p.index))
    {
        const Value *old_val = userValue(p.index).get();
    
        ValuePtr v = old_val->setValue(p.tail, value);
        
        if (old_val != v.get())
        {
            ValuePtr ret = this->clone();
            ret->asA<Option>().setUserValue(p.index, v);
            return ret;
        }
        else
            return self();
    }
    else
    {
        ValuePtr v = val->setValue(p.tail, value);
        
        ValuePtr ret = this->clone();

        ret->asA<Option>().setUserValue(p.index, v);

        return ret;
    }
}

/** Return a copy of this option which has all user-set data cleared */
ValuePtr Option::clear() const
{
    if (user_vals.isEmpty())
        return self();
    else
    {
        Option ret(*this);
        ret.user_vals.clear();
        return ret.clone();
    }
}

/** Return the key for this option */
QString Option::key() const
{
    return k;
}

/** Return the description of this option */
QString Option::description() const
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
ValuePtr Option::fromConfig(ParsedLine &line) const
{
    assertNotNull();

    if (line.been_read)
        //this line has been read already - skip it
        return self();

    if (not line.key.key.isEmpty())
    {
        if (line.key.key != key())
            //this option is unaffected
            return self();
    }
    
    if (hasUserValue(line.key.index))
    {
        const Value *old_val = userValue(line.key.index).get();
    
        ParsedLine new_line(ParsedKey(line.key.tail), line.value);
    
        ValuePtr new_val = old_val->fromConfig(new_line);
        
        if (new_line.been_read)
        {
            //the line was read :-)
            line.been_read = true;
        }
    
        if (new_val.get() != old_val)
        {
            //the value has changed
            ValuePtr ret = this->clone();
            ret->asA<Option>().setUserValue(line.key.index,new_val);
            return ret;
        }
        else
        {
            return self();
        }
    }
    else
    {
        ValuePtr ret = this->clone();
        
        ParsedLine new_line(ParsedKey(line.key.tail), line.value);
        
        ret->asA<Option>().setUserValue(line.key.index, val->fromConfig(new_line));
                
        if (new_line.been_read)
        {
            line.been_read = true;
        }
                
        return ret;
    }
}

QStringList prep(QString p, QStringList lines)
{
    if (p.isEmpty())
        return lines;
        
    else
    {
        QMutableStringListIterator it(lines);
        
        while (it.hasNext())
        {
            it.next().prepend(p);
        }
        
        return lines;
    }
}

/** Return the lines needed in the text configure file to be able to 
    set all of the user-supplied values held in this option */
QStringList Option::toConfigLines() const
{
    assertNotNull();

    if (user_vals.isEmpty())
    {
        if (is_optional)
            //no user values have been supplied. As this is an optional
            //configure option, then we don't need to save any data
            return QStringList();
        else
        {
            if (val->isContainer())
                return prep( QString("%1.").arg(key()), val->toConfigLines() );
            else
                return prep( QString("%1 = ").arg(key()), val->toConfigLines() );
        }
    }
    else
    {
        if (user_vals.count() == 1)
        {
            if (user_vals[0]->isContainer())
                return prep( QString("%1.").arg(key()), user_vals[0]->toConfigLines() );
            else
                return prep( QString("%1 = ").arg(key()), user_vals[0]->toConfigLines() );
        }
        else
        {
            QStringList lines;
        
            for (int i=0; i<user_vals.count(); ++i)
            {
                if (user_vals.at(i))
                {
                    if (user_vals.at(i)->isContainer())
                        lines += prep(QString("%1[%2].").arg(key()).arg(i+1),
                                    user_vals[i]->toConfigLines());
                    else
                        lines += prep(QString("%1[%2] = ").arg(key()).arg(i+1),
                                    user_vals[i]->toConfigLines());
                }
            }
            
            return lines;
        }
    }
}

///////////
/////////// Implementation of Options
///////////

/** Constructor */
Options::Options() : Value()
{}

/** Return the description of these options as XML. Note that
    this returns the structure of the options, and does not
    return anything about the user-supplied values. To get those,
    you need to call 'toConfig()' */
QString Options::toXML() const
{
    QDomDocument doc("SIRE_OPTIONS");
    {
        QDomProcessingInstruction xmlDeclaration 
                = doc.createProcessingInstruction("xml", 
                                    "version=\"1.0\" encoding=\"UTF-8\"");
        
        doc.appendChild(xmlDeclaration);
    }
    
    doc.appendChild( doc.createComment("Created using SireSim::Options") );
    QDomElement top = doc.createElement("simoptions");
    {
        QDomElement version = doc.createElement("version");
        version.appendChild( doc.createTextNode("1.0") );
        top.appendChild(version);
    }
    
    QDomElement elem = this->toDomElement(doc);
    
    top.appendChild(elem);
    doc.appendChild(top);
    
    return doc.toString(2);
}

/** Return the options object represented by the passed XML data.
    Note that this is the structure of the options, and does not 
    contain any user-supplied data. To add user data, you must
    combine the XML with the config file, e.g. by calling
    Options::fromXMLConfig(xml, config) */
Options Options::fromXML(const QString &xml)
{
    return Options(xml);
}

/** Return the complete options from the passed XML data and config data */
Options Options::fromXMLConfig(const QString &xml, const QString &config)
{
    return Options(xml).fromConfig(config);
}

/** Convert this set of Options to a QDomElement */
QDomElement Options::toDomElement(QDomDocument doc) const
{
    QDomElement elem = doc.createElement("options");
    
    if (color_to_option.count() > 1 or
        (color_to_option.count() == 1 and option_to_color.count() != opts.count()))
    {
        //we have multiple colors, or one color but some not colored. This
        //means that we have to write this Options block as a series of 
        //options blocks, one for each color
        
        //first add all non-exclusive options...
        {
            QList<Option> non_excl_opts;
        
            for (int i=0; i<opts.count(); ++i)
            {
                if (not option_to_color.contains(i))
                {
                    non_excl_opts.append(opts.at(i)->asA<Option>());
                }
            }
        
            elem.appendChild( Options(non_excl_opts).toDomElement(doc) );
        }
        
        //now add the options for each color in turn
        QList<int> colors = color_to_option.keys();
        qSort(colors);
        
        foreach (int color, colors)
        {
            QList<Option> color_opts;
            
            foreach (int idx, color_to_option.values(color))
            {
                color_opts.append(opts.at(idx)->asA<Option>());
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
            elem.appendChild( opts.at(i)->toDomElement(doc) );
        }
        
        return elem;
    }
}

/** Construct from the passed QDomElement */
Options::Options(QDomElement elem) : Value()
{
    if (elem.tagName() != "options")
        throw SireError::file_error( QObject::tr(
                "Can only create an Options object from an <options>...</options> "
                "XML DOM element. Cannot use a <%1>...</%1> element!")
                    .arg(elem.tagName()), CODELOC );

                    
    QDomNode n = elem.firstChild();
    
    QList<Option> options;
    QList<Options> child_options;
    bool mutually_exclusive = getBool(elem, "mutually_exclusive", false);
    
    while(not n.isNull()) 
    {
        elem = n.toElement(); // try to convert the node to an element.
        
        if (not elem.isNull()) 
        {
            if (elem.tagName() == "option")
            {
                //this is an Option :-)
                options.append( Option(elem) );
            }
            else if (elem.tagName() == "options")
            {
                child_options.append( Options(elem) );
            }
            else
                qDebug() << "Options(QDomElement elem) cannot recognise tag" 
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

/** Construct the set of options by reading the passed XML file */
Options::Options(const QString &xmlfile) : Value()
{
    QDomDocument doc;
    QFile file(xmlfile);
     
    if (!file.open(QIODevice::ReadOnly))
        throw SireError::file_error(file, CODELOC);
     
    if (!doc.setContent(&file)) 
    {
        file.close();
     
        throw SireError::file_error(QObject::tr(
                "Unable to parse the XML configure file \"%1\"").arg(xmlfile),
                    CODELOC);
     
        return;
    }
     
    file.close();

    //get the root of the XML document
    QDomElement docElem = doc.documentElement();

    //now loop over all children, and create Options objects for each
    // <options>...</options> block
    QList<Options> all_options;
    
    QDomNode n = docElem.firstChild();
     
    while(not n.isNull()) 
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        
        if (not e.isNull()) 
        {
            if (e.tagName() == "options")
            {
                //this is an Options option :-)
                all_options.append( Options(e) );
            }
            else
                qDebug() << "Options(QString xmlfile) cannot recognise tag" 
                         << e.tagName();
        }
        
        n = n.nextSibling();
    }
    
    if (all_options.isEmpty())
    {
        throw SireError::file_error( QObject::tr(
                "Could not find any <options>...</options> blocks in the "
                "XML file \"%1\"").arg(xmlfile), CODELOC );
    }
    
    this->operator=(all_options.at(0));
    
    for (int i=1; i<all_options.count(); ++i)
    {
        this->operator=( this->add(all_options.at(i) ) );
    }
}

/** Construct the set of options from the passed list of options.
    If mutually_exclusive is true, then only one of these options can
    be selected at a time. */
Options::Options(const QList<Option> &options, bool mutually_exclusive) : Value()
{
    foreach (const Option &option, options)
    {
        if (not option.isNull())
        {
            QString key = option.key();
            
            if (key.isEmpty())
            {
                if (not opts.isEmpty())
                    throw SireError::invalid_arg( QObject::tr(
                        "You can only add an unnamed option as the first option "
                        "in a group of options, otherwise the unnamed option can "
                        "never be selected! Option at index %1 is unnamed!")
                            .arg(opts.count()), CODELOC );
            }
            else if (keys.contains(key))
            {
                QStringList keynames;
                
                foreach (const Option &option2, options)
                {
                    if (not option2.isNull())
                        keynames.append(option.key());
                }
            
                throw SireError::invalid_key( QObject::tr(
                        "You cannot create a series of options if two have a duplicated "
                        "keys! Options at indexes %1 and %2 have the same key (%3). "
                        "[ %4 ]")
                            .arg(keys[key]).arg(opts.count())
                            .arg(key).arg(keynames.join(", ")),
                                CODELOC );
            }
            else
                keys.insert(key, opts.count());
            
            opts.append( option.clone() );
        }
    }
    
    //if the options are mutually exclusive, then add all of the keys
    //to a new color group
    if (mutually_exclusive)
    {
        for (int i=0; i<opts.count(); ++i)
        {
            option_to_color.insert(i, 1);
            color_to_option.insertMulti(1, i);
        }
    }
}

/** Copy constructor */
Options::Options(const Options &other) 
        : Value(other), opts(other.opts), keys(other.keys),
          option_to_color(other.option_to_color),
          color_to_option(other.color_to_option),
          color_option(other.color_option)
{}

/** Destructor */
Options::~Options()
{}

/** Copy assignment operator */
Options& Options::operator=(const Options &other)
{
    if (this != &other)
    {
        opts = other.opts;
        keys = other.keys;
        option_to_color = other.option_to_color;
        color_to_option = other.color_to_option;
        color_option = other.color_option;
    }
    
    return *this;
}

/** Comparison operator */
bool Options::operator==(const Options &other) const
{
    return this == &other or 
           (opts == other.opts and option_to_color == other.option_to_color);
}

/** Comparison operator */
bool Options::operator!=(const Options &other) const
{
    return not Options::operator==(other);
}

const char* Options::what() const
{
    return Options::typeName();
}

const char* Options::typeName()
{
    return "SireSim::Options";
}

bool Options::isContainer() const
{
    return true;
}

Options* Options::ptr_clone() const
{
    return new Options(*this);
}

int getIndex(QString key, const QHash<QString,int> &keys)
{
    if (key.isEmpty())
    {
        if (keys.isEmpty())
            throw SireError::invalid_key( QObject::tr(
                    "Cannot get the value of the option as there are no options "
                    "in this group!"), CODELOC );
                    
        return 0;
    }
    else
    {
        int idx = keys.value(key, -1);
        
        if (idx == -1)
            throw SireError::invalid_key( QObject::tr(
                    "Cannot find the option with key \"%1\". Available options are "
                    "[ %2 ].")
                        .arg(key, QStringList(keys.keys()).join(", ")),
                            CODELOC );
    
        return idx;
    }
}

/** Return the options as they have been read from the passed parsed line */
ValuePtr Options::fromConfig(detail::ParsedLine &line) const
{
    if (line.been_read)
        //this line has been read already - skip it
        return self();
        
    if (opts.isEmpty())
        return self();
    
    QString key;
    int idx = -1;
    
    if (line.key.key.isEmpty())
    {
        idx = 0;
        key = opts.at(0)->asA<Option>().key();
    }
    else
    {
        idx = keys.value(line.key.key, -1);
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
                throw SireError::invalid_state( QObject::tr(
                        "You cannot set the value of the option \"%1\" as the option "
                        "\"%2\" has been set already, and these two options are "
                        "mutually exclusive!")
                            .arg(key, color_option.value(color)), CODELOC );
        }
    }

    ValuePtr old_option = opts.at(idx);
    ValuePtr new_option = old_option->fromConfig(line);
    
    if (old_option.get() == new_option.get())
    {
        //no change in the option :-). Has this set the color though?
        if (color != 0)
        {
            if (not color_option.contains(color))
            {
                Options ret(*this);
                ret.color_option.insert(color,key);
                return ret.clone();
            }
        }
        
        return self();
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
        
        return ret.clone();
    }
}

/** Return the value of the option with the passed key */
ValuePtr Options::getValue(QString key) const
{
    return opts.at( getIndex(ParsedKey(key).key, keys) )->getValue(key);
}

/** Set the of the key 'key' to 'value'. Note that setting the value of 
    one mutually exclusive option will clear all values of the other
    mutually exclusive options in the same group */
ValuePtr Options::setValue(QString key, const Value &value) const
{
    int idx = getIndex(ParsedKey(key).key, keys);
    
    ValuePtr old_option = opts.at(idx);

    ValuePtr new_option = old_option->setValue(key, value);

    int color = option_to_color.value(idx, 0);
    
    if (old_option.get() == new_option.get())
    {
        if (color == 0)
        {
            //nothing has changed
            return self();
        }
        else if (color_option.contains(color))
        {
            QString old_color_key = color_option.value(color);
            QString new_color_key = new_option->asA<Option>().key();
            
            if (old_color_key == new_color_key)
                //the choice of color key has not changed
                return self();
                
            Options ret(*this);
            
            int old_color_index = getIndex(old_color_key, keys);
            
            ret.opts[old_color_index] = opts.at(old_color_index)->asA<Option>().clear();
            ret.color_option[color] = new_color_key;
            
            return ret.clone();
        }
        else
        {
            //we need to record that this option has been selected
            Options ret(*this);
            ret.color_option[color] = new_option->asA<Option>().key();
            return ret.clone();
        }
    }
    else
    {
        //the option has been changed
        Options ret(*this);
        
        ret.opts[idx] = new_option;
        
        if (color == 0)
            return ret.clone();
            
        else if (color_option.contains(color))
        {
            QString old_color_key = color_option.value(color);
            QString new_color_key = new_option->asA<Option>().key();
            
            if (old_color_key == new_color_key)
                //the choice of color key has not changed
                return ret.clone();
                
            int old_color_index = getIndex(old_color_key, keys);
            
            ret.opts[old_color_index] = opts.at(old_color_index)->asA<Option>().clear();
            ret.color_option[color] = new_color_key;
            
            return ret.clone();
        }
        else
        {
            //we need to record that this option has been selected
            ret.color_option[color] = new_option->asA<Option>().key();
            return ret.clone();
        }
    }
}

/** Return the list of all options */
QList<Option> Options::options() const
{
    QList<Option> ret;
    
    foreach (ValuePtr option, opts)
    {
        ret.append( option->asA<Option>() );
    }
    
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
        
        if (keys.count() <= other.keys.count())
        {
            foreach (const QString &k, keys.keys())
            {
                if (other.keys.contains(k))
                {
                    overlaps = true;
                    break;
                }
            }
        }
        else
        {
            foreach (const QString &k, other.keys.keys())
            {
                if (keys.contains(k))
                {
                    overlaps = true;
                    break;
                }
            }
        }
        
        if (overlaps)
            throw SireError::invalid_key( QObject::tr(
                    "Cannot add the options [ %1 ] to the options [ %2 ] as "
                    "some of the option names are duplicated!")
                        .arg( QStringList(other.keys.keys()).join(", "), 
                              QStringList(keys.keys()).join(", ") ), CODELOC );
    }

    //the first option of the second group cannot be unnamed
    if (other.opts.at(0)->asA<Option>().key().isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It is not possible to add a set of options where the first key "
                "in the second set is empty. This is because the unnamed key will "
                "be lost."), CODELOC );

    Options ret(*this);
    
    ret.opts += other.opts;
    
    for (int i=opts.count(); i<ret.opts.count(); ++i)
    {
        ret.keys.insert(ret.opts.at(i)->asA<Option>().key(), i);
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
            QList<int> colors = color_to_option.keys();
            qSort(colors);
            max_color = colors.at( colors.count() - 1 );
        }
        
        for (QHash<int,int>::const_iterator it = other.option_to_color.constBegin();
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
            for (QHash<int,QString>::const_iterator it = other.color_option.constBegin();
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
ValuePtr Options::clear() const
{
    Options ret(*this);
    
    for (int i=0; i<ret.opts.count(); ++i)
    {
        ret.opts[i] = ret.opts[i]->asA<Option>().clear();
    }
    
    ret.color_option.clear();
    
    return ret.clone();
}

/** Convenenient shorthand for Options::add */
Options Options::operator+(const Options &other) const
{
    return Options::add(other);
}

/** Return the lines of a configure file that would be needed to set
    all of the options in this group */
QStringList Options::toConfigLines() const
{
    QStringList lines;
    
    foreach (ValuePtr option, opts)
    {
        lines += option->toConfigLines();
    }
    
    return lines;
}

/** Return the set of options that have been modified by reading the passed
    configure lines */
Options Options::fromConfig(const QString &config) const
{
    QList<ParsedLine> parsed_lines = ParsedLine::parse(config);
    
    Options ret(*this);
    
    QStringList lost_lines;
    
    foreach (ParsedLine parsed_line,  parsed_lines)
    {
        ret = ret.fromConfig(parsed_line)->asA<Options>();
        
        if (not parsed_line.been_read)
            lost_lines.append(parsed_line.unparse());
    }
    
    if (not lost_lines.isEmpty())
        qDebug() << "FAILED TO READ\n" << lost_lines.join("\n") << "\n";
    
    return ret;
}

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
QStringList StringValue::toConfigLines() const
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
