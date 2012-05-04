// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "CoordGroupBase.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/quickcopy.hpp"

#include "SireError/errors.h"

#include "SireMaths/axisset.h"

#include "SireMaths/matrix.h"

#include "SireMaths/quaternion.h"

#include "SireMaths/rotate.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "coordgroup.h"

#include <QDebug>

#include "coordgroup.h"

const char* pvt_get_name(const SireVol::CoordGroupBase&){ return "SireVol::CoordGroupBase";}

void register_CoordGroupBase_class(){

    { //::SireVol::CoordGroupBase
        typedef bp::class_< SireVol::CoordGroupBase, boost::noncopyable > CoordGroupBase_exposer_t;
        CoordGroupBase_exposer_t CoordGroupBase_exposer = CoordGroupBase_exposer_t( "CoordGroupBase", bp::no_init );
        bp::scope CoordGroupBase_scope( CoordGroupBase_exposer );
        { //::SireVol::CoordGroupBase::aaBox
        
            typedef ::SireVol::AABox const & ( ::SireVol::CoordGroupBase::*aaBox_function_type )(  ) const;
            aaBox_function_type aaBox_function_value( &::SireVol::CoordGroupBase::aaBox );
            
            CoordGroupBase_exposer.def( 
                "aaBox"
                , aaBox_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireVol::CoordGroupBase::assertSameSize
        
            typedef void ( ::SireVol::CoordGroupBase::*assertSameSize_function_type )( ::QVector< SireMaths::Vector > const & ) const;
            assertSameSize_function_type assertSameSize_function_value( &::SireVol::CoordGroupBase::assertSameSize );
            
            CoordGroupBase_exposer.def( 
                "assertSameSize"
                , assertSameSize_function_value
                , ( bp::arg("coordinates") ) );
        
        }
        { //::SireVol::CoordGroupBase::assertSameSize
        
            typedef void ( ::SireVol::CoordGroupBase::*assertSameSize_function_type )( ::SireVol::CoordGroupBase const & ) const;
            assertSameSize_function_type assertSameSize_function_value( &::SireVol::CoordGroupBase::assertSameSize );
            
            CoordGroupBase_exposer.def( 
                "assertSameSize"
                , assertSameSize_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireVol::CoordGroupBase::assertValidIndex
        
            typedef void ( ::SireVol::CoordGroupBase::*assertValidIndex_function_type )( ::quint32 ) const;
            assertValidIndex_function_type assertValidIndex_function_value( &::SireVol::CoordGroupBase::assertValidIndex );
            
            CoordGroupBase_exposer.def( 
                "assertValidIndex"
                , assertValidIndex_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireVol::CoordGroupBase::at
        
            typedef ::SireMaths::Vector const & ( ::SireVol::CoordGroupBase::*at_function_type )( ::quint32 ) const;
            at_function_type at_function_value( &::SireVol::CoordGroupBase::at );
            
            CoordGroupBase_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireVol::CoordGroupBase::count
        
            typedef int ( ::SireVol::CoordGroupBase::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireVol::CoordGroupBase::count );
            
            CoordGroupBase_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireVol::CoordGroupBase::isEmpty
        
            typedef bool ( ::SireVol::CoordGroupBase::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireVol::CoordGroupBase::isEmpty );
            
            CoordGroupBase_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireVol::CoordGroupBase::maybeDifferent
        
            typedef bool ( ::SireVol::CoordGroupBase::*maybeDifferent_function_type )( ::SireVol::CoordGroupBase const & ) const;
            maybeDifferent_function_type maybeDifferent_function_value( &::SireVol::CoordGroupBase::maybeDifferent );
            
            CoordGroupBase_exposer.def( 
                "maybeDifferent"
                , maybeDifferent_function_value
                , ( bp::arg("other") ) );
        
        }
        CoordGroupBase_exposer.def( bp::self != bp::self );
        CoordGroupBase_exposer.def( bp::self == bp::self );
        { //::SireVol::CoordGroupBase::operator[]
        
            typedef ::SireMaths::Vector const & ( ::SireVol::CoordGroupBase::*__getitem___function_type )( ::quint32 ) const;
            __getitem___function_type __getitem___function_value( &::SireVol::CoordGroupBase::operator[] );
            
            CoordGroupBase_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireVol::CoordGroupBase::size
        
            typedef int ( ::SireVol::CoordGroupBase::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireVol::CoordGroupBase::size );
            
            CoordGroupBase_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireVol::CoordGroupBase::toVector
        
            typedef ::QVector< SireMaths::Vector > ( ::SireVol::CoordGroupBase::*toVector_function_type )(  ) const;
            toVector_function_type toVector_function_value( &::SireVol::CoordGroupBase::toVector );
            
            CoordGroupBase_exposer.def( 
                "toVector"
                , toVector_function_value );
        
        }
        { //::SireVol::CoordGroupBase::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireVol::CoordGroupBase::typeName );
            
            CoordGroupBase_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireVol::CoordGroupBase::what
        
            typedef char const * ( ::SireVol::CoordGroupBase::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireVol::CoordGroupBase::what );
            
            CoordGroupBase_exposer.def( 
                "what"
                , what_function_value );
        
        }
        CoordGroupBase_exposer.staticmethod( "typeName" );
        CoordGroupBase_exposer.def( "__str__", &pvt_get_name);
        CoordGroupBase_exposer.def( "__repr__", &pvt_get_name);
    }

}
