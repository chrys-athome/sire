
/**
  * This file contains the boost::python wrapping of ChargeTable
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

// #include "SireMol/qhash_siremol.h"
//
// #include "SireDB/atomtable.h"
// #include "SireDB/wrapAtomTable.hpp"
//
// #include "SireMM/chargetable.h"
//
// #include "SireMol/molecule.h"
//
// using namespace boost::python;
//
// using namespace SireMol;
// using namespace SireDB;
//
// namespace SireMM
// {
//
// void export_ChargeTable()
// {
//     export_AtomTable<ChargeParameter>("AtomTableT_Charge_");
//
//     class_< ChargeTable,
//
//             //dynamic casting in python from TableBase up to ChargeTable is
//             //broken by the templated AtomTableT class. I can fix this by
//             //declaring that both AtomTableT and AtomTable are bases of this
//             //class. I need to spend some time working out why this is the case,
//             //as it is probably due to either a bug in my code or a bug in
//             //boost::python (my money is on a bug in my code :-)
//             bases< AtomTableT<ChargeParameter>, AtomTable >
//
//           >( "ChargeTable", init<>() )
//         .def( init<const MoleculeInfo&>() )
//         .def( init<const ChargeTable&>() )
//
//         .def( "typeName", &ChargeTable::typeName ).staticmethod("typeName")
//
//     ;
// }
//
// }
