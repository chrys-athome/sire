// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "InternalDB_MolAngleInfo_.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

const char* pvt_get_name(const SireMM::InternalDB<SireMM::MolAngleInfo>&){ return "SireMM::InternalDB<SireMM::MolAngleInfo>";}

void register_InternalDB_MolAngleInfo__class(){

    bp::class_< SireMM::InternalDB<SireMM::MolAngleInfo>, boost::noncopyable >( "InternalDB_MolAngleInfo_", bp::no_init )    
        .def( 
            "assignParameter"
            , (bool ( ::SireMM::InternalDB<SireMM::MolAngleInfo>::* )( ::SireMol::Angle const &,::QMap<int, unsigned> const &,::SireDB::ParameterTable & ) )( &::SireMM::InternalDB<SireMM::MolAngleInfo>::assignParameter )
            , ( bp::arg("internal"), bp::arg("relateids"), bp::arg("param_table") ) )    
        .def( "__str__", &pvt_get_name);

}
