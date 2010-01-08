// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "InterGroupLJFF.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/mover.hpp"

#include "SireMol/partialmolecule.h"

#include "interljff.h"

#include "interljff.h"

SireFF::Inter2B2G3DFF<SireMM::LJPotentialInterface<SireMM::InterLJPotential> > __copy__(const SireFF::Inter2B2G3DFF<SireMM::LJPotentialInterface<SireMM::InterLJPotential> > &other){ return SireFF::Inter2B2G3DFF<SireMM::LJPotentialInterface<SireMM::InterLJPotential> >(other); }

#include "Helpers/str.hpp"

void register_InterGroupLJFF_class(){

    { //::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >
        typedef bp::class_< SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >, bp::bases< SireFF::FF3D, SireFF::Inter2B2GFF<SireMM::LJPotentialInterface<SireMM::InterLJPotential> >, SireMM::LJPotentialInterface<SireMM::InterLJPotential>, SireFF::G2FF, SireFF::FF, SireMol::MolGroupsBase, SireBase::Property > > InterGroupLJFF_exposer_t;
        InterGroupLJFF_exposer_t InterGroupLJFF_exposer = InterGroupLJFF_exposer_t( "InterGroupLJFF" );
        bp::scope InterGroupLJFF_scope( InterGroupLJFF_exposer );
        InterGroupLJFF_exposer.def( bp::init< >() );
        InterGroupLJFF_exposer.def( bp::init< QString const & >(( bp::arg("name") )) );
        InterGroupLJFF_exposer.def( bp::init< SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > > const & >(( bp::arg("other") )) );
        { //::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::force
        
            typedef SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::*force_function_type )( ::SireFF::ForceTable &,double ) ;
            force_function_type force_function_value( &::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::force );
            
            InterGroupLJFF_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("scale_force")=1 ) );
        
        }
        { //::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::force
        
            typedef SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > > exported_class_t;
            typedef void ( ::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::*force_function_type )( ::SireFF::ForceTable &,::SireCAS::Symbol const &,double ) ;
            force_function_type force_function_value( &::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::force );
            
            InterGroupLJFF_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("symbol"), bp::arg("scale_force")=1 ) );
        
        }
        InterGroupLJFF_exposer.def( bp::self != bp::self );
        { //::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::operator=
        
            typedef SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > > exported_class_t;
            typedef ::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > > & ( ::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::*assign_function_type )( ::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > > const & ) ;
            assign_function_type assign_function_value( &::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::operator= );
            
            InterGroupLJFF_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        InterGroupLJFF_exposer.def( bp::self == bp::self );
        { //::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::typeName
        
            typedef SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::typeName );
            
            InterGroupLJFF_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::what
        
            typedef SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > > exported_class_t;
            typedef char const * ( ::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireFF::Inter2B2G3DFF< SireMM::LJPotentialInterface< SireMM::InterLJPotential > >::what );
            
            InterGroupLJFF_exposer.def( 
                "what"
                , what_function_value );
        
        }
        InterGroupLJFF_exposer.staticmethod( "typeName" );
        InterGroupLJFF_exposer.def( "__copy__", &__copy__);
        InterGroupLJFF_exposer.def( "__deepcopy__", &__copy__);
        InterGroupLJFF_exposer.def( "clone", &__copy__);
        InterGroupLJFF_exposer.def( "__str__", &__str__< ::SireFF::Inter2B2G3DFF<SireMM::LJPotentialInterface<SireMM::InterLJPotential> > > );
        InterGroupLJFF_exposer.def( "__repr__", &__str__< ::SireFF::Inter2B2G3DFF<SireMM::LJPotentialInterface<SireMM::InterLJPotential> > > );
    }

}
