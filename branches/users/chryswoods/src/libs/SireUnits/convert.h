/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREUNITS_CONVERT_H
#define SIREUNITS_CONVERT_H

#include "SireUnits/units.h"

#include <QDebug>

namespace SireUnits
{

/////////////////////////////////////////////////
// Conversion functions for derived units      //
/////////////////////////////////////////////////

inline double convertFrom(double val, const Dimension::DerivedUnit &from_units)
{
    return from_units.convertToInternal(val);
}

inline double convertTo(double val, const Dimension::DerivedUnit &to_units)
{
    return to_units.convertFromInternal(val);
}

inline double convert(double val, const Dimension::DerivedUnit &from_units,
                                  const Dimension::DerivedUnit &to_units)
{
    return convertTo( convertFrom(val,from_units), to_units );
}

inline double convert(double val, const Dimension::DerivedUnit &to_units)
{
    return convertTo(val, to_units);
}

/////////////////////////////////////////////////
// Conversion functions for lengths            //
/////////////////////////////////////////////////

inline double convertFrom(double val, const Dimension::Length &from_units)
{
    return from_units.convertToInternal(val);
}

inline double convertTo(double val, const Dimension::Length &to_units)
{
    return to_units.convertFromInternal(val);
}

inline double convert(double val, const Dimension::Length &from_units,
                                  const Dimension::Length &to_units)
{
    return convertTo( convertFrom(val,from_units), to_units );
}

inline double convert(double val, const Dimension::Length &to_units)
{
    return convertTo(val, to_units);
}

/////////////////////////////////////////////////
// Conversion functions for times              //
/////////////////////////////////////////////////

inline double convertFrom(double val, const Dimension::Time &from_units)
{
    return from_units.convertToInternal(val);
}

inline double convertTo(double val, const Dimension::Time &to_units)
{
    return to_units.convertFromInternal(val);
}

inline double convert(double val, const Dimension::Time &from_units,
                                  const Dimension::Time &to_units)
{
    return convertTo( convertFrom(val,from_units), to_units );
}

inline double convert(double val, const Dimension::Time &to_units)
{
    return convertTo(val, to_units);
}

/////////////////////////////////////////////////
// Conversion functions for mass               //
/////////////////////////////////////////////////

inline double convertFrom(double val, const Dimension::Mass &from_units)
{
    return from_units.convertToInternal(val);
}

inline double convertTo(double val, const Dimension::Mass &to_units)
{
    return to_units.convertFromInternal(val);
}

inline double convert(double val, const Dimension::Mass &from_units,
                                  const Dimension::Mass &to_units)
{
    return convertTo( convertFrom(val,from_units), to_units );
}

inline double convert(double val, const Dimension::Mass &to_units)
{
    return convertTo(val, to_units);
}

/////////////////////////////////////////////////
// Conversion functions for charge             //
/////////////////////////////////////////////////

inline double convertFrom(double val, const Dimension::Charge &from_units)
{
    return from_units.convertToInternal(val);
}

inline double convertTo(double val, const Dimension::Charge &to_units)
{
    return to_units.convertFromInternal(val);
}

inline double convert(double val, const Dimension::Charge &from_units,
                                  const Dimension::Charge &to_units)
{
    return convertTo( convertFrom(val,from_units), to_units );
}

inline double convert(double val, const Dimension::Charge &to_units)
{
    return convertTo(val, to_units);
}

/////////////////////////////////////////////////
// Conversion functions for energy             //
/////////////////////////////////////////////////

inline double convertFrom(double val, const Dimension::Energy &from_units)
{
    return from_units.convertToInternal(val);
}

inline double convertTo(double val, const Dimension::Energy &to_units)
{
    return to_units.convertFromInternal(val);
}

inline double convert(double val, const Dimension::Energy &from_units,
                                  const Dimension::Energy &to_units)
{
    return convertTo( convertFrom(val,from_units), to_units );
}

inline double convert(double val, const Dimension::Energy &to_units)
{
    return convertTo(val, to_units);
}

/////////////////////////////////////////////////
// Conversion functions for temperature        //
/////////////////////////////////////////////////

inline double convertFrom(double val, const Dimension::Temperature &from_units)
{
    return from_units.convertToInternal(val);
}

inline double convertTo(double val, const Dimension::Temperature &to_units)
{
    return to_units.convertFromInternal(val);
}

inline double convert(double val, const Dimension::Temperature &from_units,
                                  const Dimension::Temperature &to_units)
{
    return convertTo( convertFrom(val,from_units), to_units );
}

inline double convert(double val, const Dimension::Temperature &to_units)
{
    return convertTo(val, to_units);
}

}

#endif
