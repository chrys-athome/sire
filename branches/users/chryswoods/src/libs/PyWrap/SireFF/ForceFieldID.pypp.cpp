// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ForceFieldID.pypp.hpp"
#include "boost/python.hpp"
#include "sireff_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"
#include "SireMol/cgatomid.h"
#include "SireMol/cutgroupnum.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/resid.h"
#include "SireMol/moleculeid.h"

namespace bp = boost::python;

void register_ForceFieldID_class(){

    bp::class_< SireFF::ForceFieldID >( "ForceFieldID" )    
        .def( bp::init< >() )    
        .def( bp::init< quint32 >(( bp::arg("id") )) );

}
