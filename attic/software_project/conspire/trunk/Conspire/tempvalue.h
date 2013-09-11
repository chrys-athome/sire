#ifndef CONSPIRE_TEMPVALUE_H
#define CONSPIRE_TEMPVALUE_H
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

#include "value.h"

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    /** This represents a temperature value in an option file */
    class CONSPIRE_EXPORT TempValue : public Value
    {
        SIREN_CLASS(TempValue,Value,1)
    
    public:
        TempValue();
        TempValue(double val, Qt::String units=Qt::String());
        TempValue(Qt::String value);

        static TempValue valueWithMinimum(double val, double minimum,
                                          Qt::String units = Qt::String());
        static TempValue valueWithMaximum(double val, double maximum,
                                         Qt::String units = Qt::String());
        static TempValue valueWithRange(double val,
                                        double minimum, double maximum,
                                        Qt::String units = Qt::String());
        
        TempValue(Qt::DomElement elem);
        
        TempValue(const TempValue &other);
        
        ~TempValue();
        
        static Qt::StringList supportedUnits();
        
        double value() const;
        double value(Qt::String units) const;
        
    protected:
        void copy_object(const TempValue &other);
        bool compare_object(const TempValue &other) const;
    
        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        static double toKelvin(double temperature, Qt::String &units);
        static double fromKelvin(double temperature, Qt::String units);

        /** The name of the unit */
        Qt::String unitname;

        /** The current temperature, in kelvin */
        double val;
        
        /** The minimum and maximum allowable temperatures, in kelvin */
        double minval, maxval;
        bool has_minval, has_maxval;
    };
}

SIREN_EXPOSE_CLASS( Conspire::TempValue )

CONSPIRE_END_HEADER

#endif
