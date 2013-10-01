// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "NullRestraint.pypp.hpp"

namespace bp = boost::python;

#include "SireCAS/errors.h"

#include "SireCAS/expression.h"

#include "SireCAS/symbols.h"

#include "SireCAS/values.h"

#include "SireError/errors.h"

#include "SireFF/forcetable.h"

#include "SireMol/moleculedata.h"

#include "SireMol/molecules.h"

#include "SireMol/molid.h"

#include "SireMol/molnum.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "restraint.h"

#include "restraint.h"

SireMM::NullRestraint __copy__(const SireMM::NullRestraint &other){ return SireMM::NullRestraint(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_NullRestraint_class(){

    { //::SireMM::NullRestraint
        typedef bp::class_< SireMM::NullRestraint, bp::bases< SireMM::Restraint3D, SireMM::Restraint, SireBase::Property > > NullRestraint_exposer_t;
        NullRestraint_exposer_t NullRestraint_exposer = NullRestraint_exposer_t( "NullRestraint", bp::init< >() );
        bp::scope NullRestraint_scope( NullRestraint_exposer );
        NullRestraint_exposer.def( bp::init< SireMM::NullRestraint const & >(( bp::arg("other") )) );
        { //::SireMM::NullRestraint::builtinSymbols
        
            typedef ::SireCAS::Symbols ( ::SireMM::NullRestraint::*builtinSymbols_function_type )(  ) const;
            builtinSymbols_function_type builtinSymbols_function_value( &::SireMM::NullRestraint::builtinSymbols );
            
            NullRestraint_exposer.def( 
                "builtinSymbols"
                , builtinSymbols_function_value );
        
        }
        { //::SireMM::NullRestraint::builtinValues
        
            typedef ::SireCAS::Values ( ::SireMM::NullRestraint::*builtinValues_function_type )(  ) const;
            builtinValues_function_type builtinValues_function_value( &::SireMM::NullRestraint::builtinValues );
            
            NullRestraint_exposer.def( 
                "builtinValues"
                , builtinValues_function_value );
        
        }
        { //::SireMM::NullRestraint::contains
        
            typedef bool ( ::SireMM::NullRestraint::*contains_function_type )( ::SireMol::MolNum ) const;
            contains_function_type contains_function_value( &::SireMM::NullRestraint::contains );
            
            NullRestraint_exposer.def( 
                "contains"
                , contains_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireMM::NullRestraint::contains
        
            typedef bool ( ::SireMM::NullRestraint::*contains_function_type )( ::SireMol::MolID const & ) const;
            contains_function_type contains_function_value( &::SireMM::NullRestraint::contains );
            
            NullRestraint_exposer.def( 
                "contains"
                , contains_function_value
                , ( bp::arg("molid") ) );
        
        }
        { //::SireMM::NullRestraint::differentiate
        
            typedef ::SireMM::RestraintPtr ( ::SireMM::NullRestraint::*differentiate_function_type )( ::SireCAS::Symbol const & ) const;
            differentiate_function_type differentiate_function_value( &::SireMM::NullRestraint::differentiate );
            
            NullRestraint_exposer.def( 
                "differentiate"
                , differentiate_function_value
                , ( bp::arg("symbol") ) );
        
        }
        { //::SireMM::NullRestraint::energy
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireMM::NullRestraint::*energy_function_type )(  ) const;
            energy_function_type energy_function_value( &::SireMM::NullRestraint::energy );
            
            NullRestraint_exposer.def( 
                "energy"
                , energy_function_value );
        
        }
        { //::SireMM::NullRestraint::force
        
            typedef void ( ::SireMM::NullRestraint::*force_function_type )( ::SireFF::MolForceTable &,double ) const;
            force_function_type force_function_value( &::SireMM::NullRestraint::force );
            
            NullRestraint_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("scale_force")=1 ) );
        
        }
        { //::SireMM::NullRestraint::force
        
            typedef void ( ::SireMM::NullRestraint::*force_function_type )( ::SireFF::ForceTable &,double ) const;
            force_function_type force_function_value( &::SireMM::NullRestraint::force );
            
            NullRestraint_exposer.def( 
                "force"
                , force_function_value
                , ( bp::arg("forcetable"), bp::arg("scale_force")=1 ) );
        
        }
        { //::SireMM::NullRestraint::getValue
        
            typedef double ( ::SireMM::NullRestraint::*getValue_function_type )( ::SireCAS::Symbol const & ) const;
            getValue_function_type getValue_function_value( &::SireMM::NullRestraint::getValue );
            
            NullRestraint_exposer.def( 
                "getValue"
                , getValue_function_value
                , ( bp::arg("symbol") ) );
        
        }
        { //::SireMM::NullRestraint::hasValue
        
            typedef bool ( ::SireMM::NullRestraint::*hasValue_function_type )( ::SireCAS::Symbol const & ) const;
            hasValue_function_type hasValue_function_value( &::SireMM::NullRestraint::hasValue );
            
            NullRestraint_exposer.def( 
                "hasValue"
                , hasValue_function_value
                , ( bp::arg("symbol") ) );
        
        }
        { //::SireMM::NullRestraint::molecules
        
            typedef ::SireMol::Molecules ( ::SireMM::NullRestraint::*molecules_function_type )(  ) const;
            molecules_function_type molecules_function_value( &::SireMM::NullRestraint::molecules );
            
            NullRestraint_exposer.def( 
                "molecules"
                , molecules_function_value );
        
        }
        NullRestraint_exposer.def( bp::self != bp::self );
        { //::SireMM::NullRestraint::operator=
        
            typedef ::SireMM::NullRestraint & ( ::SireMM::NullRestraint::*assign_function_type )( ::SireMM::NullRestraint const & ) ;
            assign_function_type assign_function_value( &::SireMM::NullRestraint::operator= );
            
            NullRestraint_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        NullRestraint_exposer.def( bp::self == bp::self );
        { //::SireMM::NullRestraint::setValue
        
            typedef void ( ::SireMM::NullRestraint::*setValue_function_type )( ::SireCAS::Symbol const &,double ) ;
            setValue_function_type setValue_function_value( &::SireMM::NullRestraint::setValue );
            
            NullRestraint_exposer.def( 
                "setValue"
                , setValue_function_value
                , ( bp::arg("symbol"), bp::arg("value") ) );
        
        }
        { //::SireMM::NullRestraint::symbols
        
            typedef ::SireCAS::Symbols ( ::SireMM::NullRestraint::*symbols_function_type )(  ) const;
            symbols_function_type symbols_function_value( &::SireMM::NullRestraint::symbols );
            
            NullRestraint_exposer.def( 
                "symbols"
                , symbols_function_value );
        
        }
        { //::SireMM::NullRestraint::toString
        
            typedef ::QString ( ::SireMM::NullRestraint::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMM::NullRestraint::toString );
            
            NullRestraint_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMM::NullRestraint::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::NullRestraint::typeName );
            
            NullRestraint_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::NullRestraint::update
        
            typedef void ( ::SireMM::NullRestraint::*update_function_type )( ::SireMol::MoleculeData const & ) ;
            update_function_type update_function_value( &::SireMM::NullRestraint::update );
            
            NullRestraint_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("moldata") ) );
        
        }
        { //::SireMM::NullRestraint::update
        
            typedef void ( ::SireMM::NullRestraint::*update_function_type )( ::SireMol::Molecules const & ) ;
            update_function_type update_function_value( &::SireMM::NullRestraint::update );
            
            NullRestraint_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireMM::NullRestraint::userSymbols
        
            typedef ::SireCAS::Symbols ( ::SireMM::NullRestraint::*userSymbols_function_type )(  ) const;
            userSymbols_function_type userSymbols_function_value( &::SireMM::NullRestraint::userSymbols );
            
            NullRestraint_exposer.def( 
                "userSymbols"
                , userSymbols_function_value );
        
        }
        { //::SireMM::NullRestraint::userValues
        
            typedef ::SireCAS::Values ( ::SireMM::NullRestraint::*userValues_function_type )(  ) const;
            userValues_function_type userValues_function_value( &::SireMM::NullRestraint::userValues );
            
            NullRestraint_exposer.def( 
                "userValues"
                , userValues_function_value );
        
        }
        { //::SireMM::NullRestraint::usesMoleculesIn
        
            typedef bool ( ::SireMM::NullRestraint::*usesMoleculesIn_function_type )( ::SireFF::ForceTable const & ) const;
            usesMoleculesIn_function_type usesMoleculesIn_function_value( &::SireMM::NullRestraint::usesMoleculesIn );
            
            NullRestraint_exposer.def( 
                "usesMoleculesIn"
                , usesMoleculesIn_function_value
                , ( bp::arg("forcetable") ) );
        
        }
        { //::SireMM::NullRestraint::usesMoleculesIn
        
            typedef bool ( ::SireMM::NullRestraint::*usesMoleculesIn_function_type )( ::SireMol::Molecules const & ) const;
            usesMoleculesIn_function_type usesMoleculesIn_function_value( &::SireMM::NullRestraint::usesMoleculesIn );
            
            NullRestraint_exposer.def( 
                "usesMoleculesIn"
                , usesMoleculesIn_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireMM::NullRestraint::values
        
            typedef ::SireCAS::Values ( ::SireMM::NullRestraint::*values_function_type )(  ) const;
            values_function_type values_function_value( &::SireMM::NullRestraint::values );
            
            NullRestraint_exposer.def( 
                "values"
                , values_function_value );
        
        }
        NullRestraint_exposer.staticmethod( "typeName" );
        NullRestraint_exposer.def( "__copy__", &__copy__);
        NullRestraint_exposer.def( "__deepcopy__", &__copy__);
        NullRestraint_exposer.def( "clone", &__copy__);
        NullRestraint_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::NullRestraint >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullRestraint_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::NullRestraint >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullRestraint_exposer.def( "__str__", &__str__< ::SireMM::NullRestraint > );
        NullRestraint_exposer.def( "__repr__", &__str__< ::SireMM::NullRestraint > );
    }

}