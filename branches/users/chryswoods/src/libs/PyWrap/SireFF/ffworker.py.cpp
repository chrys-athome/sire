
#include "boost/python.hpp"

#include "SireFF/ffprocessor.h"
#include "SireFF/forcefield.h"
#include "SireFF/ffcalculator.h"
#include "SireFF/ffworker.h"
#include "SireFF/ffthreadworker.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

namespace bp = boost::python;

namespace SireFF
{

void export_FFWorker()
{
    bp::class_< FFWorkerBase, boost::noncopyable >( "FFWorkerBase", bp::no_init )
        .def(
            "assertContains"
            , &::SireFF::FFWorkerBase::assertContains
            , ( bp::arg("component") )
            , bp::default_call_policies() )
        .def(
            "energies"
            , &::SireFF::FFWorkerBase::energies
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::FFWorkerBase::* )(  ) )( &::SireFF::FFWorkerBase::energy )
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::FFWorkerBase::* )( ::SireCAS::Function const & ) )( &::SireFF::FFWorkerBase::energy )
            , ( bp::arg("component") )
            , bp::default_call_policies() )
        .def(
            "forcefield"
            , bp::pure_virtual( &::SireFF::FFWorkerBase::forcefield )
            , bp::default_call_policies() )
        .def(
            "molecule"
            , bp::pure_virtual( &::SireFF::FFWorkerBase::molecule )
            , ( bp::arg("molid") )
            , bp::default_call_policies() )
        .def(
            "move"
            , (void ( ::SireFF::FFWorkerBase::* )( ::SireMol::Molecule const & ) )( &::SireFF::FFWorkerBase::move )
            , ( bp::arg("molecule") )
            , bp::default_call_policies() )
        .def(
            "move"
            , (void ( ::SireFF::FFWorkerBase::* )( ::SireMol::Residue const & ) )( &::SireFF::FFWorkerBase::move )
            , ( bp::arg("residue") )
            , bp::default_call_policies() )
        .def(
            "recalculateEnergy"
            , &::SireFF::FFWorkerBase::recalculateEnergy
            , bp::default_call_policies() )
        .def(
            "setForceField"
            , &::SireFF::FFWorkerBase::setForceField
            , ( bp::arg("forcefield") )
            , bp::default_call_policies() )
        .def(
            "waitUntilReady"
            , &::SireFF::FFWorkerBase::waitUntilReady
            , bp::default_call_policies() )
    ;

    bp::register_ptr_to_python< boost::shared_ptr<SireFF::FFWorkerBase> >();

    bp::class_< FFWorker, bp::bases< SireFF::FFWorkerBase, SireCluster::WorkerBase >,
                boost::noncopyable >( "FFWorker", bp::no_init )
    ;

    bp::register_ptr_to_python< boost::shared_ptr<SireFF::FFWorker> >();

    bp::class_< FFThreadWorker, bp::bases< SireFF::FFWorkerBase, SireCluster::ThreadWorker >,
                boost::noncopyable >( "FFThreadWorker", bp::no_init );
    ;
}

}
