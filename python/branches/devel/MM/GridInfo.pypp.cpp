// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "GridInfo.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireMaths/vector.h"

#include "SireStream/datastream.h"

#include "gridinfo.h"

#include "gridinfo.h"

SireMM::GridInfo __copy__(const SireMM::GridInfo &other){ return SireMM::GridInfo(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_GridInfo_class(){

    { //::SireMM::GridInfo
        typedef bp::class_< SireMM::GridInfo > GridInfo_exposer_t;
        GridInfo_exposer_t GridInfo_exposer = GridInfo_exposer_t( "GridInfo", bp::init< >() );
        bp::scope GridInfo_scope( GridInfo_exposer );
        GridInfo_exposer.def( bp::init< SireVol::AABox const &, SireUnits::Dimension::Length >(( bp::arg("dimensions"), bp::arg("spacing") )) );
        GridInfo_exposer.def( bp::init< SireMM::GridInfo const & >(( bp::arg("other") )) );
        { //::SireMM::GridInfo::arrayToGridIndex
        
            typedef ::SireMM::GridIndex ( ::SireMM::GridInfo::*arrayToGridIndex_function_type )( int ) const;
            arrayToGridIndex_function_type arrayToGridIndex_function_value( &::SireMM::GridInfo::arrayToGridIndex );
            
            GridInfo_exposer.def( 
                "arrayToGridIndex"
                , arrayToGridIndex_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::GridInfo::at
        
            typedef ::SireMM::GridIndex ( ::SireMM::GridInfo::*at_function_type )( int ) const;
            at_function_type at_function_value( &::SireMM::GridInfo::at );
            
            GridInfo_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::GridInfo::at
        
            typedef int ( ::SireMM::GridInfo::*at_function_type )( ::SireMM::GridIndex const & ) const;
            at_function_type at_function_value( &::SireMM::GridInfo::at );
            
            GridInfo_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("idx") ) );
        
        }
        { //::SireMM::GridInfo::at
        
            typedef int ( ::SireMM::GridInfo::*at_function_type )( int,int,int ) const;
            at_function_type at_function_value( &::SireMM::GridInfo::at );
            
            GridInfo_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("i"), bp::arg("j"), bp::arg("k") ) );
        
        }
        { //::SireMM::GridInfo::at
        
            typedef int ( ::SireMM::GridInfo::*at_function_type )( ::SireMaths::Vector const & ) const;
            at_function_type at_function_value( &::SireMM::GridInfo::at );
            
            GridInfo_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("point") ) );
        
        }
        { //::SireMM::GridInfo::box
        
            typedef ::SireVol::AABox ( ::SireMM::GridInfo::*box_function_type )( int ) const;
            box_function_type box_function_value( &::SireMM::GridInfo::box );
            
            GridInfo_exposer.def( 
                "box"
                , box_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::GridInfo::box
        
            typedef ::SireVol::AABox ( ::SireMM::GridInfo::*box_function_type )( ::SireMM::GridIndex const & ) const;
            box_function_type box_function_value( &::SireMM::GridInfo::box );
            
            GridInfo_exposer.def( 
                "box"
                , box_function_value
                , ( bp::arg("idx") ) );
        
        }
        { //::SireMM::GridInfo::box
        
            typedef ::SireVol::AABox ( ::SireMM::GridInfo::*box_function_type )( ::SireMaths::Vector const & ) const;
            box_function_type box_function_value( &::SireMM::GridInfo::box );
            
            GridInfo_exposer.def( 
                "box"
                , box_function_value
                , ( bp::arg("point") ) );
        
        }
        { //::SireMM::GridInfo::count
        
            typedef int ( ::SireMM::GridInfo::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMM::GridInfo::count );
            
            GridInfo_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMM::GridInfo::dimX
        
            typedef ::qint32 ( ::SireMM::GridInfo::*dimX_function_type )(  ) const;
            dimX_function_type dimX_function_value( &::SireMM::GridInfo::dimX );
            
            GridInfo_exposer.def( 
                "dimX"
                , dimX_function_value );
        
        }
        { //::SireMM::GridInfo::dimY
        
            typedef ::qint32 ( ::SireMM::GridInfo::*dimY_function_type )(  ) const;
            dimY_function_type dimY_function_value( &::SireMM::GridInfo::dimY );
            
            GridInfo_exposer.def( 
                "dimY"
                , dimY_function_value );
        
        }
        { //::SireMM::GridInfo::dimZ
        
            typedef ::qint32 ( ::SireMM::GridInfo::*dimZ_function_type )(  ) const;
            dimZ_function_type dimZ_function_value( &::SireMM::GridInfo::dimZ );
            
            GridInfo_exposer.def( 
                "dimZ"
                , dimZ_function_value );
        
        }
        { //::SireMM::GridInfo::dimensions
        
            typedef ::SireVol::AABox ( ::SireMM::GridInfo::*dimensions_function_type )(  ) const;
            dimensions_function_type dimensions_function_value( &::SireMM::GridInfo::dimensions );
            
            GridInfo_exposer.def( 
                "dimensions"
                , dimensions_function_value );
        
        }
        { //::SireMM::GridInfo::getitem
        
            typedef ::SireMM::GridIndex ( ::SireMM::GridInfo::*getitem_function_type )( int ) const;
            getitem_function_type getitem_function_value( &::SireMM::GridInfo::getitem );
            
            GridInfo_exposer.def( 
                "getitem"
                , getitem_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::GridInfo::gridToArrayIndex
        
            typedef int ( ::SireMM::GridInfo::*gridToArrayIndex_function_type )( int,int,int ) const;
            gridToArrayIndex_function_type gridToArrayIndex_function_value( &::SireMM::GridInfo::gridToArrayIndex );
            
            GridInfo_exposer.def( 
                "gridToArrayIndex"
                , gridToArrayIndex_function_value
                , ( bp::arg("i"), bp::arg("j"), bp::arg("k") ) );
        
        }
        { //::SireMM::GridInfo::gridToArrayIndex
        
            typedef int ( ::SireMM::GridInfo::*gridToArrayIndex_function_type )( ::SireMM::GridIndex const & ) const;
            gridToArrayIndex_function_type gridToArrayIndex_function_value( &::SireMM::GridInfo::gridToArrayIndex );
            
            GridInfo_exposer.def( 
                "gridToArrayIndex"
                , gridToArrayIndex_function_value
                , ( bp::arg("idx") ) );
        
        }
        { //::SireMM::GridInfo::isEmpty
        
            typedef bool ( ::SireMM::GridInfo::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMM::GridInfo::isEmpty );
            
            GridInfo_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMM::GridInfo::nPoints
        
            typedef int ( ::SireMM::GridInfo::*nPoints_function_type )(  ) const;
            nPoints_function_type nPoints_function_value( &::SireMM::GridInfo::nPoints );
            
            GridInfo_exposer.def( 
                "nPoints"
                , nPoints_function_value );
        
        }
        GridInfo_exposer.def( bp::self != bp::self );
        { //::SireMM::GridInfo::operator=
        
            typedef ::SireMM::GridInfo & ( ::SireMM::GridInfo::*assign_function_type )( ::SireMM::GridInfo const & ) ;
            assign_function_type assign_function_value( &::SireMM::GridInfo::operator= );
            
            GridInfo_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        GridInfo_exposer.def( bp::self == bp::self );
        { //::SireMM::GridInfo::operator[]
        
            typedef ::SireMM::GridIndex ( ::SireMM::GridInfo::*__getitem___function_type )( int ) const;
            __getitem___function_type __getitem___function_value( &::SireMM::GridInfo::operator[] );
            
            GridInfo_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::GridInfo::operator[]
        
            typedef int ( ::SireMM::GridInfo::*__getitem___function_type )( ::SireMM::GridIndex const & ) const;
            __getitem___function_type __getitem___function_value( &::SireMM::GridInfo::operator[] );
            
            GridInfo_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("idx") ) );
        
        }
        { //::SireMM::GridInfo::operator[]
        
            typedef int ( ::SireMM::GridInfo::*__getitem___function_type )( ::SireMaths::Vector const & ) const;
            __getitem___function_type __getitem___function_value( &::SireMM::GridInfo::operator[] );
            
            GridInfo_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("point") ) );
        
        }
        { //::SireMM::GridInfo::point
        
            typedef ::SireMaths::Vector ( ::SireMM::GridInfo::*point_function_type )( int ) const;
            point_function_type point_function_value( &::SireMM::GridInfo::point );
            
            GridInfo_exposer.def( 
                "point"
                , point_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::GridInfo::point
        
            typedef ::SireMaths::Vector ( ::SireMM::GridInfo::*point_function_type )( ::SireMM::GridIndex const & ) const;
            point_function_type point_function_value( &::SireMM::GridInfo::point );
            
            GridInfo_exposer.def( 
                "point"
                , point_function_value
                , ( bp::arg("idx") ) );
        
        }
        { //::SireMM::GridInfo::point
        
            typedef ::SireMaths::Vector ( ::SireMM::GridInfo::*point_function_type )( ::SireMaths::Vector const & ) const;
            point_function_type point_function_value( &::SireMM::GridInfo::point );
            
            GridInfo_exposer.def( 
                "point"
                , point_function_value
                , ( bp::arg("point") ) );
        
        }
        { //::SireMM::GridInfo::pointToArrayIndex
        
            typedef int ( ::SireMM::GridInfo::*pointToArrayIndex_function_type )( ::SireMaths::Vector const & ) const;
            pointToArrayIndex_function_type pointToArrayIndex_function_value( &::SireMM::GridInfo::pointToArrayIndex );
            
            GridInfo_exposer.def( 
                "pointToArrayIndex"
                , pointToArrayIndex_function_value
                , ( bp::arg("point") ) );
        
        }
        { //::SireMM::GridInfo::pointToGridCorners
        
            typedef void ( ::SireMM::GridInfo::*pointToGridCorners_function_type )( ::SireMaths::Vector const &,::QVector< int > & ) const;
            pointToGridCorners_function_type pointToGridCorners_function_value( &::SireMM::GridInfo::pointToGridCorners );
            
            GridInfo_exposer.def( 
                "pointToGridCorners"
                , pointToGridCorners_function_value
                , ( bp::arg("point"), bp::arg("indicies") ) );
        
        }
        { //::SireMM::GridInfo::pointToGridCorners
        
            typedef void ( ::SireMM::GridInfo::*pointToGridCorners_function_type )( ::SireMaths::Vector const &,::QVector< int > &,::QVector< float > & ) const;
            pointToGridCorners_function_type pointToGridCorners_function_value( &::SireMM::GridInfo::pointToGridCorners );
            
            GridInfo_exposer.def( 
                "pointToGridCorners"
                , pointToGridCorners_function_value
                , ( bp::arg("point"), bp::arg("indicies"), bp::arg("weights") ) );
        
        }
        { //::SireMM::GridInfo::pointToGridCorners
        
            typedef int ( ::SireMM::GridInfo::*pointToGridCorners_function_type )( ::SireMaths::MultiFloat const &,::SireMaths::MultiFloat const &,::SireMaths::MultiFloat const &,::QVector< SireMaths::MultiInt > & ) const;
            pointToGridCorners_function_type pointToGridCorners_function_value( &::SireMM::GridInfo::pointToGridCorners );
            
            GridInfo_exposer.def( 
                "pointToGridCorners"
                , pointToGridCorners_function_value
                , ( bp::arg("x"), bp::arg("y"), bp::arg("z"), bp::arg("indicies") ) );
        
        }
        { //::SireMM::GridInfo::pointToGridCorners
        
            typedef int ( ::SireMM::GridInfo::*pointToGridCorners_function_type )( ::SireMaths::MultiFloat const &,::SireMaths::MultiFloat const &,::SireMaths::MultiFloat const &,::QVector< SireMaths::MultiInt > &,::QVector< SireMaths::MultiFloat > & ) const;
            pointToGridCorners_function_type pointToGridCorners_function_value( &::SireMM::GridInfo::pointToGridCorners );
            
            GridInfo_exposer.def( 
                "pointToGridCorners"
                , pointToGridCorners_function_value
                , ( bp::arg("x"), bp::arg("y"), bp::arg("z"), bp::arg("indicies"), bp::arg("weights") ) );
        
        }
        { //::SireMM::GridInfo::pointToGridIndex
        
            typedef ::SireMM::GridIndex ( ::SireMM::GridInfo::*pointToGridIndex_function_type )( ::SireMaths::Vector const & ) const;
            pointToGridIndex_function_type pointToGridIndex_function_value( &::SireMM::GridInfo::pointToGridIndex );
            
            GridInfo_exposer.def( 
                "pointToGridIndex"
                , pointToGridIndex_function_value
                , ( bp::arg("point") ) );
        
        }
        { //::SireMM::GridInfo::size
        
            typedef int ( ::SireMM::GridInfo::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMM::GridInfo::size );
            
            GridInfo_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMM::GridInfo::spacing
        
            typedef ::SireUnits::Dimension::Length ( ::SireMM::GridInfo::*spacing_function_type )(  ) const;
            spacing_function_type spacing_function_value( &::SireMM::GridInfo::spacing );
            
            GridInfo_exposer.def( 
                "spacing"
                , spacing_function_value );
        
        }
        { //::SireMM::GridInfo::toString
        
            typedef ::QString ( ::SireMM::GridInfo::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMM::GridInfo::toString );
            
            GridInfo_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMM::GridInfo::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::GridInfo::typeName );
            
            GridInfo_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::GridInfo::what
        
            typedef char const * ( ::SireMM::GridInfo::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::GridInfo::what );
            
            GridInfo_exposer.def( 
                "what"
                , what_function_value );
        
        }
        GridInfo_exposer.staticmethod( "typeName" );
        GridInfo_exposer.def( "__copy__", &__copy__);
        GridInfo_exposer.def( "__deepcopy__", &__copy__);
        GridInfo_exposer.def( "clone", &__copy__);
        GridInfo_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::GridInfo >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        GridInfo_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::GridInfo >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        GridInfo_exposer.def( "__str__", &__str__< ::SireMM::GridInfo > );
        GridInfo_exposer.def( "__repr__", &__str__< ::SireMM::GridInfo > );
        GridInfo_exposer.def( "__len__", &__len_size< ::SireMM::GridInfo > );
        GridInfo_exposer.def( "__getitem__", &::SireMM::GridInfo::getitem );
    }

}
