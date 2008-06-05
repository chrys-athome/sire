// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "boost/python.hpp"
#include "sireqt_headers.h"
#include "QByteArray.pypp.hpp"

namespace bp = boost::python;

QByteArray __copy__(const QByteArray &other){ return QByteArray(other); }

#include "Qt/qdatastream.hpp"

const char* pvt_get_name(const QByteArray&){ return "QByteArray";}

void register_QByteArray_class(){

    { //::QByteArray
        typedef bp::class_< QByteArray > QByteArray_exposer_t;
        QByteArray_exposer_t QByteArray_exposer = QByteArray_exposer_t( "QByteArray" );
        bp::scope QByteArray_scope( QByteArray_exposer );
        QByteArray_exposer.def( bp::init< char const * >(( bp::arg("arg0") )) );
        QByteArray_exposer.def( bp::init< char const *, int >(( bp::arg("arg0"), bp::arg("size") )) );
        QByteArray_exposer.def( bp::init< int, char >(( bp::arg("size"), bp::arg("c") )) );
        QByteArray_exposer.def( bp::init< >() );
        QByteArray_exposer.def( bp::init< QByteArray const & >(( bp::arg("a") )) );
        { //::QByteArray::at
        
            typedef char ( ::QByteArray::*at_function_type )( int ) const;
            
            QByteArray_exposer.def( 
                "at"
                , at_function_type( &::QByteArray::at )
                , ( bp::arg("i") ) );
        
        }
        { //::QByteArray::capacity
        
            typedef int ( ::QByteArray::*capacity_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "capacity"
                , capacity_function_type( &::QByteArray::capacity ) );
        
        }
        { //::QByteArray::chop
        
            typedef void ( ::QByteArray::*chop_function_type )( int ) ;
            
            QByteArray_exposer.def( 
                "chop"
                , chop_function_type( &::QByteArray::chop )
                , ( bp::arg("n") ) );
        
        }
        { //::QByteArray::clear
        
            typedef void ( ::QByteArray::*clear_function_type )(  ) ;
            
            QByteArray_exposer.def( 
                "clear"
                , clear_function_type( &::QByteArray::clear ) );
        
        }
        { //::QByteArray::constData
        
            typedef char const * ( ::QByteArray::*constData_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "constData"
                , constData_function_type( &::QByteArray::constData ) );
        
        }
        { //::QByteArray::contains
        
            typedef ::QBool ( ::QByteArray::*contains_function_type )( char ) const;
            
            QByteArray_exposer.def( 
                "contains"
                , contains_function_type( &::QByteArray::contains )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::contains
        
            typedef ::QBool ( ::QByteArray::*contains_function_type )( char const * ) const;
            
            QByteArray_exposer.def( 
                "contains"
                , contains_function_type( &::QByteArray::contains )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::contains
        
            typedef ::QBool ( ::QByteArray::*contains_function_type )( ::QByteArray const & ) const;
            
            QByteArray_exposer.def( 
                "contains"
                , contains_function_type( &::QByteArray::contains )
                , ( bp::arg("a") ) );
        
        }
        { //::QByteArray::count
        
            typedef int ( ::QByteArray::*count_function_type )( char ) const;
            
            QByteArray_exposer.def( 
                "count"
                , count_function_type( &::QByteArray::count )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::count
        
            typedef int ( ::QByteArray::*count_function_type )( char const * ) const;
            
            QByteArray_exposer.def( 
                "count"
                , count_function_type( &::QByteArray::count )
                , ( bp::arg("a") ) );
        
        }
        { //::QByteArray::count
        
            typedef int ( ::QByteArray::*count_function_type )( ::QByteArray const & ) const;
            
            QByteArray_exposer.def( 
                "count"
                , count_function_type( &::QByteArray::count )
                , ( bp::arg("a") ) );
        
        }
        { //::QByteArray::count
        
            typedef int ( ::QByteArray::*count_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "count"
                , count_function_type( &::QByteArray::count ) );
        
        }
        { //::QByteArray::endsWith
        
            typedef bool ( ::QByteArray::*endsWith_function_type )( ::QByteArray const & ) const;
            
            QByteArray_exposer.def( 
                "endsWith"
                , endsWith_function_type( &::QByteArray::endsWith )
                , ( bp::arg("a") ) );
        
        }
        { //::QByteArray::endsWith
        
            typedef bool ( ::QByteArray::*endsWith_function_type )( char ) const;
            
            QByteArray_exposer.def( 
                "endsWith"
                , endsWith_function_type( &::QByteArray::endsWith )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::endsWith
        
            typedef bool ( ::QByteArray::*endsWith_function_type )( char const * ) const;
            
            QByteArray_exposer.def( 
                "endsWith"
                , endsWith_function_type( &::QByteArray::endsWith )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::fromBase64
        
            typedef ::QByteArray ( *fromBase64_function_type )( ::QByteArray const & );
            
            QByteArray_exposer.def( 
                "fromBase64"
                , fromBase64_function_type( &::QByteArray::fromBase64 )
                , ( bp::arg("base64") ) );
        
        }
        { //::QByteArray::fromRawData
        
            typedef ::QByteArray ( *fromRawData_function_type )( char const *,int );
            
            QByteArray_exposer.def( 
                "fromRawData"
                , fromRawData_function_type( &::QByteArray::fromRawData )
                , ( bp::arg("arg0"), bp::arg("size") ) );
        
        }
        { //::QByteArray::indexOf
        
            typedef int ( ::QByteArray::*indexOf_function_type )( char,int ) const;
            
            QByteArray_exposer.def( 
                "indexOf"
                , indexOf_function_type( &::QByteArray::indexOf )
                , ( bp::arg("c"), bp::arg("from")=(int)(0) ) );
        
        }
        { //::QByteArray::indexOf
        
            typedef int ( ::QByteArray::*indexOf_function_type )( char const *,int ) const;
            
            QByteArray_exposer.def( 
                "indexOf"
                , indexOf_function_type( &::QByteArray::indexOf )
                , ( bp::arg("c"), bp::arg("i")=(int)(0) ) );
        
        }
        { //::QByteArray::indexOf
        
            typedef int ( ::QByteArray::*indexOf_function_type )( ::QByteArray const &,int ) const;
            
            QByteArray_exposer.def( 
                "indexOf"
                , indexOf_function_type( &::QByteArray::indexOf )
                , ( bp::arg("a"), bp::arg("from")=(int)(0) ) );
        
        }
        { //::QByteArray::indexOf
        
            typedef int ( ::QByteArray::*indexOf_function_type )( ::QString const &,int ) const;
            
            QByteArray_exposer.def( 
                "indexOf"
                , indexOf_function_type( &::QByteArray::indexOf )
                , ( bp::arg("s"), bp::arg("from")=(int)(0) ) );
        
        }
        { //::QByteArray::isEmpty
        
            typedef bool ( ::QByteArray::*isEmpty_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "isEmpty"
                , isEmpty_function_type( &::QByteArray::isEmpty ) );
        
        }
        { //::QByteArray::isNull
        
            typedef bool ( ::QByteArray::*isNull_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "isNull"
                , isNull_function_type( &::QByteArray::isNull ) );
        
        }
        { //::QByteArray::lastIndexOf
        
            typedef int ( ::QByteArray::*lastIndexOf_function_type )( char,int ) const;
            
            QByteArray_exposer.def( 
                "lastIndexOf"
                , lastIndexOf_function_type( &::QByteArray::lastIndexOf )
                , ( bp::arg("c"), bp::arg("from")=(int)(-0x000000001) ) );
        
        }
        { //::QByteArray::lastIndexOf
        
            typedef int ( ::QByteArray::*lastIndexOf_function_type )( char const *,int ) const;
            
            QByteArray_exposer.def( 
                "lastIndexOf"
                , lastIndexOf_function_type( &::QByteArray::lastIndexOf )
                , ( bp::arg("c"), bp::arg("i")=(int)(-0x000000001) ) );
        
        }
        { //::QByteArray::lastIndexOf
        
            typedef int ( ::QByteArray::*lastIndexOf_function_type )( ::QByteArray const &,int ) const;
            
            QByteArray_exposer.def( 
                "lastIndexOf"
                , lastIndexOf_function_type( &::QByteArray::lastIndexOf )
                , ( bp::arg("a"), bp::arg("from")=(int)(-0x000000001) ) );
        
        }
        { //::QByteArray::lastIndexOf
        
            typedef int ( ::QByteArray::*lastIndexOf_function_type )( ::QString const &,int ) const;
            
            QByteArray_exposer.def( 
                "lastIndexOf"
                , lastIndexOf_function_type( &::QByteArray::lastIndexOf )
                , ( bp::arg("s"), bp::arg("from")=(int)(-0x000000001) ) );
        
        }
        { //::QByteArray::left
        
            typedef ::QByteArray ( ::QByteArray::*left_function_type )( int ) const;
            
            QByteArray_exposer.def( 
                "left"
                , left_function_type( &::QByteArray::left )
                , ( bp::arg("len") ) );
        
        }
        { //::QByteArray::leftJustified
        
            typedef ::QByteArray ( ::QByteArray::*leftJustified_function_type )( int,char,bool ) const;
            
            QByteArray_exposer.def( 
                "leftJustified"
                , leftJustified_function_type( &::QByteArray::leftJustified )
                , ( bp::arg("width"), bp::arg("fill")=(char)(' '), bp::arg("truncate")=(bool)(false) ) );
        
        }
        { //::QByteArray::length
        
            typedef int ( ::QByteArray::*length_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "length"
                , length_function_type( &::QByteArray::length ) );
        
        }
        { //::QByteArray::mid
        
            typedef ::QByteArray ( ::QByteArray::*mid_function_type )( int,int ) const;
            
            QByteArray_exposer.def( 
                "mid"
                , mid_function_type( &::QByteArray::mid )
                , ( bp::arg("index"), bp::arg("len")=(int)(-0x000000001) ) );
        
        }
        { //::QByteArray::number
        
            typedef ::QByteArray ( *number_function_type )( int,int );
            
            QByteArray_exposer.def( 
                "number"
                , number_function_type( &::QByteArray::number )
                , ( bp::arg("arg0"), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::number
        
            typedef ::QByteArray ( *number_function_type )( ::uint,int );
            
            QByteArray_exposer.def( 
                "number"
                , number_function_type( &::QByteArray::number )
                , ( bp::arg("arg0"), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::number
        
            typedef ::QByteArray ( *number_function_type )( ::qlonglong,int );
            
            QByteArray_exposer.def( 
                "number"
                , number_function_type( &::QByteArray::number )
                , ( bp::arg("arg0"), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::number
        
            typedef ::QByteArray ( *number_function_type )( ::qulonglong,int );
            
            QByteArray_exposer.def( 
                "number"
                , number_function_type( &::QByteArray::number )
                , ( bp::arg("arg0"), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::number
        
            typedef ::QByteArray ( *number_function_type )( double,char,int );
            
            QByteArray_exposer.def( 
                "number"
                , number_function_type( &::QByteArray::number )
                , ( bp::arg("arg0"), bp::arg("f")=(char)('g'), bp::arg("prec")=(int)(6) ) );
        
        }
        QByteArray_exposer.def( bp::self != bp::other< QString >() );
        QByteArray_exposer.def( bp::self += bp::other< char >() );
        QByteArray_exposer.def( bp::self += bp::other< char const * >() );
        QByteArray_exposer.def( bp::self += bp::self );
        QByteArray_exposer.def( bp::self += bp::other< QString >() );
        QByteArray_exposer.def( bp::self < bp::other< QString >() );
        QByteArray_exposer.def( bp::self <= bp::other< QString >() );
        QByteArray_exposer.def( bp::self == bp::other< QString >() );
        QByteArray_exposer.def( bp::self > bp::other< QString >() );
        QByteArray_exposer.def( bp::self >= bp::other< QString >() );
        { //::QByteArray::operator[]
        
            typedef char ( ::QByteArray::*__getitem___function_type )( int ) const;
            
            QByteArray_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::QByteArray::operator[] )
                , ( bp::arg("i") ) );
        
        }
        { //::QByteArray::operator[]
        
            typedef char ( ::QByteArray::*__getitem___function_type )( ::uint ) const;
            
            QByteArray_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::QByteArray::operator[] )
                , ( bp::arg("i") ) );
        
        }
        { //::QByteArray::operator[]
        
            typedef ::QByteRef ( ::QByteArray::*__getitem___function_type )( int ) ;
            
            QByteArray_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::QByteArray::operator[] )
                , ( bp::arg("i") ) );
        
        }
        { //::QByteArray::operator[]
        
            typedef ::QByteRef ( ::QByteArray::*__getitem___function_type )( ::uint ) ;
            
            QByteArray_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::QByteArray::operator[] )
                , ( bp::arg("i") ) );
        
        }
        { //::QByteArray::push_back
        
            typedef void ( ::QByteArray::*push_back_function_type )( char ) ;
            
            QByteArray_exposer.def( 
                "push_back"
                , push_back_function_type( &::QByteArray::push_back )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::push_back
        
            typedef void ( ::QByteArray::*push_back_function_type )( char const * ) ;
            
            QByteArray_exposer.def( 
                "push_back"
                , push_back_function_type( &::QByteArray::push_back )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::push_back
        
            typedef void ( ::QByteArray::*push_back_function_type )( ::QByteArray const & ) ;
            
            QByteArray_exposer.def( 
                "push_back"
                , push_back_function_type( &::QByteArray::push_back )
                , ( bp::arg("a") ) );
        
        }
        { //::QByteArray::push_front
        
            typedef void ( ::QByteArray::*push_front_function_type )( char ) ;
            
            QByteArray_exposer.def( 
                "push_front"
                , push_front_function_type( &::QByteArray::push_front )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::push_front
        
            typedef void ( ::QByteArray::*push_front_function_type )( char const * ) ;
            
            QByteArray_exposer.def( 
                "push_front"
                , push_front_function_type( &::QByteArray::push_front )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::push_front
        
            typedef void ( ::QByteArray::*push_front_function_type )( ::QByteArray const & ) ;
            
            QByteArray_exposer.def( 
                "push_front"
                , push_front_function_type( &::QByteArray::push_front )
                , ( bp::arg("a") ) );
        
        }
        { //::QByteArray::reserve
        
            typedef void ( ::QByteArray::*reserve_function_type )( int ) ;
            
            QByteArray_exposer.def( 
                "reserve"
                , reserve_function_type( &::QByteArray::reserve )
                , ( bp::arg("asize") ) );
        
        }
        { //::QByteArray::resize
        
            typedef void ( ::QByteArray::*resize_function_type )( int ) ;
            
            QByteArray_exposer.def( 
                "resize"
                , resize_function_type( &::QByteArray::resize )
                , ( bp::arg("size") ) );
        
        }
        { //::QByteArray::right
        
            typedef ::QByteArray ( ::QByteArray::*right_function_type )( int ) const;
            
            QByteArray_exposer.def( 
                "right"
                , right_function_type( &::QByteArray::right )
                , ( bp::arg("len") ) );
        
        }
        { //::QByteArray::rightJustified
        
            typedef ::QByteArray ( ::QByteArray::*rightJustified_function_type )( int,char,bool ) const;
            
            QByteArray_exposer.def( 
                "rightJustified"
                , rightJustified_function_type( &::QByteArray::rightJustified )
                , ( bp::arg("width"), bp::arg("fill")=(char)(' '), bp::arg("truncate")=(bool)(false) ) );
        
        }
        { //::QByteArray::simplified
        
            typedef ::QByteArray ( ::QByteArray::*simplified_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "simplified"
                , simplified_function_type( &::QByteArray::simplified ) );
        
        }
        { //::QByteArray::size
        
            typedef int ( ::QByteArray::*size_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "size"
                , size_function_type( &::QByteArray::size ) );
        
        }
        { //::QByteArray::split
        
            typedef ::QList< QByteArray > ( ::QByteArray::*split_function_type )( char ) const;
            
            QByteArray_exposer.def( 
                "split"
                , split_function_type( &::QByteArray::split )
                , ( bp::arg("sep") ) );
        
        }
        { //::QByteArray::squeeze
        
            typedef void ( ::QByteArray::*squeeze_function_type )(  ) ;
            
            QByteArray_exposer.def( 
                "squeeze"
                , squeeze_function_type( &::QByteArray::squeeze ) );
        
        }
        { //::QByteArray::startsWith
        
            typedef bool ( ::QByteArray::*startsWith_function_type )( ::QByteArray const & ) const;
            
            QByteArray_exposer.def( 
                "startsWith"
                , startsWith_function_type( &::QByteArray::startsWith )
                , ( bp::arg("a") ) );
        
        }
        { //::QByteArray::startsWith
        
            typedef bool ( ::QByteArray::*startsWith_function_type )( char ) const;
            
            QByteArray_exposer.def( 
                "startsWith"
                , startsWith_function_type( &::QByteArray::startsWith )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::startsWith
        
            typedef bool ( ::QByteArray::*startsWith_function_type )( char const * ) const;
            
            QByteArray_exposer.def( 
                "startsWith"
                , startsWith_function_type( &::QByteArray::startsWith )
                , ( bp::arg("c") ) );
        
        }
        { //::QByteArray::toBase64
        
            typedef ::QByteArray ( ::QByteArray::*toBase64_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "toBase64"
                , toBase64_function_type( &::QByteArray::toBase64 ) );
        
        }
        { //::QByteArray::toDouble
        
            typedef double ( ::QByteArray::*toDouble_function_type )( bool * ) const;
            
            QByteArray_exposer.def( 
                "toDouble"
                , toDouble_function_type( &::QByteArray::toDouble )
                , ( bp::arg("ok")=bp::object() ) );
        
        }
        { //::QByteArray::toFloat
        
            typedef float ( ::QByteArray::*toFloat_function_type )( bool * ) const;
            
            QByteArray_exposer.def( 
                "toFloat"
                , toFloat_function_type( &::QByteArray::toFloat )
                , ( bp::arg("ok")=bp::object() ) );
        
        }
        { //::QByteArray::toInt
        
            typedef int ( ::QByteArray::*toInt_function_type )( bool *,int ) const;
            
            QByteArray_exposer.def( 
                "toInt"
                , toInt_function_type( &::QByteArray::toInt )
                , ( bp::arg("ok")=bp::object(), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::toLong
        
            typedef long int ( ::QByteArray::*toLong_function_type )( bool *,int ) const;
            
            QByteArray_exposer.def( 
                "toLong"
                , toLong_function_type( &::QByteArray::toLong )
                , ( bp::arg("ok")=bp::object(), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::toLongLong
        
            typedef ::qlonglong ( ::QByteArray::*toLongLong_function_type )( bool *,int ) const;
            
            QByteArray_exposer.def( 
                "toLongLong"
                , toLongLong_function_type( &::QByteArray::toLongLong )
                , ( bp::arg("ok")=bp::object(), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::toLower
        
            typedef ::QByteArray ( ::QByteArray::*toLower_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "toLower"
                , toLower_function_type( &::QByteArray::toLower ) );
        
        }
        { //::QByteArray::toShort
        
            typedef short int ( ::QByteArray::*toShort_function_type )( bool *,int ) const;
            
            QByteArray_exposer.def( 
                "toShort"
                , toShort_function_type( &::QByteArray::toShort )
                , ( bp::arg("ok")=bp::object(), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::toUInt
        
            typedef ::uint ( ::QByteArray::*toUInt_function_type )( bool *,int ) const;
            
            QByteArray_exposer.def( 
                "toUInt"
                , toUInt_function_type( &::QByteArray::toUInt )
                , ( bp::arg("ok")=bp::object(), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::toULong
        
            typedef ::ulong ( ::QByteArray::*toULong_function_type )( bool *,int ) const;
            
            QByteArray_exposer.def( 
                "toULong"
                , toULong_function_type( &::QByteArray::toULong )
                , ( bp::arg("ok")=bp::object(), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::toULongLong
        
            typedef ::qulonglong ( ::QByteArray::*toULongLong_function_type )( bool *,int ) const;
            
            QByteArray_exposer.def( 
                "toULongLong"
                , toULongLong_function_type( &::QByteArray::toULongLong )
                , ( bp::arg("ok")=bp::object(), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::toUShort
        
            typedef ::ushort ( ::QByteArray::*toUShort_function_type )( bool *,int ) const;
            
            QByteArray_exposer.def( 
                "toUShort"
                , toUShort_function_type( &::QByteArray::toUShort )
                , ( bp::arg("ok")=bp::object(), bp::arg("base")=(int)(10) ) );
        
        }
        { //::QByteArray::toUpper
        
            typedef ::QByteArray ( ::QByteArray::*toUpper_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "toUpper"
                , toUpper_function_type( &::QByteArray::toUpper ) );
        
        }
        { //::QByteArray::trimmed
        
            typedef ::QByteArray ( ::QByteArray::*trimmed_function_type )(  ) const;
            
            QByteArray_exposer.def( 
                "trimmed"
                , trimmed_function_type( &::QByteArray::trimmed ) );
        
        }
        { //::QByteArray::truncate
        
            typedef void ( ::QByteArray::*truncate_function_type )( int ) ;
            
            QByteArray_exposer.def( 
                "truncate"
                , truncate_function_type( &::QByteArray::truncate )
                , ( bp::arg("pos") ) );
        
        }
        QByteArray_exposer.staticmethod( "fromBase64" );
        QByteArray_exposer.staticmethod( "fromRawData" );
        QByteArray_exposer.staticmethod( "number" );
        QByteArray_exposer.def( "__copy__", &__copy__);
        QByteArray_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::QByteArray >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        QByteArray_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::QByteArray >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        QByteArray_exposer.def( "__str__", &pvt_get_name);
    }

}
