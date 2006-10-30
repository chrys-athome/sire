
#include "boost/python.hpp"

#include "SireMM/intercljff.h"

namespace bp = boost::python;

namespace SireMM
{

void export_InterCLJFF()
{
    bp::class_< InterCLJFF, bp::bases< SireMM::CLJFF > >( "InterCLJFF" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def(
            "typeName"
            , &::SireMM::InterCLJFF::typeName
            , bp::default_call_policies() )
        .staticmethod( "typeName" )
    ;
}

}

