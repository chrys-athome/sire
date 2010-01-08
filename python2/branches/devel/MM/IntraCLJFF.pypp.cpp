// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "IntraCLJFF.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/mover.hpp"

#include "SireMol/partialmolecule.h"

#include "intracljff.h"

#include "intracljff.h"

SireFF::Intra2B3DFF<SireMM::CLJPotentialInterface<SireMM::IntraCLJPotential> > __copy__(const SireFF::Intra2B3DFF<SireMM::CLJPotentialInterface<SireMM::IntraCLJPotential> > &other){ return SireFF::Intra2B3DFF<SireMM::CLJPotentialInterface<SireMM::IntraCLJPotential> >(other); }

#include "Helpers/str.hpp"

void register_IntraCLJFF_class(){

    { //::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >
        typedef bp::class_< SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >, bp::bases< SireFF::FF3D, SireFF::Intra2BFF<SireMM::CLJPotentialInterface<SireMM::IntraCLJPotential> >, SireMM::CLJPotentialInterface<SireMM::IntraCLJPotential>, SireFF::G1FF, SireFF::FF, SireMol::MolGroupsBase, SireBase::Property > > IntraCLJFF_exposer_t;
        IntraCLJFF_exposer_t IntraCLJFF_exposer = IntraCLJFF_exposer_t( "IntraCLJFF" );
        bp::scope IntraCLJFF_scope( IntraCLJFF_exposer );
        IntraCLJFF_exposer.def( bp::init< >() );
        IntraCLJFF_exposer.def( bp::init< QString const & >(( bp::arg("name") )) );
        IntraCLJFF_exposer.def( bp::init< SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > > const & >(( bp::arg("other") )) );
        { //::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::force
        
            typedef SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::*force_function_type )( ::SireFF::ForceTable &,double ) ;
            force_function_type force_function_value( &::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::force );
            
            IntraCLJFF_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("scale_force")=1 ) );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::force
        
            typedef SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::*force_function_type )( ::SireFF::ForceTable &,::SireCAS::Symbol const &,double ) ;
            force_function_type force_function_value( &::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::force );
            
            IntraCLJFF_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("symbol"), bp::arg("scale_force")=1 ) );
        
        }
        IntraCLJFF_exposer.def( bp::self != bp::self );
        { //::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::operator=
        
            typedef SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > > exported_class_t;
            typedef ::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > > & ( ::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::*assign_function_type )( ::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > > const & ) ;
            assign_function_type assign_function_value( &::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::operator= );
            
            IntraCLJFF_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        IntraCLJFF_exposer.def( bp::self == bp::self );
        { //::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::typeName
        
            typedef SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::typeName );
            
            IntraCLJFF_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::what
        
            typedef SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > > exported_class_t;
            typedef char const * ( ::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireFF::Intra2B3DFF< SireMM::CLJPotentialInterface< SireMM::IntraCLJPotential > >::what );
            
            IntraCLJFF_exposer.def( 
                "what"
                , what_function_value );
        
        }
        IntraCLJFF_exposer.staticmethod( "typeName" );
        IntraCLJFF_exposer.def( "__copy__", &__copy__);
        IntraCLJFF_exposer.def( "__deepcopy__", &__copy__);
        IntraCLJFF_exposer.def( "clone", &__copy__);
        IntraCLJFF_exposer.def( "__str__", &__str__< ::SireFF::Intra2B3DFF<SireMM::CLJPotentialInterface<SireMM::IntraCLJPotential> > > );
        IntraCLJFF_exposer.def( "__repr__", &__str__< ::SireFF::Intra2B3DFF<SireMM::CLJPotentialInterface<SireMM::IntraCLJPotential> > > );
    }

}
