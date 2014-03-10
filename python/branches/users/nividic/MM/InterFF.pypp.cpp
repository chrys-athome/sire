// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "InterFF.pypp.hpp"

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

#include "interff.h"

#include <QDebug>

#include <QElapsedTimer>

#include "interff.h"

SireMM::InterFF __copy__(const SireMM::InterFF &other){ return SireMM::InterFF(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_InterFF_class(){

    { //::SireMM::InterFF
        typedef bp::class_< SireMM::InterFF, bp::bases< SireFF::G1FF, SireFF::FF, SireMol::MolGroupsBase, SireBase::Property > > InterFF_exposer_t;
        InterFF_exposer_t InterFF_exposer = InterFF_exposer_t( "InterFF", bp::init< >() );
        bp::scope InterFF_scope( InterFF_exposer );
        InterFF_exposer.def( bp::init< QString const & >(( bp::arg("name") )) );
        InterFF_exposer.def( bp::init< SireMM::InterFF const & >(( bp::arg("other") )) );
        { //::SireMM::InterFF::accept
        
            typedef void ( ::SireMM::InterFF::*accept_function_type )(  ) ;
            accept_function_type accept_function_value( &::SireMM::InterFF::accept );
            
            InterFF_exposer.def( 
                "accept"
                , accept_function_value );
        
        }
        { //::SireMM::InterFF::addFixedAtoms
        
            typedef void ( ::SireMM::InterFF::*addFixedAtoms_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) ;
            addFixedAtoms_function_type addFixedAtoms_function_value( &::SireMM::InterFF::addFixedAtoms );
            
            InterFF_exposer.def( 
                "addFixedAtoms"
                , addFixedAtoms_function_value
                , ( bp::arg("molecule"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMM::InterFF::addFixedAtoms
        
            typedef void ( ::SireMM::InterFF::*addFixedAtoms_function_type )( ::SireMol::Molecules const &,::SireBase::PropertyMap const & ) ;
            addFixedAtoms_function_type addFixedAtoms_function_value( &::SireMM::InterFF::addFixedAtoms );
            
            InterFF_exposer.def( 
                "addFixedAtoms"
                , addFixedAtoms_function_value
                , ( bp::arg("molecules"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMM::InterFF::addFixedAtoms
        
            typedef void ( ::SireMM::InterFF::*addFixedAtoms_function_type )( ::SireMM::CLJAtoms const & ) ;
            addFixedAtoms_function_type addFixedAtoms_function_value( &::SireMM::InterFF::addFixedAtoms );
            
            InterFF_exposer.def( 
                "addFixedAtoms"
                , addFixedAtoms_function_value
                , ( bp::arg("atoms") ) );
        
        }
        { //::SireMM::InterFF::cljFunction
        
            typedef ::SireMM::CLJFunction const & ( ::SireMM::InterFF::*cljFunction_function_type )(  ) const;
            cljFunction_function_type cljFunction_function_value( &::SireMM::InterFF::cljFunction );
            
            InterFF_exposer.def( 
                "cljFunction"
                , cljFunction_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InterFF::components
        
            typedef ::SireMM::CLJComponent const & ( ::SireMM::InterFF::*components_function_type )(  ) const;
            components_function_type components_function_value( &::SireMM::InterFF::components );
            
            InterFF_exposer.def( 
                "components"
                , components_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InterFF::containsProperty
        
            typedef bool ( ::SireMM::InterFF::*containsProperty_function_type )( ::QString const & ) const;
            containsProperty_function_type containsProperty_function_value( &::SireMM::InterFF::containsProperty );
            
            InterFF_exposer.def( 
                "containsProperty"
                , containsProperty_function_value
                , ( bp::arg("name") ) );
        
        }
        { //::SireMM::InterFF::disableGrid
        
            typedef void ( ::SireMM::InterFF::*disableGrid_function_type )(  ) ;
            disableGrid_function_type disableGrid_function_value( &::SireMM::InterFF::disableGrid );
            
            InterFF_exposer.def( 
                "disableGrid"
                , disableGrid_function_value );
        
        }
        { //::SireMM::InterFF::enableGrid
        
            typedef void ( ::SireMM::InterFF::*enableGrid_function_type )(  ) ;
            enableGrid_function_type enableGrid_function_value( &::SireMM::InterFF::enableGrid );
            
            InterFF_exposer.def( 
                "enableGrid"
                , enableGrid_function_value );
        
        }
        { //::SireMM::InterFF::fixedOnly
        
            typedef bool ( ::SireMM::InterFF::*fixedOnly_function_type )(  ) const;
            fixedOnly_function_type fixedOnly_function_value( &::SireMM::InterFF::fixedOnly );
            
            InterFF_exposer.def( 
                "fixedOnly"
                , fixedOnly_function_value );
        
        }
        { //::SireMM::InterFF::grid
        
            typedef ::SireMM::GridInfo ( ::SireMM::InterFF::*grid_function_type )(  ) const;
            grid_function_type grid_function_value( &::SireMM::InterFF::grid );
            
            InterFF_exposer.def( 
                "grid"
                , grid_function_value );
        
        }
        { //::SireMM::InterFF::gridBuffer
        
            typedef ::SireUnits::Dimension::Length ( ::SireMM::InterFF::*gridBuffer_function_type )(  ) const;
            gridBuffer_function_type gridBuffer_function_value( &::SireMM::InterFF::gridBuffer );
            
            InterFF_exposer.def( 
                "gridBuffer"
                , gridBuffer_function_value );
        
        }
        { //::SireMM::InterFF::gridSpacing
        
            typedef ::SireUnits::Dimension::Length ( ::SireMM::InterFF::*gridSpacing_function_type )(  ) const;
            gridSpacing_function_type gridSpacing_function_value( &::SireMM::InterFF::gridSpacing );
            
            InterFF_exposer.def( 
                "gridSpacing"
                , gridSpacing_function_value );
        
        }
        { //::SireMM::InterFF::mustNowRecalculateFromScratch
        
            typedef void ( ::SireMM::InterFF::*mustNowRecalculateFromScratch_function_type )(  ) ;
            mustNowRecalculateFromScratch_function_type mustNowRecalculateFromScratch_function_value( &::SireMM::InterFF::mustNowRecalculateFromScratch );
            
            InterFF_exposer.def( 
                "mustNowRecalculateFromScratch"
                , mustNowRecalculateFromScratch_function_value );
        
        }
        { //::SireMM::InterFF::needsAccepting
        
            typedef bool ( ::SireMM::InterFF::*needsAccepting_function_type )(  ) const;
            needsAccepting_function_type needsAccepting_function_value( &::SireMM::InterFF::needsAccepting );
            
            InterFF_exposer.def( 
                "needsAccepting"
                , needsAccepting_function_value );
        
        }
        InterFF_exposer.def( bp::self != bp::self );
        { //::SireMM::InterFF::operator=
        
            typedef ::SireMM::InterFF & ( ::SireMM::InterFF::*assign_function_type )( ::SireMM::InterFF const & ) ;
            assign_function_type assign_function_value( &::SireMM::InterFF::operator= );
            
            InterFF_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        InterFF_exposer.def( bp::self == bp::self );
        { //::SireMM::InterFF::properties
        
            typedef ::SireBase::Properties const & ( ::SireMM::InterFF::*properties_function_type )(  ) const;
            properties_function_type properties_function_value( &::SireMM::InterFF::properties );
            
            InterFF_exposer.def( 
                "properties"
                , properties_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::InterFF::property
        
            typedef ::SireBase::Property const & ( ::SireMM::InterFF::*property_function_type )( ::QString const & ) const;
            property_function_type property_function_value( &::SireMM::InterFF::property );
            
            InterFF_exposer.def( 
                "property"
                , property_function_value
                , ( bp::arg("name") )
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::InterFF::setCLJFunction
        
            typedef void ( ::SireMM::InterFF::*setCLJFunction_function_type )( ::SireMM::CLJFunction const & ) ;
            setCLJFunction_function_type setCLJFunction_function_value( &::SireMM::InterFF::setCLJFunction );
            
            InterFF_exposer.def( 
                "setCLJFunction"
                , setCLJFunction_function_value
                , ( bp::arg("cljfunc") ) );
        
        }
        { //::SireMM::InterFF::setFixedAtoms
        
            typedef void ( ::SireMM::InterFF::*setFixedAtoms_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) ;
            setFixedAtoms_function_type setFixedAtoms_function_value( &::SireMM::InterFF::setFixedAtoms );
            
            InterFF_exposer.def( 
                "setFixedAtoms"
                , setFixedAtoms_function_value
                , ( bp::arg("molecule"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMM::InterFF::setFixedAtoms
        
            typedef void ( ::SireMM::InterFF::*setFixedAtoms_function_type )( ::SireMol::Molecules const &,::SireBase::PropertyMap const & ) ;
            setFixedAtoms_function_type setFixedAtoms_function_value( &::SireMM::InterFF::setFixedAtoms );
            
            InterFF_exposer.def( 
                "setFixedAtoms"
                , setFixedAtoms_function_value
                , ( bp::arg("molecules"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMM::InterFF::setFixedAtoms
        
            typedef void ( ::SireMM::InterFF::*setFixedAtoms_function_type )( ::SireMM::CLJAtoms const & ) ;
            setFixedAtoms_function_type setFixedAtoms_function_value( &::SireMM::InterFF::setFixedAtoms );
            
            InterFF_exposer.def( 
                "setFixedAtoms"
                , setFixedAtoms_function_value
                , ( bp::arg("atoms") ) );
        
        }
        { //::SireMM::InterFF::setFixedOnly
        
            typedef void ( ::SireMM::InterFF::*setFixedOnly_function_type )( bool ) ;
            setFixedOnly_function_type setFixedOnly_function_value( &::SireMM::InterFF::setFixedOnly );
            
            InterFF_exposer.def( 
                "setFixedOnly"
                , setFixedOnly_function_value
                , ( bp::arg("on") ) );
        
        }
        { //::SireMM::InterFF::setGridBuffer
        
            typedef void ( ::SireMM::InterFF::*setGridBuffer_function_type )( ::SireUnits::Dimension::Length ) ;
            setGridBuffer_function_type setGridBuffer_function_value( &::SireMM::InterFF::setGridBuffer );
            
            InterFF_exposer.def( 
                "setGridBuffer"
                , setGridBuffer_function_value
                , ( bp::arg("buffer") ) );
        
        }
        { //::SireMM::InterFF::setGridSpacing
        
            typedef void ( ::SireMM::InterFF::*setGridSpacing_function_type )( ::SireUnits::Dimension::Length ) ;
            setGridSpacing_function_type setGridSpacing_function_value( &::SireMM::InterFF::setGridSpacing );
            
            InterFF_exposer.def( 
                "setGridSpacing"
                , setGridSpacing_function_value
                , ( bp::arg("spacing") ) );
        
        }
        { //::SireMM::InterFF::setProperty
        
            typedef bool ( ::SireMM::InterFF::*setProperty_function_type )( ::QString const &,::SireBase::Property const & ) ;
            setProperty_function_type setProperty_function_value( &::SireMM::InterFF::setProperty );
            
            InterFF_exposer.def( 
                "setProperty"
                , setProperty_function_value
                , ( bp::arg("name"), bp::arg("property") ) );
        
        }
        { //::SireMM::InterFF::setUseGrid
        
            typedef void ( ::SireMM::InterFF::*setUseGrid_function_type )( bool ) ;
            setUseGrid_function_type setUseGrid_function_value( &::SireMM::InterFF::setUseGrid );
            
            InterFF_exposer.def( 
                "setUseGrid"
                , setUseGrid_function_value
                , ( bp::arg("on") ) );
        
        }
        { //::SireMM::InterFF::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::InterFF::typeName );
            
            InterFF_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::InterFF::usesGrid
        
            typedef bool ( ::SireMM::InterFF::*usesGrid_function_type )(  ) const;
            usesGrid_function_type usesGrid_function_value( &::SireMM::InterFF::usesGrid );
            
            InterFF_exposer.def( 
                "usesGrid"
                , usesGrid_function_value );
        
        }
        { //::SireMM::InterFF::what
        
            typedef char const * ( ::SireMM::InterFF::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::InterFF::what );
            
            InterFF_exposer.def( 
                "what"
                , what_function_value );
        
        }
        InterFF_exposer.staticmethod( "typeName" );
        InterFF_exposer.def( "__copy__", &__copy__);
        InterFF_exposer.def( "__deepcopy__", &__copy__);
        InterFF_exposer.def( "clone", &__copy__);
        InterFF_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::InterFF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        InterFF_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::InterFF >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        InterFF_exposer.def( "__str__", &__str__< ::SireMM::InterFF > );
        InterFF_exposer.def( "__repr__", &__str__< ::SireMM::InterFF > );
        InterFF_exposer.def( "__len__", &__len_count< ::SireMM::InterFF > );
    }

}
