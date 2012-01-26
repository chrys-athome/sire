// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Sim.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "simparams.h"

#include <QDomDocument>

#include <QDomElement>

#include <QDomNode>

#include <QFile>

#include "simparams.h"

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Sim_class(){

    { //::SireSim::Sim
        typedef bp::class_< SireSim::Sim, bp::bases< SireBase::Property >, boost::noncopyable > Sim_exposer_t;
        Sim_exposer_t Sim_exposer = Sim_exposer_t( "Sim", bp::no_init );
        bp::scope Sim_scope( Sim_exposer );
        { //::SireSim::Sim::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireSim::Sim::typeName );
            
            Sim_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        Sim_exposer.staticmethod( "typeName" );
        Sim_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireSim::Sim >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Sim_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireSim::Sim >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Sim_exposer.def( "__str__", &__str__< ::SireSim::Sim > );
        Sim_exposer.def( "__repr__", &__str__< ::SireSim::Sim > );
    }

}
