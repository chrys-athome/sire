// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "AABox.pypp.hpp"
#include "boost/python.hpp"
#include "sirevol_headers.h"
#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"

namespace bp = boost::python;

SireVol::AABox __copy__(const SireVol::AABox &other){ return SireVol::AABox(other); }

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireVol::AABox&){ return "SireVol::AABox";}

void register_AABox_class(){

    bp::class_< SireVol::AABox >( "AABox" )    
        .def( bp::init< >() )    
        .def( bp::init< SireMaths::Vector const &, SireMaths::Vector const & >(( bp::arg("cent"), bp::arg("extents") )) )    
        .def( bp::init< SireVol::CoordGroup const & >(( bp::arg("coordgroup") )) )    
        .def( 
            "add"
            , (void ( ::SireVol::AABox::* )( ::SireVol::AABox const & ) )( &::SireVol::AABox::add )
            , ( bp::arg("other") ) )    
        .def( 
            "center"
            , (::SireMaths::Vector const & ( ::SireVol::AABox::* )(  ) const)( &::SireVol::AABox::center )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "halfExtents"
            , (::SireMaths::Vector const & ( ::SireVol::AABox::* )(  ) const)( &::SireVol::AABox::halfExtents )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "intersects"
            , (bool ( ::SireVol::AABox::* )( ::SireVol::AABox const & ) const)( &::SireVol::AABox::intersects )
            , ( bp::arg("other") ) )    
        .def( 
            "maxCoords"
            , (::SireMaths::Vector ( ::SireVol::AABox::* )(  ) const)( &::SireVol::AABox::maxCoords ) )    
        .def( 
            "minCoords"
            , (::SireMaths::Vector ( ::SireVol::AABox::* )(  ) const)( &::SireVol::AABox::minCoords ) )    
        .def( bp::self != bp::self )    
        .def( bp::self += bp::self )    
        .def( bp::self == bp::self )    
        .def( 
            "radius"
            , (double ( ::SireVol::AABox::* )(  ) const)( &::SireVol::AABox::radius ) )    
        .def( 
            "recalculate"
            , (void ( ::SireVol::AABox::* )( ::SireVol::CoordGroup const & ) )( &::SireVol::AABox::recalculate )
            , ( bp::arg("coordgroup") ) )    
        .def( 
            "translate"
            , (void ( ::SireVol::AABox::* )( ::SireMaths::Vector const & ) )( &::SireVol::AABox::translate )
            , ( bp::arg("delta") ) )    
        .def( 
            "withinDistance"
            , (bool ( ::SireVol::AABox::* )( double,::SireVol::AABox const & ) const)( &::SireVol::AABox::withinDistance )
            , ( bp::arg("dist"), bp::arg("box") ) )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireVol::AABox >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireVol::AABox >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
