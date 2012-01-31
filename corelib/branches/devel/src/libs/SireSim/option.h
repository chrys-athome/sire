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

#ifndef SIRESIM_OPTION_H
#define SIRESIM_OPTION_H

#include "SireSim/value.h"

#include <QHash>
#include <QList>
#include <QMultiHash>

SIRE_BEGIN_HEADER

namespace SireSim
{

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

    Option(QDomElement elem, QStringList path=QStringList());
           
    Option(const Option &other);
    
    ~Option();
    
    Option& operator=(const Option &other);
    
    bool operator==(const Option &other) const;
    bool operator!=(const Option &other) const;
    
    Option operator[](int index) const;
    Option operator[](QString key) const;
    
    int count() const;
    
    QList<int> indicies() const;
    QList<int> indiciesWithValue() const;
    
    bool hasIndiciesWithValue() const;
    
    QStringList keys() const;
    QStringList keysWithValue() const;
    
    bool hasSubOptions() const;
    bool hasValue(QString key) const;
    
    const char* what() const;
    static const char* typeName();
    
    bool isNull() const;
    
    ValuePtr clear() const;
    
    QString key() const;
    QString description() const;
    
    bool isOptional() const;
    bool allowMultiple() const;

    bool hasValue() const;
    ValuePtr value() const;
    
    ValuePtr defaultValue() const;
    
    ValuePtr getNestedValue(QString key) const;
    Option getNestedOption(QString key) const;

protected:
    Option* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &lines) const;
    bool isContainer() const;

    QDomElement toDomElement(QDomDocument doc) const;

    QStringList toConfigLines(bool include_help) const;

    ValuePtr fromValueString(QString key, QString value) const;

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
class SIRESIM_EXPORT Options : public Value
{
public:
    Options();
    Options(QString xml, QStringList path=QStringList());
    Options(QDomElement elem, QStringList path=QStringList());

    Options(const Option &option);
    Options(const QList<Option> &options,
            bool mutually_exclusive=false);
    
    Options(const Options &other);
    
    ~Options();
    
    Options& operator=(const Options &other);
    
    bool operator==(const Options &other) const;
    bool operator!=(const Options &other) const;
    
    const char* what() const;
    static const char* typeName();

    Option operator[](QString key) const;
    
    QStringList keys() const;
    QStringList keysWithValue() const;

    bool hasValue(QString key) const;
    bool hasValue() const;
    
    bool hasSubOptions() const;

    Options fromConfig(QString text) const;
    Options fromConfigFile(QString configfile) const;

    static Options fromXML(QString xml, QStringList path=QStringList());
    static Options fromXMLFile(QString xmlfile, QStringList path=QStringList());
    
    ValuePtr clear() const;
    
    QList<Option> options() const;
    
    Options add(const Options &other) const;
    
    Options operator+(const Options &other) const;

    ValuePtr getNestedValue(QString key) const;
    Option getNestedOption(QString key) const;

protected:
    Options* ptr_clone() const;

    ValuePtr fromConfig(detail::ParsedLine &line) const;
    bool isContainer() const;

    QStringList toConfigLines(bool include_help) const;

    QDomElement toDomElement(QDomDocument doc) const;

    ValuePtr fromValueString(QString key, QString value) const;

private:
    /** The set of options */
    QList<ValuePtr> opts;
    
    /** Hash to accelerate finding the option with specified key */
    QHash<QString,int> kys;
    
    /** The set(s) of mutually exclusive options. All options
        in the same color group are mutually exclusive */
    QHash<int,int> option_to_color;
    
    /** The inverse map, going from color to options */
    QMultiHash<int,int> color_to_option;
    
    /** The name of the option selected from each color group */
    QHash<int,QString> color_option;
};

}

SIRE_EXPOSE_CLASS( SireSim::Option )
SIRE_EXPOSE_CLASS( SireSim::Options )

SIRE_END_HEADER

#endif
