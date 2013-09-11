#ifndef CONSPIRE_VALUE_H
#define CONSPIRE_VALUE_H
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

#include "Conspire/conspire.h"

SIREN_BEGIN_HEADER

namespace Conspire
{
    namespace detail
    {
        using namespace Qt;
    
        class ParsedKey
        {
        public:
            ParsedKey();
            ParsedKey(String k);
            
            String unparse() const;
            
            String key;
            String tail;
            int index;
        };

        class ParsedLine
        {
        public:
            ParsedLine();
            
            ParsedLine(const ParsedKey &k, const String &v);

            String unparse() const;

            static String cleanLine(String line);

            static ParsedLine parseLine(String line);

            static List<ParsedLine>::Type parse(String config);
            
            ParsedKey key;
            String value;
            bool been_read;
        };

    } // end of namespace detail

    class Option;
    class Options;

    /** This is the base class of all value types that can appear
        in a configuration file. Note that a value provides
        a placeholder for the actual value in the configuration
        file, and provides the suggested default value, as well as
        the user-supplied value.
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Value : public Object
    {
        SIREN_VIRTUAL_CLASS(Value,Object,1)

    public:
        Value();
        Value(const Value &other);
        
        virtual ~Value();
        
        virtual Obj getNestedValue(Qt::String key) const;
        
        Obj setNestedValue(Qt::String key, Qt::String value) const;
        Obj setNestedValue(Qt::String key, double value) const;
        Obj setNestedValue(Qt::String key, Qt::int64 value) const;

        Obj addNestedValue(Qt::String key) const;
        Obj removeNestedValue(Qt::String key) const;

        Qt::String toString() const;

        Qt::String toConfig(bool include_help=false) const;

        Qt::String toXML() const;

        operator Qt::String() const;

    protected:
        void copy_object(const Value &other);
        bool compare_object(const Value &other) const;
        
        virtual Qt::StringList toConfigLines(bool include_help) const;
        
        friend class Option;
        friend class Options;
        virtual Obj fromConfig(detail::ParsedLine &line) const;
        virtual bool isContainer() const;
        
        virtual Qt::DomElement toDomElement(Qt::DomDocument doc) const=0;
        
        virtual Obj fromValueString(Qt::String key, Qt::String value) const;
        
        virtual Obj fromValueString(Qt::String value) const;
        virtual Qt::String toValueString() const;

        virtual Obj addDefaultValue(Qt::String key) const;
        virtual Obj removeValue(Qt::String key) const;
        
        static Obj createFrom(Qt::DomElement elem);
    };
}

SIREN_EXPOSE_CLASS( Conspire::Value )

SIREN_END_HEADER

#endif
