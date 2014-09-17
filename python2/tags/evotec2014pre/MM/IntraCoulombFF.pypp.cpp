// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "IntraCoulombFF.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/mover.hpp"

#include "SireMol/partialmolecule.h"

#include "intracoulombff.h"

#include "intracoulombff.h"

SireFF::Intra2B3DFF<SireMM::CoulombPotentialInterface<SireMM::IntraCoulombPotential> > __copy__(const SireFF::Intra2B3DFF<SireMM::CoulombPotentialInterface<SireMM::IntraCoulombPotential> > &other){ return SireFF::Intra2B3DFF<SireMM::CoulombPotentialInterface<SireMM::IntraCoulombPotential> >(other); }

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_IntraCoulombFF_class(){

    { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >
        typedef bp::class_< SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >, bp::bases< SireFF::FF3D, SireFF::Intra2BFF<SireMM::CoulombPotentialInterface<SireMM::IntraCoulombPotential> >, SireMM::CoulombPotentialInterface<SireMM::IntraCoulombPotential>, SireFF::G1FF, SireFF::FF, SireMol::MolGroupsBase, SireBase::Property > > IntraCoulombFF_exposer_t;
        IntraCoulombFF_exposer_t IntraCoulombFF_exposer = IntraCoulombFF_exposer_t( "IntraCoulombFF", bp::init< >() );
        bp::scope IntraCoulombFF_scope( IntraCoulombFF_exposer );
        IntraCoulombFF_exposer.def( bp::init< QString const & >(( bp::arg("name") )) );
        IntraCoulombFF_exposer.def( bp::init< SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > const & >(( bp::arg("other") )) );
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::energy
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*energy_function_type )( ::SireFF::EnergyTable &,double ) ;
            energy_function_type energy_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::energy );
            
            IntraCoulombFF_exposer.def( 
                "energy"
                , energy_function_value
                , ( bp::arg("energytable"), bp::arg("scale_energy")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::energy
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*energy_function_type )( ::SireFF::EnergyTable &,::SireCAS::Symbol const &,double ) ;
            energy_function_type energy_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::energy );
            
            IntraCoulombFF_exposer.def( 
                "energy"
                , energy_function_value
                , ( bp::arg("energytable"), bp::arg("symbol"), bp::arg("scale_energy")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::field
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*field_function_type )( ::SireFF::FieldTable &,double ) ;
            field_function_type field_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::field );
            
            IntraCoulombFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::field
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*field_function_type )( ::SireFF::FieldTable &,::SireCAS::Symbol const &,double ) ;
            field_function_type field_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::field );
            
            IntraCoulombFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("component"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::field
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*field_function_type )( ::SireFF::FieldTable &,::SireFF::Probe const &,double ) ;
            field_function_type field_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::field );
            
            IntraCoulombFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("probe"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::field
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*field_function_type )( ::SireFF::FieldTable &,::SireCAS::Symbol const &,::SireFF::Probe const &,double ) ;
            field_function_type field_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::field );
            
            IntraCoulombFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("component"), bp::arg("probe"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::force
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*force_function_type )( ::SireFF::ForceTable &,double ) ;
            force_function_type force_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::force );
            
            IntraCoulombFF_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("scale_force")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::force
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*force_function_type )( ::SireFF::ForceTable &,::SireCAS::Symbol const &,double ) ;
            force_function_type force_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::force );
            
            IntraCoulombFF_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("symbol"), bp::arg("scale_force")=1 ) );
        
        }
        IntraCoulombFF_exposer.def( bp::self != bp::self );
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::operator=
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > & ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*assign_function_type )( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > const & ) ;
            assign_function_type assign_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::operator= );
            
            IntraCoulombFF_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        IntraCoulombFF_exposer.def( bp::self == bp::self );
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::potential
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*potential_function_type )( ::SireFF::PotentialTable &,double ) ;
            potential_function_type potential_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::potential );
            
            IntraCoulombFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::potential
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*potential_function_type )( ::SireFF::PotentialTable &,::SireCAS::Symbol const &,double ) ;
            potential_function_type potential_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::potential );
            
            IntraCoulombFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("component"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::potential
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*potential_function_type )( ::SireFF::PotentialTable &,::SireFF::Probe const &,double ) ;
            potential_function_type potential_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::potential );
            
            IntraCoulombFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("probe"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::potential
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*potential_function_type )( ::SireFF::PotentialTable &,::SireCAS::Symbol const &,::SireFF::Probe const &,double ) ;
            potential_function_type potential_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::potential );
            
            IntraCoulombFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("component"), bp::arg("probe"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::typeName
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::typeName );
            
            IntraCoulombFF_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::what
        
            typedef SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > > exported_class_t;
            typedef char const * ( ::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireFF::Intra2B3DFF< SireMM::CoulombPotentialInterface< SireMM::IntraCoulombPotential > >::what );
            
            IntraCoulombFF_exposer.def( 
                "what"
                , what_function_value );
        
        }
        IntraCoulombFF_exposer.staticmethod( "typeName" );
        IntraCoulombFF_exposer.def( "__copy__", &__copy__);
        IntraCoulombFF_exposer.def( "__deepcopy__", &__copy__);
        IntraCoulombFF_exposer.def( "clone", &__copy__);
        IntraCoulombFF_exposer.def( "__str__", &__str__< ::SireFF::Intra2B3DFF<SireMM::CoulombPotentialInterface<SireMM::IntraCoulombPotential> > > );
        IntraCoulombFF_exposer.def( "__repr__", &__str__< ::SireFF::Intra2B3DFF<SireMM::CoulombPotentialInterface<SireMM::IntraCoulombPotential> > > );
        IntraCoulombFF_exposer.def( "__len__", &__len_count< ::SireFF::Intra2B3DFF<SireMM::CoulombPotentialInterface<SireMM::IntraCoulombPotential> > > );
    }

}
