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

#ifndef SQUIRE_HF_H
#define SQUIRE_HF_H

#include <QVector>

#include "SireMaths/vector.h"

SIRE_BEGIN_HEADER

namespace Squire
{

using SireMaths::Vector;

class S_GTO;
class PointCharge;

/** This is my first attempt at a small HF program */
class SQUIRE_EXPORT HF
{
public:
    HF();
    
    ~HF();
    
    void solve();

    void add(const S_GTO &orbital);
    void add(const PointCharge &charge);

private:
    /** All of the S-basis functions */
    QVector<S_GTO> orbs;
    
    /** All of the point charges (nuclei) */
    QVector<PointCharge> chgs;
};

class SQUIRE_EXPORT PointCharge
{
public:
    PointCharge();
    PointCharge(const Vector &center, double chg);
    PointCharge(const PointCharge &other);
    
    ~PointCharge();
    
    PointCharge& operator=(const PointCharge &other);
    
    const Vector& center() const;
    double charge() const;
    
private:
    Vector cent;
    double chg;
};

double kinetic_integral(const S_GTO &s0, const S_GTO &s1);
double overlap_integral(const S_GTO &s0, const S_GTO &s1);
double potential_integral(const PointCharge &chg, const S_GTO &s0, const S_GTO &s1);
double electron_integral(const S_GTO &A, const S_GTO &B,
                         const S_GTO &C, const S_GTO &D);

}

SIRE_EXPOSE_CLASS( Squire::HF )
SIRE_EXPOSE_CLASS( Squire::S_GTO )
SIRE_EXPOSE_CLASS( Squire::PointCharge )

SIRE_EXPOSE_FUNCTION( Squire::kinetic_integral )
SIRE_EXPOSE_FUNCTION( Squire::overlap_integral )
SIRE_EXPOSE_FUNCTION( Squire::potential_integral )
SIRE_EXPOSE_FUNCTION( Squire::electron_integral )

SIRE_END_HEADER

#endif
