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

#ifndef SIRESIM_VALUE_H
#define SIRESIM_VALUE_H

#include "sireglobal.h"

#include <QStringList>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

SIRE_BEGIN_HEADER

class QDomDocument;
class QDomElement;

namespace SireSim
{

class Value;
typedef boost::shared_ptr<Value> ValuePtr;

    namespace detail
    {
        class ParsedKey
        {
        public:
            ParsedKey();
            ParsedKey(QString k);
            
            QString unparse() const;
            
            QString key;
            QString tail;
            int index;
        };

        class ParsedLine
        {
        public:
            ParsedLine();
            
            ParsedLine(const ParsedKey &k, const QString &v);

            QString unparse() const;

            static QString cleanLine(QString line);

            static ParsedLine parseLine(QString line);

            static QList<ParsedLine> parse(QString config);
            
            ParsedKey key;
            QString value;
            bool been_read;
        };
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

    QString toConfig(bool include_help=false) const;

    operator ValuePtr() const;

protected:
    virtual Value* ptr_clone() const=0;
    
    virtual QStringList toConfigLines(bool include_help) const=0;
    
    ValuePtr self() const;
    
    friend class Option;
    friend class Options;
    virtual ValuePtr fromConfig(detail::ParsedLine &line) const=0;
    virtual bool isContainer() const;
    
    virtual QDomElement toDomElement(QDomDocument doc) const=0;
    
    static ValuePtr createFrom(QDomElement elem);
    
private:
    static void throwInvalidCast(const char* this_type, const char* other_type);
    
    boost::weak_ptr<Value> self_ptr;
};

}

SIRE_EXPOSE_CLASS( SireSim::Value )

SIRE_END_HEADER

#endif
