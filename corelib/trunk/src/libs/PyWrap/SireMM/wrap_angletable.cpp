
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

void export_AngleTable()
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
        
        .def( init<const Molecule&>() )
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
