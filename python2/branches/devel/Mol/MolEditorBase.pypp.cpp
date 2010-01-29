// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "MolEditorBase.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "atom.h"

#include "atomeditor.h"

#include "cgeditor.h"

#include "chain.h"

#include "chaineditor.h"

#include "cutgroup.h"

#include "moleditor.h"

#include "mover.hpp"

#include "reseditor.h"

#include "residue.h"

#include "segeditor.h"

#include "segment.h"

#include "selector.hpp"

#include "moleditor.h"

SireMol::MolEditorBase& set_Metadata_function1(
                              SireMol::MolEditorBase &molview,
                              const QString &metakey, const SireBase::Property &p)
                              { return molview.setMetadata<SireBase::Property>(metakey, p); }

SireMol::MolEditorBase& set_Metadata_function2(
                              SireMol::MolEditorBase &molview,
                              const QString &key, const QString &metakey, 
                              const SireBase::Property &p)
                              { return molview.setMetadata<SireBase::Property>(key, metakey, p); }

#include "Helpers/str.hpp"

void register_MolEditorBase_class(){

    { //::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >
        typedef bp::class_< SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >, bp::bases< SireMol::Molecule, SireMol::MoleculeView, SireBase::Property >, boost::noncopyable > MolEditorBase_exposer_t;
        MolEditorBase_exposer_t MolEditorBase_exposer = MolEditorBase_exposer_t( "MolEditorBase", bp::no_init );
        bp::scope MolEditorBase_scope( MolEditorBase_exposer );
        { //::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::operator=
        
            typedef SireMol::Editor< SireMol::MolEditor, SireMol::Molecule > exported_class_t;
            typedef ::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule > & ( ::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::*assign_function_type )( ::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule > const & ) ;
            assign_function_type assign_function_value( &::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::operator= );
            
            MolEditorBase_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::operator=
        
            typedef SireMol::Editor< SireMol::MolEditor, SireMol::Molecule > exported_class_t;
            typedef ::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule > & ( ::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::*assign_function_type )( ::SireMol::Molecule const & ) ;
            assign_function_type assign_function_value( &::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::operator= );
            
            MolEditorBase_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::removeMetadata
        
            typedef SireMol::Editor< SireMol::MolEditor, SireMol::Molecule > exported_class_t;
            typedef ::SireMol::MolEditor & ( ::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::*removeMetadata_function_type )( ::SireBase::PropertyName const & ) ;
            removeMetadata_function_type removeMetadata_function_value( &::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::removeMetadata );
            
            MolEditorBase_exposer.def( 
                "removeMetadata"
                , removeMetadata_function_value
                , ( bp::arg("metakey") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::removeMetadata
        
            typedef SireMol::Editor< SireMol::MolEditor, SireMol::Molecule > exported_class_t;
            typedef ::SireMol::MolEditor & ( ::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::*removeMetadata_function_type )( ::SireBase::PropertyName const &,::SireBase::PropertyName const & ) ;
            removeMetadata_function_type removeMetadata_function_value( &::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::removeMetadata );
            
            MolEditorBase_exposer.def( 
                "removeMetadata"
                , removeMetadata_function_value
                , ( bp::arg("key"), bp::arg("metakey") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::removeProperty
        
            typedef SireMol::Editor< SireMol::MolEditor, SireMol::Molecule > exported_class_t;
            typedef ::SireMol::MolEditor & ( ::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::*removeProperty_function_type )( ::SireBase::PropertyName const & ) ;
            removeProperty_function_type removeProperty_function_value( &::SireMol::Editor< SireMol::MolEditor, SireMol::Molecule >::removeProperty );
            
            MolEditorBase_exposer.def( 
                "removeProperty"
                , removeProperty_function_value
                , ( bp::arg("key") )
                , bp::return_self< >() );
        
        }
        MolEditorBase_exposer.def( "setProperty",
                                       &SireMol::MolEditorBase::setProperty<SireBase::Property>, bp::return_self< >() );
        MolEditorBase_exposer.def( "setMetadata", &set_Metadata_function1, bp::return_self< >());
        MolEditorBase_exposer.def( "setMetadata", &set_Metadata_function2, bp::return_self< >());
        MolEditorBase_exposer.def( "__str__", &__str__< ::SireMol::Editor<SireMol::MolEditor, SireMol::Molecule> > );
        MolEditorBase_exposer.def( "__repr__", &__str__< ::SireMol::Editor<SireMol::MolEditor, SireMol::Molecule> > );
    }

}