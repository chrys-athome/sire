
#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireMM/intercljff.h"
#include "SireMM/chargetable.h"
#include "SireMM/ljtable.h"

#include "SireMM/switchingfunction.h"
#include "SireMM/combiningrules.h"

#include "SireVol/space.h"

#include "SireMol/molecule.h"

using namespace boost::python;
using namespace SireVol;
using namespace SireMol;

namespace SireMM
{

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(intercljff_add_overloads, add, 1, 2);

void SIREMM_EXPORT export_InterCLJFF()
{
    class_< InterCLJFF, bases< SireMM::CLJFF > >( "InterCLJFF" )
        .def( init<>() )
        .def( init<const Space&, const SwitchingFunction&>() )
        .def( "typeName", &InterCLJFF::typeName )

        .def( "add", &InterCLJFF::add, intercljff_add_overloads() )

        .staticmethod( "typeName" )
    ;
}

}

