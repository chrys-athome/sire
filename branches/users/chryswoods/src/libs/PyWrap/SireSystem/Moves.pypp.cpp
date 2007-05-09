// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "Moves.pypp.hpp"
#include "boost/python.hpp"
#include "siresystem_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireFF/ffgroupid.h"

namespace bp = boost::python;

SireSystem::Moves __copy__(const SireSystem::Moves &other){ return SireSystem::Moves(other); }

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireSystem::Moves&){ return "SireSystem::Moves";}

void register_Moves_class(){

    bp::class_< SireSystem::Moves >( "Moves" )    
        .def( bp::init< >() )    
        .def( bp::init< SireSystem::Move const & >(( bp::arg("move") )) )    
        .def( bp::init< SireSystem::MovesBase const & >(( bp::arg("moves") )) )    
        .def( 
            "count"
            , (int ( ::SireSystem::Moves::* )(  ) const)( &::SireSystem::Moves::count ) )    
        .def( 
            "initialise"
            , (void ( ::SireSystem::Moves::* )( ::SireSystem::QuerySystem & ) )( &::SireSystem::Moves::initialise )
            , ( bp::arg("system") ) )    
        .def( 
            "pause"
            , (void ( ::SireSystem::Moves::* )(  ) )( &::SireSystem::Moves::pause ) )    
        .def( 
            "percentProgress"
            , (int ( ::SireSystem::Moves::* )(  ) const)( &::SireSystem::Moves::percentProgress ) )    
        .def( 
            "play"
            , (void ( ::SireSystem::Moves::* )(  ) )( &::SireSystem::Moves::play ) )    
        .def( 
            "rerun"
            , (void ( ::SireSystem::Moves::* )( ::SireSystem::SimSystem & ) )( &::SireSystem::Moves::rerun )
            , ( bp::arg("system") ) )    
        .def( 
            "resume"
            , (void ( ::SireSystem::Moves::* )( ::SireSystem::SimSystem & ) )( &::SireSystem::Moves::resume )
            , ( bp::arg("system") ) )    
        .def( 
            "run"
            , (void ( ::SireSystem::Moves::* )( ::SireSystem::SimSystem &,::quint32 ) )( &::SireSystem::Moves::run )
            , ( bp::arg("system"), bp::arg("nmoves") ) )    
        .def( 
            "stop"
            , (void ( ::SireSystem::Moves::* )(  ) )( &::SireSystem::Moves::stop ) )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireSystem::Moves >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireSystem::Moves >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
