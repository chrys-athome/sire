
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "wrapInternalGenerator.hpp"

#include "SireMM/anglegenerator.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireMM
{

void export_AngleGenerator()
{
    wrap_InternalGenerator<MolAngleInfo>("InternalGenerator_MolAngleInfo_");
    wrap_UsePassedInternals<MolAngleInfo>("UsePassedInternals_MolAngleInfo_");
    
    class_<AngleGenerator, bases<AngleGeneratorBase> >("AngleGenerator", init<>())
        
        .def( "typeName", &AngleGenerator::typeName )
        
        .def( "__rlshift__", &__rlshift__QDataStream<AngleGenerator>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<AngleGenerator>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
    
    class_<UsePassedAngles, bases<UsePassedAnglesBase> >("UsePassedAngles", init<>())
    
        .def( init<const QSet<Angle>&>() )
        
        .def( "typeName", &UsePassedAngles::typeName )
    
        .def( "__rlshift__", &__rlshift__QDataStream<UsePassedAngles>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<UsePassedAngles>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
}

}
