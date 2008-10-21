/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREMATHS_ACCUMULATOR_H
#define SIREMATHS_ACCUMULATOR_H

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Accumulator;
class Average;
class AverageAndStddev;
class ExpAverage;
class ExpAverageAndStddev;
class MaxMin;
class Median;
}

QDataStream& operator<<(QDataStream&, const SireMaths::Accumulator&);
QDataStream& operator>>(QDataStream&, SireMaths::Accumulator&);

namespace SireMaths
{

class Accumulator...

typedef SireBase::PropPtr<Accumulator> AccumulatorPtr;

}

Q_DECLARE_METATYPE( SireMaths::Average )
Q_DECLARE_METATYPE( SireMaths::AverageAndStddev )
Q_DECLARE_METATYPE( SireMaths::ExpAverage )
Q_DECLARE_METATYPE( SireMaths::ExpAverageAndStddev )
Q_DECLARE_METATYPE( SireMaths::MaxMin )
Q_DECLARE_METATYPE( SireMaths::Median )

SIRE_EXPOSE_CLASS( SireMaths::Accumulator )
SIRE_EXPOSE_CLASS( SireMaths::Average )
SIRE_EXPOSE_CLASS( SireMaths::AverageAndStddev )
SIRE_EXPOSE_CLASS( SireMaths::ExpAverage )
SIRE_EXPOSE_CLASS( SireMaths::ExpAverageAndStddev )
SIRE_EXPOSE_CLASS( SireMaths::MaxMin )
SIRE_EXPOSE_CLASS( SireMaths::Median )

SIRE_EXPOSE_PROPERTY( SireMaths::AccumulatorPtr, SireMaths::Accumulator )

SIRE_END_HEADER

#endif
