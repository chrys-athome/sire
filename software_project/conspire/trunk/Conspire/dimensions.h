#ifndef CONSPIRE_DIMENSIONS_H
#define CONSPIRE_DIMENSIONS_H
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
    namespace detail
    {
        using namespace Conspire::Qt;

        struct CONSPIRE_EXPORT LengthData
        {
            static const char* typeName();
            static String tagName();
            static Hash<String,double>::Type units();
        };
        
        struct CONSPIRE_EXPORT AreaData
        {
            static const char* typeName();
            static String tagName();
            static Hash<String,double>::Type units();
        };
        
        struct CONSPIRE_EXPORT PressureData
        {
            static const char* typeName();
            static String tagName();
            static Hash<String,double>::Type units();
        };
    }

    template<class T>
    class CONSPIRE_EXPORT DimensionValue : public Value
    {
        SIREN_CLASS(DimensionValue<T>,Value,1)
    
    public:
        DimensionValue();
        DimensionValue(double val, Qt::String units=Qt::String());
        DimensionValue(Qt::String value);

        static DimensionValue<T> valueWithMinimum(double val, double minimum,
                                                  Qt::String units = Qt::String());
        static DimensionValue<T> valueWithMaximum(double val, double maximum,
                                                  Qt::String units = Qt::String());
        static DimensionValue<T> valueWithRange(double val,
                                                double minimum, double maximum,
                                                Qt::String units = Qt::String());
        
        DimensionValue(Qt::DomElement elem);
        
        DimensionValue(const DimensionValue<T> &other);
        
        ~DimensionValue();
        
        static Qt::StringList supportedUnits();
        
        double value() const;
        double value(Qt::String units) const;
        
    protected:
        void copy_object(const DimensionValue<T> &other);
        bool compare_object(const DimensionValue<T> &other) const;
    
        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        static double scaleFactor(Qt::String &units);

        /** The name of the unit used to get the scaling factor */
        Qt::String unitname;

        /** The scaling factor to convert from the units of this
            value to base units */
        double sclfac;

        /** The current value, in base units */
        double val;
        
        /** The minimum and maximum allowable values */
        double minval, maxval;
        bool has_minval, has_maxval;
    };

    typedef DimensionValue<detail::LengthData> LengthValue;
    typedef DimensionValue<detail::AreaData> AreaValue;
    typedef DimensionValue<detail::PressureData> PressureValue;

}

#ifdef CONSPIRE_INSTANTIATE_TEMPLATES

#include "Conspire/dimensions.hpp"

namespace Conspire
{
    template class DimensionValue<detail::LengthData>;
    template class DimensionValue<detail::AreaData>;
    template class DimensionValue<detail::PressureData>;
}
#endif // CONSPIRE_INSTANTIATE TEMPLATES

SIREN_EXPOSE_ALIAS( Conspire::DimensionValue<Conspire::detail::LengthData>,
                    Conspire::LengthValue )
SIREN_EXPOSE_ALIAS( Conspire::DimensionValue<Conspire::detail::AreaData>,
                    Conspire::AreaValue )
SIREN_EXPOSE_ALIAS( Conspire::DimensionValue<Conspire::detail::PressureData>,
                    Conspire::PressureValue )

CONSPIRE_END_HEADER

#endif
