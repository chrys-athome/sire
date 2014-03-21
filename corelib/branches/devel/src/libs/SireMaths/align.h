/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#ifndef SIREMATHS_ALIGN_H
#define SIREMATHS_ALIGN_H

#include "SireMaths/vector.h"
#include "SireMaths/matrix.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/axisset.h"

#include <QVector>
#include <QList>

SIRE_BEGIN_HEADER

namespace SireMaths
{
    Vector getCentroid(const QVector<Vector> &p, int n=-1);
    
    double getRMSD(const QVector<Vector> &p, const QVector<Vector> &q, int n=-1);

    AxisSet kabaschFit(const QVector<Vector> &p,
                       const QVector<Vector> &q);

    Matrix kabasch(const QVector<Vector> &p,
                   const QVector<Vector> &q);

    AxisSet getAlignment(const QVector<Vector> &p,
                         const QVector<Vector> &q,
                         bool fit=true);
    
    QVector<Vector> align(const QVector<Vector> &p,
                          const QVector<Vector> &q,
                          bool fit=true);
}

SIRE_EXPOSE_FUNCTION( SireMaths::getCentroid )
SIRE_EXPOSE_FUNCTION( SireMaths::getRMSD )
SIRE_EXPOSE_FUNCTION( SireMaths::kabasch )
SIRE_EXPOSE_FUNCTION( SireMaths::kabaschFit )
SIRE_EXPOSE_FUNCTION( SireMaths::getAlignment )
SIRE_EXPOSE_FUNCTION( SireMaths::align )

SIRE_END_HEADER

#endif
