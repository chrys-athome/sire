// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "FFThreadProcessor.pypp.hpp"
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

void register_FFThreadProcessor_class(){

    bp::class_< SireFF::FFThreadProcessor, bp::bases< SireFF::FFProcessorBase > >( "FFThreadProcessor" )    
        .def( bp::init< >() )    
        .def( bp::init< SireFF::ForceField const & >(( bp::arg("forcefield") )) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireFF::FFThreadProcessor::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireFF::FFThreadProcessor::* )(  ) const)( &::SireFF::FFThreadProcessor::what ) )    
        .staticmethod( "typeName" );

}
