// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ChainEditor.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "atom.h"

#include "atomeditor.h"

#include "chain.h"

#include "chaineditor.h"

#include "chainresid.h"

#include "groupatomids.h"

#include "molecule.h"

#include "moleditor.h"

#include "mover.hpp"

#include "reseditor.h"

#include "residue.h"

#include "segeditor.h"

#include "segment.h"

#include "selector.hpp"

#include "chaineditor.h"

SireMol::ChainEditor __copy__(const SireMol::ChainEditor &other){ return SireMol::ChainEditor(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_ChainEditor_class(){

    { //::SireMol::ChainEditor
        typedef bp::class_< SireMol::ChainEditor, bp::bases< SireMol::Editor<SireMol::ChainEditor, SireMol::Chain>, SireMol::Chain, SireMol::MoleculeView, SireBase::Property > > ChainEditor_exposer_t;
        ChainEditor_exposer_t ChainEditor_exposer = ChainEditor_exposer_t( "ChainEditor", bp::init< >() );
        bp::scope ChainEditor_scope( ChainEditor_exposer );
        ChainEditor_exposer.def( bp::init< SireMol::Chain const & >(( bp::arg("chain") )) );
        ChainEditor_exposer.def( bp::init< SireMol::ChainEditor const & >(( bp::arg("other") )) );
        { //::SireMol::ChainEditor::add
        
            typedef ::SireMol::ResStructureEditor ( ::SireMol::ChainEditor::*add_function_type )( ::SireMol::ResName const & ) const;
            add_function_type add_function_value( &::SireMol::ChainEditor::add );
            
            ChainEditor_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("resname") ) );
        
        }
        { //::SireMol::ChainEditor::add
        
            typedef ::SireMol::ResStructureEditor ( ::SireMol::ChainEditor::*add_function_type )( ::SireMol::ResNum ) const;
            add_function_type add_function_value( &::SireMol::ChainEditor::add );
            
            ChainEditor_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("atomnum") ) );
        
        }
        { //::SireMol::ChainEditor::atom
        
            typedef ::SireMol::AtomEditor ( ::SireMol::ChainEditor::*atom_function_type )( ::SireMol::AtomID const & ) const;
            atom_function_type atom_function_value( &::SireMol::ChainEditor::atom );
            
            ChainEditor_exposer.def( 
                "atom"
                , atom_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::ChainEditor::commit
        
            typedef ::SireMol::Chain ( ::SireMol::ChainEditor::*commit_function_type )(  ) const;
            commit_function_type commit_function_value( &::SireMol::ChainEditor::commit );
            
            ChainEditor_exposer.def( 
                "commit"
                , commit_function_value );
        
        }
        { //::SireMol::ChainEditor::molecule
        
            typedef ::SireMol::MolEditor ( ::SireMol::ChainEditor::*molecule_function_type )(  ) const;
            molecule_function_type molecule_function_value( &::SireMol::ChainEditor::molecule );
            
            ChainEditor_exposer.def( 
                "molecule"
                , molecule_function_value );
        
        }
        { //::SireMol::ChainEditor::operator=
        
            typedef ::SireMol::ChainEditor & ( ::SireMol::ChainEditor::*assign_function_type )( ::SireMol::Chain const & ) ;
            assign_function_type assign_function_value( &::SireMol::ChainEditor::operator= );
            
            ChainEditor_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("chain") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::ChainEditor::operator=
        
            typedef ::SireMol::ChainEditor & ( ::SireMol::ChainEditor::*assign_function_type )( ::SireMol::ChainEditor const & ) ;
            assign_function_type assign_function_value( &::SireMol::ChainEditor::operator= );
            
            ChainEditor_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::ChainEditor::reindex
        
            typedef ::SireMol::ChainStructureEditor ( ::SireMol::ChainEditor::*reindex_function_type )( ::SireMol::ChainIdx ) const;
            reindex_function_type reindex_function_value( &::SireMol::ChainEditor::reindex );
            
            ChainEditor_exposer.def( 
                "reindex"
                , reindex_function_value
                , ( bp::arg("index") ) );
        
        }
        { //::SireMol::ChainEditor::remove
        
            typedef ::SireMol::MolStructureEditor ( ::SireMol::ChainEditor::*remove_function_type )(  ) const;
            remove_function_type remove_function_value( &::SireMol::ChainEditor::remove );
            
            ChainEditor_exposer.def( 
                "remove"
                , remove_function_value );
        
        }
        { //::SireMol::ChainEditor::remove
        
            typedef ::SireMol::ChainStructureEditor ( ::SireMol::ChainEditor::*remove_function_type )( ::SireMol::AtomID const & ) const;
            remove_function_type remove_function_value( &::SireMol::ChainEditor::remove );
            
            ChainEditor_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::ChainEditor::remove
        
            typedef ::SireMol::ChainStructureEditor ( ::SireMol::ChainEditor::*remove_function_type )( ::SireMol::ResID const & ) const;
            remove_function_type remove_function_value( &::SireMol::ChainEditor::remove );
            
            ChainEditor_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::ChainEditor::remove
        
            typedef ::SireMol::ChainStructureEditor ( ::SireMol::ChainEditor::*remove_function_type )( int ) const;
            remove_function_type remove_function_value( &::SireMol::ChainEditor::remove );
            
            ChainEditor_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMol::ChainEditor::rename
        
            typedef ::SireMol::ChainEditor & ( ::SireMol::ChainEditor::*rename_function_type )( ::SireMol::ChainName const & ) ;
            rename_function_type rename_function_value( &::SireMol::ChainEditor::rename );
            
            ChainEditor_exposer.def( 
                "rename"
                , rename_function_value
                , ( bp::arg("name") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::ChainEditor::residue
        
            typedef ::SireMol::ResEditor ( ::SireMol::ChainEditor::*residue_function_type )( int ) const;
            residue_function_type residue_function_value( &::SireMol::ChainEditor::residue );
            
            ChainEditor_exposer.def( 
                "residue"
                , residue_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMol::ChainEditor::residue
        
            typedef ::SireMol::ResEditor ( ::SireMol::ChainEditor::*residue_function_type )( ::SireMol::ResID const & ) const;
            residue_function_type residue_function_value( &::SireMol::ChainEditor::residue );
            
            ChainEditor_exposer.def( 
                "residue"
                , residue_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::ChainEditor::select
        
            typedef ::SireMol::AtomEditor ( ::SireMol::ChainEditor::*select_function_type )( ::SireMol::AtomID const & ) const;
            select_function_type select_function_value( &::SireMol::ChainEditor::select );
            
            ChainEditor_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::ChainEditor::select
        
            typedef ::SireMol::ResEditor ( ::SireMol::ChainEditor::*select_function_type )( int ) const;
            select_function_type select_function_value( &::SireMol::ChainEditor::select );
            
            ChainEditor_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMol::ChainEditor::select
        
            typedef ::SireMol::ResEditor ( ::SireMol::ChainEditor::*select_function_type )( ::SireMol::ResID const & ) const;
            select_function_type select_function_value( &::SireMol::ChainEditor::select );
            
            ChainEditor_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::ChainEditor::toString
        
            typedef ::QString ( ::SireMol::ChainEditor::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::ChainEditor::toString );
            
            ChainEditor_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::ChainEditor::transfer
        
            typedef ::SireMol::ChainStructureEditor ( ::SireMol::ChainEditor::*transfer_function_type )( ::SireMol::ResID const &,::SireMol::ChainID const & ) const;
            transfer_function_type transfer_function_value( &::SireMol::ChainEditor::transfer );
            
            ChainEditor_exposer.def( 
                "transfer"
                , transfer_function_value
                , ( bp::arg("resid"), bp::arg("chainid") ) );
        
        }
        { //::SireMol::ChainEditor::transfer
        
            typedef ::SireMol::ChainStructureEditor ( ::SireMol::ChainEditor::*transfer_function_type )( int,::SireMol::ChainID const & ) const;
            transfer_function_type transfer_function_value( &::SireMol::ChainEditor::transfer );
            
            ChainEditor_exposer.def( 
                "transfer"
                , transfer_function_value
                , ( bp::arg("i"), bp::arg("chainid") ) );
        
        }
        { //::SireMol::ChainEditor::transferAll
        
            typedef ::SireMol::ChainStructureEditor ( ::SireMol::ChainEditor::*transferAll_function_type )( ::SireMol::ChainID const & ) const;
            transferAll_function_type transferAll_function_value( &::SireMol::ChainEditor::transferAll );
            
            ChainEditor_exposer.def( 
                "transferAll"
                , transferAll_function_value
                , ( bp::arg("chainid") ) );
        
        }
        { //::SireMol::ChainEditor::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::ChainEditor::typeName );
            
            ChainEditor_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        ChainEditor_exposer.staticmethod( "typeName" );
        ChainEditor_exposer.def( "__copy__", &__copy__);
        ChainEditor_exposer.def( "__deepcopy__", &__copy__);
        ChainEditor_exposer.def( "clone", &__copy__);
        ChainEditor_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::ChainEditor >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ChainEditor_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::ChainEditor >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ChainEditor_exposer.def( "__str__", &__str__< ::SireMol::ChainEditor > );
        ChainEditor_exposer.def( "__repr__", &__str__< ::SireMol::ChainEditor > );
    }

}
