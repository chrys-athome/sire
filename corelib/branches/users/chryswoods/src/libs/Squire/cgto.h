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

#ifndef SQUIRE_CGTO_H
#define SQUIRE_CGTO_H

#include <QVector>

#include "gto.h"

SIRE_BEGIN_HEADER

namespace Squire
{
class CGTO;
}

QDataStream& operator<<(QDataStream&, const Squire::CGTO&);
QDataStream& operator>>(QDataStream&, Squire::CGTO&);

namespace Squire
{

/** This class holds a contracted set of gaussian type orbitals.
    The contracted set represents only a single orbital shell.
    All shells in the contraction must have the same
    angular momentum
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT CGTO : public SireBase::ConcreteProperty<CGTO,GTO>
{

friend QDataStream& ::operator<<(QDataStream&, const CGTO&);
friend QDataStream& ::operator>>(QDataStream&, CGTO&);

public:
	CGTO();
    
    CGTO(const CGTO &other);
    
    ~CGTO();
};

}


SIRE_END_HEADER

#endif
