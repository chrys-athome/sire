// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "InternalComponent.pypp.hpp"

namespace bp = boost::python;

#include "SireFF/ff.h"

#include "SireStream/datastream.h"

#include "internalcomponent.h"

#include "internalcomponent.h"

SireMM::InternalComponent __copy__(const SireMM::InternalComponent &other){ return SireMM::InternalComponent(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_InternalComponent_class(){

    { //::SireMM::InternalComponent
        typedef bp::class_< SireMM::InternalComponent, bp::bases< SireFF::FFComponent, SireCAS::Symbol, SireCAS::ExBase > > InternalComponent_exposer_t;
        InternalComponent_exposer_t InternalComponent_exposer = InternalComponent_exposer_t( "InternalComponent", bp::init< bp::optional< SireFF::FFName const & > >(( bp::arg("name")=SireFF::FFName() )) );
        bp::scope InternalComponent_scope( InternalComponent_exposer );
        InternalComponent_exposer.def( bp::init< SireCAS::Symbol const & >(( bp::arg("symbol") )) );
        InternalComponent_exposer.def( bp::init< SireMM::InternalComponent const & >(( bp::arg("other") )) );
        { //::SireMM::InternalComponent::angle
        
            typedef ::SireMM::AngleComponent const & ( ::SireMM::InternalComponent::*angle_function_type )(  ) const;
            angle_function_type angle_function_value( &::SireMM::InternalComponent::angle );
            
            InternalComponent_exposer.def( 
                "angle"
                , angle_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InternalComponent::bendBend
        
            typedef ::SireMM::BendBendComponent const & ( ::SireMM::InternalComponent::*bendBend_function_type )(  ) const;
            bendBend_function_type bendBend_function_value( &::SireMM::InternalComponent::bendBend );
            
            InternalComponent_exposer.def( 
                "bendBend"
                , bendBend_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InternalComponent::bond
        
            typedef ::SireMM::BondComponent const & ( ::SireMM::InternalComponent::*bond_function_type )(  ) const;
            bond_function_type bond_function_value( &::SireMM::InternalComponent::bond );
            
            InternalComponent_exposer.def( 
                "bond"
                , bond_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InternalComponent::changeEnergy
        
            typedef void ( ::SireMM::InternalComponent::*changeEnergy_function_type )( ::SireFF::FF &,::SireMM::InternalEnergy const & ) const;
            changeEnergy_function_type changeEnergy_function_value( &::SireMM::InternalComponent::changeEnergy );
            
            InternalComponent_exposer.def( 
                "changeEnergy"
                , changeEnergy_function_value
                , ( bp::arg("ff"), bp::arg("nrg") ) );
        
        }
        { //::SireMM::InternalComponent::dihedral
        
            typedef ::SireMM::DihedralComponent const & ( ::SireMM::InternalComponent::*dihedral_function_type )(  ) const;
            dihedral_function_type dihedral_function_value( &::SireMM::InternalComponent::dihedral );
            
            InternalComponent_exposer.def( 
                "dihedral"
                , dihedral_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InternalComponent::improper
        
            typedef ::SireMM::ImproperComponent const & ( ::SireMM::InternalComponent::*improper_function_type )(  ) const;
            improper_function_type improper_function_value( &::SireMM::InternalComponent::improper );
            
            InternalComponent_exposer.def( 
                "improper"
                , improper_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InternalComponent::setEnergy
        
            typedef void ( ::SireMM::InternalComponent::*setEnergy_function_type )( ::SireFF::FF &,::SireMM::InternalEnergy const & ) const;
            setEnergy_function_type setEnergy_function_value( &::SireMM::InternalComponent::setEnergy );
            
            InternalComponent_exposer.def( 
                "setEnergy"
                , setEnergy_function_value
                , ( bp::arg("ff"), bp::arg("nrg") ) );
        
        }
        { //::SireMM::InternalComponent::stretchBend
        
            typedef ::SireMM::StretchBendComponent const & ( ::SireMM::InternalComponent::*stretchBend_function_type )(  ) const;
            stretchBend_function_type stretchBend_function_value( &::SireMM::InternalComponent::stretchBend );
            
            InternalComponent_exposer.def( 
                "stretchBend"
                , stretchBend_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InternalComponent::stretchBendTorsion
        
            typedef ::SireMM::StretchBendTorsionComponent const & ( ::SireMM::InternalComponent::*stretchBendTorsion_function_type )(  ) const;
            stretchBendTorsion_function_type stretchBendTorsion_function_value( &::SireMM::InternalComponent::stretchBendTorsion );
            
            InternalComponent_exposer.def( 
                "stretchBendTorsion"
                , stretchBendTorsion_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InternalComponent::stretchStretch
        
            typedef ::SireMM::StretchStretchComponent const & ( ::SireMM::InternalComponent::*stretchStretch_function_type )(  ) const;
            stretchStretch_function_type stretchStretch_function_value( &::SireMM::InternalComponent::stretchStretch );
            
            InternalComponent_exposer.def( 
                "stretchStretch"
                , stretchStretch_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InternalComponent::symbols
        
            typedef ::SireCAS::Symbols ( ::SireMM::InternalComponent::*symbols_function_type )(  ) const;
            symbols_function_type symbols_function_value( &::SireMM::InternalComponent::symbols );
            
            InternalComponent_exposer.def( 
                "symbols"
                , symbols_function_value );
        
        }
        { //::SireMM::InternalComponent::total
        
            typedef ::SireMM::InternalComponent const & ( ::SireMM::InternalComponent::*total_function_type )(  ) const;
            total_function_type total_function_value( &::SireMM::InternalComponent::total );
            
            InternalComponent_exposer.def( 
                "total"
                , total_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InternalComponent::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::InternalComponent::typeName );
            
            InternalComponent_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::InternalComponent::ureyBradley
        
            typedef ::SireMM::UreyBradleyComponent const & ( ::SireMM::InternalComponent::*ureyBradley_function_type )(  ) const;
            ureyBradley_function_type ureyBradley_function_value( &::SireMM::InternalComponent::ureyBradley );
            
            InternalComponent_exposer.def( 
                "ureyBradley"
                , ureyBradley_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InternalComponent::what
        
            typedef char const * ( ::SireMM::InternalComponent::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::InternalComponent::what );
            
            InternalComponent_exposer.def( 
                "what"
                , what_function_value );
        
        }
        InternalComponent_exposer.staticmethod( "typeName" );
        InternalComponent_exposer.def( "__copy__", &__copy__);
        InternalComponent_exposer.def( "__deepcopy__", &__copy__);
        InternalComponent_exposer.def( "clone", &__copy__);
        InternalComponent_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::InternalComponent >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        InternalComponent_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::InternalComponent >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        InternalComponent_exposer.def( "__str__", &__str__< ::SireMM::InternalComponent > );
        InternalComponent_exposer.def( "__repr__", &__str__< ::SireMM::InternalComponent > );
    }

}
