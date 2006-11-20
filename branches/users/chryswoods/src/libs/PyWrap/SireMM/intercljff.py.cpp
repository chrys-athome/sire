
#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireMM/intercljff.h"
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
export_InterCLJFF()
{
    bp::class_< InterCLJFF, bp::bases< SireMM::CLJFF > >( "InterCLJFF" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init<const SireVol::Space&,
                       const SireMM::SwitchingFunction&>()[bp::default_call_policies()] )
        .def(
            "typeName"
            , &::SireMM::InterCLJFF::typeName
            , bp::default_call_policies() )

        .def(
            "add"
            , (void (::SireMM::InterCLJFF::*)(::SireMM::Molecule const &,
                                              ::SireMM::ChargeTable const &,
                                              ::SireMM::LJTable const &))
              &::SireMM::InterCLJFF::add
            , bp::default_call_policies() )

        .staticmethod( "typeName" )
    ;
}

}

