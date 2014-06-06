// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "CLJWorkspace.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "cljworkspace.h"

#include <QThreadStorage>

#include <QVarLengthArray>

#include "cljworkspace.h"

SireMM::CLJWorkspace __copy__(const SireMM::CLJWorkspace &other){ return SireMM::CLJWorkspace(other); }

#include "Qt/qdatastream.hpp"

const char* pvt_get_name(const SireMM::CLJWorkspace&){ return "SireMM::CLJWorkspace";}

#include "Helpers/len.hpp"

void register_CLJWorkspace_class(){

    { //::SireMM::CLJWorkspace
        typedef bp::class_< SireMM::CLJWorkspace > CLJWorkspace_exposer_t;
        CLJWorkspace_exposer_t CLJWorkspace_exposer = CLJWorkspace_exposer_t( "CLJWorkspace", bp::init< >() );
        bp::scope CLJWorkspace_scope( CLJWorkspace_exposer );
        CLJWorkspace_exposer.def( bp::init< SireMM::CLJWorkspace const & >(( bp::arg("other") )) );
        { //::SireMM::CLJWorkspace::at
        
            typedef ::SireMM::CLJDelta const & ( ::SireMM::CLJWorkspace::*at_function_type )( int ) const;
            at_function_type at_function_value( &::SireMM::CLJWorkspace::at );
            
            CLJWorkspace_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::CLJWorkspace::clear
        
            typedef void ( ::SireMM::CLJWorkspace::*clear_function_type )(  ) ;
            clear_function_type clear_function_value( &::SireMM::CLJWorkspace::clear );
            
            CLJWorkspace_exposer.def( 
                "clear"
                , clear_function_value );
        
        }
        { //::SireMM::CLJWorkspace::count
        
            typedef int ( ::SireMM::CLJWorkspace::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMM::CLJWorkspace::count );
            
            CLJWorkspace_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMM::CLJWorkspace::getitem
        
            typedef ::SireMM::CLJDelta ( ::SireMM::CLJWorkspace::*getitem_function_type )( int ) const;
            getitem_function_type getitem_function_value( &::SireMM::CLJWorkspace::getitem );
            
            CLJWorkspace_exposer.def( 
                "getitem"
                , getitem_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::CLJWorkspace::isEmpty
        
            typedef bool ( ::SireMM::CLJWorkspace::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMM::CLJWorkspace::isEmpty );
            
            CLJWorkspace_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMM::CLJWorkspace::isSingleID
        
            typedef bool ( ::SireMM::CLJWorkspace::*isSingleID_function_type )(  ) const;
            isSingleID_function_type isSingleID_function_value( &::SireMM::CLJWorkspace::isSingleID );
            
            CLJWorkspace_exposer.def( 
                "isSingleID"
                , isSingleID_function_value );
        
        }
        { //::SireMM::CLJWorkspace::merge
        
            typedef ::SireMM::CLJDelta ( ::SireMM::CLJWorkspace::*merge_function_type )(  ) const;
            merge_function_type merge_function_value( &::SireMM::CLJWorkspace::merge );
            
            CLJWorkspace_exposer.def( 
                "merge"
                , merge_function_value );
        
        }
        { //::SireMM::CLJWorkspace::nDeltas
        
            typedef int ( ::SireMM::CLJWorkspace::*nDeltas_function_type )(  ) const;
            nDeltas_function_type nDeltas_function_value( &::SireMM::CLJWorkspace::nDeltas );
            
            CLJWorkspace_exposer.def( 
                "nDeltas"
                , nDeltas_function_value );
        
        }
        CLJWorkspace_exposer.def( bp::self != bp::self );
        { //::SireMM::CLJWorkspace::operator=
        
            typedef ::SireMM::CLJWorkspace & ( ::SireMM::CLJWorkspace::*assign_function_type )( ::SireMM::CLJWorkspace const & ) ;
            assign_function_type assign_function_value( &::SireMM::CLJWorkspace::operator= );
            
            CLJWorkspace_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        CLJWorkspace_exposer.def( bp::self == bp::self );
        { //::SireMM::CLJWorkspace::operator[]
        
            typedef ::SireMM::CLJDelta const & ( ::SireMM::CLJWorkspace::*__getitem___function_type )( int ) const;
            __getitem___function_type __getitem___function_value( &::SireMM::CLJWorkspace::operator[] );
            
            CLJWorkspace_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::CLJWorkspace::push
        
            typedef void ( ::SireMM::CLJWorkspace::*push_function_type )( ::SireMM::CLJDelta const & ) ;
            push_function_type push_function_value( &::SireMM::CLJWorkspace::push );
            
            CLJWorkspace_exposer.def( 
                "push"
                , push_function_value
                , ( bp::arg("delta") ) );
        
        }
        { //::SireMM::CLJWorkspace::push
        
            typedef void ( ::SireMM::CLJWorkspace::*push_function_type )( ::quint32,::SireMM::CLJBoxes const &,::QVector< SireMM::CLJBoxIndex > const &,::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) ;
            push_function_type push_function_value( &::SireMM::CLJWorkspace::push );
            
            CLJWorkspace_exposer.def( 
                "push"
                , push_function_value
                , ( bp::arg("idnum"), bp::arg("boxes"), bp::arg("old_atoms"), bp::arg("new_atoms"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMM::CLJWorkspace::push
        
            typedef void ( ::SireMM::CLJWorkspace::*push_function_type )( ::quint32,::SireMM::CLJBoxes const &,::QVector< SireMM::CLJBoxIndex > const &,::SireMol::MoleculeView const &,::SireMM::CLJAtoms::ID_SOURCE,::SireBase::PropertyMap const & ) ;
            push_function_type push_function_value( &::SireMM::CLJWorkspace::push );
            
            CLJWorkspace_exposer.def( 
                "push"
                , push_function_value
                , ( bp::arg("idnum"), bp::arg("boxes"), bp::arg("old_atoms"), bp::arg("new_atoms"), bp::arg("source"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMM::CLJWorkspace::size
        
            typedef int ( ::SireMM::CLJWorkspace::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMM::CLJWorkspace::size );
            
            CLJWorkspace_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMM::CLJWorkspace::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::CLJWorkspace::typeName );
            
            CLJWorkspace_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::CLJWorkspace::what
        
            typedef char const * ( ::SireMM::CLJWorkspace::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::CLJWorkspace::what );
            
            CLJWorkspace_exposer.def( 
                "what"
                , what_function_value );
        
        }
        CLJWorkspace_exposer.staticmethod( "typeName" );
        CLJWorkspace_exposer.def( "__copy__", &__copy__);
        CLJWorkspace_exposer.def( "__deepcopy__", &__copy__);
        CLJWorkspace_exposer.def( "clone", &__copy__);
        CLJWorkspace_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::CLJWorkspace >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CLJWorkspace_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::CLJWorkspace >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CLJWorkspace_exposer.def( "__str__", &pvt_get_name);
        CLJWorkspace_exposer.def( "__repr__", &pvt_get_name);
        CLJWorkspace_exposer.def( "__len__", &__len_size< ::SireMM::CLJWorkspace > );
        CLJWorkspace_exposer.def( "__getitem__", &::SireMM::CLJWorkspace::getitem );
    }

}
