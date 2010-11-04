// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "RepExSubMove.pypp.hpp"

namespace bp = boost::python;

#include "SireCluster/cluster.h"

#include "SireCluster/node.h"

#include "SireCluster/nodes.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireUnits/units.h"

#include "repexmove.h"

#include "replica.h"

#include "replicas.h"

#include "suprasubsim.h"

#include "suprasystem.h"

#include <QDebug>

#include <QPair>

#include "repexmove.h"

SireMove::RepExSubMove __copy__(const SireMove::RepExSubMove &other){ return SireMove::RepExSubMove(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_RepExSubMove_class(){

    { //::SireMove::RepExSubMove
        typedef bp::class_< SireMove::RepExSubMove, bp::bases< SireMove::SupraSubMove, SireBase::Property > > RepExSubMove_exposer_t;
        RepExSubMove_exposer_t RepExSubMove_exposer = RepExSubMove_exposer_t( "RepExSubMove" );
        bp::scope RepExSubMove_scope( RepExSubMove_exposer );
        RepExSubMove_exposer.def( bp::init< >() );
        RepExSubMove_exposer.def( bp::init< SireMove::Replica const &, SireMove::Replica const & >(( bp::arg("replica_a"), bp::arg("replica_b") )) );
        RepExSubMove_exposer.def( bp::init< SireMove::RepExSubMove const & >(( bp::arg("other") )) );
        { //::SireMove::RepExSubMove::energy_i
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireMove::RepExSubMove::*energy_i_function_type )(  ) const;
            energy_i_function_type energy_i_function_value( &::SireMove::RepExSubMove::energy_i );
            
            RepExSubMove_exposer.def( 
                "energy_i"
                , energy_i_function_value );
        
        }
        { //::SireMove::RepExSubMove::energy_j
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireMove::RepExSubMove::*energy_j_function_type )(  ) const;
            energy_j_function_type energy_j_function_value( &::SireMove::RepExSubMove::energy_j );
            
            RepExSubMove_exposer.def( 
                "energy_j"
                , energy_j_function_value );
        
        }
        { //::SireMove::RepExSubMove::move
        
            typedef void ( ::SireMove::RepExSubMove::*move_function_type )( ::SireMove::SupraSubSystem &,int,int,bool ) ;
            move_function_type move_function_value( &::SireMove::RepExSubMove::move );
            
            RepExSubMove_exposer.def( 
                "move"
                , move_function_value
                , ( bp::arg("system"), bp::arg("n_supra_moves"), bp::arg("n_supra_moves_per_block"), bp::arg("record_stats") ) );
        
        }
        RepExSubMove_exposer.def( bp::self != bp::self );
        { //::SireMove::RepExSubMove::operator=
        
            typedef ::SireMove::RepExSubMove & ( ::SireMove::RepExSubMove::*assign_function_type )( ::SireMove::RepExSubMove const & ) ;
            assign_function_type assign_function_value( &::SireMove::RepExSubMove::operator= );
            
            RepExSubMove_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        RepExSubMove_exposer.def( bp::self == bp::self );
        { //::SireMove::RepExSubMove::toString
        
            typedef ::QString ( ::SireMove::RepExSubMove::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMove::RepExSubMove::toString );
            
            RepExSubMove_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMove::RepExSubMove::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMove::RepExSubMove::typeName );
            
            RepExSubMove_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMove::RepExSubMove::volume_i
        
            typedef ::SireUnits::Dimension::Volume ( ::SireMove::RepExSubMove::*volume_i_function_type )(  ) const;
            volume_i_function_type volume_i_function_value( &::SireMove::RepExSubMove::volume_i );
            
            RepExSubMove_exposer.def( 
                "volume_i"
                , volume_i_function_value );
        
        }
        { //::SireMove::RepExSubMove::volume_j
        
            typedef ::SireUnits::Dimension::Volume ( ::SireMove::RepExSubMove::*volume_j_function_type )(  ) const;
            volume_j_function_type volume_j_function_value( &::SireMove::RepExSubMove::volume_j );
            
            RepExSubMove_exposer.def( 
                "volume_j"
                , volume_j_function_value );
        
        }
        RepExSubMove_exposer.staticmethod( "typeName" );
        RepExSubMove_exposer.def( "__copy__", &__copy__);
        RepExSubMove_exposer.def( "__deepcopy__", &__copy__);
        RepExSubMove_exposer.def( "clone", &__copy__);
        RepExSubMove_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMove::RepExSubMove >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RepExSubMove_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMove::RepExSubMove >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RepExSubMove_exposer.def( "__str__", &__str__< ::SireMove::RepExSubMove > );
        RepExSubMove_exposer.def( "__repr__", &__str__< ::SireMove::RepExSubMove > );
    }

}
