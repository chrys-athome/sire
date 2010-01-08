// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Specify_AtomID_.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "atom.h"

#include "atomid.h"

#include "atomidentifier.h"

#include "chain.h"

#include "cutgroup.h"

#include "editor.hpp"

#include "groupatomids.h"

#include "molatomid.h"

#include "moleculegroup.h"

#include "moleculegroups.h"

#include "molecules.h"

#include "molinfo.h"

#include "mover.hpp"

#include "residue.h"

#include "segment.h"

#include "selector.hpp"

#include "tostring.h"

#include <QDebug>

#include "atomid.h"

SireID::Specify<SireMol::AtomID> __copy__(const SireID::Specify<SireMol::AtomID> &other){ return SireID::Specify<SireMol::AtomID>(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Specify_AtomID__class(){

    { //::SireID::Specify< SireMol::AtomID >
        typedef bp::class_< SireID::Specify< SireMol::AtomID >, bp::bases< SireMol::AtomID, SireID::ID > > Specify_AtomID__exposer_t;
        Specify_AtomID__exposer_t Specify_AtomID__exposer = Specify_AtomID__exposer_t( "Specify_AtomID_" );
        bp::scope Specify_AtomID__scope( Specify_AtomID__exposer );
        Specify_AtomID__exposer.def( bp::init< >() );
        Specify_AtomID__exposer.def( bp::init< SireMol::AtomID const &, qint32 >(( bp::arg("id"), bp::arg("i") )) );
        Specify_AtomID__exposer.def( bp::init< SireMol::AtomID const &, qint32, qint32 >(( bp::arg("id"), bp::arg("i"), bp::arg("j") )) );
        Specify_AtomID__exposer.def( bp::init< SireID::Specify< SireMol::AtomID > const & >(( bp::arg("other") )) );
        { //::SireID::Specify< SireMol::AtomID >::hash
        
            typedef SireID::Specify< SireMol::AtomID > exported_class_t;
            typedef ::uint ( ::SireID::Specify< SireMol::AtomID >::*hash_function_type )(  ) const;
            hash_function_type hash_function_value( &::SireID::Specify< SireMol::AtomID >::hash );
            
            Specify_AtomID__exposer.def( 
                "hash"
                , hash_function_value );
        
        }
        { //::SireID::Specify< SireMol::AtomID >::isNull
        
            typedef SireID::Specify< SireMol::AtomID > exported_class_t;
            typedef bool ( ::SireID::Specify< SireMol::AtomID >::*isNull_function_type )(  ) const;
            isNull_function_type isNull_function_value( &::SireID::Specify< SireMol::AtomID >::isNull );
            
            Specify_AtomID__exposer.def( 
                "isNull"
                , isNull_function_value );
        
        }
        { //::SireID::Specify< SireMol::AtomID >::map
        
            typedef SireID::Specify< SireMol::AtomID > exported_class_t;
            typedef ::QList< SireMol::AtomIdx > ( ::SireID::Specify< SireMol::AtomID >::*map_function_type )( ::SireMol::MolInfo const & ) const;
            map_function_type map_function_value( &::SireID::Specify< SireMol::AtomID >::map );
            
            Specify_AtomID__exposer.def( 
                "map"
                , map_function_value
                , ( bp::arg("obj") ) );
        
        }
        Specify_AtomID__exposer.def( bp::self != bp::self );
        Specify_AtomID__exposer.def( bp::self != bp::other< SireID::ID >() );
        { //::SireID::Specify< SireMol::AtomID >::operator()
        
            typedef SireID::Specify< SireMol::AtomID > exported_class_t;
            typedef ::SireID::Specify< SireMol::AtomID > ( ::SireID::Specify< SireMol::AtomID >::*__call___function_type )( int ) const;
            __call___function_type __call___function_value( &::SireID::Specify< SireMol::AtomID >::operator() );
            
            Specify_AtomID__exposer.def( 
                "__call__"
                , __call___function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireID::Specify< SireMol::AtomID >::operator()
        
            typedef SireID::Specify< SireMol::AtomID > exported_class_t;
            typedef ::SireID::Specify< SireMol::AtomID > ( ::SireID::Specify< SireMol::AtomID >::*__call___function_type )( int,int ) const;
            __call___function_type __call___function_value( &::SireID::Specify< SireMol::AtomID >::operator() );
            
            Specify_AtomID__exposer.def( 
                "__call__"
                , __call___function_value
                , ( bp::arg("i"), bp::arg("j") ) );
        
        }
        { //::SireID::Specify< SireMol::AtomID >::operator=
        
            typedef SireID::Specify< SireMol::AtomID > exported_class_t;
            typedef ::SireID::Specify< SireMol::AtomID > & ( ::SireID::Specify< SireMol::AtomID >::*assign_function_type )( ::SireID::Specify< SireMol::AtomID > const & ) ;
            assign_function_type assign_function_value( &::SireID::Specify< SireMol::AtomID >::operator= );
            
            Specify_AtomID__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Specify_AtomID__exposer.def( bp::self == bp::self );
        Specify_AtomID__exposer.def( bp::self == bp::other< SireID::ID >() );
        { //::SireID::Specify< SireMol::AtomID >::operator[]
        
            typedef SireID::Specify< SireMol::AtomID > exported_class_t;
            typedef ::SireID::Specify< SireMol::AtomID > ( ::SireID::Specify< SireMol::AtomID >::*__getitem___function_type )( int ) const;
            __getitem___function_type __getitem___function_value( &::SireID::Specify< SireMol::AtomID >::operator[] );
            
            Specify_AtomID__exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireID::Specify< SireMol::AtomID >::toString
        
            typedef SireID::Specify< SireMol::AtomID > exported_class_t;
            typedef ::QString ( ::SireID::Specify< SireMol::AtomID >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireID::Specify< SireMol::AtomID >::toString );
            
            Specify_AtomID__exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireID::Specify< SireMol::AtomID >::typeName
        
            typedef SireID::Specify< SireMol::AtomID > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireID::Specify< SireMol::AtomID >::typeName );
            
            Specify_AtomID__exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireID::Specify< SireMol::AtomID >::what
        
            typedef SireID::Specify< SireMol::AtomID > exported_class_t;
            typedef char const * ( ::SireID::Specify< SireMol::AtomID >::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireID::Specify< SireMol::AtomID >::what );
            
            Specify_AtomID__exposer.def( 
                "what"
                , what_function_value );
        
        }
        Specify_AtomID__exposer.staticmethod( "typeName" );
        Specify_AtomID__exposer.def( "__copy__", &__copy__);
        Specify_AtomID__exposer.def( "__deepcopy__", &__copy__);
        Specify_AtomID__exposer.def( "clone", &__copy__);
        Specify_AtomID__exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireID::Specify<SireMol::AtomID> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Specify_AtomID__exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireID::Specify<SireMol::AtomID> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Specify_AtomID__exposer.def( "__str__", &__str__< ::SireID::Specify<SireMol::AtomID> > );
        Specify_AtomID__exposer.def( "__repr__", &__str__< ::SireID::Specify<SireMol::AtomID> > );
    }

}
