
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "wrapInternalTable.hpp"
#include "wrapBondTable.hpp"

#include "SireMM/bondtable.h"
#include "SireMM/molbondinfo.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMM
{

void export_BondTable()
{
    wrap_InternalTableBase<MolBondInfo>("InternalTableBase_MolBondInfo_");
    wrap_InternalTable<MolBondInfo,Expression>("InternalTable_MolBondInfo_Expression_");
    
    wrap_BondTable<Expression>("BondTableT_Expression_");
    
    class_<BondTable, 
            //dynamic casting in python from TableBase up to BondTable is 
            //broken by the templated BondTableT and InternalTable classes. 
            //I can fix this by declaring that both BondTableT and TableBase 
            //are bases of this class. I need to spend some time working out 
            //why this is the case, as it is probably due to either a bug in 
            //my code or a bug in boost::python (my money is on a bug in my code :-)
            bases< BondTableT<Expression>, SireDB::TableBase > 
          >( "BondTable", init<>() )
        
        .def( init<const Molecule&>() )
        .def( init<const Molecule&, const BondGeneratorBase&>() )
        .def( init<const MolBondInfo&>() )
        .def( init<const BondTable&>() )
    
        .def( "__rlshift__", &__rlshift__QDataStream<BondTable>,
                return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<BondTable>,
                return_internal_reference<1, with_custodian_and_ward<1,2> >() )
                
        .def( "typeName", &BondTable::typeName ).staticmethod("typeName")
    ;
}

}
