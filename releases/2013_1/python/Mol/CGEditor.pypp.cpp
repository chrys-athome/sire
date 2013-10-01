// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "CGEditor.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "atom.h"

#include "atomeditor.h"

#include "cgeditor.h"

#include "chain.h"

#include "chaineditor.h"

#include "cutgroup.h"

#include "groupatomids.h"

#include "molecule.h"

#include "moleditor.h"

#include "mover.hpp"

#include "reseditor.h"

#include "residue.h"

#include "segeditor.h"

#include "segment.h"

#include "selector.hpp"

#include "cgeditor.h"

SireMol::CGEditor __copy__(const SireMol::CGEditor &other){ return SireMol::CGEditor(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_CGEditor_class(){

    { //::SireMol::CGEditor
        typedef bp::class_< SireMol::CGEditor, bp::bases< SireMol::Editor<SireMol::CGEditor, SireMol::CutGroup>, SireMol::CutGroup, SireMol::MoleculeView, SireBase::Property > > CGEditor_exposer_t;
        CGEditor_exposer_t CGEditor_exposer = CGEditor_exposer_t( "CGEditor", bp::init< >() );
        bp::scope CGEditor_scope( CGEditor_exposer );
        CGEditor_exposer.def( bp::init< SireMol::CutGroup const & >(( bp::arg("cutgroup") )) );
        CGEditor_exposer.def( bp::init< SireMol::CGEditor const & >(( bp::arg("other") )) );
        { //::SireMol::CGEditor::add
        
            typedef ::SireMol::AtomStructureEditor ( ::SireMol::CGEditor::*add_function_type )( ::SireMol::AtomName const & ) const;
            add_function_type add_function_value( &::SireMol::CGEditor::add );
            
            CGEditor_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("atomname") ) );
        
        }
        { //::SireMol::CGEditor::add
        
            typedef ::SireMol::AtomStructureEditor ( ::SireMol::CGEditor::*add_function_type )( ::SireMol::AtomNum ) const;
            add_function_type add_function_value( &::SireMol::CGEditor::add );
            
            CGEditor_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("atomnum") ) );
        
        }
        { //::SireMol::CGEditor::commit
        
            typedef ::SireMol::CutGroup ( ::SireMol::CGEditor::*commit_function_type )(  ) const;
            commit_function_type commit_function_value( &::SireMol::CGEditor::commit );
            
            CGEditor_exposer.def( 
                "commit"
                , commit_function_value );
        
        }
        { //::SireMol::CGEditor::operator=
        
            typedef ::SireMol::CGEditor & ( ::SireMol::CGEditor::*assign_function_type )( ::SireMol::CutGroup const & ) ;
            assign_function_type assign_function_value( &::SireMol::CGEditor::operator= );
            
            CGEditor_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("cutgroup") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::CGEditor::operator=
        
            typedef ::SireMol::CGEditor & ( ::SireMol::CGEditor::*assign_function_type )( ::SireMol::CGEditor const & ) ;
            assign_function_type assign_function_value( &::SireMol::CGEditor::operator= );
            
            CGEditor_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::CGEditor::reindex
        
            typedef ::SireMol::CGStructureEditor ( ::SireMol::CGEditor::*reindex_function_type )( ::SireMol::CGIdx ) const;
            reindex_function_type reindex_function_value( &::SireMol::CGEditor::reindex );
            
            CGEditor_exposer.def( 
                "reindex"
                , reindex_function_value
                , ( bp::arg("index") ) );
        
        }
        { //::SireMol::CGEditor::remove
        
            typedef ::SireMol::MolStructureEditor ( ::SireMol::CGEditor::*remove_function_type )(  ) const;
            remove_function_type remove_function_value( &::SireMol::CGEditor::remove );
            
            CGEditor_exposer.def( 
                "remove"
                , remove_function_value );
        
        }
        { //::SireMol::CGEditor::remove
        
            typedef ::SireMol::CGStructureEditor ( ::SireMol::CGEditor::*remove_function_type )( ::SireMol::AtomID const & ) const;
            remove_function_type remove_function_value( &::SireMol::CGEditor::remove );
            
            CGEditor_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::CGEditor::remove
        
            typedef ::SireMol::CGStructureEditor ( ::SireMol::CGEditor::*remove_function_type )( int ) const;
            remove_function_type remove_function_value( &::SireMol::CGEditor::remove );
            
            CGEditor_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMol::CGEditor::rename
        
            typedef ::SireMol::CGEditor & ( ::SireMol::CGEditor::*rename_function_type )( ::SireMol::CGName const & ) ;
            rename_function_type rename_function_value( &::SireMol::CGEditor::rename );
            
            CGEditor_exposer.def( 
                "rename"
                , rename_function_value
                , ( bp::arg("name") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::CGEditor::toString
        
            typedef ::QString ( ::SireMol::CGEditor::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::CGEditor::toString );
            
            CGEditor_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::CGEditor::transfer
        
            typedef ::SireMol::CGStructureEditor ( ::SireMol::CGEditor::*transfer_function_type )( ::SireMol::AtomID const &,::SireMol::CGID const & ) const;
            transfer_function_type transfer_function_value( &::SireMol::CGEditor::transfer );
            
            CGEditor_exposer.def( 
                "transfer"
                , transfer_function_value
                , ( bp::arg("atomid"), bp::arg("cgid") ) );
        
        }
        { //::SireMol::CGEditor::transfer
        
            typedef ::SireMol::CGStructureEditor ( ::SireMol::CGEditor::*transfer_function_type )( int,::SireMol::CGID const & ) const;
            transfer_function_type transfer_function_value( &::SireMol::CGEditor::transfer );
            
            CGEditor_exposer.def( 
                "transfer"
                , transfer_function_value
                , ( bp::arg("i"), bp::arg("cgid") ) );
        
        }
        { //::SireMol::CGEditor::transferAll
        
            typedef ::SireMol::CGStructureEditor ( ::SireMol::CGEditor::*transferAll_function_type )( ::SireMol::CGID const & ) const;
            transferAll_function_type transferAll_function_value( &::SireMol::CGEditor::transferAll );
            
            CGEditor_exposer.def( 
                "transferAll"
                , transferAll_function_value
                , ( bp::arg("cgid") ) );
        
        }
        { //::SireMol::CGEditor::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::CGEditor::typeName );
            
            CGEditor_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        CGEditor_exposer.staticmethod( "typeName" );
        CGEditor_exposer.def( "__copy__", &__copy__);
        CGEditor_exposer.def( "__deepcopy__", &__copy__);
        CGEditor_exposer.def( "clone", &__copy__);
        CGEditor_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::CGEditor >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CGEditor_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::CGEditor >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CGEditor_exposer.def( "__str__", &__str__< ::SireMol::CGEditor > );
        CGEditor_exposer.def( "__repr__", &__str__< ::SireMol::CGEditor > );
    }

}