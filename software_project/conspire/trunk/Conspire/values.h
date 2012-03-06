#ifndef CONSPIRE_VALUES_H
#define CONSPIRE_VALUES_H
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

    /** This class represents a string value */
    class CONSPIRE_EXPORT StringValue : public Value
    {
        SIREN_CLASS(StringValue,Value,1)
    
    public:
        StringValue();
        StringValue(Qt::String value);
        
        StringValue(Qt::DomElement elem);
        
        StringValue(const StringValue &other);
        
        ~StringValue();
        
        Qt::String value() const;
        
    protected:
        void copy_object(const StringValue &other);
        bool compare_object(const StringValue &other) const;

        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        /** The current value of this string. */
        Qt::String val;
    };

    /** This class represents a directory */
    class CONSPIRE_EXPORT DirValue : public Value
    {
        SIREN_CLASS(DirValue,Value,1)
    
    public:
        DirValue();
        DirValue(Qt::String dir);
        DirValue(Qt::DomElement elem);
        
        DirValue(const DirValue &other);
        
        ~DirValue();
        
        Qt::String value() const;
        
    protected:
        void copy_object(const DirValue &other);
        bool compare_object(const DirValue &other) const;

        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        /** The current value of the directory */
        Qt::String val;
    };

    /** This class represents a filename value */
    class CONSPIRE_EXPORT FileValue : public Value
    {
        SIREN_CLASS(FileValue,Value,1)
    
    public:
        FileValue();
        FileValue(Qt::String file);
        FileValue(Qt::DomElement elem);
        
        FileValue(const FileValue &other);
        
        ~FileValue();
        
        Qt::String value() const;
        
    protected:
        void copy_object(const FileValue &other);
        bool compare_object(const FileValue &other) const;

        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        /** The current value of the file. */
        Qt::String val;
    };

    /** This class represents an integer value */
    class CONSPIRE_EXPORT IntegerValue : public Value
    {
        SIREN_CLASS(IntegerValue,Value,1)
    
    public:
        IntegerValue();
        IntegerValue(Qt::int64 val);
        IntegerValue(Qt::String val);

        static IntegerValue valueWithMinimum(Qt::int64 val, Qt::int64 minimum);
        static IntegerValue valueWithMaximum(Qt::int64 val, Qt::int64 maximum);
        static IntegerValue valueWithRange(Qt::int64 val,
                                           Qt::int64 minimum, Qt::int64 maximum);
        
        IntegerValue(Qt::DomElement elem);
        
        IntegerValue(const IntegerValue &other);
        
        ~IntegerValue();
        
        Qt::int64 value() const;
        
    protected:
        void copy_object(const IntegerValue &other);
        bool compare_object(const IntegerValue &other) const;

        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        /** The current value */
        Qt::int64 val;
        
        /** The minimum and maximum allowable values */
        Qt::int64 minval, maxval;
    };

    /** This class represents a floating point value */
    class CONSPIRE_EXPORT FloatValue : public Value
    {
        SIREN_CLASS(FloatValue,Value,1)
    
    public:
        FloatValue();
        FloatValue(double val);
        FloatValue(Qt::String value);

        static FloatValue valueWithMinimum(double val, double minimum);
        static FloatValue valueWithMaximum(double val, double maximum);
        static FloatValue valueWithRange(double val,
                                         double minimum, double maximum);
        
        FloatValue(Qt::DomElement elem);
        
        FloatValue(const FloatValue &other);
        
        ~FloatValue();
        
        double value() const;
        
    protected:
        void copy_object(const FloatValue &other);
        bool compare_object(const FloatValue &other) const;
    
        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        /** The current value */
        double val;
        
        /** The minimum and maximum allowable values */
        double minval, maxval;
        bool has_minval, has_maxval;
    };

    /** This class represents a boolean (logical, true/false) value */
    class CONSPIRE_EXPORT BoolValue : public Value
    {
        SIREN_CLASS(BoolValue,Value,1)
    
    public:
        BoolValue();
        BoolValue(bool value);
        BoolValue(Qt::String value);
        
        BoolValue(Qt::DomElement elem);
        
        BoolValue(const BoolValue &other);
        
        ~BoolValue();
        
        bool value() const;
        
    protected:
        void copy_object(const BoolValue &other);
        bool compare_object(const BoolValue &other) const;

        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        /** The current value */
        bool val;
    };

    /** This class represents an enumeration of options. Each option
        is represented by a string.
        
        @author Christopher Woods
    */
    class CONSPIRE_EXPORT EnumValue : public Value
    {
        SIREN_CLASS(EnumValue,Value,1)
    
    public:
        EnumValue();
        EnumValue(Qt::StringList options);
        EnumValue(Qt::StringList options, Qt::StringList docs);
        
        EnumValue(Qt::StringList options, int index);
        EnumValue(Qt::StringList options, Qt::StringList docs, int index);
        
        EnumValue(Qt::String value);
        EnumValue(Qt::String value, Qt::String doc);
        
        EnumValue(Qt::DomElement elem);
        
        EnumValue(const EnumValue &other);
        
        ~EnumValue();

        Qt::StringList options() const;
        Qt::StringList documentation() const;
        
        Qt::String value() const;
            
    protected:
        void copy_object(const EnumValue &other);
        bool compare_object(const EnumValue &other) const;

        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        /** The set of possible options */
        Qt::StringList opts;
        
        /** The documentation for each possible option */
        Qt::StringList docs;
        
        /** The index of the selected option */
        Qt::int32 idx;
    };

}

SIREN_EXPOSE_CLASS( SireSim::StringValue )
SIREN_EXPOSE_CLASS( SireSim::DirValue )
SIREN_EXPOSE_CLASS( SireSim::FileValue )

SIREN_EXPOSE_CLASS( SireSim::IntegerValue )
SIREN_EXPOSE_CLASS( SireSim::FloatValue )
SIREN_EXPOSE_CLASS( SireSim::BoolValue )

SIREN_EXPOSE_CLASS( SireSim::EnumValue )

CONSPIRE_END_HEADER

#endif
