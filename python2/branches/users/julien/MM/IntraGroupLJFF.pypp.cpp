// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "IntraGroupLJFF.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/mover.hpp"

#include "SireMol/partialmolecule.h"

#include "intraljff.h"

#include "intraljff.h"

SireFF::Intra2B2G3DFF<SireMM::LJPotentialInterface<SireMM::IntraLJPotential> > __copy__(const SireFF::Intra2B2G3DFF<SireMM::LJPotentialInterface<SireMM::IntraLJPotential> > &other){ return SireFF::Intra2B2G3DFF<SireMM::LJPotentialInterface<SireMM::IntraLJPotential> >(other); }

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_IntraGroupLJFF_class(){

    { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >
        typedef bp::class_< SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >, bp::bases< SireFF::FF3D, SireFF::Intra2B2GFF<SireMM::LJPotentialInterface<SireMM::IntraLJPotential> >, SireMM::LJPotentialInterface<SireMM::IntraLJPotential>, SireFF::G2FF, SireFF::FF, SireMol::MolGroupsBase, SireBase::Property > > IntraGroupLJFF_exposer_t;
        IntraGroupLJFF_exposer_t IntraGroupLJFF_exposer = IntraGroupLJFF_exposer_t( "IntraGroupLJFF", bp::init< >() );
        bp::scope IntraGroupLJFF_scope( IntraGroupLJFF_exposer );
        IntraGroupLJFF_exposer.def( bp::init< QString const & >(( bp::arg("name") )) );
        IntraGroupLJFF_exposer.def( bp::init< SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > const & >(( bp::arg("other") )) );
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::energy
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*energy_function_type )( ::SireFF::EnergyTable &,double ) ;
            energy_function_type energy_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::energy );
            
            IntraGroupLJFF_exposer.def( 
                "energy"
                , energy_function_value
                , ( bp::arg("energytable"), bp::arg("scale_energy")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::energy
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*energy_function_type )( ::SireFF::EnergyTable &,::SireCAS::Symbol const &,double ) ;
            energy_function_type energy_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::energy );
            
            IntraGroupLJFF_exposer.def( 
                "energy"
                , energy_function_value
                , ( bp::arg("energytable"), bp::arg("symbol"), bp::arg("scale_energy")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::field
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*field_function_type )( ::SireFF::FieldTable &,double ) ;
            field_function_type field_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::field );
            
            IntraGroupLJFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::field
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*field_function_type )( ::SireFF::FieldTable &,::SireCAS::Symbol const &,double ) ;
            field_function_type field_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::field );
            
            IntraGroupLJFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("component"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::field
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*field_function_type )( ::SireFF::FieldTable &,::SireFF::Probe const &,double ) ;
            field_function_type field_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::field );
            
            IntraGroupLJFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("probe"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::field
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*field_function_type )( ::SireFF::FieldTable &,::SireCAS::Symbol const &,::SireFF::Probe const &,double ) ;
            field_function_type field_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::field );
            
            IntraGroupLJFF_exposer.def( 
                "field"
                , field_function_value
                , ( bp::arg("fieldtable"), bp::arg("component"), bp::arg("probe"), bp::arg("scale_field")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::force
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*force_function_type )( ::SireFF::ForceTable &,double ) ;
            force_function_type force_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::force );
            
            IntraGroupLJFF_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("scale_force")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::force
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*force_function_type )( ::SireFF::ForceTable &,::SireCAS::Symbol const &,double ) ;
            force_function_type force_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::force );
            
            IntraGroupLJFF_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("symbol"), bp::arg("scale_force")=1 ) );
        
        }
        IntraGroupLJFF_exposer.def( bp::self != bp::self );
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::operator=
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > & ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*assign_function_type )( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > const & ) ;
            assign_function_type assign_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::operator= );
            
            IntraGroupLJFF_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        IntraGroupLJFF_exposer.def( bp::self == bp::self );
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::potential
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*potential_function_type )( ::SireFF::PotentialTable &,double ) ;
            potential_function_type potential_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::potential );
            
            IntraGroupLJFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::potential
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*potential_function_type )( ::SireFF::PotentialTable &,::SireCAS::Symbol const &,double ) ;
            potential_function_type potential_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::potential );
            
            IntraGroupLJFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("component"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::potential
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*potential_function_type )( ::SireFF::PotentialTable &,::SireFF::Probe const &,double ) ;
            potential_function_type potential_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::potential );
            
            IntraGroupLJFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("probe"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::potential
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*potential_function_type )( ::SireFF::PotentialTable &,::SireCAS::Symbol const &,::SireFF::Probe const &,double ) ;
            potential_function_type potential_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::potential );
            
            IntraGroupLJFF_exposer.def( 
                "potential"
                , potential_function_value
                , ( bp::arg("potentialtable"), bp::arg("component"), bp::arg("probe"), bp::arg("scale_potential")=1 ) );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::typeName
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::typeName );
            
            IntraGroupLJFF_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::what
        
            typedef SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > > exported_class_t;
            typedef char const * ( ::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireFF::Intra2B2G3DFF< SireMM::LJPotentialInterface< SireMM::IntraLJPotential > >::what );
            
            IntraGroupLJFF_exposer.def( 
                "what"
                , what_function_value );
        
        }
        IntraGroupLJFF_exposer.staticmethod( "typeName" );
        IntraGroupLJFF_exposer.def( "__copy__", &__copy__);
        IntraGroupLJFF_exposer.def( "__deepcopy__", &__copy__);
        IntraGroupLJFF_exposer.def( "clone", &__copy__);
        IntraGroupLJFF_exposer.def( "__str__", &__str__< ::SireFF::Intra2B2G3DFF<SireMM::LJPotentialInterface<SireMM::IntraLJPotential> > > );
        IntraGroupLJFF_exposer.def( "__repr__", &__str__< ::SireFF::Intra2B2G3DFF<SireMM::LJPotentialInterface<SireMM::IntraLJPotential> > > );
        IntraGroupLJFF_exposer.def( "__len__", &__len_count< ::SireFF::Intra2B2G3DFF<SireMM::LJPotentialInterface<SireMM::IntraLJPotential> > > );
    }

}
