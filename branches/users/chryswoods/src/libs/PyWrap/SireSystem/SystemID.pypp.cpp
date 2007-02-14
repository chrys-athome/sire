// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "SystemID.pypp.hpp"
#include "boost/python.hpp"
#include "siresystem_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"

namespace bp = boost::python;

void register_SystemID_class(){

    bp::class_< SireSystem::SystemID, bp::bases< SireMol::IDBase > >( "SystemID" )    
        .def( bp::init< >() )    
        .def( bp::init< quint32 >(( bp::arg("id") )) );

}
