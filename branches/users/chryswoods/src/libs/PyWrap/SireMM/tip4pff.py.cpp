
#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireMM/tip4pff.h"
#include "SireMM/chargetable.h"
#include "SireMM/ljtable.h"

#include "SireMM/switchingfunction.h"
#include "SireMM/combiningrules.h"

#include "SireVol/space.h"

#include "SireMol/molecule.h"

namespace bp = boost::python;

namespace SireMM
{

void  
SIREMM_EXPORT
export_Tip4PFF()
{
    bp::class_< Tip4PFF, bp::bases< SireMM::CLJFF > >( "Tip4PFF" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init<const SireVol::Space&,
                       const SireMM::SwitchingFunction&>()[bp::default_call_policies()] )
        .def(
            "typeName"
            , &::SireMM::Tip4PFF::typeName
            , bp::default_call_policies() )

        .def(
            "add"
            , (void (::SireMM::Tip4PFF::*)(::SireMM::Molecule const &,
                                              ::SireMM::ChargeTable const &,
                                              ::SireMM::LJTable const &))
              &::SireMM::Tip4PFF::add
            , bp::default_call_policies() )

        .staticmethod( "typeName" )
    ;
}

}

