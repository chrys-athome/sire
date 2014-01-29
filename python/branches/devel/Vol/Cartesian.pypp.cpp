// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Cartesian.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/countflops.h"

#include "SireError/errors.h"

#include "SireMaths/rangenerator.h"

#include "SireStream/datastream.h"

#include "SireVol/errors.h"

#include "cartesian.h"

#include "coordgroup.h"

#include <QDebug>

#include <cmath>

#include <limits>

#include "cartesian.h"

SireVol::Cartesian __copy__(const SireVol::Cartesian &other){ return SireVol::Cartesian(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Cartesian_class(){

    { //::SireVol::Cartesian
        typedef bp::class_< SireVol::Cartesian, bp::bases< SireVol::Space, SireBase::Property > > Cartesian_exposer_t;
        Cartesian_exposer_t Cartesian_exposer = Cartesian_exposer_t( "Cartesian", bp::init< >() );
        bp::scope Cartesian_scope( Cartesian_exposer );
        Cartesian_exposer.def( bp::init< SireVol::Cartesian const & >(( bp::arg("other") )) );
        { //::SireVol::Cartesian::beyond
        
            typedef bool ( ::SireVol::Cartesian::*beyond_function_type )( double,::SireVol::AABox const &,::SireVol::AABox const & ) const;
            beyond_function_type beyond_function_value( &::SireVol::Cartesian::beyond );
            
            Cartesian_exposer.def( 
                "beyond"
                , beyond_function_value
                , ( bp::arg("dist"), bp::arg("aabox0"), bp::arg("aabox1") ) );
        
        }
        { //::SireVol::Cartesian::beyond
        
            typedef bool ( ::SireVol::Cartesian::*beyond_function_type )( double,::SireVol::CoordGroup const &,::SireVol::CoordGroup const & ) const;
            beyond_function_type beyond_function_value( &::SireVol::Cartesian::beyond );
            
            Cartesian_exposer.def( 
                "beyond"
                , beyond_function_value
                , ( bp::arg("dist"), bp::arg("group0"), bp::arg("group1") ) );
        
        }
        { //::SireVol::Cartesian::calcAngle
        
            typedef ::SireUnits::Dimension::Angle ( ::SireVol::Cartesian::*calcAngle_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            calcAngle_function_type calcAngle_function_value( &::SireVol::Cartesian::calcAngle );
            
            Cartesian_exposer.def( 
                "calcAngle"
                , calcAngle_function_value
                , ( bp::arg("point0"), bp::arg("point1"), bp::arg("point2") ) );
        
        }
        { //::SireVol::Cartesian::calcDihedral
        
            typedef ::SireUnits::Dimension::Angle ( ::SireVol::Cartesian::*calcDihedral_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const &,::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            calcDihedral_function_type calcDihedral_function_value( &::SireVol::Cartesian::calcDihedral );
            
            Cartesian_exposer.def( 
                "calcDihedral"
                , calcDihedral_function_value
                , ( bp::arg("point0"), bp::arg("point1"), bp::arg("point2"), bp::arg("point3") ) );
        
        }
        { //::SireVol::Cartesian::calcDist
        
            typedef double ( ::SireVol::Cartesian::*calcDist_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            calcDist_function_type calcDist_function_value( &::SireVol::Cartesian::calcDist );
            
            Cartesian_exposer.def( 
                "calcDist"
                , calcDist_function_value
                , ( bp::arg("point0"), bp::arg("point1") ) );
        
        }
        { //::SireVol::Cartesian::calcDist
        
            typedef double ( ::SireVol::Cartesian::*calcDist_function_type )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcDist_function_type calcDist_function_value( &::SireVol::Cartesian::calcDist );
            
            Cartesian_exposer.def( 
                "calcDist"
                , calcDist_function_value
                , ( bp::arg("group"), bp::arg("mat") ) );
        
        }
        { //::SireVol::Cartesian::calcDist
        
            typedef double ( ::SireVol::Cartesian::*calcDist_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcDist_function_type calcDist_function_value( &::SireVol::Cartesian::calcDist );
            
            Cartesian_exposer.def( 
                "calcDist"
                , calcDist_function_value
                , ( bp::arg("group1"), bp::arg("group2"), bp::arg("mat") ) );
        
        }
        { //::SireVol::Cartesian::calcDist
        
            typedef double ( ::SireVol::Cartesian::*calcDist_function_type )( ::SireVol::CoordGroup const &,::SireMaths::Vector const &,::SireVol::DistMatrix & ) const;
            calcDist_function_type calcDist_function_value( &::SireVol::Cartesian::calcDist );
            
            Cartesian_exposer.def( 
                "calcDist"
                , calcDist_function_value
                , ( bp::arg("group"), bp::arg("point"), bp::arg("mat") ) );
        
        }
        { //::SireVol::Cartesian::calcDist2
        
            typedef double ( ::SireVol::Cartesian::*calcDist2_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            calcDist2_function_type calcDist2_function_value( &::SireVol::Cartesian::calcDist2 );
            
            Cartesian_exposer.def( 
                "calcDist2"
                , calcDist2_function_value
                , ( bp::arg("point0"), bp::arg("point1") ) );
        
        }
        { //::SireVol::Cartesian::calcDist2
        
            typedef double ( ::SireVol::Cartesian::*calcDist2_function_type )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcDist2_function_type calcDist2_function_value( &::SireVol::Cartesian::calcDist2 );
            
            Cartesian_exposer.def( 
                "calcDist2"
                , calcDist2_function_value
                , ( bp::arg("group"), bp::arg("mat") ) );
        
        }
        { //::SireVol::Cartesian::calcDist2
        
            typedef double ( ::SireVol::Cartesian::*calcDist2_function_type )( ::SireVol::CoordGroup const &,::SireMaths::Vector const &,::SireVol::DistMatrix & ) const;
            calcDist2_function_type calcDist2_function_value( &::SireVol::Cartesian::calcDist2 );
            
            Cartesian_exposer.def( 
                "calcDist2"
                , calcDist2_function_value
                , ( bp::arg("group"), bp::arg("point"), bp::arg("mat") ) );
        
        }
        { //::SireVol::Cartesian::calcDist2
        
            typedef double ( ::SireVol::Cartesian::*calcDist2_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcDist2_function_type calcDist2_function_value( &::SireVol::Cartesian::calcDist2 );
            
            Cartesian_exposer.def( 
                "calcDist2"
                , calcDist2_function_value
                , ( bp::arg("group1"), bp::arg("group2"), bp::arg("mat") ) );
        
        }
        { //::SireVol::Cartesian::calcDistVector
        
            typedef ::SireMaths::DistVector ( ::SireVol::Cartesian::*calcDistVector_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            calcDistVector_function_type calcDistVector_function_value( &::SireVol::Cartesian::calcDistVector );
            
            Cartesian_exposer.def( 
                "calcDistVector"
                , calcDistVector_function_value
                , ( bp::arg("point0"), bp::arg("point1") ) );
        
        }
        { //::SireVol::Cartesian::calcDistVectors
        
            typedef double ( ::SireVol::Cartesian::*calcDistVectors_function_type )( ::SireVol::CoordGroup const &,::SireVol::DistVectorMatrix & ) const;
            calcDistVectors_function_type calcDistVectors_function_value( &::SireVol::Cartesian::calcDistVectors );
            
            Cartesian_exposer.def( 
                "calcDistVectors"
                , calcDistVectors_function_value
                , ( bp::arg("group"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Cartesian::calcDistVectors
        
            typedef double ( ::SireVol::Cartesian::*calcDistVectors_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistVectorMatrix & ) const;
            calcDistVectors_function_type calcDistVectors_function_value( &::SireVol::Cartesian::calcDistVectors );
            
            Cartesian_exposer.def( 
                "calcDistVectors"
                , calcDistVectors_function_value
                , ( bp::arg("group1"), bp::arg("group2"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Cartesian::calcDistVectors
        
            typedef double ( ::SireVol::Cartesian::*calcDistVectors_function_type )( ::SireVol::CoordGroup const &,::SireMaths::Vector const &,::SireVol::DistVectorMatrix & ) const;
            calcDistVectors_function_type calcDistVectors_function_value( &::SireVol::Cartesian::calcDistVectors );
            
            Cartesian_exposer.def( 
                "calcDistVectors"
                , calcDistVectors_function_value
                , ( bp::arg("group"), bp::arg("point"), bp::arg("distmat") ) );
        
        }
        { //::SireVol::Cartesian::calcInvDist
        
            typedef double ( ::SireVol::Cartesian::*calcInvDist_function_type )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcInvDist_function_type calcInvDist_function_value( &::SireVol::Cartesian::calcInvDist );
            
            Cartesian_exposer.def( 
                "calcInvDist"
                , calcInvDist_function_value
                , ( bp::arg("group"), bp::arg("mat") ) );
        
        }
        { //::SireVol::Cartesian::calcInvDist
        
            typedef double ( ::SireVol::Cartesian::*calcInvDist_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcInvDist_function_type calcInvDist_function_value( &::SireVol::Cartesian::calcInvDist );
            
            Cartesian_exposer.def( 
                "calcInvDist"
                , calcInvDist_function_value
                , ( bp::arg("group1"), bp::arg("group2"), bp::arg("mat") ) );
        
        }
        { //::SireVol::Cartesian::calcInvDist2
        
            typedef double ( ::SireVol::Cartesian::*calcInvDist2_function_type )( ::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcInvDist2_function_type calcInvDist2_function_value( &::SireVol::Cartesian::calcInvDist2 );
            
            Cartesian_exposer.def( 
                "calcInvDist2"
                , calcInvDist2_function_value
                , ( bp::arg("group"), bp::arg("mat") ) );
        
        }
        { //::SireVol::Cartesian::calcInvDist2
        
            typedef double ( ::SireVol::Cartesian::*calcInvDist2_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,::SireVol::DistMatrix & ) const;
            calcInvDist2_function_type calcInvDist2_function_value( &::SireVol::Cartesian::calcInvDist2 );
            
            Cartesian_exposer.def( 
                "calcInvDist2"
                , calcInvDist2_function_value
                , ( bp::arg("group1"), bp::arg("group2"), bp::arg("mat") ) );
        
        }
        { //::SireVol::Cartesian::getBoxCenter
        
            typedef ::SireMaths::Vector ( ::SireVol::Cartesian::*getBoxCenter_function_type )( ::SireMaths::Vector const & ) const;
            getBoxCenter_function_type getBoxCenter_function_value( &::SireVol::Cartesian::getBoxCenter );
            
            Cartesian_exposer.def( 
                "getBoxCenter"
                , getBoxCenter_function_value
                , ( bp::arg("p") ) );
        
        }
        { //::SireVol::Cartesian::getBoxCenter
        
            typedef ::SireMaths::Vector ( ::SireVol::Cartesian::*getBoxCenter_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            getBoxCenter_function_type getBoxCenter_function_value( &::SireVol::Cartesian::getBoxCenter );
            
            Cartesian_exposer.def( 
                "getBoxCenter"
                , getBoxCenter_function_value
                , ( bp::arg("p"), bp::arg("center") ) );
        
        }
        { //::SireVol::Cartesian::getCopiesWithin
        
            typedef ::QList< boost::tuples::tuple< double, SireVol::CoordGroup, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > > ( ::SireVol::Cartesian::*getCopiesWithin_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const &,double ) const;
            getCopiesWithin_function_type getCopiesWithin_function_value( &::SireVol::Cartesian::getCopiesWithin );
            
            Cartesian_exposer.def( 
                "getCopiesWithin"
                , getCopiesWithin_function_value
                , ( bp::arg("group"), bp::arg("center"), bp::arg("dist") ) );
        
        }
        { //::SireVol::Cartesian::getImagesWithin
        
            typedef ::QVector< SireMaths::Vector > ( ::SireVol::Cartesian::*getImagesWithin_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const &,double ) const;
            getImagesWithin_function_type getImagesWithin_function_value( &::SireVol::Cartesian::getImagesWithin );
            
            Cartesian_exposer.def( 
                "getImagesWithin"
                , getImagesWithin_function_value
                , ( bp::arg("point"), bp::arg("center"), bp::arg("dist") ) );
        
        }
        { //::SireVol::Cartesian::getMinimumImage
        
            typedef ::SireVol::CoordGroup ( ::SireVol::Cartesian::*getMinimumImage_function_type )( ::SireVol::CoordGroup const &,::SireMaths::Vector const & ) const;
            getMinimumImage_function_type getMinimumImage_function_value( &::SireVol::Cartesian::getMinimumImage );
            
            Cartesian_exposer.def( 
                "getMinimumImage"
                , getMinimumImage_function_value
                , ( bp::arg("group"), bp::arg("center") ) );
        
        }
        { //::SireVol::Cartesian::getMinimumImage
        
            typedef ::SireVol::CoordGroupArray ( ::SireVol::Cartesian::*getMinimumImage_function_type )( ::SireVol::CoordGroupArray const &,::SireMaths::Vector const &,bool ) const;
            getMinimumImage_function_type getMinimumImage_function_value( &::SireVol::Cartesian::getMinimumImage );
            
            Cartesian_exposer.def( 
                "getMinimumImage"
                , getMinimumImage_function_value
                , ( bp::arg("groups"), bp::arg("center"), bp::arg("translate_as_one")=(bool)(false) ) );
        
        }
        { //::SireVol::Cartesian::getMinimumImage
        
            typedef ::SireVol::AABox ( ::SireVol::Cartesian::*getMinimumImage_function_type )( ::SireVol::AABox const &,::SireMaths::Vector const & ) const;
            getMinimumImage_function_type getMinimumImage_function_value( &::SireVol::Cartesian::getMinimumImage );
            
            Cartesian_exposer.def( 
                "getMinimumImage"
                , getMinimumImage_function_value
                , ( bp::arg("aabox"), bp::arg("center") ) );
        
        }
        { //::SireVol::Cartesian::getMinimumImage
        
            typedef ::SireMaths::Vector ( ::SireVol::Cartesian::*getMinimumImage_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & ) const;
            getMinimumImage_function_type getMinimumImage_function_value( &::SireVol::Cartesian::getMinimumImage );
            
            Cartesian_exposer.def( 
                "getMinimumImage"
                , getMinimumImage_function_value
                , ( bp::arg("point"), bp::arg("center") ) );
        
        }
        { //::SireVol::Cartesian::getRandomPoint
        
            typedef ::SireMaths::Vector ( ::SireVol::Cartesian::*getRandomPoint_function_type )( ::SireMaths::Vector const &,::SireMaths::RanGenerator const & ) const;
            getRandomPoint_function_type getRandomPoint_function_value( &::SireVol::Cartesian::getRandomPoint );
            
            Cartesian_exposer.def( 
                "getRandomPoint"
                , getRandomPoint_function_value
                , ( bp::arg("center"), bp::arg("generator") ) );
        
        }
        { //::SireVol::Cartesian::isCartesian
        
            typedef bool ( ::SireVol::Cartesian::*isCartesian_function_type )(  ) const;
            isCartesian_function_type isCartesian_function_value( &::SireVol::Cartesian::isCartesian );
            
            Cartesian_exposer.def( 
                "isCartesian"
                , isCartesian_function_value );
        
        }
        { //::SireVol::Cartesian::isPeriodic
        
            typedef bool ( ::SireVol::Cartesian::*isPeriodic_function_type )(  ) const;
            isPeriodic_function_type isPeriodic_function_value( &::SireVol::Cartesian::isPeriodic );
            
            Cartesian_exposer.def( 
                "isPeriodic"
                , isPeriodic_function_value );
        
        }
        { //::SireVol::Cartesian::minimumDistance
        
            typedef double ( ::SireVol::Cartesian::*minimumDistance_function_type )( ::SireVol::CoordGroup const &,::SireVol::CoordGroup const & ) const;
            minimumDistance_function_type minimumDistance_function_value( &::SireVol::Cartesian::minimumDistance );
            
            Cartesian_exposer.def( 
                "minimumDistance"
                , minimumDistance_function_value
                , ( bp::arg("group0"), bp::arg("group1") ) );
        
        }
        { //::SireVol::Cartesian::minimumDistance
        
            typedef double ( ::SireVol::Cartesian::*minimumDistance_function_type )( ::SireVol::CoordGroup const & ) const;
            minimumDistance_function_type minimumDistance_function_value( &::SireVol::Cartesian::minimumDistance );
            
            Cartesian_exposer.def( 
                "minimumDistance"
                , minimumDistance_function_value
                , ( bp::arg("group") ) );
        
        }
        { //::SireVol::Cartesian::minimumDistance
        
            typedef double ( ::SireVol::Cartesian::*minimumDistance_function_type )( ::SireVol::AABox const &,::SireVol::AABox const & ) const;
            minimumDistance_function_type minimumDistance_function_value( &::SireVol::Cartesian::minimumDistance );
            
            Cartesian_exposer.def( 
                "minimumDistance"
                , minimumDistance_function_value
                , ( bp::arg("box0"), bp::arg("box1") ) );
        
        }
        Cartesian_exposer.def( bp::self != bp::self );
        { //::SireVol::Cartesian::operator=
        
            typedef ::SireVol::Cartesian & ( ::SireVol::Cartesian::*assign_function_type )( ::SireVol::Cartesian const & ) ;
            assign_function_type assign_function_value( &::SireVol::Cartesian::operator= );
            
            Cartesian_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Cartesian_exposer.def( bp::self == bp::self );
        { //::SireVol::Cartesian::setVolume
        
            typedef ::SireVol::SpacePtr ( ::SireVol::Cartesian::*setVolume_function_type )( ::SireUnits::Dimension::Volume ) const;
            setVolume_function_type setVolume_function_value( &::SireVol::Cartesian::setVolume );
            
            Cartesian_exposer.def( 
                "setVolume"
                , setVolume_function_value
                , ( bp::arg("volume") ) );
        
        }
        { //::SireVol::Cartesian::toString
        
            typedef ::QString ( ::SireVol::Cartesian::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireVol::Cartesian::toString );
            
            Cartesian_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireVol::Cartesian::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireVol::Cartesian::typeName );
            
            Cartesian_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireVol::Cartesian::volume
        
            typedef ::SireUnits::Dimension::Volume ( ::SireVol::Cartesian::*volume_function_type )(  ) const;
            volume_function_type volume_function_value( &::SireVol::Cartesian::volume );
            
            Cartesian_exposer.def( 
                "volume"
                , volume_function_value );
        
        }
        Cartesian_exposer.staticmethod( "typeName" );
        Cartesian_exposer.def( "__copy__", &__copy__);
        Cartesian_exposer.def( "__deepcopy__", &__copy__);
        Cartesian_exposer.def( "clone", &__copy__);
        Cartesian_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireVol::Cartesian >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Cartesian_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireVol::Cartesian >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Cartesian_exposer.def( "__str__", &__str__< ::SireVol::Cartesian > );
        Cartesian_exposer.def( "__repr__", &__str__< ::SireVol::Cartesian > );
    }

}
