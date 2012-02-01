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

#ifndef SIRESIM_TEMPVALUE_H
#define SIRESIM_TEMPVALUE_H

#include "value.h"

namespace SireSim
{

/** This represents a temperature value in an option file */
class SIRESIM_EXPORT TempValue : public Value
{
public:
    TempValue();
    TempValue(double val, QString units=QString());
    TempValue(QString value);

    static TempValue valueWithMinimum(double val, double minimum,
                                      QString units = QString());
    static TempValue valueWithMaximum(double val, double maximum,
                                     QString units = QString());
    static TempValue valueWithRange(double val,
                                    double minimum, double maximum,
                                    QString units = QString());
    
    TempValue(QDomElement elem);
    
    TempValue(const TempValue &other);
    
    ~TempValue();
    
    TempValue& operator=(const TempValue &other);
    
    bool operator==(const TempValue &other) const;
    bool operator!=(const TempValue &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    static QStringList supportedUnits();
    
    double value() const;
    double value(QString units) const;
    
protected:
    TempValue* ptr_clone() const;

    QDomElement toDomElement(QDomDocument doc) const;
    
    QString toValueString() const;
    ValuePtr fromValueString(QString value) const;

private:
    static double toKelvin(double temperature, QString &units);
    static double fromKelvin(double temperature, QString units);

    /** The name of the unit */
    QString unitname;

    /** The current temperature, in kelvin */
    double val;
    
    /** The minimum and maximum allowable temperatures, in kelvin */
    double minval, maxval;
    bool has_minval, has_maxval;
};

}

SIRE_EXPOSE_CLASS( SireSim::TempValue )

#endif
