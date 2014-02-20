// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "CLJBox.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireMM/cljboxes.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireVol/aabox.h"

#include "SireVol/periodicbox.h"

#include "SireVol/space.h"

#include "cljboxes.h"

#include "tostring.h"

#include <QDebug>

#include <QElapsedTimer>

#include "cljboxes.h"

SireMM::CLJBox __copy__(const SireMM::CLJBox &other){ return SireMM::CLJBox(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_CLJBox_class(){

    { //::SireMM::CLJBox
        typedef bp::class_< SireMM::CLJBox > CLJBox_exposer_t;
        CLJBox_exposer_t CLJBox_exposer = CLJBox_exposer_t( "CLJBox", bp::init< >() );
        bp::scope CLJBox_scope( CLJBox_exposer );
        CLJBox_exposer.def( bp::init< SireMM::CLJAtoms const & >(( bp::arg("atoms") )) );
        CLJBox_exposer.def( bp::init< SireMM::CLJBox const & >(( bp::arg("other") )) );
        { //::SireMM::CLJBox::add
        
            typedef ::QVector< int > ( ::SireMM::CLJBox::*add_function_type )( ::SireMM::CLJAtoms const & ) ;
            add_function_type add_function_value( &::SireMM::CLJBox::add );
            
            CLJBox_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("atoms") ) );
        
        }
        { //::SireMM::CLJBox::at
        
            typedef ::SireMM::CLJAtom ( ::SireMM::CLJBox::*at_function_type )( int ) const;
            at_function_type at_function_value( &::SireMM::CLJBox::at );
            
            CLJBox_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::CLJBox::atoms
        
            typedef ::SireMM::CLJAtoms const & ( ::SireMM::CLJBox::*atoms_function_type )(  ) const;
            atoms_function_type atoms_function_value( &::SireMM::CLJBox::atoms );
            
            CLJBox_exposer.def( 
                "atoms"
                , atoms_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::CLJBox::count
        
            typedef int ( ::SireMM::CLJBox::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMM::CLJBox::count );
            
            CLJBox_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMM::CLJBox::getitem
        
            typedef ::SireMM::CLJAtom ( ::SireMM::CLJBox::*getitem_function_type )( int ) const;
            getitem_function_type getitem_function_value( &::SireMM::CLJBox::getitem );
            
            CLJBox_exposer.def( 
                "getitem"
                , getitem_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::CLJBox::isEmpty
        
            typedef bool ( ::SireMM::CLJBox::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMM::CLJBox::isEmpty );
            
            CLJBox_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMM::CLJBox::nAtoms
        
            typedef int ( ::SireMM::CLJBox::*nAtoms_function_type )(  ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMM::CLJBox::nAtoms );
            
            CLJBox_exposer.def( 
                "nAtoms"
                , nAtoms_function_value );
        
        }
        CLJBox_exposer.def( bp::self != bp::self );
        CLJBox_exposer.def( bp::self + bp::self );
        { //::SireMM::CLJBox::operator=
        
            typedef ::SireMM::CLJBox & ( ::SireMM::CLJBox::*assign_function_type )( ::SireMM::CLJBox const & ) ;
            assign_function_type assign_function_value( &::SireMM::CLJBox::operator= );
            
            CLJBox_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        CLJBox_exposer.def( bp::self == bp::self );
        { //::SireMM::CLJBox::operator[]
        
            typedef ::SireMM::CLJAtom ( ::SireMM::CLJBox::*__getitem___function_type )( int ) const;
            __getitem___function_type __getitem___function_value( &::SireMM::CLJBox::operator[] );
            
            CLJBox_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireMM::CLJBox::remove
        
            typedef void ( ::SireMM::CLJBox::*remove_function_type )( int ) ;
            remove_function_type remove_function_value( &::SireMM::CLJBox::remove );
            
            CLJBox_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("atom") ) );
        
        }
        { //::SireMM::CLJBox::remove
        
            typedef void ( ::SireMM::CLJBox::*remove_function_type )( ::QList< int > const & ) ;
            remove_function_type remove_function_value( &::SireMM::CLJBox::remove );
            
            CLJBox_exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("atoms") ) );
        
        }
        { //::SireMM::CLJBox::size
        
            typedef int ( ::SireMM::CLJBox::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMM::CLJBox::size );
            
            CLJBox_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMM::CLJBox::squeeze
        
            typedef ::SireMM::CLJBox ( ::SireMM::CLJBox::*squeeze_function_type )(  ) const;
            squeeze_function_type squeeze_function_value( &::SireMM::CLJBox::squeeze );
            
            CLJBox_exposer.def( 
                "squeeze"
                , squeeze_function_value );
        
        }
        { //::SireMM::CLJBox::toString
        
            typedef ::QString ( ::SireMM::CLJBox::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMM::CLJBox::toString );
            
            CLJBox_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMM::CLJBox::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::CLJBox::typeName );
            
            CLJBox_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::CLJBox::what
        
            typedef char const * ( ::SireMM::CLJBox::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::CLJBox::what );
            
            CLJBox_exposer.def( 
                "what"
                , what_function_value );
        
        }
        CLJBox_exposer.staticmethod( "typeName" );
        CLJBox_exposer.def( "__copy__", &__copy__);
        CLJBox_exposer.def( "__deepcopy__", &__copy__);
        CLJBox_exposer.def( "clone", &__copy__);
        CLJBox_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::CLJBox >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CLJBox_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::CLJBox >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CLJBox_exposer.def( "__str__", &__str__< ::SireMM::CLJBox > );
        CLJBox_exposer.def( "__repr__", &__str__< ::SireMM::CLJBox > );
        CLJBox_exposer.def( "__len__", &__len_size< ::SireMM::CLJBox > );
        CLJBox_exposer.def( "__getitem__", &::SireMM::CLJBox::getitem );
    }

}
