// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "XMLStream.pypp.hpp"

namespace bp = boost::python;

#include "Siren/errors.h"

#include "logger.h"

#include "tester.h"

#include "xmlstream.h"

#include <QDebug>

#include <QTextCodec>

#include <QTextStream>

#include "xmlstream.h"

Siren::XMLStream __copy__(const Siren::XMLStream &other){ return Siren::XMLStream(other); }

#include "Helpers/str.hpp"

void register_XMLStream_class(){

    { //::Siren::XMLStream
        typedef bp::class_< Siren::XMLStream, bp::bases< Siren::Stream, Siren::Handle > > XMLStream_exposer_t;
        XMLStream_exposer_t XMLStream_exposer = XMLStream_exposer_t( "XMLStream" );
        bp::scope XMLStream_scope( XMLStream_exposer );
        XMLStream_exposer.def( bp::init< >() );
        XMLStream_exposer.def( bp::init< QIODevice * >(( bp::arg("d") )) );
        XMLStream_exposer.def( bp::init< QString *, QFlags< QIODevice::OpenModeFlag > >(( bp::arg("s"), bp::arg("mode") )) );
        XMLStream_exposer.def( bp::init< QString const & >(( bp::arg("s") )) );
        XMLStream_exposer.def( bp::init< Siren::XMLStream const & >(( bp::arg("other") )) );
        { //::Siren::XMLStream::hashCode
        
            typedef ::uint ( ::Siren::XMLStream::*hashCode_function_type )(  ) const;
            hashCode_function_type hashCode_function_value( &::Siren::XMLStream::hashCode );
            
            XMLStream_exposer.def( 
                "hashCode"
                , hashCode_function_value );
        
        }
        XMLStream_exposer.def( bp::self != bp::self );
        { //::Siren::XMLStream::operator=
        
            typedef ::Siren::XMLStream & ( ::Siren::XMLStream::*assign_function_type )( ::Siren::XMLStream const & ) ;
            assign_function_type assign_function_value( &::Siren::XMLStream::operator= );
            
            XMLStream_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        XMLStream_exposer.def( bp::self == bp::self );
        { //::Siren::XMLStream::toString
        
            typedef ::QString ( ::Siren::XMLStream::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::Siren::XMLStream::toString );
            
            XMLStream_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        XMLStream_exposer.def( "__copy__", &__copy__);
        XMLStream_exposer.def( "__deepcopy__", &__copy__);
        XMLStream_exposer.def( "clone", &__copy__);
        XMLStream_exposer.def( "__str__", &__str__< ::Siren::XMLStream > );
        XMLStream_exposer.def( "__repr__", &__str__< ::Siren::XMLStream > );
    }

}
