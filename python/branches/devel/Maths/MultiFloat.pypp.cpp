// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "MultiFloat.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "multidouble.h"

#include "multifloat.h"

#include "multiuint.h"

#include <QDebug>

#include <QStringList>

#include "multifloat.h"

#include "multifloat.h"

#include "multiint.h"

SireMaths::MultiFloat __copy__(const SireMaths::MultiFloat &other){ return SireMaths::MultiFloat(other); }

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_MultiFloat_class(){

    { //::SireMaths::MultiFloat
        typedef bp::class_< SireMaths::MultiFloat > MultiFloat_exposer_t;
        MultiFloat_exposer_t MultiFloat_exposer = MultiFloat_exposer_t( "MultiFloat", bp::init< >() );
        bp::scope MultiFloat_scope( MultiFloat_exposer );
        MultiFloat_exposer.def( bp::init< float >(( bp::arg("value") )) );
        MultiFloat_exposer.def( bp::init< float const *, int >(( bp::arg("array"), bp::arg("size") )) );
        MultiFloat_exposer.def( bp::init< QVector< float > const & >(( bp::arg("array") )) );
        MultiFloat_exposer.def( bp::init< QVector< double > const & >(( bp::arg("array") )) );
        MultiFloat_exposer.def( bp::init< SireMaths::MultiDouble const & >(( bp::arg("other") )) );
        MultiFloat_exposer.def( bp::init< SireMaths::MultiFloat const & >(( bp::arg("other") )) );
        { //::SireMaths::MultiFloat::compareEqual
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*compareEqual_function_type )( ::SireMaths::MultiFloat const & ) const;
            compareEqual_function_type compareEqual_function_value( &::SireMaths::MultiFloat::compareEqual );
            
            MultiFloat_exposer.def( 
                "compareEqual"
                , compareEqual_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::compareGreater
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*compareGreater_function_type )( ::SireMaths::MultiFloat const & ) const;
            compareGreater_function_type compareGreater_function_value( &::SireMaths::MultiFloat::compareGreater );
            
            MultiFloat_exposer.def( 
                "compareGreater"
                , compareGreater_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::compareGreaterEqual
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*compareGreaterEqual_function_type )( ::SireMaths::MultiFloat const & ) const;
            compareGreaterEqual_function_type compareGreaterEqual_function_value( &::SireMaths::MultiFloat::compareGreaterEqual );
            
            MultiFloat_exposer.def( 
                "compareGreaterEqual"
                , compareGreaterEqual_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::compareLess
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*compareLess_function_type )( ::SireMaths::MultiFloat const & ) const;
            compareLess_function_type compareLess_function_value( &::SireMaths::MultiFloat::compareLess );
            
            MultiFloat_exposer.def( 
                "compareLess"
                , compareLess_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::compareLessEqual
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*compareLessEqual_function_type )( ::SireMaths::MultiFloat const & ) const;
            compareLessEqual_function_type compareLessEqual_function_value( &::SireMaths::MultiFloat::compareLessEqual );
            
            MultiFloat_exposer.def( 
                "compareLessEqual"
                , compareLessEqual_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::compareNotEqual
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*compareNotEqual_function_type )( ::SireMaths::MultiFloat const & ) const;
            compareNotEqual_function_type compareNotEqual_function_value( &::SireMaths::MultiFloat::compareNotEqual );
            
            MultiFloat_exposer.def( 
                "compareNotEqual"
                , compareNotEqual_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::count
        
            typedef int ( *count_function_type )(  );
            count_function_type count_function_value( &::SireMaths::MultiFloat::count );
            
            MultiFloat_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMaths::MultiFloat::doubleSum
        
            typedef double ( ::SireMaths::MultiFloat::*doubleSum_function_type )(  ) const;
            doubleSum_function_type doubleSum_function_value( &::SireMaths::MultiFloat::doubleSum );
            
            MultiFloat_exposer.def( 
                "doubleSum"
                , doubleSum_function_value );
        
        }
        { //::SireMaths::MultiFloat::fromArray
        
            typedef ::QVector< SireMaths::MultiFloat > ( *fromArray_function_type )( ::QVector< double > const & );
            fromArray_function_type fromArray_function_value( &::SireMaths::MultiFloat::fromArray );
            
            MultiFloat_exposer.def( 
                "fromArray"
                , fromArray_function_value
                , ( bp::arg("array") ) );
        
        }
        { //::SireMaths::MultiFloat::fromArray
        
            typedef ::QVector< SireMaths::MultiFloat > ( *fromArray_function_type )( ::QVector< float > const & );
            fromArray_function_type fromArray_function_value( &::SireMaths::MultiFloat::fromArray );
            
            MultiFloat_exposer.def( 
                "fromArray"
                , fromArray_function_value
                , ( bp::arg("array") ) );
        
        }
        { //::SireMaths::MultiFloat::fromArray
        
            typedef ::QVector< SireMaths::MultiFloat > ( *fromArray_function_type )( double const *,int );
            fromArray_function_type fromArray_function_value( &::SireMaths::MultiFloat::fromArray );
            
            MultiFloat_exposer.def( 
                "fromArray"
                , fromArray_function_value
                , ( bp::arg("array"), bp::arg("size") ) );
        
        }
        { //::SireMaths::MultiFloat::fromArray
        
            typedef ::QVector< SireMaths::MultiFloat > ( *fromArray_function_type )( float const *,int );
            fromArray_function_type fromArray_function_value( &::SireMaths::MultiFloat::fromArray );
            
            MultiFloat_exposer.def( 
                "fromArray"
                , fromArray_function_value
                , ( bp::arg("array"), bp::arg("size") ) );
        
        }
        { //::SireMaths::MultiFloat::get
        
            typedef float ( ::SireMaths::MultiFloat::*get_function_type )( int ) const;
            get_function_type get_function_value( &::SireMaths::MultiFloat::get );
            
            MultiFloat_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMaths::MultiFloat::hasBinaryOne
        
            typedef bool ( ::SireMaths::MultiFloat::*hasBinaryOne_function_type )(  ) const;
            hasBinaryOne_function_type hasBinaryOne_function_value( &::SireMaths::MultiFloat::hasBinaryOne );
            
            MultiFloat_exposer.def( 
                "hasBinaryOne"
                , hasBinaryOne_function_value );
        
        }
        { //::SireMaths::MultiFloat::hasBinaryZero
        
            typedef bool ( ::SireMaths::MultiFloat::*hasBinaryZero_function_type )(  ) const;
            hasBinaryZero_function_type hasBinaryZero_function_value( &::SireMaths::MultiFloat::hasBinaryZero );
            
            MultiFloat_exposer.def( 
                "hasBinaryZero"
                , hasBinaryZero_function_value );
        
        }
        { //::SireMaths::MultiFloat::isAligned
        
            typedef bool ( ::SireMaths::MultiFloat::*isAligned_function_type )(  ) const;
            isAligned_function_type isAligned_function_value( &::SireMaths::MultiFloat::isAligned );
            
            MultiFloat_exposer.def( 
                "isAligned"
                , isAligned_function_value );
        
        }
        { //::SireMaths::MultiFloat::isBinaryOne
        
            typedef bool ( ::SireMaths::MultiFloat::*isBinaryOne_function_type )(  ) const;
            isBinaryOne_function_type isBinaryOne_function_value( &::SireMaths::MultiFloat::isBinaryOne );
            
            MultiFloat_exposer.def( 
                "isBinaryOne"
                , isBinaryOne_function_value );
        
        }
        { //::SireMaths::MultiFloat::isBinaryZero
        
            typedef bool ( ::SireMaths::MultiFloat::*isBinaryZero_function_type )(  ) const;
            isBinaryZero_function_type isBinaryZero_function_value( &::SireMaths::MultiFloat::isBinaryZero );
            
            MultiFloat_exposer.def( 
                "isBinaryZero"
                , isBinaryZero_function_value );
        
        }
        { //::SireMaths::MultiFloat::isNotBinaryOne
        
            typedef bool ( ::SireMaths::MultiFloat::*isNotBinaryOne_function_type )(  ) const;
            isNotBinaryOne_function_type isNotBinaryOne_function_value( &::SireMaths::MultiFloat::isNotBinaryOne );
            
            MultiFloat_exposer.def( 
                "isNotBinaryOne"
                , isNotBinaryOne_function_value );
        
        }
        { //::SireMaths::MultiFloat::isNotBinaryZero
        
            typedef bool ( ::SireMaths::MultiFloat::*isNotBinaryZero_function_type )(  ) const;
            isNotBinaryZero_function_type isNotBinaryZero_function_value( &::SireMaths::MultiFloat::isNotBinaryZero );
            
            MultiFloat_exposer.def( 
                "isNotBinaryZero"
                , isNotBinaryZero_function_value );
        
        }
        { //::SireMaths::MultiFloat::logicalAnd
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*logicalAnd_function_type )( ::SireMaths::MultiFloat const & ) const;
            logicalAnd_function_type logicalAnd_function_value( &::SireMaths::MultiFloat::logicalAnd );
            
            MultiFloat_exposer.def( 
                "logicalAnd"
                , logicalAnd_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::logicalAnd
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*logicalAnd_function_type )( ::SireMaths::MultiUInt const & ) const;
            logicalAnd_function_type logicalAnd_function_value( &::SireMaths::MultiFloat::logicalAnd );
            
            MultiFloat_exposer.def( 
                "logicalAnd"
                , logicalAnd_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::logicalAnd
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*logicalAnd_function_type )( ::SireMaths::MultiInt const & ) const;
            logicalAnd_function_type logicalAnd_function_value( &::SireMaths::MultiFloat::logicalAnd );
            
            MultiFloat_exposer.def( 
                "logicalAnd"
                , logicalAnd_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::logicalAndNot
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*logicalAndNot_function_type )( ::SireMaths::MultiFloat const & ) const;
            logicalAndNot_function_type logicalAndNot_function_value( &::SireMaths::MultiFloat::logicalAndNot );
            
            MultiFloat_exposer.def( 
                "logicalAndNot"
                , logicalAndNot_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::logicalAndNot
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*logicalAndNot_function_type )( ::SireMaths::MultiInt const & ) const;
            logicalAndNot_function_type logicalAndNot_function_value( &::SireMaths::MultiFloat::logicalAndNot );
            
            MultiFloat_exposer.def( 
                "logicalAndNot"
                , logicalAndNot_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::logicalNot
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*logicalNot_function_type )(  ) const;
            logicalNot_function_type logicalNot_function_value( &::SireMaths::MultiFloat::logicalNot );
            
            MultiFloat_exposer.def( 
                "logicalNot"
                , logicalNot_function_value );
        
        }
        { //::SireMaths::MultiFloat::logicalOr
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*logicalOr_function_type )( ::SireMaths::MultiFloat const & ) const;
            logicalOr_function_type logicalOr_function_value( &::SireMaths::MultiFloat::logicalOr );
            
            MultiFloat_exposer.def( 
                "logicalOr"
                , logicalOr_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::logicalXor
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*logicalXor_function_type )( ::SireMaths::MultiFloat const & ) const;
            logicalXor_function_type logicalXor_function_value( &::SireMaths::MultiFloat::logicalXor );
            
            MultiFloat_exposer.def( 
                "logicalXor"
                , logicalXor_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::max
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*max_function_type )( ::SireMaths::MultiFloat const & ) const;
            max_function_type max_function_value( &::SireMaths::MultiFloat::max );
            
            MultiFloat_exposer.def( 
                "max"
                , max_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::min
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*min_function_type )( ::SireMaths::MultiFloat const & ) const;
            min_function_type min_function_value( &::SireMaths::MultiFloat::min );
            
            MultiFloat_exposer.def( 
                "min"
                , min_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMaths::MultiFloat::multiplyAdd
        
            typedef ::SireMaths::MultiFloat & ( ::SireMaths::MultiFloat::*multiplyAdd_function_type )( ::SireMaths::MultiFloat const &,::SireMaths::MultiFloat const & ) ;
            multiplyAdd_function_type multiplyAdd_function_value( &::SireMaths::MultiFloat::multiplyAdd );
            
            MultiFloat_exposer.def( 
                "multiplyAdd"
                , multiplyAdd_function_value
                , ( bp::arg("val0"), bp::arg("val1") )
                , bp::return_self< >() );
        
        }
        MultiFloat_exposer.def( !bp::self );
        MultiFloat_exposer.def( bp::self != bp::self );
        MultiFloat_exposer.def( bp::self & bp::self );
        MultiFloat_exposer.def( bp::self * bp::self );
        MultiFloat_exposer.def( bp::self + bp::self );
        MultiFloat_exposer.def( -bp::self );
        MultiFloat_exposer.def( bp::self - bp::self );
        MultiFloat_exposer.def( bp::self / bp::self );
        MultiFloat_exposer.def( bp::self < bp::self );
        MultiFloat_exposer.def( bp::self <= bp::self );
        { //::SireMaths::MultiFloat::operator=
        
            typedef ::SireMaths::MultiFloat & ( ::SireMaths::MultiFloat::*assign_function_type )( ::SireMaths::MultiFloat const & ) ;
            assign_function_type assign_function_value( &::SireMaths::MultiFloat::operator= );
            
            MultiFloat_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMaths::MultiFloat::operator=
        
            typedef ::SireMaths::MultiFloat & ( ::SireMaths::MultiFloat::*assign_function_type )( ::SireMaths::MultiDouble const & ) ;
            assign_function_type assign_function_value( &::SireMaths::MultiFloat::operator= );
            
            MultiFloat_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMaths::MultiFloat::operator=
        
            typedef ::SireMaths::MultiFloat & ( ::SireMaths::MultiFloat::*assign_function_type )( float ) ;
            assign_function_type assign_function_value( &::SireMaths::MultiFloat::operator= );
            
            MultiFloat_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("value") )
                , bp::return_self< >() );
        
        }
        MultiFloat_exposer.def( bp::self == bp::self );
        MultiFloat_exposer.def( bp::self > bp::self );
        MultiFloat_exposer.def( bp::self >= bp::self );
        { //::SireMaths::MultiFloat::operator[]
        
            typedef float ( ::SireMaths::MultiFloat::*__getitem___function_type )( int ) const;
            __getitem___function_type __getitem___function_value( &::SireMaths::MultiFloat::operator[] );
            
            MultiFloat_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") ) );
        
        }
        MultiFloat_exposer.def( bp::self ^ bp::self );
        MultiFloat_exposer.def( bp::self | bp::self );
        { //::SireMaths::MultiFloat::reciprocal
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*reciprocal_function_type )(  ) const;
            reciprocal_function_type reciprocal_function_value( &::SireMaths::MultiFloat::reciprocal );
            
            MultiFloat_exposer.def( 
                "reciprocal"
                , reciprocal_function_value );
        
        }
        { //::SireMaths::MultiFloat::reciprocal_approx
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*reciprocal_approx_function_type )(  ) const;
            reciprocal_approx_function_type reciprocal_approx_function_value( &::SireMaths::MultiFloat::reciprocal_approx );
            
            MultiFloat_exposer.def( 
                "reciprocal_approx"
                , reciprocal_approx_function_value );
        
        }
        { //::SireMaths::MultiFloat::reciprocal_approx_nr
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*reciprocal_approx_nr_function_type )(  ) const;
            reciprocal_approx_nr_function_type reciprocal_approx_nr_function_value( &::SireMaths::MultiFloat::reciprocal_approx_nr );
            
            MultiFloat_exposer.def( 
                "reciprocal_approx_nr"
                , reciprocal_approx_nr_function_value );
        
        }
        { //::SireMaths::MultiFloat::rotate
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*rotate_function_type )(  ) const;
            rotate_function_type rotate_function_value( &::SireMaths::MultiFloat::rotate );
            
            MultiFloat_exposer.def( 
                "rotate"
                , rotate_function_value );
        
        }
        { //::SireMaths::MultiFloat::rsqrt
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*rsqrt_function_type )(  ) const;
            rsqrt_function_type rsqrt_function_value( &::SireMaths::MultiFloat::rsqrt );
            
            MultiFloat_exposer.def( 
                "rsqrt"
                , rsqrt_function_value );
        
        }
        { //::SireMaths::MultiFloat::rsqrt_approx
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*rsqrt_approx_function_type )(  ) const;
            rsqrt_approx_function_type rsqrt_approx_function_value( &::SireMaths::MultiFloat::rsqrt_approx );
            
            MultiFloat_exposer.def( 
                "rsqrt_approx"
                , rsqrt_approx_function_value );
        
        }
        { //::SireMaths::MultiFloat::rsqrt_approx_nr
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*rsqrt_approx_nr_function_type )(  ) const;
            rsqrt_approx_nr_function_type rsqrt_approx_nr_function_value( &::SireMaths::MultiFloat::rsqrt_approx_nr );
            
            MultiFloat_exposer.def( 
                "rsqrt_approx_nr"
                , rsqrt_approx_nr_function_value );
        
        }
        { //::SireMaths::MultiFloat::set
        
            typedef void ( ::SireMaths::MultiFloat::*set_function_type )( int,float ) ;
            set_function_type set_function_value( &::SireMaths::MultiFloat::set );
            
            MultiFloat_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("i"), bp::arg("value") ) );
        
        }
        { //::SireMaths::MultiFloat::size
        
            typedef int ( *size_function_type )(  );
            size_function_type size_function_value( &::SireMaths::MultiFloat::size );
            
            MultiFloat_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMaths::MultiFloat::sqrt
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*sqrt_function_type )(  ) const;
            sqrt_function_type sqrt_function_value( &::SireMaths::MultiFloat::sqrt );
            
            MultiFloat_exposer.def( 
                "sqrt"
                , sqrt_function_value );
        
        }
        { //::SireMaths::MultiFloat::sqrt_approx
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*sqrt_approx_function_type )(  ) const;
            sqrt_approx_function_type sqrt_approx_function_value( &::SireMaths::MultiFloat::sqrt_approx );
            
            MultiFloat_exposer.def( 
                "sqrt_approx"
                , sqrt_approx_function_value );
        
        }
        { //::SireMaths::MultiFloat::sqrt_approx_nr
        
            typedef ::SireMaths::MultiFloat ( ::SireMaths::MultiFloat::*sqrt_approx_nr_function_type )(  ) const;
            sqrt_approx_nr_function_type sqrt_approx_nr_function_value( &::SireMaths::MultiFloat::sqrt_approx_nr );
            
            MultiFloat_exposer.def( 
                "sqrt_approx_nr"
                , sqrt_approx_nr_function_value );
        
        }
        { //::SireMaths::MultiFloat::sum
        
            typedef float ( ::SireMaths::MultiFloat::*sum_function_type )(  ) const;
            sum_function_type sum_function_value( &::SireMaths::MultiFloat::sum );
            
            MultiFloat_exposer.def( 
                "sum"
                , sum_function_value );
        
        }
        { //::SireMaths::MultiFloat::toArray
        
            typedef ::QVector< float > ( *toArray_function_type )( ::QVector< SireMaths::MultiFloat > const & );
            toArray_function_type toArray_function_value( &::SireMaths::MultiFloat::toArray );
            
            MultiFloat_exposer.def( 
                "toArray"
                , toArray_function_value
                , ( bp::arg("array") ) );
        
        }
        { //::SireMaths::MultiFloat::toBinaryString
        
            typedef ::QString ( ::SireMaths::MultiFloat::*toBinaryString_function_type )(  ) const;
            toBinaryString_function_type toBinaryString_function_value( &::SireMaths::MultiFloat::toBinaryString );
            
            MultiFloat_exposer.def( 
                "toBinaryString"
                , toBinaryString_function_value );
        
        }
        { //::SireMaths::MultiFloat::toDoubleArray
        
            typedef ::QVector< double > ( *toDoubleArray_function_type )( ::QVector< SireMaths::MultiFloat > const & );
            toDoubleArray_function_type toDoubleArray_function_value( &::SireMaths::MultiFloat::toDoubleArray );
            
            MultiFloat_exposer.def( 
                "toDoubleArray"
                , toDoubleArray_function_value
                , ( bp::arg("array") ) );
        
        }
        { //::SireMaths::MultiFloat::toString
        
            typedef ::QString ( ::SireMaths::MultiFloat::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMaths::MultiFloat::toString );
            
            MultiFloat_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMaths::MultiFloat::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMaths::MultiFloat::typeName );
            
            MultiFloat_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMaths::MultiFloat::what
        
            typedef char const * ( ::SireMaths::MultiFloat::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMaths::MultiFloat::what );
            
            MultiFloat_exposer.def( 
                "what"
                , what_function_value );
        
        }
        MultiFloat_exposer.staticmethod( "count" );
        MultiFloat_exposer.staticmethod( "fromArray" );
        MultiFloat_exposer.staticmethod( "size" );
        MultiFloat_exposer.staticmethod( "toArray" );
        MultiFloat_exposer.staticmethod( "toDoubleArray" );
        MultiFloat_exposer.staticmethod( "typeName" );
        MultiFloat_exposer.def( "__copy__", &__copy__);
        MultiFloat_exposer.def( "__deepcopy__", &__copy__);
        MultiFloat_exposer.def( "clone", &__copy__);
        MultiFloat_exposer.def( "__str__", &__str__< ::SireMaths::MultiFloat > );
        MultiFloat_exposer.def( "__repr__", &__str__< ::SireMaths::MultiFloat > );
        MultiFloat_exposer.def( "__len__", &__len_size< ::SireMaths::MultiFloat > );
    }

}
