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

#ifndef SIREMM_GRIDFF_H
#define SIREMM_GRIDFF_H

#include "intercljff.h"

namespace SireMM
{
class GridFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::GridFF&);
QDataStream& operator>>(QDataStream&, SireMM::GridFF&);

namespace SireMM
{

/** This class calculates the coulomb and LJ energy between
    all molecules in group 1 and all molecules in group 2.
    The calculation is optimised, as the molecules in group 2
    are represented using a grid. This is ideal for situations
    where the molecules on group 2 move little, or not at all.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT GridFF 
            : public SireBase::ConcreteProperty<GridFF,InterGroupCLJFF>
{

friend QDataStream& ::operator<<(QDataStream&, const GridFF&);
friend QDataStream& ::operator>>(QDataStream&, GridFF&);

public:
    GridFF();
    GridFF(const QString &name);
    
    GridFF(const GridFF &other);
    
    ~GridFF();
    
    static const char* typeName();
    
    const char* what() const;
    
    GridFF& operator=(const GridFF &other);
    
    bool operator==(const GridFF &other) const;
    bool operator!=(const GridFF &other) const;
    
    GridFF* clone() const;

protected:
    void recalculateEnergy();

};

}

Q_DECLARE_METATYPE( SireMM::GridFF )

SIRE_EXPOSE_CLASS( SireMM::GridFF )

#endif
