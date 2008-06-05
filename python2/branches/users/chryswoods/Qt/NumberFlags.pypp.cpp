// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "boost/python.hpp"
#include "sireqt_headers.h"
#include "NumberFlags.pypp.hpp"

namespace bp = boost::python;

QFlags<QTextStream::NumberFlag> __copy__(const QFlags<QTextStream::NumberFlag> &other){ return QFlags<QTextStream::NumberFlag>(other); }

const char* pvt_get_name(const QFlags<QTextStream::NumberFlag>&){ return "QFlags<QTextStream::NumberFlag>";}

void register_NumberFlags_class(){

    { //::QFlags< QTextStream::NumberFlag >
        typedef bp::class_< QFlags< QTextStream::NumberFlag > > NumberFlags_exposer_t;
        NumberFlags_exposer_t NumberFlags_exposer = NumberFlags_exposer_t( "NumberFlags", bp::init< QFlags< QTextStream::NumberFlag > const & >(( bp::arg("f") )) );
        bp::scope NumberFlags_scope( NumberFlags_exposer );
        NumberFlags_exposer.def( bp::init< QTextStream::NumberFlag >(( bp::arg("f") )) );
        NumberFlags_exposer.def( bp::init< bp::optional< void * * > >(( bp::arg("arg0")=bp::object() )) );
        NumberFlags_exposer.def( bp::init< QFlag >(( bp::arg("f") )) );
        NumberFlags_exposer.def( "__int__", &QFlags< QTextStream::NumberFlag >::operator int  );
        NumberFlags_exposer.def( !bp::self );
        NumberFlags_exposer.def( bp::self & bp::other< int >() );
        NumberFlags_exposer.def( bp::self & bp::other< uint >() );
        NumberFlags_exposer.def( bp::self & bp::other< QTextStream::NumberFlag >() );
        NumberFlags_exposer.def( bp::self &= bp::other< int >() );
        NumberFlags_exposer.def( bp::self &= bp::other< uint >() );
        { //::QFlags< QTextStream::NumberFlag >::operator=
        
            typedef QFlags< QTextStream::NumberFlag > exported_class_t;
            typedef ::QFlags< QTextStream::NumberFlag > & ( exported_class_t::*assign_function_type )( ::QFlags< QTextStream::NumberFlag > const & ) ;
            
            NumberFlags_exposer.def( 
                "assign"
                , assign_function_type( &::QFlags< QTextStream::NumberFlag >::operator= )
                , ( bp::arg("f") )
                , bp::return_self< >() );
        
        }
        NumberFlags_exposer.def( bp::self ^ bp::self );
        NumberFlags_exposer.def( bp::self ^ bp::other< QTextStream::NumberFlag >() );
        NumberFlags_exposer.def( bp::self ^= bp::self );
        NumberFlags_exposer.def( bp::self ^= bp::other< QTextStream::NumberFlag >() );
        NumberFlags_exposer.def( bp::self | bp::self );
        NumberFlags_exposer.def( bp::self | bp::other< QTextStream::NumberFlag >() );
        NumberFlags_exposer.def( bp::self |= bp::self );
        NumberFlags_exposer.def( bp::self |= bp::other< QTextStream::NumberFlag >() );
        NumberFlags_exposer.def( ~bp::self );
        { //::QFlags< QTextStream::NumberFlag >::testFlag
        
            typedef QFlags< QTextStream::NumberFlag > exported_class_t;
            typedef bool ( exported_class_t::*testFlag_function_type )( ::QTextStream::NumberFlag ) const;
            
            NumberFlags_exposer.def( 
                "testFlag"
                , testFlag_function_type( &::QFlags< QTextStream::NumberFlag >::testFlag )
                , ( bp::arg("f") ) );
        
        }
        NumberFlags_exposer.def( "__copy__", &__copy__);
        NumberFlags_exposer.def( "__str__", &pvt_get_name);
    }

}
