
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "wrapInternalTable.hpp"
#include "wrapDihedralTable.hpp"

#include "SireMM/dihedraltable.h"
#include "SireMM/moldihedralinfo.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMM
{

void export_DihedralTable()
{
    wrap_InternalTableBase<MolDihedralInfo>("InternalTableBase_MolDihedralInfo_");
    wrap_InternalTable<MolDihedralInfo,Expression>("InternalTable_MolDihedralInfo_Expression_");

    wrap_DihedralTable<Expression>("DihedralTableT_Expression_");

    class_<DihedralTable,
            //dynamic casting in python from TableBase up to DihedralTable is
            //broken by the templated DihedralTableT and InternalTable classes.
            //I can fix this by declaring that both DihedralTableT and TableBase
            //are bases of this class. I need to spend some time working out
            //why this is the case, as it is probably due to either a bug in
            //my code or a bug in boost::python (my money is on a bug in my code :-)
            bases< DihedralTableT<Expression>, SireDB::TableBase >
          >( "DihedralTable", init<>() )

        .def( init<const MoleculeInfo&>() )
        .def( init<const Molecule&, const DihedralGeneratorBase&>() )
        .def( init<const MolDihedralInfo&>() )
        .def( init<const DihedralTable&>() )

        .def( "__rlshift__", &__rlshift__QDataStream<DihedralTable>,
                return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<DihedralTable>,
                return_internal_reference<1, with_custodian_and_ward<1,2> >() )

        .def( "typeName", &DihedralTable::typeName ).staticmethod("typeName")
    ;
}

}
