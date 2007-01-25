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

#ifndef SPIER_COLOR_H
#define SPIER_COLOR_H

#include "SireMaths/vector.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMaths::Vector;

/**
This class represents a color (32 bit red, green, blue, alpha). Each component has 256 values, from 0 to 255.
 
@author Christopher Woods
*/
class SPIER_EXPORT Color
{
public:
    Color();
    Color(uchar red, uchar green, uchar blue, uchar alpha=255);
    Color(const Vector &colorvec, double opacity=1.0);
    ~Color();

    void set(const Vector &colorvec, double opacity=1.0);
    
    void setRed(const uchar &r);
    void setBlue(const uchar &b);
    void setGreen(const uchar &g);
    void setAlpha(const uchar &a);

    const uchar& red() const;
    const uchar& green() const;
    const uchar& blue() const;
    const uchar& alpha() const;
    
    double r() const;
    double g() const;
    double b() const;
    double a() const;

    operator Vector() const;

private:
    /** The four components of the color (red, green, blue and alpha) */
    uchar rd, gn, bl, al;

};

}

SIRE_END_HEADER

#endif
