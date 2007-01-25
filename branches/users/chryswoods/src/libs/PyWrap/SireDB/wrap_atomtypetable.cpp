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

/**
  * This file contains the boost::python wrapping of AtomTypeTable
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireDB/atomtable.h"
#include "SireDB/atomtypetable.h"
#include "SireDB/wrapAtomTable.hpp"

#include "SireMol/molecule.h"
#include "SireQt/qdatastream.hpp"

using namespace boost::python;

using namespace SireMol;
using namespace SireQt;

namespace SireDB
{

void  
SIREDB_EXPORT
export_AtomTypeTable()
{
    export_AtomTable<AtomType>("AtomTableT_AtomType_");

    class_< AtomTypeTable,

            //dynamic casting in python from TableBase up to AtomTypeTable is
            //broken by the templated AtomTableT class. I can fix this by
            //declaring that both AtomTableT and AtomTable are bases of this
            //class. I need to spend some time working out why this is the case,
            //as it is probably due to either a bug in my code or a bug in
            //boost::python (my money is on a bug in my code :-)
            bases< AtomTableT<AtomType>, AtomTable >

          >( "AtomTypeTable", init<>() )
        .def( init<const MoleculeInfo&>() )
        .def( init<const AtomTypeTable&>() )

        .def( "__rlshift__", &__rlshift__QDataStream<AtomTypeTable>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<AtomTypeTable>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "typeName", &AtomTypeTable::typeName ).staticmethod("typeName")
    ;
}

}
