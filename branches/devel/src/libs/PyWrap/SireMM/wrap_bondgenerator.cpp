
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "wrapInternalGenerator.hpp"

#include "SireMM/bondgenerator.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMM
{

void  
SIREMM_EXPORT
export_BondGenerator()
{
    wrap_InternalGenerator<MolBondInfo>("InternalGenerator_MolBondInfo_");
    wrap_UsePassedInternals<MolBondInfo>("UsePassedInternals_MolBondInfo_");
    
    class_<BondGenerator, bases<BondGeneratorBase> >("BondGenerator", init<>())
        
        .def( "typeName", &BondGenerator::typeName )
        
        .def( "__rlshift__", &__rlshift__QDataStream<BondGenerator>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<BondGenerator>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
    
    class_<UsePassedBonds, bases<UsePassedBondsBase> >("UsePassedBonds", init<>())
    
        .def( init<const QSet<Bond>&>() )
        
        .def( "typeName", &UsePassedBonds::typeName )
    
        .def( "__rlshift__", &__rlshift__QDataStream<UsePassedBonds>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<UsePassedBonds>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
}

}
