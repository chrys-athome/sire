// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "AABox.pypp.hpp"

namespace bp = boost::python;

#include "SireMaths/sphere.h"

#include "SireStream/datastream.h"

#include "aabox.h"

#include "coordgroup.h"

#include <QDebug>

#include "aabox.h"

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_AABox_class(){

    { //::SireVol::AABox
        typedef bp::class_< SireVol::AABox, boost::noncopyable > AABox_exposer_t;
        AABox_exposer_t AABox_exposer = AABox_exposer_t( "AABox" );
        bp::scope AABox_scope( AABox_exposer );
        { //::SireVol::AABox::add
        
            typedef void ( ::SireVol::AABox::*add_function_type )( ::SireVol::AABox const & ) ;
            add_function_type add_function_value( &::SireVol::AABox::add );
            
            AABox_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireVol::AABox::add
        
            typedef void ( ::SireVol::AABox::*add_function_type )( ::SireMaths::Vector const & ) ;
            add_function_type add_function_value( &::SireVol::AABox::add );
            
            AABox_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("point") ) );
        
        }
        { //::SireVol::AABox::add
        
            typedef void ( ::SireVol::AABox::*add_function_type )( ::QVector< SireMaths::Vector > const & ) ;
            add_function_type add_function_value( &::SireVol::AABox::add );
            
            AABox_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("points") ) );
        
        }
        { //::SireVol::AABox::boundingSphere
        
            typedef ::SireMaths::Sphere ( ::SireVol::AABox::*boundingSphere_function_type )(  ) const;
            boundingSphere_function_type boundingSphere_function_value( &::SireVol::AABox::boundingSphere );
            
            AABox_exposer.def( 
                "boundingSphere"
                , boundingSphere_function_value );
        
        }
        { //::SireVol::AABox::center
        
            typedef ::SireMaths::Vector const & ( ::SireVol::AABox::*center_function_type )(  ) const;
            center_function_type center_function_value( &::SireVol::AABox::center );
            
            AABox_exposer.def( 
                "center"
                , center_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireVol::AABox::from
        
            typedef ::SireVol::AABox ( *from_function_type )( ::SireMaths::Vector const & );
            from_function_type from_function_value( &::SireVol::AABox::from );
            
            AABox_exposer.def( 
                "from"
                , from_function_value
                , ( bp::arg("point") ) );
        
        }
        { //::SireVol::AABox::from
        
            typedef ::SireVol::AABox ( *from_function_type )( ::SireVol::CoordGroupBase const & );
            from_function_type from_function_value( &::SireVol::AABox::from );
            
            AABox_exposer.def( 
                "from"
                , from_function_value
                , ( bp::arg("coordgroup") ) );
        
        }
        { //::SireVol::AABox::from
        
            typedef ::SireVol::AABox ( *from_function_type )( ::SireVol::CoordGroupArray const & );
            from_function_type from_function_value( &::SireVol::AABox::from );
            
            AABox_exposer.def( 
                "from"
                , from_function_value
                , ( bp::arg("cgarray") ) );
        
        }
        { //::SireVol::AABox::from
        
            typedef ::SireVol::AABox ( *from_function_type )( ::SireVol::CoordGroupArrayArray const & );
            from_function_type from_function_value( &::SireVol::AABox::from );
            
            AABox_exposer.def( 
                "from"
                , from_function_value
                , ( bp::arg("cgarrays") ) );
        
        }
        { //::SireVol::AABox::from
        
            typedef ::SireVol::AABox ( *from_function_type )( ::QVector< SireMaths::Vector > const & );
            from_function_type from_function_value( &::SireVol::AABox::from );
            
            AABox_exposer.def( 
                "from"
                , from_function_value
                , ( bp::arg("coords") ) );
        
        }
        { //::SireVol::AABox::from
        
            typedef ::SireVol::AABox ( *from_function_type )( ::SireMaths::Vector const &,::SireMaths::Vector const & );
            from_function_type from_function_value( &::SireVol::AABox::from );
            
            AABox_exposer.def( 
                "from"
                , from_function_value
                , ( bp::arg("mincoords"), bp::arg("maxcoords") ) );
        
        }
        { //::SireVol::AABox::halfExtents
        
            typedef ::SireMaths::Vector const & ( ::SireVol::AABox::*halfExtents_function_type )(  ) const;
            halfExtents_function_type halfExtents_function_value( &::SireVol::AABox::halfExtents );
            
            AABox_exposer.def( 
                "halfExtents"
                , halfExtents_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireVol::AABox::intersects
        
            typedef bool ( ::SireVol::AABox::*intersects_function_type )( ::SireVol::AABox const & ) const;
            intersects_function_type intersects_function_value( &::SireVol::AABox::intersects );
            
            AABox_exposer.def( 
                "intersects"
                , intersects_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireVol::AABox::isEmpty
        
            typedef bool ( ::SireVol::AABox::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireVol::AABox::isEmpty );
            
            AABox_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireVol::AABox::isNull
        
            typedef bool ( ::SireVol::AABox::*isNull_function_type )(  ) const;
            isNull_function_type isNull_function_value( &::SireVol::AABox::isNull );
            
            AABox_exposer.def( 
                "isNull"
                , isNull_function_value );
        
        }
        { //::SireVol::AABox::maxCoords
        
            typedef ::SireMaths::Vector ( ::SireVol::AABox::*maxCoords_function_type )(  ) const;
            maxCoords_function_type maxCoords_function_value( &::SireVol::AABox::maxCoords );
            
            AABox_exposer.def( 
                "maxCoords"
                , maxCoords_function_value );
        
        }
        { //::SireVol::AABox::minCoords
        
            typedef ::SireMaths::Vector ( ::SireVol::AABox::*minCoords_function_type )(  ) const;
            minCoords_function_type minCoords_function_value( &::SireVol::AABox::minCoords );
            
            AABox_exposer.def( 
                "minCoords"
                , minCoords_function_value );
        
        }
        AABox_exposer.def( bp::self != bp::self );
        AABox_exposer.def( bp::self + bp::self );
        AABox_exposer.def( bp::self + bp::other< SireMaths::Vector >() );
        AABox_exposer.def( bp::self + bp::other< QVector< SireMaths::Vector > >() );
        { //::SireVol::AABox::operator=
        
            typedef ::SireVol::AABox const & ( ::SireVol::AABox::*assign_function_type )( ::SireVol::AABox const & ) ;
            assign_function_type assign_function_value( &::SireVol::AABox::operator= );
            
            AABox_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        AABox_exposer.def( bp::self == bp::self );
        { //::SireVol::AABox::radius
        
            typedef double ( ::SireVol::AABox::*radius_function_type )(  ) const;
            radius_function_type radius_function_value( &::SireVol::AABox::radius );
            
            AABox_exposer.def( 
                "radius"
                , radius_function_value );
        
        }
        { //::SireVol::AABox::recalculate
        
            typedef void ( ::SireVol::AABox::*recalculate_function_type )( ::SireVol::CoordGroupBase const & ) ;
            recalculate_function_type recalculate_function_value( &::SireVol::AABox::recalculate );
            
            AABox_exposer.def( 
                "recalculate"
                , recalculate_function_value
                , ( bp::arg("coordgroup") ) );
        
        }
        { //::SireVol::AABox::recalculate
        
            typedef void ( ::SireVol::AABox::*recalculate_function_type )( ::SireVol::CoordGroupArray const & ) ;
            recalculate_function_type recalculate_function_value( &::SireVol::AABox::recalculate );
            
            AABox_exposer.def( 
                "recalculate"
                , recalculate_function_value
                , ( bp::arg("cgarray") ) );
        
        }
        { //::SireVol::AABox::recalculate
        
            typedef void ( ::SireVol::AABox::*recalculate_function_type )( ::SireVol::CoordGroupArrayArray const & ) ;
            recalculate_function_type recalculate_function_value( &::SireVol::AABox::recalculate );
            
            AABox_exposer.def( 
                "recalculate"
                , recalculate_function_value
                , ( bp::arg("cgarrays") ) );
        
        }
        { //::SireVol::AABox::recalculate
        
            typedef void ( ::SireVol::AABox::*recalculate_function_type )( ::QVector< SireMaths::Vector > const & ) ;
            recalculate_function_type recalculate_function_value( &::SireVol::AABox::recalculate );
            
            AABox_exposer.def( 
                "recalculate"
                , recalculate_function_value
                , ( bp::arg("coordinates") ) );
        
        }
        { //::SireVol::AABox::toString
        
            typedef ::QString ( ::SireVol::AABox::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireVol::AABox::toString );
            
            AABox_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireVol::AABox::translate
        
            typedef void ( ::SireVol::AABox::*translate_function_type )( ::SireMaths::Vector const & ) ;
            translate_function_type translate_function_value( &::SireVol::AABox::translate );
            
            AABox_exposer.def( 
                "translate"
                , translate_function_value
                , ( bp::arg("delta") ) );
        
        }
        { //::SireVol::AABox::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireVol::AABox::typeName );
            
            AABox_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireVol::AABox::what
        
            typedef char const * ( ::SireVol::AABox::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireVol::AABox::what );
            
            AABox_exposer.def( 
                "what"
                , what_function_value );
        
        }
        { //::SireVol::AABox::withinDistance
        
            typedef bool ( ::SireVol::AABox::*withinDistance_function_type )( double,::SireVol::AABox const & ) const;
            withinDistance_function_type withinDistance_function_value( &::SireVol::AABox::withinDistance );
            
            AABox_exposer.def( 
                "withinDistance"
                , withinDistance_function_value
                , ( bp::arg("dist"), bp::arg("box") ) );
        
        }
        AABox_exposer.staticmethod( "from" );
        AABox_exposer.staticmethod( "typeName" );
        AABox_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireVol::AABox >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AABox_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireVol::AABox >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AABox_exposer.def( "__str__", &__str__< ::SireVol::AABox > );
        AABox_exposer.def( "__repr__", &__str__< ::SireVol::AABox > );
    }

}
