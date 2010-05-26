// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "SupraSim.pypp.hpp"

namespace bp = boost::python;

#include "SireCluster/nodes.h"

#include "SireError/errors.h"

#include "supramoves.h"

#include "suprasim.h"

#include "suprasim.h"

SireMove::SupraSim __copy__(const SireMove::SupraSim &other){ return SireMove::SupraSim(other); }

const char* pvt_get_name(const SireMove::SupraSim&){ return "SireMove::SupraSim";}

void register_SupraSim_class(){

    { //::SireMove::SupraSim
        typedef bp::class_< SireMove::SupraSim > SupraSim_exposer_t;
        SupraSim_exposer_t SupraSim_exposer = SupraSim_exposer_t( "SupraSim" );
        bp::scope SupraSim_scope( SupraSim_exposer );
        SupraSim_exposer.def( bp::init< >() );
        SupraSim_exposer.def( bp::init< SireMove::SupraSim const & >(( bp::arg("other") )) );
        { //::SireMove::SupraSim::abort
        
            typedef void ( ::SireMove::SupraSim::*abort_function_type )(  ) ;
            abort_function_type abort_function_value( &::SireMove::SupraSim::abort );
            
            SupraSim_exposer.def( 
                "abort"
                , abort_function_value );
        
        }
        { //::SireMove::SupraSim::hasFinished
        
            typedef bool ( ::SireMove::SupraSim::*hasFinished_function_type )(  ) ;
            hasFinished_function_type hasFinished_function_value( &::SireMove::SupraSim::hasFinished );
            
            SupraSim_exposer.def( 
                "hasFinished"
                , hasFinished_function_value );
        
        }
        { //::SireMove::SupraSim::initialMoves
        
            typedef ::SireMove::SupraMovesPtr ( ::SireMove::SupraSim::*initialMoves_function_type )(  ) ;
            initialMoves_function_type initialMoves_function_value( &::SireMove::SupraSim::initialMoves );
            
            SupraSim_exposer.def( 
                "initialMoves"
                , initialMoves_function_value );
        
        }
        { //::SireMove::SupraSim::initialSystem
        
            typedef ::SireMove::SupraSystemPtr ( ::SireMove::SupraSim::*initialSystem_function_type )(  ) ;
            initialSystem_function_type initialSystem_function_value( &::SireMove::SupraSim::initialSystem );
            
            SupraSim_exposer.def( 
                "initialSystem"
                , initialSystem_function_value );
        
        }
        { //::SireMove::SupraSim::input
        
            typedef ::SireMove::SupraSimPacket ( ::SireMove::SupraSim::*input_function_type )(  ) ;
            input_function_type input_function_value( &::SireMove::SupraSim::input );
            
            SupraSim_exposer.def( 
                "input"
                , input_function_value );
        
        }
        { //::SireMove::SupraSim::interimMoves
        
            typedef ::SireMove::SupraMovesPtr ( ::SireMove::SupraSim::*interimMoves_function_type )(  ) ;
            interimMoves_function_type interimMoves_function_value( &::SireMove::SupraSim::interimMoves );
            
            SupraSim_exposer.def( 
                "interimMoves"
                , interimMoves_function_value );
        
        }
        { //::SireMove::SupraSim::interimResult
        
            typedef ::SireMove::SupraSimPacket ( ::SireMove::SupraSim::*interimResult_function_type )(  ) ;
            interimResult_function_type interimResult_function_value( &::SireMove::SupraSim::interimResult );
            
            SupraSim_exposer.def( 
                "interimResult"
                , interimResult_function_value );
        
        }
        { //::SireMove::SupraSim::interimSystem
        
            typedef ::SireMove::SupraSystemPtr ( ::SireMove::SupraSim::*interimSystem_function_type )(  ) ;
            interimSystem_function_type interimSystem_function_value( &::SireMove::SupraSim::interimSystem );
            
            SupraSim_exposer.def( 
                "interimSystem"
                , interimSystem_function_value );
        
        }
        { //::SireMove::SupraSim::isError
        
            typedef bool ( ::SireMove::SupraSim::*isError_function_type )(  ) ;
            isError_function_type isError_function_value( &::SireMove::SupraSim::isError );
            
            SupraSim_exposer.def( 
                "isError"
                , isError_function_value );
        
        }
        { //::SireMove::SupraSim::isRunning
        
            typedef bool ( ::SireMove::SupraSim::*isRunning_function_type )(  ) ;
            isRunning_function_type isRunning_function_value( &::SireMove::SupraSim::isRunning );
            
            SupraSim_exposer.def( 
                "isRunning"
                , isRunning_function_value );
        
        }
        { //::SireMove::SupraSim::moves
        
            typedef ::SireMove::SupraMovesPtr ( ::SireMove::SupraSim::*moves_function_type )(  ) ;
            moves_function_type moves_function_value( &::SireMove::SupraSim::moves );
            
            SupraSim_exposer.def( 
                "moves"
                , moves_function_value );
        
        }
        SupraSim_exposer.def( bp::self != bp::self );
        { //::SireMove::SupraSim::operator=
        
            typedef ::SireMove::SupraSim & ( ::SireMove::SupraSim::*assign_function_type )( ::SireMove::SupraSim const & ) ;
            assign_function_type assign_function_value( &::SireMove::SupraSim::operator= );
            
            SupraSim_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        SupraSim_exposer.def( bp::self == bp::self );
        { //::SireMove::SupraSim::progress
        
            typedef float ( ::SireMove::SupraSim::*progress_function_type )(  ) ;
            progress_function_type progress_function_value( &::SireMove::SupraSim::progress );
            
            SupraSim_exposer.def( 
                "progress"
                , progress_function_value );
        
        }
        { //::SireMove::SupraSim::result
        
            typedef ::SireMove::SupraSimPacket ( ::SireMove::SupraSim::*result_function_type )(  ) ;
            result_function_type result_function_value( &::SireMove::SupraSim::result );
            
            SupraSim_exposer.def( 
                "result"
                , result_function_value );
        
        }
        { //::SireMove::SupraSim::run
        
            typedef ::SireMove::SupraSim ( *run_function_type )( ::SireMove::SupraSystem const &,::SireMove::SupraMoves const &,int,bool );
            run_function_type run_function_value( &::SireMove::SupraSim::run );
            
            SupraSim_exposer.def( 
                "run"
                , run_function_value
                , ( bp::arg("system"), bp::arg("moves"), bp::arg("nmoves"), bp::arg("record_stats")=(bool)(true) ) );
        
        }
        { //::SireMove::SupraSim::run
        
            typedef ::SireMove::SupraSim ( *run_function_type )( ::SireMove::SupraSystem const &,::SireMove::SupraMove const &,int,bool );
            run_function_type run_function_value( &::SireMove::SupraSim::run );
            
            SupraSim_exposer.def( 
                "run"
                , run_function_value
                , ( bp::arg("system"), bp::arg("move"), bp::arg("nmoves"), bp::arg("record_stats")=(bool)(true) ) );
        
        }
        { //::SireMove::SupraSim::run
        
            typedef ::SireMove::SupraSim ( *run_function_type )( ::SireMove::SupraSimPacket const & );
            run_function_type run_function_value( &::SireMove::SupraSim::run );
            
            SupraSim_exposer.def( 
                "run"
                , run_function_value
                , ( bp::arg("simpacket") ) );
        
        }
        { //::SireMove::SupraSim::run
        
            typedef ::SireMove::SupraSim ( *run_function_type )( ::SireCluster::Node &,::SireMove::SupraSystem const &,::SireMove::SupraMoves const &,int,bool );
            run_function_type run_function_value( &::SireMove::SupraSim::run );
            
            SupraSim_exposer.def( 
                "run"
                , run_function_value
                , ( bp::arg("node"), bp::arg("system"), bp::arg("moves"), bp::arg("nmoves"), bp::arg("record_stats")=(bool)(true) ) );
        
        }
        { //::SireMove::SupraSim::run
        
            typedef ::SireMove::SupraSim ( *run_function_type )( ::SireCluster::Node &,::SireMove::SupraSystem const &,::SireMove::SupraMove const &,int,bool );
            run_function_type run_function_value( &::SireMove::SupraSim::run );
            
            SupraSim_exposer.def( 
                "run"
                , run_function_value
                , ( bp::arg("node"), bp::arg("system"), bp::arg("move"), bp::arg("nmoves"), bp::arg("record_stats")=(bool)(true) ) );
        
        }
        { //::SireMove::SupraSim::run
        
            typedef ::SireMove::SupraSim ( *run_function_type )( ::SireCluster::Node &,::SireMove::SupraSimPacket const & );
            run_function_type run_function_value( &::SireMove::SupraSim::run );
            
            SupraSim_exposer.def( 
                "run"
                , run_function_value
                , ( bp::arg("node"), bp::arg("simpacket") ) );
        
        }
        { //::SireMove::SupraSim::stop
        
            typedef void ( ::SireMove::SupraSim::*stop_function_type )(  ) ;
            stop_function_type stop_function_value( &::SireMove::SupraSim::stop );
            
            SupraSim_exposer.def( 
                "stop"
                , stop_function_value );
        
        }
        { //::SireMove::SupraSim::system
        
            typedef ::SireMove::SupraSystemPtr ( ::SireMove::SupraSim::*system_function_type )(  ) ;
            system_function_type system_function_value( &::SireMove::SupraSim::system );
            
            SupraSim_exposer.def( 
                "system"
                , system_function_value );
        
        }
        { //::SireMove::SupraSim::throwError
        
            typedef void ( ::SireMove::SupraSim::*throwError_function_type )(  ) ;
            throwError_function_type throwError_function_value( &::SireMove::SupraSim::throwError );
            
            SupraSim_exposer.def( 
                "throwError"
                , throwError_function_value );
        
        }
        { //::SireMove::SupraSim::wait
        
            typedef void ( ::SireMove::SupraSim::*wait_function_type )(  ) ;
            wait_function_type wait_function_value( &::SireMove::SupraSim::wait );
            
            SupraSim_exposer.def( 
                "wait"
                , wait_function_value );
        
        }
        { //::SireMove::SupraSim::wait
        
            typedef bool ( ::SireMove::SupraSim::*wait_function_type )( int ) ;
            wait_function_type wait_function_value( &::SireMove::SupraSim::wait );
            
            SupraSim_exposer.def( 
                "wait"
                , wait_function_value
                , ( bp::arg("timeout") ) );
        
        }
        { //::SireMove::SupraSim::wasAborted
        
            typedef bool ( ::SireMove::SupraSim::*wasAborted_function_type )(  ) ;
            wasAborted_function_type wasAborted_function_value( &::SireMove::SupraSim::wasAborted );
            
            SupraSim_exposer.def( 
                "wasAborted"
                , wasAborted_function_value );
        
        }
        { //::SireMove::SupraSim::wasStopped
        
            typedef bool ( ::SireMove::SupraSim::*wasStopped_function_type )(  ) ;
            wasStopped_function_type wasStopped_function_value( &::SireMove::SupraSim::wasStopped );
            
            SupraSim_exposer.def( 
                "wasStopped"
                , wasStopped_function_value );
        
        }
        SupraSim_exposer.staticmethod( "run" );
        SupraSim_exposer.def( "__copy__", &__copy__);
        SupraSim_exposer.def( "__deepcopy__", &__copy__);
        SupraSim_exposer.def( "clone", &__copy__);
        SupraSim_exposer.def( "__str__", &pvt_get_name);
        SupraSim_exposer.def( "__repr__", &pvt_get_name);
    }

}