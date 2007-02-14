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

#ifndef PYWRAP_SIREQT_QHASH_REAL_H
#define PYWRAP_SIREQT_QHASH_REAL_H

inline unsigned int qHash(const double &val)
{
    //reinterpret the bits of the double as an unsigned int
    union { double d; unsigned int i; } converter;
    
    //clear the memory of the union - this should
    //prevent any surprises
    converter.i = 0;
    
    //assign the memory of the union to hold the double
    converter.d = val;
    
    //now return the union as an integer - this forces
    //the bit pattern of the double to be interpreted
    //as an integer
    return converter.i;
}

inline unsigned int qHash(const float &val)
{
    //reinterpret the bits of the double as an unsigned int
    union { double d; unsigned int i; } converter;
    
    //clear the memory of the union - this should
    //prevent any surprises
    converter.i = 0;
    
    //assign the memory of the union to hold the double
    converter.d = val;
    
    //now return the union as an integer - this forces
    //the bit pattern of the double to be interpreted
    //as an integer
    return converter.i;
}

#endif
