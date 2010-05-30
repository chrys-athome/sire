// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "SegEditor.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "atom.h"

#include "atomeditor.h"

#include "chain.h"

#include "chaineditor.h"

#include "groupatomids.h"

#include "molecule.h"

#include "moleditor.h"

#include "mover.hpp"

#include "reseditor.h"

#include "residue.h"

#include "segeditor.h"

#include "segment.h"

#include "selector.hpp"

#include "segeditor.h"

SireMol::SegEditor __copy__(const SireMol::SegEditor &other){ return SireMol::SegEditor(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_SegEditor_class(){

    { //::SireMol::SegEditor
        typedef bp::class_< SireMol::SegEditor, bp::bases< SireMol::Editor<SireMol::SegEditor, SireMol::Segment>, SireMol::Segment, SireMol::MoleculeView, SireBase::Property > > SegEditor_exposer_t;
        SegEditor_exposer_t SegEditor_exposer = SegEditor_exposer_t( "SegEditor", bp::init< >() );
        bp::scope SegEditor_scope( SegEditor_exposer );
        SegEditor_exposer.def( bp::init< SireMol::Segment const & >(( bp::arg("residue") )) );
        SegEditor_exposer.def( bp::init< SireMol::SegEditor const & >(( bp::arg("other") )) );
        { //::SireMol::SegEditor::add
        
            typedef ::SireMol::AtomStructureEditor ( ::SireMol::SegEditor::*add_function_type )( ::SireMol::AtomName const & ) const;
            add_function_type add_function_value( &::SireMol::SegEditor::add );
            
            SegEditor_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("atomname") ) );
        
        }
        { //::SireMol::SegEditor::add
        
            typedef ::SireMol::AtomStructureEditor ( ::SireMol::SegEditor::*add_function_type )( ::SireMol::AtomNum ) const;
            add_function_type add_function_value( &::SireMol::SegEditor::add );
            
            SegEditor_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("atomnum") ) );
        
        }
        { //::SireMol::SegEditor::atom
        
            typedef ::SireMol::AtomEditor ( ::SireMol::SegEditor::*atom_function_type )( int ) const;
            atom_function_type atom_function_value( &::SireMol::SegEditor::atom );
            
            SegEditor_exposer.def( 
                "atom"
                , atom_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMol::SegEditor::atom
        
            typedef ::SireMol::AtomEditor ( ::SireMol::SegEditor::*atom_function_type )( ::SireMol::AtomID const & ) const;
            atom_function_type atom_function_value( &::SireMol::SegEditor::atom );
            
            SegEditor_exposer.def( 
                "atom"
                , atom_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::SegEditor::commit
        
            typedef ::SireMol::Segment ( ::SireMol::SegEditor::*commit_function_type )(  ) const;
            commit_function_type commit_function_value( &::SireMol::SegEditor::commit );
            
            SegEditor_exposer.def( 
                "commit"
                , commit_function_value );
        
        }
        { //::SireMol::SegEditor::molecule
        
            typedef ::SireMol::MolEditor ( ::SireMol::SegEditor::*molecule_function_type )(  ) const;
            molecule_function_type molecule_function_value( &::SireMol::SegEditor::molecule );
            
            SegEditor_exposer.def( 
                "molecule"
                , molecule_function_value );
        
        }
        { //::SireMol::SegEditor::operator=
        
            typedef ::SireMol::SegEditor & ( ::SireMol::SegEditor::*assign_function_type )( ::SireMol::Segment const & ) ;
            assign_function_type assign_function_value( &::SireMol::SegEditor::operator= );
            
            SegEditor_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("residue") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::SegEditor::operator=
        
            typedef ::SireMol::SegEditor & ( ::SireMol::SegEditor::*assign_function_type )( ::SireMol::SegEditor const & ) ;
            assign_function_type assign_function_value( &::SireMol::SegEditor::operator= );
            
            SegEditor_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::SegEditor::reindex
        
            typedef ::SireMol::SegStructureEditor ( ::SireMol::SegEditor::*reindex_function_type )( ::SireMol::SegIdx ) const;
            reindex_function_type reindex_function_value( &::SireMol::SegEditor::reindex );
            
            SegEditor_exposer.def( 
                "reindex"
                , reindex_function_value
                , ( bp::arg("index") ) );
        
        }
        { //::SireMol::SegEditor::remove
        
            typedef ::SireMol::MolStructureEditor ( ::SireMol::SegEditor::*remove_function_type )(  ) const;
            remove_function_type remove_function_value( &::SireMol::SegEditor::remove );
            
            SegEditor_exposer.def( 
                "remove"
                , remove_function_value );
        
        }
        { //::SireMol::SegEditor::remove
        
            typedef ::SireMol::SegStructureEditor ( ::SireMol::SegEditor::*remove_function_type )( ::SireMol::AtomID const & ) const;
            remove_function_type remove_function_value( &::SireMol::SegEditor::remove );
            
            SegEditor_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::SegEditor::remove
        
            typedef ::SireMol::SegStructureEditor ( ::SireMol::SegEditor::*remove_function_type )( int ) const;
            remove_function_type remove_function_value( &::SireMol::SegEditor::remove );
            
            SegEditor_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMol::SegEditor::rename
        
            typedef ::SireMol::SegEditor & ( ::SireMol::SegEditor::*rename_function_type )( ::SireMol::SegName const & ) ;
            rename_function_type rename_function_value( &::SireMol::SegEditor::rename );
            
            SegEditor_exposer.def( 
                "rename"
                , rename_function_value
                , ( bp::arg("name") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::SegEditor::select
        
            typedef ::SireMol::AtomEditor ( ::SireMol::SegEditor::*select_function_type )( ::SireMol::AtomID const & ) const;
            select_function_type select_function_value( &::SireMol::SegEditor::select );
            
            SegEditor_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::SegEditor::select
        
            typedef ::SireMol::AtomEditor ( ::SireMol::SegEditor::*select_function_type )( int ) const;
            select_function_type select_function_value( &::SireMol::SegEditor::select );
            
            SegEditor_exposer.def( 
                "select"
                , select_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMol::SegEditor::toString
        
            typedef ::QString ( ::SireMol::SegEditor::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::SegEditor::toString );
            
            SegEditor_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::SegEditor::transfer
        
            typedef ::SireMol::SegStructureEditor ( ::SireMol::SegEditor::*transfer_function_type )( ::SireMol::AtomID const &,::SireMol::SegID const & ) const;
            transfer_function_type transfer_function_value( &::SireMol::SegEditor::transfer );
            
            SegEditor_exposer.def( 
                "transfer"
                , transfer_function_value
                , ( bp::arg("atomid"), bp::arg("segid") ) );
        
        }
        { //::SireMol::SegEditor::transfer
        
            typedef ::SireMol::SegStructureEditor ( ::SireMol::SegEditor::*transfer_function_type )( int,::SireMol::SegID const & ) const;
            transfer_function_type transfer_function_value( &::SireMol::SegEditor::transfer );
            
            SegEditor_exposer.def( 
                "transfer"
                , transfer_function_value
                , ( bp::arg("i"), bp::arg("segid") ) );
        
        }
        { //::SireMol::SegEditor::transferAll
        
            typedef ::SireMol::SegStructureEditor ( ::SireMol::SegEditor::*transferAll_function_type )( ::SireMol::SegID const & ) const;
            transferAll_function_type transferAll_function_value( &::SireMol::SegEditor::transferAll );
            
            SegEditor_exposer.def( 
                "transferAll"
                , transferAll_function_value
                , ( bp::arg("segid") ) );
        
        }
        { //::SireMol::SegEditor::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::SegEditor::typeName );
            
            SegEditor_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        SegEditor_exposer.staticmethod( "typeName" );
        SegEditor_exposer.def( "__copy__", &__copy__);
        SegEditor_exposer.def( "__deepcopy__", &__copy__);
        SegEditor_exposer.def( "clone", &__copy__);
        SegEditor_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::SegEditor >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        SegEditor_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::SegEditor >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        SegEditor_exposer.def( "__str__", &__str__< ::SireMol::SegEditor > );
        SegEditor_exposer.def( "__repr__", &__str__< ::SireMol::SegEditor > );
    }

}
