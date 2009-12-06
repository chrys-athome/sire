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

#include "trigarray2d.h"
#include "trigarray2d.hpp"

#include "Siren/errors.h"
#include "Siren/stream.h"

using namespace SireBase;
using namespace Siren;

static const RegisterObject<TrigArray2DBase> r_array2d(VIRTUAL_CLASS);
static const RegisterObject< TrigArray2D<double> > r_array2d_double;

/** Null constructor */
TrigArray2DBase::TrigArray2DBase() : Extends<TrigArray2DBase,Object>(), dim(0)
{}

/** Construct with the specified dimension [dim,dim] */
TrigArray2DBase::TrigArray2DBase(int dimension)
            : Extends<TrigArray2DBase,Object>(), dim(dimension)
{
    if (dimension < 0)
        dim = 0;
}

/** Copy constructor */
TrigArray2DBase::TrigArray2DBase(const TrigArray2DBase &other)
                : Extends<TrigArray2DBase,Object>(other), dim(other.dim)
{}

/** Destructor */
TrigArray2DBase::~TrigArray2DBase()
{}

QString TrigArray2DBase::typeName()
{
    return "SireBase::TrigArray2DBase";
}

uint TrigArray2DBase::hashCode() const
{
    return qHash( TrigArray2DBase::typeName() ) + qHash(dim);
}

void TrigArray2DBase::stream(Stream &s)
{
    s.assertVersion<TrigArray2DBase>(1);
    
    Schema schema = s.item<TrigArray2DBase>();
    
    schema.data("dimension") & dim;
    
    Object::stream( schema.base() );
}

/** Copy assignment operator */
TrigArray2DBase& TrigArray2DBase::operator=(const TrigArray2DBase &other)
{
    dim = other.dim;
    return *this;
}

/** Comparison operator */
bool TrigArray2DBase::operator==(const TrigArray2DBase &other) const
{
    return dim == other.dim;
}

/** Comparison operator */
bool TrigArray2DBase::operator!=(const TrigArray2DBase &other) const
{
    return dim != other.dim;
}

/** Throw an invalid index exception */
void TrigArray2DBase::throwInvalidIndex(int i, int j) const
{
    throw Siren::invalid_index( QObject::tr(
        "Index (%1,%2) is not valid for this matrix. The "
        "number of rows is %3, and number of columns is %4.")
            .arg(i).arg(j).arg(dim).arg(dim), CODELOC );
}
