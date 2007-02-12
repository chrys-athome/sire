// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License


#include "boost/python.hpp"

#include "sireio_headers.h"

#include "SireMol/molecule.h"

#include "SireMol/editmol.h"

#include "IOBase.pypp.hpp"

#include "PDB.pypp.hpp"

#include "ProtoMS.pypp.hpp"

namespace bp = boost::python;

BOOST_PYTHON_MODULE(_IO){
    register_IOBase_class();

    register_PDB_class();

    register_ProtoMS_class();
}

