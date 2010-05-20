/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "color.h"

using namespace Spier;
using namespace SireMaths;

/** Construct a default color (solid black) */
Color::Color() : rd(0), gn(0), bl(0), al(255)
{}

/** Construct a color from the specified components */
Color::Color( uchar red, uchar green, uchar blue, uchar alpha )
      : rd(red), gn(green), bl(blue), al(alpha)
{}

/** Construct a color from the vector and opacity value */
Color::Color( const Vector &colorvec, double opacity )
      : rd( uchar(colorvec.r() * 255) ), gn( uchar(colorvec.g() * 255) ),
        bl( uchar(colorvec.b() * 255) ), al( uchar(opacity * 255) )
{}

/** Destructor */
Color::~Color()
{}

/** Set the color from the vector and opacity value */
void Color::set(const Vector &colorvec, double opacity)
{
    rd = uchar(colorvec.r() * 255);
    gn = uchar(colorvec.g() * 255);
    bl = uchar(colorvec.b() * 255);
    al = uchar(opacity * 255);
}

/** Set the red component of the color */
void Color::setRed(const uchar &r)
{
    rd = r;
}

/** Set the blue component of the color */
void Color::setBlue(const uchar &b)
{
    bl = b;
}

/** Set the green component of the color */
void Color::setGreen(const uchar &g)
{
    gn = g;
}

/** Set the alpha component of the color */
void Color::setAlpha(const uchar &a)
{
    al = a;
}

/** Return the red component of the color */
const uchar& Color::red() const
{
    return rd;
}

/** Return the green component of the color */
const uchar& Color::green() const
{
    return gn;
}

/** Return the blue component of the color */
const uchar& Color::blue() const
{
    return bl;
}

/** Return the alpha component of the color */
const uchar& Color::alpha() const
{
    return al;
}

/** Return the red component as a ColorDouble (double between 0 and 1) */
double Color::r() const
{
    return rd / 255.0;
}

/** Return the green component as a ColorDouble (double between 0 and 1) */
double Color::g() const
{
    return gn / 255.0;
}

/** Return the blue component as a ColorDouble (double between 0 and 1) */
double Color::b() const
{
    return bl / 255.0;
}

/** Return the alpha component as a ColorDouble (double between 0 and 1) */
double Color::a() const
{
    return al / 255.0;
}

/** Cast this color as a vector, with the x,y,z values being the red, green and blue
    components of the color as ColorDoubles (doubles between 0 and 1, with 0 = 0.0 and 
    255 = 1.0 */
Color::operator Vector() const
{
    return Vector( rd/255.0, gn/255.0, bl/255.0 );
}
