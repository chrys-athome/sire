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

void Value::throwInvalidCast(const char* this_type, const char* other_type)
{
    throw SireError::invalid_cast( QObject::tr(
            "You cannot cast from an object of type %1 to an object of type %2.")
                .arg(other_type).arg(this_type), CODELOC );
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
       : Value(), k(key), desc(description), val(value.clone()),
         is_optional(is_opt), allow_multiple(allow_mult)
{}
  
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

void Option::assertNotNull() const
{
    if (val.get() == 0)
    {
        throw SireError::invalid_state( QObject::tr(
                "You can not perform any operations on a null Option object!"),
                    CODELOC );
    }
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
                    key = k.mid(0,idx-1).trimmed();
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
                    QString idx_string = key.mid(idx+1, idx2-idx2-1);
                    index = idx_string.toInt(&ok);
                    
                    if (not ok)
                        throw SireError::invalid_key( QObject::tr(
                                "Could not get the index integer from the index "
                                "string \"%1\" read from the key \"%2\". The index "
                                "should be an integer in square brackets, e.g. key[1].")
                                    .arg(idx_string, key), CODELOC );
                                    
                    key = key.mid(0,idx-1).trimmed();
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
            }
            
            ParsedLine(const ParsedKey &k, const QString &v)
            {
                key = k;
                value = v;
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
                    
                    return line.mid(0,idx-1).trimmed();
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

            static QList<ParsedLine> parse(QStringList lines)
            {
                QList<ParsedLine> parsed_lines;
                QString parse_line;
            
                foreach (QString line, lines)
                {
                    parse_line += cleanLine(line);
                
                    if (not parse_line.endsWith("\\"))
                    {
                        parsed_lines.append( parseLine(parse_line) );
                        parse_line = QString::null;
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
        };
    }
}

/** Return whether not we have a user-supplied value for the passed index */
bool Option::hasUserValue(int index) const
{
    assertNotNull();

    if (user_vals.count() <= index)
    {
        return user_vals.at(index-1).get() != 0;
    }
    else
        return false;
}

/** Return the user value at the supplied index. This raises an exception
    if there is no value set for this index */
const Value& Option::userValue(int index) const
{
    assertNotNull();

    if (user_vals.count() <= index)
    {
        ValuePtr ptr = user_vals.at(index-1);
        
        if (ptr)
            return *ptr;
    }
    
    throw SireError::invalid_index( QObject::tr(
              "No value for option %1[%2] has been set.")
                    .arg(k).arg(index), CODELOC );

    //below code will never be executed, but prevent a warning message ;-)
    return *(user_vals.at(0));
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
        return userValue(p.index).getValue(p.tail);
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
        const Value *old_val = &(userValue(p.index));
    
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
ValuePtr Option::fromConfig(const ParsedLine &line) const
{
    assertNotNull();

    if (not line.key.key.isEmpty())
    {
        if (line.key.key != key())
            //this option is unaffected
            return self();
    }
    
    if (hasUserValue(line.key.index))
    {
        const Value *old_val = &(userValue(line.key.index));
    
        ValuePtr new_val = old_val->fromConfig( ParsedLine( 
                                                    ParsedKey(line.key.tail),
                                                    line.value) );
    
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
        
        ret->asA<Option>().setUserValue(line.key.index,
                val->fromConfig( ParsedLine(ParsedKey(line.key.tail), line.value) ) );
                
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
QStringList Option::toConfig() const
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
            return prep( QString("%1.").arg(key()), val->toConfig() );
        }
    }
    else
    {
        if (user_vals.count() == 1)
        {
            return prep( QString("%1.").arg(key()), user_vals[0]->toConfig() );
        }
        else
        {
            QStringList lines;
        
            for (int i=0; i<user_vals.count(); ++i)
            {
                if (user_vals.at(i))
                {
                    lines += prep(QString("%1[%2].").arg(key()).arg(i+1),
                                  user_vals[i]->toConfig());
                }
            }
            
            return lines;
        }
    }
}
