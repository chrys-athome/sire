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

#ifndef SIREDB_ASSIGN_PARAMETERS_H
#define SIREDB_ASSIGN_PARAMETERS_H

#include "assignbase.h"
#include "matchmrdata.h"

#include "SireBase/sharedpolypointer.hpp"

#include <QList>

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
}

namespace SireDB
{
class assign_parameters;
}

QDataStream& operator<<(QDataStream&, const SireDB::assign_parameters&);
QDataStream& operator>>(QDataStream&, SireDB::assign_parameters&);

namespace SireDB
{

class ParameterTable;

using SireMol::Molecule;
using SireBase::SharedPolyPointer;

/**
This is the master object that is used to control the assignment and transfer of parameters from the database classes to the table classes. This object holds the instructions for parametisation as a collection of AssignInstruction objects. These instructions can either be information as to what needs to be parametised and where the parameters are stored (e.g. assign_atoms and using_parameters) or which relationships to use to assign the parameters (e.g. using_relationships).

@author Christopher Woods
*/
class SIREDB_EXPORT assign_parameters
{

friend QDataStream& ::operator<<(QDataStream&, const assign_parameters&);
friend QDataStream& ::operator>>(QDataStream&, assign_parameters&);

public:
    assign_parameters();

    assign_parameters(const AssignInstruction &i0);

    assign_parameters(const AssignInstruction &i0, const AssignInstruction &i1);

    assign_parameters(const AssignInstruction &i0, const AssignInstruction &i1,
                      const AssignInstruction &i2);

    assign_parameters(const AssignInstruction &i0, const AssignInstruction &i1,
                      const AssignInstruction &i2, const AssignInstruction &i3);

    assign_parameters(const AssignInstruction &i0, const AssignInstruction &i1,
                      const AssignInstruction &i2, const AssignInstruction &i3,
                      const AssignInstruction &i4);

    assign_parameters(const AssignInstruction &i0, const AssignInstruction &i1,
                      const AssignInstruction &i2, const AssignInstruction &i3,
                      const AssignInstruction &i4, const AssignInstruction &i5);

    assign_parameters(const AssignInstruction &i0, const AssignInstruction &i1,
                      const AssignInstruction &i2, const AssignInstruction &i3,
                      const AssignInstruction &i4, const AssignInstruction &i5,
                      const AssignInstruction &i6);

    assign_parameters(const AssignInstruction &i0, const AssignInstruction &i1,
                      const AssignInstruction &i2, const AssignInstruction &i3,
                      const AssignInstruction &i4, const AssignInstruction &i5,
                      const AssignInstruction &i6, const AssignInstruction &i7);

    assign_parameters(const AssignInstruction &i0, const AssignInstruction &i1,
                      const AssignInstruction &i2, const AssignInstruction &i3,
                      const AssignInstruction &i4, const AssignInstruction &i5,
                      const AssignInstruction &i6, const AssignInstruction &i7,
                      const AssignInstruction &i8);

    assign_parameters(const AssignInstruction &i0, const AssignInstruction &i1,
                      const AssignInstruction &i2, const AssignInstruction &i3,
                      const AssignInstruction &i4, const AssignInstruction &i5,
                      const AssignInstruction &i6, const AssignInstruction &i7,
                      const AssignInstruction &i8, const AssignInstruction &i9);

    assign_parameters(const assign_parameters &other);

    ~assign_parameters();

    void addInstruction(const AssignInstruction &instruc);

    ParameterTable assign(const Molecule &molecule, ParameterDB &database,
                          const MatchMRData &matchmr = MatchMRData()) const;

    ParameterTable assign(const Molecule &molecule,
                          const ParameterTable &orig_table, ParameterDB &database,
                          const MatchMRData &matchmr = MatchMRData()) const;

private:
    /** The collection of assignment instructions, which will
        be processed in turn. */
    QList< SharedPolyPointer<AssignBase> > instrctns;
};

}

Q_DECLARE_METATYPE(SireDB::assign_parameters)

SIRE_END_HEADER

#endif
