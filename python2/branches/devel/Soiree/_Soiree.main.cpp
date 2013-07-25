// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License


#include "boost/python.hpp"

#include "DataPoint.pypp.hpp"

#include "Gradients.pypp.hpp"

#include "PMF.pypp.hpp"

#include "TI.pypp.hpp"

namespace bp = boost::python;

#include "Soiree_containers.h"

#include "Soiree_registrars.h"

BOOST_PYTHON_MODULE(_Soiree){
    register_Soiree_objects();

    register_Soiree_containers();

    register_DataPoint_class();

    register_Gradients_class();

    register_PMF_class();

    register_TI_class();
}

