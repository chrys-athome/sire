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

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "wrapInternalTable.hpp"
#include "wrapAngleTable.hpp"

#include "SireMM/angletable.h"
#include "SireMM/molangleinfo.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMM
{

void  
SIREMM_EXPORT
export_AngleTable()
{
    wrap_InternalTableBase<MolAngleInfo>("InternalTableBase_MolAngleInfo_");
    wrap_InternalTable<MolAngleInfo,Expression>("InternalTable_MolAngleInfo_Expression_");

    wrap_AngleTable<Expression>("AngleTableT_Expression_");

    class_<AngleTable,
            //dynamic casting in python from TableBase up to AngleTable is
            //broken by the templated AngleTableT and InternalTable classes.
            //I can fix this by declaring that both AngleTableT and TableBase
            //are bases of this class. I need to spend some time working out
            //why this is the case, as it is probably due to either a bug in
            //my code or a bug in boost::python (my money is on a bug in my code :-)
            bases< AngleTableT<Expression>, SireDB::TableBase >
          >( "AngleTable", init<>() )

        .def( init<const MoleculeInfo&>() )
        .def( init<const Molecule&, const AngleGeneratorBase&>() )
        .def( init<const MolAngleInfo&>() )
        .def( init<const AngleTable&>() )

        .def( "__rlshift__", &__rlshift__QDataStream<AngleTable>,
                return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<AngleTable>,
                return_internal_reference<1, with_custodian_and_ward<1,2> >() )

        .def( "typeName", &AngleTable::typeName ).staticmethod("typeName")
    ;
}

}
