
#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireMM/tip4pff.h"
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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(tip4pff_add_overloads, add, 1, 2);

void SIREMM_EXPORT export_Tip4PFF()
{
    class_< Tip4PFF, bases< SireMM::CLJFF > >( "Tip4PFF" )
        .def( init<>() )
        .def( init<const Space&, const SwitchingFunction&>() )
        .def( "typeName", &Tip4PFF::typeName )

        .def( "add", &Tip4PFF::add, tip4pff_add_overloads() )

        .staticmethod( "typeName" )
    ;
}

}

