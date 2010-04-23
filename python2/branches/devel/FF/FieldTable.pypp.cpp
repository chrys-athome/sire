// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "FieldTable.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireID/index.h"

#include "SireMol/atomselection.h"

#include "SireMol/errors.h"

#include "SireMol/moleculegroup.h"

#include "SireMol/moleculeview.h"

#include "SireMol/mover.hpp"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "fieldtable.h"

#include "fieldtable.h"

SireFF::FieldTable __copy__(const SireFF::FieldTable &other){ return SireFF::FieldTable(other); }

#include "Qt/qdatastream.hpp"

const char* pvt_get_name(const SireFF::FieldTable&){ return "SireFF::FieldTable";}

void register_FieldTable_class(){

    { //::SireFF::FieldTable
        typedef bp::class_< SireFF::FieldTable > FieldTable_exposer_t;
        FieldTable_exposer_t FieldTable_exposer = FieldTable_exposer_t( "FieldTable" );
        bp::scope FieldTable_scope( FieldTable_exposer );
        FieldTable_exposer.def( bp::init< >() );
        FieldTable_exposer.def( bp::init< SireMol::MoleculeGroup const & >(( bp::arg("molgroup") )) );
        FieldTable_exposer.def( bp::init< SireVol::Grid const & >(( bp::arg("grid") )) );
        FieldTable_exposer.def( bp::init< QVector< SireBase::PropPtr< SireVol::Grid > > const & >(( bp::arg("grids") )) );
        FieldTable_exposer.def( bp::init< SireMol::MoleculeGroup const &, SireVol::Grid const & >(( bp::arg("molgroup"), bp::arg("grid") )) );
        FieldTable_exposer.def( bp::init< SireMol::MoleculeGroup const &, QVector< SireBase::PropPtr< SireVol::Grid > > const & >(( bp::arg("molgroup"), bp::arg("grids") )) );
        FieldTable_exposer.def( bp::init< SireFF::FieldTable const & >(( bp::arg("other") )) );
        { //::SireFF::FieldTable::add
        
            typedef void ( ::SireFF::FieldTable::*add_function_type )( ::SireFF::FieldTable const & ) ;
            add_function_type add_function_value( &::SireFF::FieldTable::add );
            
            FieldTable_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireFF::FieldTable::add
        
            typedef void ( ::SireFF::FieldTable::*add_function_type )( ::SireMaths::Vector const & ) ;
            add_function_type add_function_value( &::SireFF::FieldTable::add );
            
            FieldTable_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("field") ) );
        
        }
        { //::SireFF::FieldTable::assertContainsTableFor
        
            typedef void ( ::SireFF::FieldTable::*assertContainsTableFor_function_type )( ::SireMol::MolNum ) const;
            assertContainsTableFor_function_type assertContainsTableFor_function_value( &::SireFF::FieldTable::assertContainsTableFor );
            
            FieldTable_exposer.def( 
                "assertContainsTableFor"
                , assertContainsTableFor_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireFF::FieldTable::assertContainsTableFor
        
            typedef void ( ::SireFF::FieldTable::*assertContainsTableFor_function_type )( ::SireVol::Grid const & ) const;
            assertContainsTableFor_function_type assertContainsTableFor_function_value( &::SireFF::FieldTable::assertContainsTableFor );
            
            FieldTable_exposer.def( 
                "assertContainsTableFor"
                , assertContainsTableFor_function_value
                , ( bp::arg("grid") ) );
        
        }
        { //::SireFF::FieldTable::constGetTable
        
            typedef ::SireFF::MolFieldTable const & ( ::SireFF::FieldTable::*constGetTable_function_type )( ::SireMol::MolNum ) const;
            constGetTable_function_type constGetTable_function_value( &::SireFF::FieldTable::constGetTable );
            
            FieldTable_exposer.def( 
                "constGetTable"
                , constGetTable_function_value
                , ( bp::arg("molnum") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireFF::FieldTable::constGetTable
        
            typedef ::SireFF::GridFieldTable const & ( ::SireFF::FieldTable::*constGetTable_function_type )( ::SireVol::Grid const & ) const;
            constGetTable_function_type constGetTable_function_value( &::SireFF::FieldTable::constGetTable );
            
            FieldTable_exposer.def( 
                "constGetTable"
                , constGetTable_function_value
                , ( bp::arg("grid") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireFF::FieldTable::contains
        
            typedef bool ( ::SireFF::FieldTable::*contains_function_type )( ::SireMol::MolNum ) const;
            contains_function_type contains_function_value( &::SireFF::FieldTable::contains );
            
            FieldTable_exposer.def( 
                "contains"
                , contains_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireFF::FieldTable::contains
        
            typedef bool ( ::SireFF::FieldTable::*contains_function_type )( ::SireVol::Grid const & ) const;
            contains_function_type contains_function_value( &::SireFF::FieldTable::contains );
            
            FieldTable_exposer.def( 
                "contains"
                , contains_function_value
                , ( bp::arg("grid") ) );
        
        }
        { //::SireFF::FieldTable::divide
        
            typedef void ( ::SireFF::FieldTable::*divide_function_type )( double ) ;
            divide_function_type divide_function_value( &::SireFF::FieldTable::divide );
            
            FieldTable_exposer.def( 
                "divide"
                , divide_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireFF::FieldTable::getTable
        
            typedef ::SireFF::MolFieldTable const & ( ::SireFF::FieldTable::*getTable_function_type )( ::SireMol::MolNum ) const;
            getTable_function_type getTable_function_value( &::SireFF::FieldTable::getTable );
            
            FieldTable_exposer.def( 
                "getTable"
                , getTable_function_value
                , ( bp::arg("molnum") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireFF::FieldTable::getTable
        
            typedef ::SireFF::GridFieldTable const & ( ::SireFF::FieldTable::*getTable_function_type )( ::SireVol::Grid const & ) const;
            getTable_function_type getTable_function_value( &::SireFF::FieldTable::getTable );
            
            FieldTable_exposer.def( 
                "getTable"
                , getTable_function_value
                , ( bp::arg("grid") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireFF::FieldTable::index
        
            typedef ::QHash< SireMol::MolNum, int > const & ( ::SireFF::FieldTable::*index_function_type )(  ) const;
            index_function_type index_function_value( &::SireFF::FieldTable::index );
            
            FieldTable_exposer.def( 
                "index"
                , index_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireFF::FieldTable::indexOf
        
            typedef int ( ::SireFF::FieldTable::*indexOf_function_type )( ::SireMol::MolNum ) const;
            indexOf_function_type indexOf_function_value( &::SireFF::FieldTable::indexOf );
            
            FieldTable_exposer.def( 
                "indexOf"
                , indexOf_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireFF::FieldTable::initialiseTable
        
            typedef void ( ::SireFF::FieldTable::*initialiseTable_function_type )( ::SireMol::MolNum ) ;
            initialiseTable_function_type initialiseTable_function_value( &::SireFF::FieldTable::initialiseTable );
            
            FieldTable_exposer.def( 
                "initialiseTable"
                , initialiseTable_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireFF::FieldTable::initialiseTable
        
            typedef void ( ::SireFF::FieldTable::*initialiseTable_function_type )( ::SireVol::Grid const & ) ;
            initialiseTable_function_type initialiseTable_function_value( &::SireFF::FieldTable::initialiseTable );
            
            FieldTable_exposer.def( 
                "initialiseTable"
                , initialiseTable_function_value
                , ( bp::arg("grid") ) );
        
        }
        { //::SireFF::FieldTable::initialiseTables
        
            typedef void ( ::SireFF::FieldTable::*initialiseTables_function_type )(  ) ;
            initialiseTables_function_type initialiseTables_function_value( &::SireFF::FieldTable::initialiseTables );
            
            FieldTable_exposer.def( 
                "initialiseTables"
                , initialiseTables_function_value );
        
        }
        { //::SireFF::FieldTable::isEmpty
        
            typedef bool ( ::SireFF::FieldTable::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireFF::FieldTable::isEmpty );
            
            FieldTable_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireFF::FieldTable::molNums
        
            typedef ::QList< SireMol::MolNum > ( ::SireFF::FieldTable::*molNums_function_type )(  ) const;
            molNums_function_type molNums_function_value( &::SireFF::FieldTable::molNums );
            
            FieldTable_exposer.def( 
                "molNums"
                , molNums_function_value );
        
        }
        { //::SireFF::FieldTable::multiply
        
            typedef void ( ::SireFF::FieldTable::*multiply_function_type )( double ) ;
            multiply_function_type multiply_function_value( &::SireFF::FieldTable::multiply );
            
            FieldTable_exposer.def( 
                "multiply"
                , multiply_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireFF::FieldTable::nGrids
        
            typedef int ( ::SireFF::FieldTable::*nGrids_function_type )(  ) const;
            nGrids_function_type nGrids_function_value( &::SireFF::FieldTable::nGrids );
            
            FieldTable_exposer.def( 
                "nGrids"
                , nGrids_function_value );
        
        }
        { //::SireFF::FieldTable::nMolecules
        
            typedef int ( ::SireFF::FieldTable::*nMolecules_function_type )(  ) const;
            nMolecules_function_type nMolecules_function_value( &::SireFF::FieldTable::nMolecules );
            
            FieldTable_exposer.def( 
                "nMolecules"
                , nMolecules_function_value );
        
        }
        FieldTable_exposer.def( bp::self != bp::self );
        FieldTable_exposer.def( bp::self * bp::other< double >() );
        FieldTable_exposer.def( bp::self + bp::self );
        FieldTable_exposer.def( bp::self + bp::other< SireMaths::Vector >() );
        FieldTable_exposer.def( bp::self - bp::self );
        FieldTable_exposer.def( bp::self - bp::other< SireMaths::Vector >() );
        FieldTable_exposer.def( -bp::self );
        FieldTable_exposer.def( bp::self / bp::other< double >() );
        { //::SireFF::FieldTable::operator=
        
            typedef ::SireFF::FieldTable & ( ::SireFF::FieldTable::*assign_function_type )( ::SireFF::FieldTable const & ) ;
            assign_function_type assign_function_value( &::SireFF::FieldTable::operator= );
            
            FieldTable_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireFF::FieldTable::operator=
        
            typedef ::SireFF::FieldTable & ( ::SireFF::FieldTable::*assign_function_type )( ::SireMaths::Vector const & ) ;
            assign_function_type assign_function_value( &::SireFF::FieldTable::operator= );
            
            FieldTable_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("field") )
                , bp::return_self< >() );
        
        }
        FieldTable_exposer.def( bp::self == bp::self );
        { //::SireFF::FieldTable::setAll
        
            typedef void ( ::SireFF::FieldTable::*setAll_function_type )( ::SireMaths::Vector const & ) ;
            setAll_function_type setAll_function_value( &::SireFF::FieldTable::setAll );
            
            FieldTable_exposer.def( 
                "setAll"
                , setAll_function_value
                , ( bp::arg("field") ) );
        
        }
        { //::SireFF::FieldTable::subtract
        
            typedef void ( ::SireFF::FieldTable::*subtract_function_type )( ::SireFF::FieldTable const & ) ;
            subtract_function_type subtract_function_value( &::SireFF::FieldTable::subtract );
            
            FieldTable_exposer.def( 
                "subtract"
                , subtract_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireFF::FieldTable::subtract
        
            typedef void ( ::SireFF::FieldTable::*subtract_function_type )( ::SireMaths::Vector const & ) ;
            subtract_function_type subtract_function_value( &::SireFF::FieldTable::subtract );
            
            FieldTable_exposer.def( 
                "subtract"
                , subtract_function_value
                , ( bp::arg("field") ) );
        
        }
        { //::SireFF::FieldTable::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireFF::FieldTable::typeName );
            
            FieldTable_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireFF::FieldTable::what
        
            typedef char const * ( ::SireFF::FieldTable::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireFF::FieldTable::what );
            
            FieldTable_exposer.def( 
                "what"
                , what_function_value );
        
        }
        FieldTable_exposer.staticmethod( "typeName" );
        FieldTable_exposer.def( "__copy__", &__copy__);
        FieldTable_exposer.def( "__deepcopy__", &__copy__);
        FieldTable_exposer.def( "clone", &__copy__);
        FieldTable_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireFF::FieldTable >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        FieldTable_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireFF::FieldTable >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        FieldTable_exposer.def( "__str__", &pvt_get_name);
        FieldTable_exposer.def( "__repr__", &pvt_get_name);
    }

}
