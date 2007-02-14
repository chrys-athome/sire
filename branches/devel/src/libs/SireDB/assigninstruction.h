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

#ifndef SIREDB_ASSIGNINSTRUCTION_H
#define SIREDB_ASSIGNINSTRUCTION_H

#include "sireglobal.h"

#include <QSharedData>

SIRE_BEGIN_HEADER

namespace SireDB
{
class AssignInstruction;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::AssignInstruction&);
QDataStream& operator>>(QDataStream&, SireDB::AssignInstruction&);

namespace SireDB
{

/** This is the virtual base class of all assignment instructions.
    There are currently three types of instruction;

    (1) assignment instructions - these say what should be assigned and how
                                  (see AssignBase and assign_atoms, assign_bonds etc.)

    (2) relationship instructions - these instruct which relationships to use
                                    during the assignment (see using_relationships)

    (3) parameter instructions - these instruct which parameter databases to use
                                 during the assignment (see using_parameters)

    This class is empty, acting only to provide a common virtual base
    class so that dynamic casting may be performed.

    @author Christopher Woods
*/
class SIREDB_EXPORT AssignInstruction : public QSharedData
{
public:
    AssignInstruction()
    {}

    AssignInstruction(const AssignInstruction&)
    {}

    virtual ~AssignInstruction()
    {}

    virtual AssignInstruction* clone() const=0;
    virtual const char* what() const=0;

};

}

SIRE_END_HEADER

#endif
