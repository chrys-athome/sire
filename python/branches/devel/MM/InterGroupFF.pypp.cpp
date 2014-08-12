// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "InterGroupFF.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/booleanproperty.h"

#include "SireBase/lengthproperty.h"

#include "SireError/errors.h"

#include "SireMol/atomselection.h"

#include "SireMol/molecule.h"

#include "SireMol/molecules.h"

#include "SireMol/molresid.h"

#include "SireMol/partialmolecule.h"

#include "SireMol/residue.h"

#include "SireMol/selector.hpp"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "cljcalculator.h"

#include "cljshiftfunction.h"

#include "intergroupff.h"

#include <QDebug>

#include <QElapsedTimer>

#include "intergroupff.h"

SireMM::InterGroupFF __copy__(const SireMM::InterGroupFF &other){ return SireMM::InterGroupFF(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_InterGroupFF_class(){

    { //::SireMM::InterGroupFF
        typedef bp::class_< SireMM::InterGroupFF, bp::bases< SireFF::G2FF, SireFF::FF, SireMol::MolGroupsBase, SireBase::Property > > InterGroupFF_exposer_t;
        InterGroupFF_exposer_t InterGroupFF_exposer = InterGroupFF_exposer_t( "InterGroupFF", bp::init< >() );
        bp::scope InterGroupFF_scope( InterGroupFF_exposer );
        InterGroupFF_exposer.def( bp::init< QString const & >(( bp::arg("name") )) );
        InterGroupFF_exposer.def( bp::init< SireMM::InterGroupFF const & >(( bp::arg("other") )) );
        { //::SireMM::InterGroupFF::accept
        
            typedef void ( ::SireMM::InterGroupFF::*accept_function_type )(  ) ;
            accept_function_type accept_function_value( &::SireMM::InterGroupFF::accept );
            
            InterGroupFF_exposer.def( 
                "accept"
                , accept_function_value );
        
        }
        { //::SireMM::InterGroupFF::cljFunction
        
            typedef ::SireMM::CLJFunction const & ( ::SireMM::InterGroupFF::*cljFunction_function_type )(  ) const;
            cljFunction_function_type cljFunction_function_value( &::SireMM::InterGroupFF::cljFunction );
            
            InterGroupFF_exposer.def( 
                "cljFunction"
                , cljFunction_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InterGroupFF::components
        
            typedef ::SireMM::CLJComponent const & ( ::SireMM::InterGroupFF::*components_function_type )(  ) const;
            components_function_type components_function_value( &::SireMM::InterGroupFF::components );
            
            InterGroupFF_exposer.def( 
                "components"
                , components_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InterGroupFF::containsProperty
        
            typedef bool ( ::SireMM::InterGroupFF::*containsProperty_function_type )( ::QString const & ) const;
            containsProperty_function_type containsProperty_function_value( &::SireMM::InterGroupFF::containsProperty );
            
            InterGroupFF_exposer.def( 
                "containsProperty"
                , containsProperty_function_value
                , ( bp::arg("name") ) );
        
        }
        { //::SireMM::InterGroupFF::disableParallelCalculation
        
            typedef void ( ::SireMM::InterGroupFF::*disableParallelCalculation_function_type )(  ) ;
            disableParallelCalculation_function_type disableParallelCalculation_function_value( &::SireMM::InterGroupFF::disableParallelCalculation );
            
            InterGroupFF_exposer.def( 
                "disableParallelCalculation"
                , disableParallelCalculation_function_value );
        
        }
        { //::SireMM::InterGroupFF::disableReproducibleCalculation
        
            typedef void ( ::SireMM::InterGroupFF::*disableReproducibleCalculation_function_type )(  ) ;
            disableReproducibleCalculation_function_type disableReproducibleCalculation_function_value( &::SireMM::InterGroupFF::disableReproducibleCalculation );
            
            InterGroupFF_exposer.def( 
                "disableReproducibleCalculation"
                , disableReproducibleCalculation_function_value );
        
        }
        { //::SireMM::InterGroupFF::enableParallelCalculation
        
            typedef void ( ::SireMM::InterGroupFF::*enableParallelCalculation_function_type )(  ) ;
            enableParallelCalculation_function_type enableParallelCalculation_function_value( &::SireMM::InterGroupFF::enableParallelCalculation );
            
            InterGroupFF_exposer.def( 
                "enableParallelCalculation"
                , enableParallelCalculation_function_value );
        
        }
        { //::SireMM::InterGroupFF::enableReproducibleCalculation
        
            typedef void ( ::SireMM::InterGroupFF::*enableReproducibleCalculation_function_type )(  ) ;
            enableReproducibleCalculation_function_type enableReproducibleCalculation_function_value( &::SireMM::InterGroupFF::enableReproducibleCalculation );
            
            InterGroupFF_exposer.def( 
                "enableReproducibleCalculation"
                , enableReproducibleCalculation_function_value );
        
        }
        { //::SireMM::InterGroupFF::mustNowRecalculateFromScratch
        
            typedef void ( ::SireMM::InterGroupFF::*mustNowRecalculateFromScratch_function_type )(  ) ;
            mustNowRecalculateFromScratch_function_type mustNowRecalculateFromScratch_function_value( &::SireMM::InterGroupFF::mustNowRecalculateFromScratch );
            
            InterGroupFF_exposer.def( 
                "mustNowRecalculateFromScratch"
                , mustNowRecalculateFromScratch_function_value );
        
        }
        { //::SireMM::InterGroupFF::needsAccepting
        
            typedef bool ( ::SireMM::InterGroupFF::*needsAccepting_function_type )(  ) const;
            needsAccepting_function_type needsAccepting_function_value( &::SireMM::InterGroupFF::needsAccepting );
            
            InterGroupFF_exposer.def( 
                "needsAccepting"
                , needsAccepting_function_value );
        
        }
        InterGroupFF_exposer.def( bp::self != bp::self );
        { //::SireMM::InterGroupFF::operator=
        
            typedef ::SireMM::InterGroupFF & ( ::SireMM::InterGroupFF::*assign_function_type )( ::SireMM::InterGroupFF const & ) ;
            assign_function_type assign_function_value( &::SireMM::InterGroupFF::operator= );
            
            InterGroupFF_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        InterGroupFF_exposer.def( bp::self == bp::self );
        { //::SireMM::InterGroupFF::properties
        
            typedef ::SireBase::Properties const & ( ::SireMM::InterGroupFF::*properties_function_type )(  ) const;
            properties_function_type properties_function_value( &::SireMM::InterGroupFF::properties );
            
            InterGroupFF_exposer.def( 
                "properties"
                , properties_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::InterGroupFF::property
        
            typedef ::SireBase::Property const & ( ::SireMM::InterGroupFF::*property_function_type )( ::QString const & ) const;
            property_function_type property_function_value( &::SireMM::InterGroupFF::property );
            
            InterGroupFF_exposer.def( 
                "property"
                , property_function_value
                , ( bp::arg("name") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InterGroupFF::setCLJFunction
        
            typedef void ( ::SireMM::InterGroupFF::*setCLJFunction_function_type )( ::SireMM::CLJFunction const & ) ;
            setCLJFunction_function_type setCLJFunction_function_value( &::SireMM::InterGroupFF::setCLJFunction );
            
            InterGroupFF_exposer.def( 
                "setCLJFunction"
                , setCLJFunction_function_value
                , ( bp::arg("cljfunc") ) );
        
        }
        { //::SireMM::InterGroupFF::setProperty
        
            typedef bool ( ::SireMM::InterGroupFF::*setProperty_function_type )( ::QString const &,::SireBase::Property const & ) ;
            setProperty_function_type setProperty_function_value( &::SireMM::InterGroupFF::setProperty );
            
            InterGroupFF_exposer.def( 
                "setProperty"
                , setProperty_function_value
                , ( bp::arg("name"), bp::arg("property") ) );
        
        }
        { //::SireMM::InterGroupFF::setUseParallelCalculation
        
            typedef void ( ::SireMM::InterGroupFF::*setUseParallelCalculation_function_type )( bool ) ;
            setUseParallelCalculation_function_type setUseParallelCalculation_function_value( &::SireMM::InterGroupFF::setUseParallelCalculation );
            
            InterGroupFF_exposer.def( 
                "setUseParallelCalculation"
                , setUseParallelCalculation_function_value
                , ( bp::arg("on") ) );
        
        }
        { //::SireMM::InterGroupFF::setUseReproducibleCalculation
        
            typedef void ( ::SireMM::InterGroupFF::*setUseReproducibleCalculation_function_type )( bool ) ;
            setUseReproducibleCalculation_function_type setUseReproducibleCalculation_function_value( &::SireMM::InterGroupFF::setUseReproducibleCalculation );
            
            InterGroupFF_exposer.def( 
                "setUseReproducibleCalculation"
                , setUseReproducibleCalculation_function_value
                , ( bp::arg("on") ) );
        
        }
        { //::SireMM::InterGroupFF::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::InterGroupFF::typeName );
            
            InterGroupFF_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::InterGroupFF::usesParallelCalculation
        
            typedef bool ( ::SireMM::InterGroupFF::*usesParallelCalculation_function_type )(  ) const;
            usesParallelCalculation_function_type usesParallelCalculation_function_value( &::SireMM::InterGroupFF::usesParallelCalculation );
            
            InterGroupFF_exposer.def( 
                "usesParallelCalculation"
                , usesParallelCalculation_function_value );
        
        }
        { //::SireMM::InterGroupFF::usesReproducibleCalculation
        
            typedef bool ( ::SireMM::InterGroupFF::*usesReproducibleCalculation_function_type )(  ) const;
            usesReproducibleCalculation_function_type usesReproducibleCalculation_function_value( &::SireMM::InterGroupFF::usesReproducibleCalculation );
            
            InterGroupFF_exposer.def( 
                "usesReproducibleCalculation"
                , usesReproducibleCalculation_function_value );
        
        }
        { //::SireMM::InterGroupFF::what
        
            typedef char const * ( ::SireMM::InterGroupFF::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::InterGroupFF::what );
            
            InterGroupFF_exposer.def( 
                "what"
                , what_function_value );
        
        }
        InterGroupFF_exposer.staticmethod( "typeName" );
        InterGroupFF_exposer.def( "__copy__", &__copy__);
        InterGroupFF_exposer.def( "__deepcopy__", &__copy__);
        InterGroupFF_exposer.def( "clone", &__copy__);
        InterGroupFF_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::InterGroupFF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        InterGroupFF_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::InterGroupFF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        InterGroupFF_exposer.def( "__str__", &__str__< ::SireMM::InterGroupFF > );
        InterGroupFF_exposer.def( "__repr__", &__str__< ::SireMM::InterGroupFF > );
        InterGroupFF_exposer.def( "__len__", &__len_count< ::SireMM::InterGroupFF > );
    }

}
