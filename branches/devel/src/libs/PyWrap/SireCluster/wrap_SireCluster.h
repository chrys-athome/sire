#ifndef SIRECluster_WRAP_SIRECLUSTER_H
#define SIRECluster_WRAP_SIRECLUSTER_H

#include <Python.h>
#include <boost/python.hpp>

namespace SireCluster
{
    void export_SireCluster();
}

BOOST_PYTHON_MODULE(_Cluster)
{
    SireCluster::export_SireCluster();
}

#endif
