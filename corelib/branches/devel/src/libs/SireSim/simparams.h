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

#ifndef SIRESIM_SIMPARAMS_H
#define SIRESIM_SIMPARAMS_H

#include "SireBase/property.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace SireSim
{
class Sim;
class SimParams;
}

QDataStream& operator<<(QDataStream&, const SireSim::Sim&);
QDataStream& operator>>(QDataStream&, SireSim::Sim&);

QDataStream& operator<<(QDataStream&, const SireSim::SimParams&);
QDataStream& operator>>(QDataStream&, SireSim::SimParams&);

namespace SireSim
{

/** This is the base class of the helper classes that are
    used to run the template simulations available in Sire.
    These provide a simple, high-level interface that allow
    users to quickly and easily use Sire to run common types
    of simulation
    
    @author Christopher Woods
*/
class SIRESIM_EXPORT Sim : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Sim&);
friend QDataStream& ::operator>>(QDataStream&, Sim&);

public:
    Sim();
    Sim(const Sim &other);
    
    virtual ~Sim();

    static const char* typeName();
    
    virtual Sim* clone() const=0;
};

/** This is the base class of the helper classes that are
    used to store user parameters for a simulation. These are used
    to allow the user to configure a simulation that is about to 
    run. Normally, you should provide a SimParams helper class for
    each simulation class that you create.
    
    @author Christopher Woods
*/
class SIRESIM_EXPORT SimParams : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const SimParams&);
friend QDataStream& ::operator>>(QDataStream&, SimParams&);

public:
    SimParams();
    SimParams(const SimParams &other);
    
    virtual ~SimParams();
    
    static const char* typeName();
    
    virtual SimParams* clone() const=0;
};

class Value;
typedef boost::shared_ptr<Value> ValuePtr;

namespace detail
{
    class ParsedKey;
    class ParsedLine;
}

class Option;
class Options;

/** This is the base class of all value types that can appear
    in a configuration file. Note that a value provides
    a placeholder for the actual value in the configuration
    file, and provides the suggested default value, as well as
    the user-supplied value.
    
    @author Christopher Woods
*/
class SIRESIM_EXPORT Value
{
public:
    Value();
    
    virtual ~Value();
    
    virtual const char* what() const=0;
    
    ValuePtr clone() const;
    
    virtual ValuePtr getValue(QString key) const=0;
    virtual ValuePtr setValue(QString key, const Value &value) const=0;
    
    ValuePtr operator[](QString key) const;
    
    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }
    
    template<class T>
    const T& asA() const
    {
        const T* ptr = dynamic_cast<const T*>(this);
        
        if (!ptr)
            Value::throwInvalidCast(this->what(), T::typeName());
            
        return *ptr;
    }
    
    template<class T>
    T& asA()
    {
        T* ptr = dynamic_cast<T*>(this);
        
        if (!ptr)
            Value::throwInvalidCast(this->what(), T::typeName());
            
        return *ptr;
    }
    
    virtual QStringList toConfig() const=0;

protected:
    virtual Value* ptr_clone() const=0;
    
    ValuePtr self() const;
    
    friend class Option;
    friend class Options;
    virtual ValuePtr fromConfig(detail::ParsedLine &line) const=0;
    
private:
    static void throwInvalidCast(const char* this_type, const char* other_type);
    
    boost::weak_ptr<Value> self_ptr;
};

/** This class represents a single option in the configuration file.
    An option is a thing that the user can configure, that has a single 
    key (name used to identify the option to the user) and a single
    value (the actual value of the option). Note that the value can itself
    be another group of options, or an array, so options can be nested
    into quite complicated and dynamic structures.
    
    @author Christopher Woods
*/
class SIRESIM_EXPORT Option : public Value
{
public:
    Option();
    Option(const QString &key, const QString &description,
           const Value &value,
           bool is_optional=false, bool allow_multiple=false);
           
    Option(const Option &other);
    
    ~Option();
    
    Option& operator=(const Option &other);
    
    bool operator==(const Option &other) const;
    bool operator!=(const Option &other) const;
    
    const char* what() const;
    static const char* typeName();
    
    bool isNull() const;
    
    ValuePtr clear() const;
    
    ValuePtr getValue(QString key) const;
    ValuePtr setValue(QString key, const Value &value) const;
    
    QString key() const;
    QString description() const;
    
    bool isOptional() const;
    bool allowMultiple() const;
    
    const Value& defaultValue() const;

    bool hasUserValue(int index) const;
    const Value& userValue(int index) const;

    QStringList toConfig() const;

protected:
    Option* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &lines) const;

private:
    void assertNotNull() const;
    void setUserValue(int index, ValuePtr value);

    /** The key for the option */
    QString k;
    
    /** The description of the option */
    QString desc;
    
    /** The suggested value */
    ValuePtr val;

    /** The user-supplied value(s), if one or more have been provided */
    QList<ValuePtr> user_vals;

    /** Whether or not this option is optional, and whether or
        not multiple values of this option are allowed */
    bool is_optional, allow_multiple;
};

/** This class represents a set of options. This allows a single option
    to have a whole set of sub-options.
    
    @author Christopher Woods
*/
class Options : public Value
{
public:
    Options();
    Options(const QString &xmlfile);

    Options(const QList<Option> &options,
            bool mutually_exclusive=false);
    
    Options(const Options &other);
    
    ~Options();
    
    Options& operator=(const Options &other);
    
    bool operator==(const Options &other) const;
    bool operator!=(const Options &other) const;
    
    const char* what() const;
    static const char* typeName();

    ValuePtr getValue(QString key) const;
    ValuePtr setValue(QString key, const Value &value) const;
    
    ValuePtr clear() const;
    
    QList<Option> options() const;
    
    Options add(const Options &other) const;
    
    Options operator+(const Options &other) const;
    
    QStringList toConfig() const;

protected:
    Options* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &line) const;
     
private:
    /** The set of options */
    QList<ValuePtr> opts;
    
    /** Hash to accelerate finding the option with specified key */
    QHash<QString,int> keys;
    
    /** The set(s) of mutually exclusive options. All options
        in the same color group are mutually exclusive */
    QHash<int,int> option_to_color;
    
    /** The inverse map, going from color to options */
    QMultiHash<int,int> color_to_option;
    
    /** The name of the option selected from each color group */
    QHash<int,QString> color_option;
};

/** This class represents a string value */
class SIRESIM_EXPORT StringValue : public Value
{
public:
    StringValue();
    StringValue(const QString &value);
    
    StringValue(const StringValue &other);
    
    ~StringValue();
    
    StringValue& operator=(const StringValue &other);
    
    bool operator==(const StringValue &other) const;
    bool operator!=(const StringValue &other) const;
    
    const char* what() const;
    
    ValuePtr getValue(QString key) const;
    ValuePtr setValue(QString key, const Value &value) const;
    
    QStringList toConfig() const;
    
protected:
    StringValue* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &lines) const;
};

}

SIRE_EXPOSE_CLASS( SireSim::Sim )
SIRE_EXPOSE_CLASS( SireSim::SimParams )

SIRE_EXPOSE_CLASS( SireSim::Value )
SIRE_EXPOSE_CLASS( SireSim::Option )
SIRE_EXPOSE_CLASS( SireSim::Options )
SIRE_EXPOSE_CLASS( SireSim::StringValue )

#endif
