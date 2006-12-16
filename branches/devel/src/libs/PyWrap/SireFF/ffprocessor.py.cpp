
#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireFF/ffprocessor.h"
#include "SireFF/ffthreadprocessor.h"
#include "SireFF/forcefield.h"
#include "SireFF/ffcalculator.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

namespace bp = boost::python;

namespace SireFF
{

void SIREFF_EXPORT export_FFProcessor()
{
    bp::class_< FFProcessorBase, bp::bases<SireCluster::Processor>,
                boost::noncopyable >( "FFProcessorBase", bp::no_init )
        .def(
            "forcefield"
            , &::SireFF::FFProcessorBase::forcefield
            , bp::default_call_policies() )
        .def(
            "molecule"
            , &::SireFF::FFProcessorBase::molecule
            , ( bp::arg("molid") )
            , bp::default_call_policies() )
        .def(
            "setForceField"
            , &::SireFF::FFProcessorBase::setForceField
            , ( bp::arg("forcefield") )
            , bp::default_call_policies() )
    ;

    bp::class_< FFProcessor, bp::bases< SireFF::FFProcessorBase > >( "FFProcessor" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< SireFF::ForceField const & >(( bp::arg("ffield") )) )
        .def( bp::init< SireFF::FFProcessor const & >(( bp::arg("other") )) )
        .def(
            "typeName"
            , &::SireFF::FFProcessor::typeName
            , bp::default_call_policies() )
        .staticmethod( "typeName" )
    ;

    bp::class_< FFThreadProcessor, bp::bases< SireFF::FFProcessorBase > >( "FFThreadProcessor" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< SireFF::ForceField const & >(( bp::arg("ffield") )) )
        .def( bp::init< SireFF::FFThreadProcessor const & >(( bp::arg("other") )) )
        .def(
            "typeName"
            , &::SireFF::FFThreadProcessor::typeName
            , bp::default_call_policies() )
        .staticmethod( "typeName" )
    ;

}

}
