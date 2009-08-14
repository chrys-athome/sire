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

#ifndef SIREMATHS_SIRE_LAPACK_H
#define SIREMATHS_SIRE_LAPACK_H

#include "sireglobal.h"

#include <QVector>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>

SIRE_BEGIN_HEADER

namespace SireMaths
{

class NMatrix;
class NVector;

std::pair<NVector,NMatrix> dsyev(const NMatrix &A, bool upper=true);

NVector dsyev_eigenvalues(const NMatrix &A, bool upper=true);

std::pair< NMatrix,QVector<int> > deco(const NMatrix &A);

double dgedi_determinant(const NMatrix &A, const QVector<int> &IPVT);

std::pair<double,NMatrix> dgedi(const NMatrix &A, const QVector<int> &IPVT);

}

SIRE_EXPOSE_FUNCTION( SireMaths::dsyev )
SIRE_EXPOSE_FUNCTION( SireMaths::dsyev_eigenvalues )

SIRE_EXPOSE_FUNCTION( SireMaths::deco )

SIRE_EXPOSE_FUNCTION( SireMaths::dgedi )
SIRE_EXPOSE_FUNCTION( SireMaths::dgedi_determinant )

SIRE_END_HEADER

#endif
