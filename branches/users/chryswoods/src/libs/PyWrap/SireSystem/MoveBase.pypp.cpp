// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "MoveBase.pypp.hpp"
#include "boost/python.hpp"
#include "siresystem_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireFF/ffgroupid.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireSystem::MoveBase&){ return "SireSystem::MoveBase";}

void register_MoveBase_class(){

    bp::class_< SireSystem::MoveBase, boost::noncopyable >( "MoveBase", bp::no_init )    
        .def( 
            "assertCompatibleWith"
            , (void ( ::SireSystem::MoveBase::* )( ::SireSystem::QuerySystem & ) const)( &::SireSystem::MoveBase::assertCompatibleWith )
            , ( bp::arg("system") ) )    
        .def( 
            "energyComponent"
            , (::SireCAS::Symbol const & ( ::SireSystem::MoveBase::* )(  ) const)( &::SireSystem::MoveBase::energyComponent )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "move"
            , (void ( ::SireSystem::MoveBase::* )( ::SireSystem::SimSystem & ) )( &::SireSystem::MoveBase::move )
            , ( bp::arg("system") ) )    
        .def( 
            "setEnergyComponent"
            , (void ( ::SireSystem::MoveBase::* )( ::SireCAS::Symbol const & ) )( &::SireSystem::MoveBase::setEnergyComponent )
            , ( bp::arg("symbol") ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireSystem::MoveBase::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireSystem::MoveBase::* )(  ) const)( &::SireSystem::MoveBase::what ) )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireSystem::MoveBase >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireSystem::MoveBase >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
