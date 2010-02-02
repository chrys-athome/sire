// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Space.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/rangenerator.h"

#include "Siren/errors.h"

#include "Siren/stream.h"

#include "cartesian.h"

#include "space.h"

#include "space.h"

#include "Siren/logger.h"

#include "Siren/str.hpp"

void register_Space_class(){

    { //::SireVol::Space
        typedef bp::class_< SireVol::Space, bp::bases< Siren::Object >, boost::noncopyable > Space_exposer_t;
        Space_exposer_t Space_exposer = Space_exposer_t( "Space", bp::no_init );
        bp::scope Space_scope( Space_exposer );
        { //::SireVol::Space::beyond
        
            typedef bool ( ::SireVol::Space::*beyond_function_type )( double,::SireVol::CoordGroup const &,::SireVol::CoordGroup const & ) const;
            beyond_function_type beyond_function_value( &::SireVol::Space::beyond );
            
            Space_exposer.def( 
                "beyond"
                , beyond_function_value
                , ( bp::arg("dist"), bp::arg("group0"), bp::arg("group1") ) );
        
        }
        { //::SireVol::Space::beyond
        
            typedef bool ( ::SireVol::Space::*beyond_function_type )( double,::SireVol::AABox const &,::SireVol::AABox const & ) const;
            beyond_function_type beyond_function_value( &::SireVol::Space::beyond );
            
            Space_exposer.def( 
                "beyond"
                , beyond_function_value
                , ( bp::arg("dist"), bp::arg("aabox0"), bp::arg("aabox1") ) );
        
        }
        { //::SireVol::Space::calcAngle
        
            typedef ::SireUnits::Dimension::Angle ( ::SireVol::Space::*calcAngle_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            calcAngle_function_type calcAngle_function_value( &::SireVol::Space::calcAngle );
            
            Space_exposer.def( 
                "calcAngle"
                , calcAngle_function_value
                , ( bp::arg("point0"), bp::arg("point1"), bp::arg("point2") ) );
        
        }
        { //::SireVol::Space::calcDihedral
        
            typedef ::SireUnits::Dimension::Angle ( ::SireVol::Space::*calcDihedral_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const &,::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            calcDihedral_function_type calcDihedral_function_value( &::SireVol::Space::calcDihedral );
            
            Space_exposer.def( 
                "calcDihedral"
                , calcDihedral_function_value
                , ( bp::arg("point0"), bp::arg("point1"), bp::arg("point2"), bp::arg("point3") ) );
        
        }
        { //::SireVol::Space::calcDist
        
            typedef double ( ::SireVol::Space::*calcDist_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            calcDist_function_type calcDist_function_value( &::SireVol::Space::calcDist );
            
            Space_exposer.def( 
                "calcDist"
                , calcDist_function_value
                , ( bp::arg("point0"), bp::arg("point1") ) );
        
        }
        { //::SireVol::Space::calcDist
        
            typedef double ( ::SireVol::Space::*calcDist_function_type )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcDist_function_type calcDist_function_value( &::SireVol::Space::calcDist );
            
            Space_exposer.def( 
                "calcDist"
                , calcDist_function_value
                , ( bp::arg("group"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Space::calcDist
        
            typedef double ( ::SireVol::Space::*calcDist_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcDist_function_type calcDist_function_value( &::SireVol::Space::calcDist );
            
            Space_exposer.def( 
                "calcDist"
                , calcDist_function_value
                , ( bp::arg("group1"), bp::arg("group2"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Space::calcDist2
        
            typedef double ( ::SireVol::Space::*calcDist2_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            calcDist2_function_type calcDist2_function_value( &::SireVol::Space::calcDist2 );
            
            Space_exposer.def( 
                "calcDist2"
                , calcDist2_function_value
                , ( bp::arg("point0"), bp::arg("point1") ) );
        
        }
        { //::SireVol::Space::calcDist2
        
            typedef double ( ::SireVol::Space::*calcDist2_function_type )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcDist2_function_type calcDist2_function_value( &::SireVol::Space::calcDist2 );
            
            Space_exposer.def( 
                "calcDist2"
                , calcDist2_function_value
                , ( bp::arg("group"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Space::calcDist2
        
            typedef double ( ::SireVol::Space::*calcDist2_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcDist2_function_type calcDist2_function_value( &::SireVol::Space::calcDist2 );
            
            Space_exposer.def( 
                "calcDist2"
                , calcDist2_function_value
                , ( bp::arg("group1"), bp::arg("group2"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Space::calcDistVector
        
            typedef ::SireMaths::DistVector ( ::SireVol::Space::*calcDistVector_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            calcDistVector_function_type calcDistVector_function_value( &::SireVol::Space::calcDistVector );
            
            Space_exposer.def( 
                "calcDistVector"
                , calcDistVector_function_value
                , ( bp::arg("point0"), bp::arg("point1") ) );
        
        }
        { //::SireVol::Space::calcDistVectors
        
            typedef double ( ::SireVol::Space::*calcDistVectors_function_type )( ::SireVol::CoordGroup const &,::SireVol::DistVectorMatrix & ) const;
            calcDistVectors_function_type calcDistVectors_function_value( &::SireVol::Space::calcDistVectors );
            
            Space_exposer.def( 
                "calcDistVectors"
                , calcDistVectors_function_value
                , ( bp::arg("group"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Space::calcDistVectors
        
            typedef double ( ::SireVol::Space::*calcDistVectors_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistVectorMatrix & ) const;
            calcDistVectors_function_type calcDistVectors_function_value( &::SireVol::Space::calcDistVectors );
            
            Space_exposer.def( 
                "calcDistVectors"
                , calcDistVectors_function_value
                , ( bp::arg("group1"), bp::arg("group2"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Space::calcInvDist
        
            typedef double ( ::SireVol::Space::*calcInvDist_function_type )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcInvDist_function_type calcInvDist_function_value( &::SireVol::Space::calcInvDist );
            
            Space_exposer.def( 
                "calcInvDist"
                , calcInvDist_function_value
                , ( bp::arg("group"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Space::calcInvDist
        
            typedef double ( ::SireVol::Space::*calcInvDist_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcInvDist_function_type calcInvDist_function_value( &::SireVol::Space::calcInvDist );
            
            Space_exposer.def( 
                "calcInvDist"
                , calcInvDist_function_value
                , ( bp::arg("group1"), bp::arg("group2"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Space::calcInvDist2
        
            typedef double ( ::SireVol::Space::*calcInvDist2_function_type )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcInvDist2_function_type calcInvDist2_function_value( &::SireVol::Space::calcInvDist2 );
            
            Space_exposer.def( 
                "calcInvDist2"
                , calcInvDist2_function_value
                , ( bp::arg("group"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Space::calcInvDist2
        
            typedef double ( ::SireVol::Space::*calcInvDist2_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcInvDist2_function_type calcInvDist2_function_value( &::SireVol::Space::calcInvDist2 );
            
            Space_exposer.def( 
                "calcInvDist2"
                , calcInvDist2_function_value
                , ( bp::arg("group1"), bp::arg("group2"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Space::changeVolume
        
            typedef ::SireVol::SpacePtr ( ::SireVol::Space::*changeVolume_function_type )( ::SireUnits::Dimension::Volume ) const;
            changeVolume_function_type changeVolume_function_value( &::SireVol::Space::changeVolume );
            
            Space_exposer.def( 
                "changeVolume"
                , changeVolume_function_value
                , ( bp::arg("delta") ) );
        
        }
        { //::SireVol::Space::getBoxCenter
        
            typedef ::SireMaths::Vector ( ::SireVol::Space::*getBoxCenter_function_type )( ::SireMaths::Vector const & ) const;
            getBoxCenter_function_type getBoxCenter_function_value( &::SireVol::Space::getBoxCenter );
            
            Space_exposer.def( 
                "getBoxCenter"
                , getBoxCenter_function_value
                , ( bp::arg("p") ) );
        
        }
        { //::SireVol::Space::getBoxCenter
        
            typedef ::SireMaths::Vector ( ::SireVol::Space::*getBoxCenter_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            getBoxCenter_function_type getBoxCenter_function_value( &::SireVol::Space::getBoxCenter );
            
            Space_exposer.def( 
                "getBoxCenter"
                , getBoxCenter_function_value
                , ( bp::arg("p"), bp::arg("center") ) );
        
        }
        { //::SireVol::Space::getCopiesWithin
        
            typedef ::QList< boost::tuples::tuple< double, SireVol::CoordGroup, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > > ( ::SireVol::Space::*getCopiesWithin_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,double ) const;
            getCopiesWithin_function_type getCopiesWithin_function_value( &::SireVol::Space::getCopiesWithin );
            
            Space_exposer.def( 
                "getCopiesWithin"
                , getCopiesWithin_function_value
                , ( bp::arg("group"), bp::arg("center"), bp::arg("dist") ) );
        
        }
        { //::SireVol::Space::getMinimumImage
        
            typedef ::SireVol::CoordGroup ( ::SireVol::Space::*getMinimumImage_function_type )( ::SireVol::CoordGroup const &,::SireMaths::Vector const & ) const;
            getMinimumImage_function_type getMinimumImage_function_value( &::SireVol::Space::getMinimumImage );
            
            Space_exposer.def( 
                "getMinimumImage"
                , getMinimumImage_function_value
                , ( bp::arg("group"), bp::arg("center") ) );
        
        }
        { //::SireVol::Space::getMinimumImage
        
            typedef ::SireVol::CoordGroupArray ( ::SireVol::Space::*getMinimumImage_function_type )( ::SireVol::CoordGroupArray const &,::SireMaths::Vector const &,bool ) const;
            getMinimumImage_function_type getMinimumImage_function_value( &::SireVol::Space::getMinimumImage );
            
            Space_exposer.def( 
                "getMinimumImage"
                , getMinimumImage_function_value
                , ( bp::arg("groups"), bp::arg("center"), bp::arg("translate_as_one")=(bool)(false) ) );
        
        }
        { //::SireVol::Space::getMinimumImage
        
            typedef ::SireVol::AABox ( ::SireVol::Space::*getMinimumImage_function_type )( ::SireVol::AABox const &,::SireMaths::Vector const & ) const;
            getMinimumImage_function_type getMinimumImage_function_value( &::SireVol::Space::getMinimumImage );
            
            Space_exposer.def( 
                "getMinimumImage"
                , getMinimumImage_function_value
                , ( bp::arg("aabox"), bp::arg("center") ) );
        
        }
        { //::SireVol::Space::getMinimumImage
        
            typedef ::SireMaths::Vector ( ::SireVol::Space::*getMinimumImage_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            getMinimumImage_function_type getMinimumImage_function_value( &::SireVol::Space::getMinimumImage );
            
            Space_exposer.def( 
                "getMinimumImage"
                , getMinimumImage_function_value
                , ( bp::arg("point"), bp::arg("center") ) );
        
        }
        { //::SireVol::Space::getRandomPoint
        
            typedef ::SireMaths::Vector ( ::SireVol::Space::*getRandomPoint_function_type )( ::SireMaths::Vector const &,::SireMaths::RanGenerator const & ) const;
            getRandomPoint_function_type getRandomPoint_function_value( &::SireVol::Space::getRandomPoint );
            
            Space_exposer.def( 
                "getRandomPoint"
                , getRandomPoint_function_value
                , ( bp::arg("center"), bp::arg("generator") ) );
        
        }
        { //::SireVol::Space::getRandomPoint
        
            typedef ::SireMaths::Vector ( ::SireVol::Space::*getRandomPoint_function_type )( ::SireMaths::Vector const & ) const;
            getRandomPoint_function_type getRandomPoint_function_value( &::SireVol::Space::getRandomPoint );
            
            Space_exposer.def( 
                "getRandomPoint"
                , getRandomPoint_function_value
                , ( bp::arg("center") ) );
        
        }
        { //::SireVol::Space::getRandomPoint
        
            typedef ::SireMaths::Vector ( ::SireVol::Space::*getRandomPoint_function_type )( ::SireMaths::RanGenerator const & ) const;
            getRandomPoint_function_type getRandomPoint_function_value( &::SireVol::Space::getRandomPoint );
            
            Space_exposer.def( 
                "getRandomPoint"
                , getRandomPoint_function_value
                , ( bp::arg("generator") ) );
        
        }
        { //::SireVol::Space::getRandomPoint
        
            typedef ::SireMaths::Vector ( ::SireVol::Space::*getRandomPoint_function_type )(  ) const;
            getRandomPoint_function_type getRandomPoint_function_value( &::SireVol::Space::getRandomPoint );
            
            Space_exposer.def( 
                "getRandomPoint"
                , getRandomPoint_function_value );
        
        }
        { //::SireVol::Space::isCartesian
        
            typedef bool ( ::SireVol::Space::*isCartesian_function_type )(  ) const;
            isCartesian_function_type isCartesian_function_value( &::SireVol::Space::isCartesian );
            
            Space_exposer.def( 
                "isCartesian"
                , isCartesian_function_value );
        
        }
        { //::SireVol::Space::isPeriodic
        
            typedef bool ( ::SireVol::Space::*isPeriodic_function_type )(  ) const;
            isPeriodic_function_type isPeriodic_function_value( &::SireVol::Space::isPeriodic );
            
            Space_exposer.def( 
                "isPeriodic"
                , isPeriodic_function_value );
        
        }
        { //::SireVol::Space::minimumDistance
        
            typedef double ( ::SireVol::Space::*minimumDistance_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const & ) const;
            minimumDistance_function_type minimumDistance_function_value( &::SireVol::Space::minimumDistance );
            
            Space_exposer.def( 
                "minimumDistance"
                , minimumDistance_function_value
                , ( bp::arg("group0"), bp::arg("group1") ) );
        
        }
        { //::SireVol::Space::minimumDistance
        
            typedef double ( ::SireVol::Space::*minimumDistance_function_type )( ::SireVol::CoordGroup const & ) const;
            minimumDistance_function_type minimumDistance_function_value( &::SireVol::Space::minimumDistance );
            
            Space_exposer.def( 
                "minimumDistance"
                , minimumDistance_function_value
                , ( bp::arg("group") ) );
        
        }
        { //::SireVol::Space::setVolume
        
            typedef ::SireVol::SpacePtr ( ::SireVol::Space::*setVolume_function_type )( ::SireUnits::Dimension::Volume ) const;
            setVolume_function_type setVolume_function_value( &::SireVol::Space::setVolume );
            
            Space_exposer.def( 
                "setVolume"
                , setVolume_function_value
                , ( bp::arg("volume") ) );
        
        }
        { //::SireVol::Space::stream
        
            typedef void ( ::SireVol::Space::*stream_function_type )( ::Siren::Stream & ) ;
            stream_function_type stream_function_value( &::SireVol::Space::stream );
            
            Space_exposer.def( 
                "stream"
                , stream_function_value
                , ( bp::arg("s") ) );
        
        }
        { //::SireVol::Space::typeName
        
            typedef ::QString ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireVol::Space::typeName );
            
            Space_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireVol::Space::volume
        
            typedef ::SireUnits::Dimension::Volume ( ::SireVol::Space::*volume_function_type )(  ) const;
            volume_function_type volume_function_value( &::SireVol::Space::volume );
            
            Space_exposer.def( 
                "volume"
                , volume_function_value );
        
        }
        Space_exposer.staticmethod( "typeName" );
        Space_exposer.def( "__str__", &__str__< ::SireVol::Space > );
        Space_exposer.def( "__repr__", &__str__< ::SireVol::Space > );
    }

}
