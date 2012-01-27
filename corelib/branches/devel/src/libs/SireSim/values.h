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

#ifndef SIRESIM_VALUES_H
#define SIRESIM_VALUES_H

#include "SireSim/value.h"

SIRE_BEGIN_HEADER

namespace SireSim
{

/** This class represents a string value */
class SIRESIM_EXPORT StringValue : public Value
{
public:
    StringValue();
    StringValue(const QString &value);
    
    StringValue(QDomElement elem);
    
    StringValue(const StringValue &other);
    
    ~StringValue();
    
    StringValue& operator=(const StringValue &other);
    
    bool operator==(const StringValue &other) const;
    bool operator!=(const StringValue &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    ValuePtr getValue(QString key) const;
    ValuePtr setValue(QString key, const Value &value) const;
    
    QString value() const;
    
protected:
    StringValue* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &lines) const;

    QDomElement toDomElement(QDomDocument doc) const;
    
    QStringList toConfigLines(bool include_help) const;

private:
    /** The current value of this string. */
    QString val;
};

/** This class represents a directory */
class SIRESIM_EXPORT DirValue : public Value
{
public:
    DirValue();
    DirValue(const QString &dir);
    
    DirValue(QDomElement elem);
    
    DirValue(const DirValue &other);
    
    ~DirValue();
    
    DirValue& operator=(const DirValue &other);
    
    bool operator==(const DirValue &other) const;
    bool operator!=(const DirValue &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    ValuePtr getValue(QString key) const;
    ValuePtr setValue(QString key, const Value &value) const;
    
    QString value() const;
    
protected:
    DirValue* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &lines) const;

    QDomElement toDomElement(QDomDocument doc) const;
    
    QStringList toConfigLines(bool include_help) const;

private:
    /** The current value of the directory */
    QString val;
};

/** This class represents a filename value */
class SIRESIM_EXPORT FileValue : public Value
{
public:
    FileValue();
    FileValue(const QString &file);
    
    FileValue(QDomElement elem);
    
    FileValue(const FileValue &other);
    
    ~FileValue();
    
    FileValue& operator=(const FileValue &other);
    
    bool operator==(const FileValue &other) const;
    bool operator!=(const FileValue &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    ValuePtr getValue(QString key) const;
    ValuePtr setValue(QString key, const Value &value) const;
    
    QString value() const;
    
protected:
    FileValue* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &lines) const;

    QDomElement toDomElement(QDomDocument doc) const;
    
    QStringList toConfigLines(bool include_help) const;

private:
    /** The current value of the file. */
    QString val;
};

/** This class represents an integer value */
class SIRESIM_EXPORT IntegerValue : public Value
{
public:
    IntegerValue();
    IntegerValue(qint64 val);

    static IntegerValue valueWithMinimum(qint64 val, qint64 minimum);
    static IntegerValue valueWithMaximum(qint64 val, qint64 maximum);
    static IntegerValue valueWithRange(qint64 val,
                                       qint64 minimum, qint64 maximum);
    
    IntegerValue(QDomElement elem);
    
    IntegerValue(const IntegerValue &other);
    
    ~IntegerValue();
    
    IntegerValue& operator=(const IntegerValue &other);
    
    bool operator==(const IntegerValue &other) const;
    bool operator!=(const IntegerValue &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    ValuePtr getValue(QString key) const;
    ValuePtr setValue(QString key, const Value &value) const;
    
    qint64 value() const;
    
protected:
    IntegerValue* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &lines) const;

    QDomElement toDomElement(QDomDocument doc) const;
    
    QStringList toConfigLines(bool include_help) const;

private:
    /** The current value */
    qint64 val;
    
    /** The minimum and maximum allowable values */
    qint64 minval, maxval;
};

/** This class represents a floating point value */
class SIRESIM_EXPORT FloatValue : public Value
{
public:
    FloatValue();
    FloatValue(double val);

    static FloatValue valueWithMinimum(double val, double minimum);
    static FloatValue valueWithMaximum(double val, double maximum);
    static FloatValue valueWithRange(double val,
                                     double minimum, double maximum);
    
    FloatValue(QDomElement elem);
    
    FloatValue(const FloatValue &other);
    
    ~FloatValue();
    
    FloatValue& operator=(const FloatValue &other);
    
    bool operator==(const FloatValue &other) const;
    bool operator!=(const FloatValue &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    ValuePtr getValue(QString key) const;
    ValuePtr setValue(QString key, const Value &value) const;
    
    double value() const;
    
protected:
    FloatValue* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &lines) const;

    QDomElement toDomElement(QDomDocument doc) const;
    
    QStringList toConfigLines(bool include_help) const;

private:
    /** The current value */
    double val;
    
    /** The minimum and maximum allowable values */
    double minval, maxval;
    bool has_minval, has_maxval;
};

/** This class represents a boolean (logical, true/false) value */
class SIRESIM_EXPORT BoolValue : public Value
{
public:
    BoolValue();
    BoolValue(bool value);
    
    BoolValue(QDomElement elem);
    
    BoolValue(const BoolValue &other);
    
    ~BoolValue();
    
    BoolValue& operator=(const BoolValue &other);
    
    bool operator==(const BoolValue &other) const;
    bool operator!=(const BoolValue &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    ValuePtr getValue(QString key) const;
    ValuePtr setValue(QString key, const Value &value) const;
    
    bool value() const;
    
protected:
    BoolValue* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &lines) const;

    QDomElement toDomElement(QDomDocument doc) const;
    
    QStringList toConfigLines(bool include_help) const;

private:
    /** The current value */
    bool val;
};

}

SIRE_EXPOSE_CLASS( SireSim::StringValue )
SIRE_EXPOSE_CLASS( SireSim::DirValue )
SIRE_EXPOSE_CLASS( SireSim::FileValue )

SIRE_EXPOSE_CLASS( SireSim::IntegerValue )
SIRE_EXPOSE_CLASS( SireSim::FloatValue )
SIRE_EXPOSE_CLASS( SireSim::BoolValue )

SIRE_END_HEADER

#endif
