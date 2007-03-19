// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ResInternalInfo_Bond_.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

SireMM::ResInternalInfo<SireMol::Bond> __copy__(const SireMM::ResInternalInfo<SireMol::Bond> &other){ return SireMM::ResInternalInfo<SireMol::Bond>(other); }

const char* pvt_get_name(const SireMM::ResInternalInfo<SireMol::Bond>&){ return "SireMM::ResInternalInfo<SireMol::Bond>";}

void register_ResInternalInfo_Bond__class(){

    bp::class_< SireMM::ResInternalInfo<SireMol::Bond>, bp::bases< SireMM::detail::InternalInfo<SireMol::Bond> > >( "ResInternalInfo_Bond_" )    
        .def( bp::init< >() )    
        .def( bp::init< SireMM::MolInternalInfo<SireMol::Bond> const &, SireMol::ResNum >(( bp::arg("molinfo"), bp::arg("resnum") )) )    
        .def( bp::init< SireMM::detail::InternalInfo<SireMol::Bond> const & >(( bp::arg("other") )) )    
        .def( "__copy__", &__copy__)    
        .def( "__str__", &pvt_get_name);

}
