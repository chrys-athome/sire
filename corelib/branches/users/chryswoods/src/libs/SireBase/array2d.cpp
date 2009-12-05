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

#include "array2d.h"
#include "array2d.hpp"

#include "Siren/errors.h"
#include "Siren/stream.h"

using namespace SireBase;
using namespace Siren;

static const RegisterObject<Array2DBase> r_array2d(VIRTUAL_CLASS);
static const RegisterObject< Array2D<double> > r_array2d_double;

/** Null constructor */
Array2DBase::Array2DBase() : Extends<Array2DBase,Object>(), nrows(0), ncolumns(0)
{}

/** Construct with the specified number of rows and columns */
Array2DBase::Array2DBase(int nr, int nc)
            : Extends<Array2DBase,Object>(), nrows(nr), ncolumns(nc)
{
    if (nr < 0)
        nrows = 0;
        
    if (nc < 0)
        ncolumns = 0;
}

/** Copy constructor */
Array2DBase::Array2DBase(const Array2DBase &other)
            : Extends<Array2DBase,Object>(other), 
              nrows(other.nrows), ncolumns(other.ncolumns)
{}

/** Destructor */
Array2DBase::~Array2DBase()
{}

/** Copy assignment operator */
Array2DBase& Array2DBase::operator=(const Array2DBase &other)
{
    nrows = other.nrows;
    ncolumns = other.ncolumns;
    return *this;
}

QString Array2DBase::typeName()
{
    return "SireBase::Array2DBase";
}

void Array2DBase::stream(Stream &s)
{
    s.assertVersion<Array2DBase>(1);
    
    Schema schema = s.item<Array2DBase>();
    
    schema.data("nrows") & nrows;
    schema.data("ncolumns") & ncolumns;
    
    Object::stream( schema.base() );
}

/** Comparison operator */
bool Array2DBase::operator==(const Array2DBase &other) const
{
    return nrows == other.nrows and ncolumns == other.ncolumns;
}

/** Comparison operator */
bool Array2DBase::operator!=(const Array2DBase &other) const
{
    return nrows != other.nrows or ncolumns != other.ncolumns;
}

uint Array2DBase::hashCode() const
{
    return qHash( this->what() ) + qHash(nrows) + qHash(ncolumns);
}

/** Throw an invalid index exception */
void Array2DBase::throwInvalidIndex(int i, int j) const
{
    throw SireError::invalid_index( QObject::tr(
        "Index (%1,%2) is not valid for this matrix. The "
        "number of rows is %3, and number of columns is %4.")
            .arg(i).arg(j).arg(nrows).arg(ncolumns), CODELOC );
}
