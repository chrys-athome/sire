
#include "boost/python.hpp"

#include "SireCluster/threadworker.h"

namespace bp = boost::python;

namespace SireCluster
{

void export_ThreadWorker()
{
    bp::class_< ThreadWorker, bp::bases< SireCluster::WorkerBase >,
                                boost::noncopyable >( "ThreadWorker", bp::no_init )
    ;
}

}
