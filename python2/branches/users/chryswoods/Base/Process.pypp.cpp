// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Process.pypp.hpp"

namespace bp = boost::python;

#include "Siren/errors.h"

#include "Siren/hanref.h"

#include "Siren/mutex.h"

#include "process.h"

#include <QDebug>

#include <QList>

#include <QTime>

#include <errno.h>

#include <signal.h>

#include <string.h>

#include <sys/types.h>

#include <sys/wait.h>

#include <unistd.h>

#include "process.h"

#include "Siren/logger.h"

SireBase::Process __copy__(const SireBase::Process &other){ return SireBase::Process(other); }

#include "Siren/str.hpp"

void register_Process_class(){

    { //::SireBase::Process
        typedef bp::class_< SireBase::Process, bp::bases< Siren::Handle > > Process_exposer_t;
        Process_exposer_t Process_exposer = Process_exposer_t( "Process" );
        bp::scope Process_scope( Process_exposer );
        Process_exposer.def( bp::init< >() );
        Process_exposer.def( bp::init< SireBase::Process const & >(( bp::arg("other") )) );
        { //::SireBase::Process::hasFinished
        
            typedef bool ( ::SireBase::Process::*hasFinished_function_type )(  ) ;
            hasFinished_function_type hasFinished_function_value( &::SireBase::Process::hasFinished );
            
            Process_exposer.def( 
                "hasFinished"
                , hasFinished_function_value );
        
        }
        { //::SireBase::Process::hashCode
        
            typedef ::uint ( ::SireBase::Process::*hashCode_function_type )(  ) const;
            hashCode_function_type hashCode_function_value( &::SireBase::Process::hashCode );
            
            Process_exposer.def( 
                "hashCode"
                , hashCode_function_value );
        
        }
        { //::SireBase::Process::isError
        
            typedef bool ( ::SireBase::Process::*isError_function_type )(  ) ;
            isError_function_type isError_function_value( &::SireBase::Process::isError );
            
            Process_exposer.def( 
                "isError"
                , isError_function_value );
        
        }
        { //::SireBase::Process::isRunning
        
            typedef bool ( ::SireBase::Process::*isRunning_function_type )(  ) ;
            isRunning_function_type isRunning_function_value( &::SireBase::Process::isRunning );
            
            Process_exposer.def( 
                "isRunning"
                , isRunning_function_value );
        
        }
        { //::SireBase::Process::kill
        
            typedef void ( ::SireBase::Process::*kill_function_type )(  ) ;
            kill_function_type kill_function_value( &::SireBase::Process::kill );
            
            Process_exposer.def( 
                "kill"
                , kill_function_value );
        
        }
        { //::SireBase::Process::killAll
        
            typedef void ( *killAll_function_type )(  );
            killAll_function_type killAll_function_value( &::SireBase::Process::killAll );
            
            Process_exposer.def( 
                "killAll"
                , killAll_function_value );
        
        }
        Process_exposer.def( bp::self != bp::self );
        { //::SireBase::Process::operator=
        
            typedef ::SireBase::Process & ( ::SireBase::Process::*assign_function_type )( ::SireBase::Process const & ) ;
            assign_function_type assign_function_value( &::SireBase::Process::operator= );
            
            Process_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Process_exposer.def( bp::self == bp::self );
        { //::SireBase::Process::run
        
            typedef ::SireBase::Process ( *run_function_type )( ::QString const & );
            run_function_type run_function_value( &::SireBase::Process::run );
            
            Process_exposer.def( 
                "run"
                , run_function_value
                , ( bp::arg("command") ) );
        
        }
        { //::SireBase::Process::run
        
            typedef ::SireBase::Process ( *run_function_type )( ::QString const &,::QString const & );
            run_function_type run_function_value( &::SireBase::Process::run );
            
            Process_exposer.def( 
                "run"
                , run_function_value
                , ( bp::arg("command"), bp::arg("arg") ) );
        
        }
        { //::SireBase::Process::run
        
            typedef ::SireBase::Process ( *run_function_type )( ::QString const &,::QStringList const & );
            run_function_type run_function_value( &::SireBase::Process::run );
            
            Process_exposer.def( 
                "run"
                , run_function_value
                , ( bp::arg("command"), bp::arg("arguments") ) );
        
        }
        { //::SireBase::Process::toString
        
            typedef ::QString ( ::SireBase::Process::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireBase::Process::toString );
            
            Process_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireBase::Process::wait
        
            typedef void ( ::SireBase::Process::*wait_function_type )(  ) ;
            wait_function_type wait_function_value( &::SireBase::Process::wait );
            
            Process_exposer.def( 
                "wait"
                , wait_function_value );
        
        }
        { //::SireBase::Process::wait
        
            typedef bool ( ::SireBase::Process::*wait_function_type )( int ) ;
            wait_function_type wait_function_value( &::SireBase::Process::wait );
            
            Process_exposer.def( 
                "wait"
                , wait_function_value
                , ( bp::arg("ms") ) );
        
        }
        { //::SireBase::Process::wasKilled
        
            typedef bool ( ::SireBase::Process::*wasKilled_function_type )(  ) ;
            wasKilled_function_type wasKilled_function_value( &::SireBase::Process::wasKilled );
            
            Process_exposer.def( 
                "wasKilled"
                , wasKilled_function_value );
        
        }
        Process_exposer.staticmethod( "killAll" );
        Process_exposer.staticmethod( "run" );
        Process_exposer.def( "__copy__", &__copy__);
        Process_exposer.def( "__deepcopy__", &__copy__);
        Process_exposer.def( "clone", &__copy__);
        Process_exposer.def( "__str__", &__str__< ::SireBase::Process > );
        Process_exposer.def( "__repr__", &__str__< ::SireBase::Process > );
    }

}
