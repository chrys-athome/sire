/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "gridff.h"
#include "cljpotential.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireStream;
using namespace SireBase;

static const RegisterMetaType<GridFF> r_gridff;

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const GridFF &gridff)
{
    writeHeader(ds, r_gridff, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const InterGroupCLJFF&>(gridff);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, GridFF &gridff)
{
    VersionID v = readHeader(ds, r_gridff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> static_cast<InterGroupCLJFF&>(gridff);
    }
    else
        throw version_error(v, "1", r_gridff, CODELOC);
        
    return ds;
}

/** Empty constructor */
GridFF::GridFF() : ConcreteProperty<GridFF,InterGroupCLJFF>()
{}

/** Construct a grid forcefield with a specified name */
GridFF::GridFF(const QString &name) 
       : ConcreteProperty<GridFF,InterGroupCLJFF>(name)
{}

/** Copy constructor */
GridFF::GridFF(const GridFF &other)
       : ConcreteProperty<GridFF,InterGroupCLJFF>(other)
{}

/** Destructor */
GridFF::~GridFF()
{}

const char* GridFF::typeName()
{
    return QMetaType::typeName( qMetaTypeId<GridFF>() );
}

const char* GridFF::what() const
{
    return GridFF::typeName();
}

/** Copy assignment operator */
GridFF& GridFF::operator=(const GridFF &other)
{
    if (this != &other)
    {
        InterGroupCLJFF::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool GridFF::operator==(const GridFF &other) const
{
    return InterGroupCLJFF::operator==(other);
}

/** Comparison operator */
bool GridFF::operator!=(const GridFF &other) const
{
    return not GridFF::operator==(other);
}

GridFF* GridFF::clone() const
{
    return new GridFF(*this);
}

/** Recalculate the total energy */
void GridFF::recalculateEnergy()
{
    InterGroupCLJFF::recalculateEnergy();
}
