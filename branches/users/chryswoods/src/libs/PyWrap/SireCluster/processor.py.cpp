
#include "boost/python.hpp"

#include "SireCluster/processor.h"

namespace bp = boost::python;

namespace SireCluster
{

void SIRECLUSTER_EXPORT export_Processor()
{
    bp::class_< Processor, boost::noncopyable >( "Processor", bp::no_init )
        .def(
            "activate"
            , &::SireCluster::Processor::activate
            , bp::default_call_policies() )
        .def(
            "isActive"
            , &::SireCluster::Processor::isActive
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireCluster::Processor::name
            , bp::default_call_policies() )
        .def(
            "setName"
            , &::SireCluster::Processor::setName
            , ( bp::arg("newname") )
            , bp::default_call_policies() )
        .def(
            "what"
            , &::SireCluster::Processor::what
            , bp::default_call_policies() )
    ;

    bp::class_< WorkerBase, boost::noncopyable >( "WorkerBase", bp::no_init )
    ;

    bp::register_ptr_to_python< boost::shared_ptr<WorkerBase> >();
}

}
