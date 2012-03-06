#ifndef CONSPIRE_OPTION_H
#define CONSPIRE_OPTION_H
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

#include "Conspire/value.h"

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    /** This class represents a single option in the configuration file.
        An option is a thing that the user can configure, that has a single 
        key (name used to identify the option to the user) and a single
        value (the actual value of the option). Note that the value can itself
        be another group of options, or an array, so options can be nested
        into quite complicated and dynamic structures.
        
        @author Christopher Woods
    */
    class CONSPIRE_EXPORT Option : public Value
    {
        SIREN_CLASS(Option,Value,1)
    
    public:
        Option();
        Option(const Qt::String &key, const Qt::String &description,
               const Value &value,
               bool is_optional=false, bool allow_multiple=false);

        Option(Qt::DomElement elem, Qt::StringList path=Qt::StringList());
               
        Option(const Option &other);
        
        ~Option();
        
        Option operator[](int index) const;
        Option operator[](Qt::String key) const;
        
        int count() const;
        
        Qt::List<int>::Type indicies() const;
        Qt::List<int>::Type indiciesWithValue() const;
        
        bool hasIndiciesWithValue() const;
        
        Qt::StringList keys() const;
        Qt::StringList keysWithValue() const;
        
        bool hasSubOptions() const;
        bool hasValue(Qt::String key) const;
        
        bool isNull() const;
        
        Obj clear() const;
        
        Qt::String key() const;
        Qt::String description() const;
        
        bool isOptional() const;
        bool allowMultiple() const;

        bool hasValue() const;
        Obj value() const;
        
        Obj defaultValue() const;
        
        Obj getNestedValue(Qt::String key) const;
        Option getNestedOption(Qt::String key) const;

    protected:
        void copy_object(const Option &other);
        bool compare_object(const Option &other) const;
        
        Obj fromConfig(detail::ParsedLine &lines) const;
        bool isContainer() const;

        Qt::DomElement toDomElement(Qt::DomDocument doc) const;

        Qt::StringList toConfigLines(bool include_help) const;

        Obj fromValueString(Qt::String key, Qt::String value) const;

    private:
        void assertNotNull() const;
        void setUserValue(int index, Obj value);

        /** The key for the option */
        Qt::String k;
        
        /** The description of the option */
        Qt::String desc;
        
        /** The suggested value */
        Obj val;

        /** The user-supplied value(s), if one or more have been provided */
        Qt::List<Obj>::Type user_vals;

        /** The default index - this is the index of the user option to 
            use when calling 'value' - this is used to simplify querying 
            of this object */
        int default_idx;

        /** Whether or not this option is optional, and whether or
            not multiple values of this option are allowed */
        bool is_optional, allow_multiple;
    };

    /** This class represents a set of options. This allows a single option
        to have a whole set of sub-options.
        
        @author Christopher Woods
    */
    class CONSPIRE_EXPORT Options : public Value
    {
        SIREN_CLASS(Options,Value,1)
        
    public:
        Options();
        Options(Qt::String xml, Qt::StringList path=Qt::StringList());
        Options(Qt::DomElement elem, Qt::StringList path=Qt::StringList());

        Options(const Option &option);
        Options(const Qt::List<Option>::Type &options,
                bool mutually_exclusive=false);
        
        Options(const Options &other);
        
        ~Options();

        Option operator[](Qt::String key) const;
        
        Qt::StringList keys() const;
        Qt::StringList keysWithValue() const;

        bool hasValue(Qt::String key) const;
        bool hasValue() const;
        
        bool hasSubOptions() const;

        Options fromConfig(Qt::String text) const;
        Options fromConfigFile(Qt::String configfile) const;

        static Options fromXML(Qt::String xml, Qt::StringList path=Qt::StringList());
        static Options fromXMLFile(Qt::String xmlfile, 
                                   Qt::StringList path=Qt::StringList());
        
        Obj clear() const;
        
        Qt::List<Option>::Type options() const;
        
        Options add(const Options &other) const;
        
        Options operator+(const Options &other) const;

        Obj getNestedValue(Qt::String key) const;
        Option getNestedOption(Qt::String key) const;

    protected:
        void copy_object(const Options &other);
        bool compare_object(const Options &other) const;

        Obj fromConfig(detail::ParsedLine &line) const;
        bool isContainer() const;

        Qt::StringList toConfigLines(bool include_help) const;

        Qt::DomElement toDomElement(Qt::DomDocument doc) const;

        Obj fromValueString(Qt::String key, Qt::String value) const;

    private:
        /** The set of options */
        Qt::List<Obj>::Type opts;
        
        /** Hash to accelerate finding the option with specified key */
        Qt::Hash<Qt::String,int>::Type kys;
        
        /** The set(s) of mutually exclusive options. All options
            in the same color group are mutually exclusive */
        Qt::Hash<int,int>::Type option_to_color;
        
        /** The inverse map, going from color to options */
        Qt::MultiHash<int,int>::Type color_to_option;
        
        /** The name of the option selected from each color group */
        Qt::Hash<int,Qt::String>::Type color_option;
    };
}

SIREN_EXPOSE_CLASS( SireSim::Option )
SIREN_EXPOSE_CLASS( SireSim::Options )

CONSPIRE_END_HEADER

#endif
