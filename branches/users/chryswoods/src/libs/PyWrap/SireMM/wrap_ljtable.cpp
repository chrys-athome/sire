
/**
  * This file contains the boost::python wrapping of LJTable
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMM/ljtable.h"

#include "SireDB/atomtable.h"
#include "SireDB/wrapAtomTable.hpp"

#include "SireMol/molecule.h"
#include "SireQt/qdatastream.hpp"

using namespace boost::python;

using namespace SireDB;
using namespace SireMol;
using namespace SireQt;

namespace SireMM
{

void export_LJTable()
{
    export_AtomTable<LJParameter>("AtomTableT_LJParameter_");

    class_< LJTable,

            //dynamic casting in python from TableBase up to AtomTypeTable is
            //broken by the templated AtomTableT class. I can fix this by
            //declaring that both AtomTableT and AtomTable are bases of this
            //class. I need to spend some time working out why this is the case,
            //as it is probably due to either a bug in my code or a bug in
            //boost::python (my money is on a bug in my code :-)
            bases< AtomTableT<LJParameter>, AtomTable >

          >( "LJTable", init<>() )
        
        .def( init<const MoleculeInfo&>() )
        .def( init<const LJTable&>() )

        .def( "__rlshift__", &__rlshift__QDataStream<LJTable>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<LJTable>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "typeName", &LJTable::typeName ).staticmethod("typeName")
    ;
}

}
