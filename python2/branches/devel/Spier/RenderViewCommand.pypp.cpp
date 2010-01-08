// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "RenderViewCommand.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "renderview.h"

#include "renderviewcommand.h"

#include "renderviewcommand.h"

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_RenderViewCommand_class(){

    { //::Spier::RenderViewCommand
        typedef bp::class_< Spier::RenderViewCommand, bp::bases< Spier::Command, SireBase::Property >, boost::noncopyable > RenderViewCommand_exposer_t;
        RenderViewCommand_exposer_t RenderViewCommand_exposer = RenderViewCommand_exposer_t( "RenderViewCommand", bp::no_init );
        bp::scope RenderViewCommand_scope( RenderViewCommand_exposer );
        { //::Spier::RenderViewCommand::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::Spier::RenderViewCommand::typeName );
            
            RenderViewCommand_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        RenderViewCommand_exposer.staticmethod( "typeName" );
        RenderViewCommand_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::Spier::RenderViewCommand >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RenderViewCommand_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::Spier::RenderViewCommand >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RenderViewCommand_exposer.def( "__str__", &__str__< ::Spier::RenderViewCommand > );
        RenderViewCommand_exposer.def( "__repr__", &__str__< ::Spier::RenderViewCommand > );
    }

}
