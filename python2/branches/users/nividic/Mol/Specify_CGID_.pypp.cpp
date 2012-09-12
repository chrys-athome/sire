// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Specify_CGID_.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include "atom.h"

#include "cgid.h"

#include "cgidentifier.h"

#include "chain.h"

#include "cutgroup.h"

#include "editor.hpp"

#include "groupatomids.h"

#include "groupgroupids.h"

#include "moleculegroup.h"

#include "moleculegroups.h"

#include "molecules.h"

#include "molinfo.h"

#include "mover.hpp"

#include "partialmolecule.h"

#include "residue.h"

#include "segment.h"

#include "selector.hpp"

#include "tostring.h"

#include "cgid.h"

SireID::Specify<SireMol::CGID> __copy__(const SireID::Specify<SireMol::CGID> &other){ return SireID::Specify<SireMol::CGID>(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Specify_CGID__class(){

    { //::SireID::Specify< SireMol::CGID >
        typedef bp::class_< SireID::Specify< SireMol::CGID >, bp::bases< SireMol::CGID, SireID::ID > > Specify_CGID__exposer_t;
        Specify_CGID__exposer_t Specify_CGID__exposer = Specify_CGID__exposer_t( "Specify_CGID_", bp::init< >() );
        bp::scope Specify_CGID__scope( Specify_CGID__exposer );
        Specify_CGID__exposer.def( bp::init< SireMol::CGID const &, qint32 >(( bp::arg("id"), bp::arg("i") )) );
        Specify_CGID__exposer.def( bp::init< SireMol::CGID const &, qint32, qint32 >(( bp::arg("id"), bp::arg("i"), bp::arg("j") )) );
        Specify_CGID__exposer.def( bp::init< SireID::Specify< SireMol::CGID > const & >(( bp::arg("other") )) );
        { //::SireID::Specify< SireMol::CGID >::hash
        
            typedef SireID::Specify< SireMol::CGID > exported_class_t;
            typedef ::uint ( ::SireID::Specify< SireMol::CGID >::*hash_function_type )(  ) const;
            hash_function_type hash_function_value( &::SireID::Specify< SireMol::CGID >::hash );
            
            Specify_CGID__exposer.def( 
                "hash"
                , hash_function_value );
        
        }
        { //::SireID::Specify< SireMol::CGID >::isNull
        
            typedef SireID::Specify< SireMol::CGID > exported_class_t;
            typedef bool ( ::SireID::Specify< SireMol::CGID >::*isNull_function_type )(  ) const;
            isNull_function_type isNull_function_value( &::SireID::Specify< SireMol::CGID >::isNull );
            
            Specify_CGID__exposer.def( 
                "isNull"
                , isNull_function_value );
        
        }
        { //::SireID::Specify< SireMol::CGID >::map
        
            typedef SireID::Specify< SireMol::CGID > exported_class_t;
            typedef ::QList< SireMol::CGIdx > ( ::SireID::Specify< SireMol::CGID >::*map_function_type )( ::SireMol::MolInfo const & ) const;
            map_function_type map_function_value( &::SireID::Specify< SireMol::CGID >::map );
            
            Specify_CGID__exposer.def( 
                "map"
                , map_function_value
                , ( bp::arg("obj") ) );
        
        }
        Specify_CGID__exposer.def( bp::self != bp::self );
        Specify_CGID__exposer.def( bp::self != bp::other< SireID::ID >() );
        { //::SireID::Specify< SireMol::CGID >::operator()
        
            typedef SireID::Specify< SireMol::CGID > exported_class_t;
            typedef ::SireID::Specify< SireMol::CGID > ( ::SireID::Specify< SireMol::CGID >::*__call___function_type )( int ) const;
            __call___function_type __call___function_value( &::SireID::Specify< SireMol::CGID >::operator() );
            
            Specify_CGID__exposer.def( 
                "__call__"
                , __call___function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireID::Specify< SireMol::CGID >::operator()
        
            typedef SireID::Specify< SireMol::CGID > exported_class_t;
            typedef ::SireID::Specify< SireMol::CGID > ( ::SireID::Specify< SireMol::CGID >::*__call___function_type )( int,int ) const;
            __call___function_type __call___function_value( &::SireID::Specify< SireMol::CGID >::operator() );
            
            Specify_CGID__exposer.def( 
                "__call__"
                , __call___function_value
                , ( bp::arg("i"), bp::arg("j") ) );
        
        }
        { //::SireID::Specify< SireMol::CGID >::operator=
        
            typedef SireID::Specify< SireMol::CGID > exported_class_t;
            typedef ::SireID::Specify< SireMol::CGID > & ( ::SireID::Specify< SireMol::CGID >::*assign_function_type )( ::SireID::Specify< SireMol::CGID > const & ) ;
            assign_function_type assign_function_value( &::SireID::Specify< SireMol::CGID >::operator= );
            
            Specify_CGID__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Specify_CGID__exposer.def( bp::self == bp::self );
        Specify_CGID__exposer.def( bp::self == bp::other< SireID::ID >() );
        { //::SireID::Specify< SireMol::CGID >::operator[]
        
            typedef SireID::Specify< SireMol::CGID > exported_class_t;
            typedef ::SireID::Specify< SireMol::CGID > ( ::SireID::Specify< SireMol::CGID >::*__getitem___function_type )( int ) const;
            __getitem___function_type __getitem___function_value( &::SireID::Specify< SireMol::CGID >::operator[] );
            
            Specify_CGID__exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireID::Specify< SireMol::CGID >::toString
        
            typedef SireID::Specify< SireMol::CGID > exported_class_t;
            typedef ::QString ( ::SireID::Specify< SireMol::CGID >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireID::Specify< SireMol::CGID >::toString );
            
            Specify_CGID__exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireID::Specify< SireMol::CGID >::typeName
        
            typedef SireID::Specify< SireMol::CGID > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireID::Specify< SireMol::CGID >::typeName );
            
            Specify_CGID__exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireID::Specify< SireMol::CGID >::what
        
            typedef SireID::Specify< SireMol::CGID > exported_class_t;
            typedef char const * ( ::SireID::Specify< SireMol::CGID >::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireID::Specify< SireMol::CGID >::what );
            
            Specify_CGID__exposer.def( 
                "what"
                , what_function_value );
        
        }
        Specify_CGID__exposer.staticmethod( "typeName" );
        Specify_CGID__exposer.def( "__copy__", &__copy__);
        Specify_CGID__exposer.def( "__deepcopy__", &__copy__);
        Specify_CGID__exposer.def( "clone", &__copy__);
        Specify_CGID__exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireID::Specify<SireMol::CGID> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Specify_CGID__exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireID::Specify<SireMol::CGID> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Specify_CGID__exposer.def( "__str__", &__str__< ::SireID::Specify<SireMol::CGID> > );
        Specify_CGID__exposer.def( "__repr__", &__str__< ::SireID::Specify<SireMol::CGID> > );
    }

}
