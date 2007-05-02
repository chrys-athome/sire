// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "Move.pypp.hpp"
#include "boost/python.hpp"
#include "siresystem_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireFF/ffgroupid.h"

namespace bp = boost::python;

SireSystem::Move __copy__(const SireSystem::Move &other){ return SireSystem::Move(other); }

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireSystem::Move&){ return "SireSystem::Move";}

void register_Move_class(){

    bp::class_< SireSystem::Move >( "Move" )    
        .def( bp::init< >() )    
        .def( bp::init< SireSystem::MoveBase const & >(( bp::arg("move") )) )    
        .def( bp::init< SireBase::SharedPolyPointer<SireSystem::MoveBase> const & >(( bp::arg("ptr") )) )    
        .def( 
            "initialise"
            , (void ( ::SireSystem::Move::* )( ::SireSystem::SimSystem const & ) )( &::SireSystem::Move::initialise )
            , ( bp::arg("system") ) )    
        .def( 
            "move"
            , (void ( ::SireSystem::Move::* )( ::SireSystem::SimSystem & ) )( &::SireSystem::Move::move )
            , ( bp::arg("system") ) )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireSystem::Move >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireSystem::Move >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
