
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "wrapInternalGenerator.hpp"

#include "SireMM/dihedralgenerator.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMM
{

void export_DihedralGenerator()
{
    wrap_InternalGenerator<MolDihedralInfo>("InternalGenerator_MolDihedralInfo_");
    wrap_UsePassedInternals<MolDihedralInfo>("UsePassedInternals_MolDihedralInfo_");
    
    class_<DihedralGenerator, bases<DihedralGeneratorBase> >("DihedralGenerator", init<>())
        
        .def( "typeName", &DihedralGenerator::typeName )
        
        .def( "__rlshift__", &__rlshift__QDataStream<DihedralGenerator>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<DihedralGenerator>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
    
    class_<UsePassedDihedrals, bases<UsePassedDihedralsBase> >("UsePassedDihedrals", init<>())
    
        .def( init<const QSet<Dihedral>&>() )
        
        .def( "typeName", &UsePassedDihedrals::typeName )
    
        .def( "__rlshift__", &__rlshift__QDataStream<UsePassedDihedrals>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<UsePassedDihedrals>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
}

}
