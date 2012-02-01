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

#ifndef SIRESIM_DIMENSIONS_H
#define SIRESIM_DIMENSIONS_H

#include "SireSim/value.h"

#include <QHash>

namespace SireSim
{

namespace detail
{
    struct SIRESIM_EXPORT LengthData
    {
        static const char* typeName();
        static QString tagName();
        static QHash<QString,double> units();
    };
    
    struct SIRESIM_EXPORT AreaData
    {
        static const char* typeName();
        static QString tagName();
        static QHash<QString,double> units();
    };
    
    struct SIRESIM_EXPORT PressureData
    {
        static const char* typeName();
        static QString tagName();
        static QHash<QString,double> units();
    };
}

template<class T>
class SIRESIM_EXPORT DimensionValue : public Value
{
public:
    DimensionValue();
    DimensionValue(double val, QString units=QString());
    DimensionValue(QString value);

    static DimensionValue<T> valueWithMinimum(double val, double minimum,
                                              QString units = QString());
    static DimensionValue<T> valueWithMaximum(double val, double maximum,
                                              QString units = QString());
    static DimensionValue<T> valueWithRange(double val,
                                            double minimum, double maximum,
                                            QString units = QString());
    
    DimensionValue(QDomElement elem);
    
    DimensionValue(const DimensionValue<T> &other);
    
    ~DimensionValue();
    
    DimensionValue<T>& operator=(const DimensionValue<T> &other);
    
    bool operator==(const DimensionValue<T> &other) const;
    bool operator!=(const DimensionValue<T> &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    double value() const;
    double value(QString units) const;
    
protected:
    DimensionValue<T>* ptr_clone() const;

    QDomElement toDomElement(QDomDocument doc) const;
    
    QString toValueString() const;
    ValuePtr fromValueString(QString value) const;

private:
    static double scaleFactor(QString &units);

    /** The name of the unit used to get the scaling factor */
    QString unitname;

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

#ifdef SIRE_INSTANTIATE_TEMPLATES

#include "SireSim/dimensions.hpp"

namespace SireSim
{
    template class DimensionValue<detail::LengthData>;
    template class DimensionValue<detail::AreaData>;
    template class DimensionValue<detail::PressureData>;
}
#endif // SIRE_INSTANTIATE TEMPLATES

SIRE_EXPOSE_ALIAS( SireSim::DimensionValue<SireSim::detail::LengthData>,
                   SireSim::LengthValue )
SIRE_EXPOSE_ALIAS( SireSim::DimensionValue<SireSim::detail::AreaData>,
                   SireSim::AreaValue )
SIRE_EXPOSE_ALIAS( SireSim::DimensionValue<SireSim::detail::PressureData>,
                   SireSim::PressureValue )

#endif
