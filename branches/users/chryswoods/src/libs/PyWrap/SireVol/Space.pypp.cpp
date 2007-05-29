// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "Space.pypp.hpp"
#include "boost/python.hpp"
#include "sirevol_headers.h"
#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"

namespace bp = boost::python;

SireVol::Space __copy__(const SireVol::Space &other){ return SireVol::Space(other); }

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireVol::Space&){ return "SireVol::Space";}

void register_Space_class(){

    bp::class_< SireVol::Space >( "Space" )    
        .def( bp::init< >() )    
        .def( bp::init< SireVol::SpaceBase const & >(( bp::arg("other") )) )    
        .def( bp::init< SireBase::Property const & >(( bp::arg("property") )) )    
        .def( 
            "base"
            , (::SireVol::SpaceBase const & ( ::SireVol::Space::* )(  ) const)( &::SireVol::Space::base )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "beyond"
            , (bool ( ::SireVol::Space::* )( double,::SireVol::CoordGroup const &,::SireVol::CoordGroup const & ) const)( &::SireVol::Space::beyond )
            , ( bp::arg("dist"), bp::arg("group0"), bp::arg("group1") ) )    
        .def( 
            "calcDist"
            , (double ( ::SireVol::Space::* )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const)( &::SireVol::Space::calcDist )
            , ( bp::arg("point0"), bp::arg("point1") ) )    
        .def( 
            "calcDist"
            , (double ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const)( &::SireVol::Space::calcDist )
            , ( bp::arg("group"), bp::arg("distmat") ) )    
        .def( 
            "calcDist"
            , (double ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const)( &::SireVol::Space::calcDist )
            , ( bp::arg("group0"), bp::arg("group1"), bp::arg("distmat") ) )    
        .def( 
            "calcDist2"
            , (double ( ::SireVol::Space::* )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const)( &::SireVol::Space::calcDist2 )
            , ( bp::arg("point0"), bp::arg("point1") ) )    
        .def( 
            "calcDist2"
            , (double ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const)( &::SireVol::Space::calcDist2 )
            , ( bp::arg("group"), bp::arg("distmat") ) )    
        .def( 
            "calcDist2"
            , (double ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const)( &::SireVol::Space::calcDist2 )
            , ( bp::arg("group0"), bp::arg("group1"), bp::arg("distmat") ) )    
        .def( 
            "calcInvDist"
            , (double ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const)( &::SireVol::Space::calcInvDist )
            , ( bp::arg("group"), bp::arg("distmat") ) )    
        .def( 
            "calcInvDist"
            , (double ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const)( &::SireVol::Space::calcInvDist )
            , ( bp::arg("group0"), bp::arg("group1"), bp::arg("distmat") ) )    
        .def( 
            "calcInvDist2"
            , (double ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const)( &::SireVol::Space::calcInvDist2 )
            , ( bp::arg("group"), bp::arg("distmat") ) )    
        .def( 
            "calcInvDist2"
            , (double ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const)( &::SireVol::Space::calcInvDist2 )
            , ( bp::arg("group0"), bp::arg("group1"), bp::arg("distmat") ) )    
        .def( 
            "changeVolume"
            , (::SireVol::Space ( ::SireVol::Space::* )( ::SireUnits::Dimension::Volume ) const)( &::SireVol::Space::changeVolume )
            , ( bp::arg("delta") ) )    
        .def( 
            "getCopiesWithin"
            , (::QList<boost::tuples::tuple<double, SireVol::CoordGroup, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type> > ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,double ) const)( &::SireVol::Space::getCopiesWithin )
            , ( bp::arg("group"), bp::arg("center"), bp::arg("dist") ) )    
        .def( 
            "getMinimumImage"
            , (::SireVol::CoordGroup ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireMaths::Vector const & ) const)( &::SireVol::Space::getMinimumImage )
            , ( bp::arg("group"), bp::arg("center") ) )    
        .def( 
            "getMinimumImage"
            , (::QVector<SireVol::CoordGroup> ( ::SireVol::Space::* )( ::QVector<SireVol::CoordGroup> const &,::SireMaths::Vector const & ) const)( &::SireVol::Space::getMinimumImage )
            , ( bp::arg("groups"), bp::arg("center") ) )    
        .def( 
            "mapFromCartesian"
            , (::SireVol::CoordGroup ( ::SireVol::Space::* )( ::SireVol::CoordGroup const & ) const)( &::SireVol::Space::mapFromCartesian )
            , ( bp::arg("group") ) )    
        .def( 
            "mapFromCartesian"
            , (::QVector<SireVol::CoordGroup> ( ::SireVol::Space::* )( ::QVector<SireVol::CoordGroup> const & ) const)( &::SireVol::Space::mapFromCartesian )
            , ( bp::arg("groups") ) )    
        .def( 
            "mapFromSelf"
            , (::SireVol::CoordGroup ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::Space const & ) const)( &::SireVol::Space::mapFromSelf )
            , ( bp::arg("group"), bp::arg("other") ) )    
        .def( 
            "mapFromSelf"
            , (::QVector<SireVol::CoordGroup> ( ::SireVol::Space::* )( ::QVector<SireVol::CoordGroup> const &,::SireVol::Space const & ) const)( &::SireVol::Space::mapFromSelf )
            , ( bp::arg("groups"), bp::arg("other") ) )    
        .def( 
            "minimumDistance"
            , (double ( ::SireVol::Space::* )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const & ) const)( &::SireVol::Space::minimumDistance )
            , ( bp::arg("group0"), bp::arg("group1") ) )    
        .def( 
            "minimumDistance"
            , (double ( ::SireVol::Space::* )( ::SireVol::CoordGroup const & ) const)( &::SireVol::Space::minimumDistance )
            , ( bp::arg("group") ) )    
        .def( "as__scope_SireBase_scope_Property", &SireVol::Space::operator ::SireBase::Property  )    
        .def( bp::self != bp::self )    
        .def( bp::self == bp::self )    
        .def( 
            "setVolume"
            , (::SireVol::Space ( ::SireVol::Space::* )( ::SireUnits::Dimension::Volume ) const)( &::SireVol::Space::setVolume )
            , ( bp::arg("volume") ) )    
        .def( 
            "volume"
            , (::SireUnits::Dimension::Volume ( ::SireVol::Space::* )(  ) const)( &::SireVol::Space::volume ) )    
        .def( 
            "what"
            , (char const * ( ::SireVol::Space::* )(  ) const)( &::SireVol::Space::what ) )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireVol::Space >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireVol::Space >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
