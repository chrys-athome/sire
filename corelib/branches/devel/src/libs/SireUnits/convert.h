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

namespace SireUnits
{

/** Convert 'val' from units of 'fromunits' to units of 'tounits' */
inline double convert(double val, double fromunits, double tounits)
{
    return val * (fromunits / tounits);
}

/** Convert 'val' from units of 'fromunits' to internal units. */
inline double convertFrom(double val, double fromunits)
{
    return val * fromunits;
}

/** Convert 'val' from internal units to value of 'tounits' */
inline double convertTo(double val, double tounits)
{
    return val / tounits;
}

}

#endif
