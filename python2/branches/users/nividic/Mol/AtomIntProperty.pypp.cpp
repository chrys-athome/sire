// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "AtomIntProperty.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireMaths/vector.h"

#include "atombeading.h"

#include "atomcharges.h"

#include "atomelements.h"

#include "atomenergies.h"

#include "atomforces.h"

#include "atommasses.h"

#include "atompolarisabilities.h"

#include "atomproperty.hpp"

#include "atomvelocities.h"

#include "atomproperty.hpp"

#include "SireMaths/vector.h"

SireMol::AtomProperty<long long> __copy__(const SireMol::AtomProperty<long long> &other){ return SireMol::AtomProperty<long long>(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_AtomIntProperty_class(){

    { //::SireMol::AtomProperty< long long >
        typedef bp::class_< SireMol::AtomProperty< long long >, bp::bases< SireMol::AtomProp, SireMol::MolViewProperty, SireBase::Property > > AtomIntProperty_exposer_t;
        AtomIntProperty_exposer_t AtomIntProperty_exposer = AtomIntProperty_exposer_t( "AtomIntProperty", bp::init< >() );
        bp::scope AtomIntProperty_scope( AtomIntProperty_exposer );
        AtomIntProperty_exposer.def( bp::init< SireMol::MoleculeInfoData const & >(( bp::arg("molinfo") )) );
        AtomIntProperty_exposer.def( bp::init< SireMol::MoleculeInfoData const &, long long int const & >(( bp::arg("molinfo"), bp::arg("default_value") )) );
        AtomIntProperty_exposer.def( bp::init< long long int const & >(( bp::arg("value") )) );
        AtomIntProperty_exposer.def( bp::init< SireBase::PackedArray2D< long long > const & >(( bp::arg("values") )) );
        AtomIntProperty_exposer.def( bp::init< SireMol::AtomProperty< long long > const & >(( bp::arg("other") )) );
        { //::SireMol::AtomProperty< long long >::array
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::SireBase::PackedArray2D< long long > const & ( ::SireMol::AtomProperty< long long >::*array_function_type )(  ) const;
            array_function_type array_function_value( &::SireMol::AtomProperty< long long >::array );
            
            AtomIntProperty_exposer.def( 
                "array"
                , array_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< long long >::assertCanConvert
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< long long >::*assertCanConvert_function_type )( ::QVariant const & ) const;
            assertCanConvert_function_type assertCanConvert_function_value( &::SireMol::AtomProperty< long long >::assertCanConvert );
            
            AtomIntProperty_exposer.def( 
                "assertCanConvert"
                , assertCanConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::AtomProperty< long long >::assignFrom
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< long long >::*assignFrom_function_type )( ::SireMol::AtomProperty< QVariant > const & ) ;
            assignFrom_function_type assignFrom_function_value( &::SireMol::AtomProperty< long long >::assignFrom );
            
            AtomIntProperty_exposer.def( 
                "assignFrom"
                , assignFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::AtomProperty< long long >::at
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< long long int > const & ( ::SireMol::AtomProperty< long long >::*at_function_type )( ::SireMol::CGIdx ) const;
            at_function_type at_function_value( &::SireMol::AtomProperty< long long >::at );
            
            AtomIntProperty_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< long long >::at
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef long long int const & ( ::SireMol::AtomProperty< long long >::*at_function_type )( ::SireMol::CGAtomIdx const & ) const;
            at_function_type at_function_value( &::SireMol::AtomProperty< long long >::at );
            
            AtomIntProperty_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< long long >::canConvert
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< long long >::*canConvert_function_type )( ::QVariant const & ) const;
            canConvert_function_type canConvert_function_value( &::SireMol::AtomProperty< long long >::canConvert );
            
            AtomIntProperty_exposer.def( 
                "canConvert"
                , canConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::AtomProperty< long long >::copyFrom
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< long long >::*copyFrom_function_type )( ::QVector< long long > const & ) ;
            copyFrom_function_type copyFrom_function_value( &::SireMol::AtomProperty< long long >::copyFrom );
            
            AtomIntProperty_exposer.def( 
                "copyFrom"
                , copyFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::AtomProperty< long long >::copyFrom
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< long long >::*copyFrom_function_type )( ::QVector< long long > const &,::SireMol::AtomSelection const & ) ;
            copyFrom_function_type copyFrom_function_value( &::SireMol::AtomProperty< long long >::copyFrom );
            
            AtomIntProperty_exposer.def( 
                "copyFrom"
                , copyFrom_function_value
                , ( bp::arg("values"), bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< long long >::count
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< long long >::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMol::AtomProperty< long long >::count );
            
            AtomIntProperty_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMol::AtomProperty< long long >::fromVariant
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::SireMol::AtomProperty< long long > ( *fromVariant_function_type )( ::SireMol::AtomProperty< QVariant > const & );
            fromVariant_function_type fromVariant_function_value( &::SireMol::AtomProperty< long long >::fromVariant );
            
            AtomIntProperty_exposer.def( 
                "fromVariant"
                , fromVariant_function_value
                , ( bp::arg("variant") ) );
        
        }
        { //::SireMol::AtomProperty< long long >::get
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< long long int > const & ( ::SireMol::AtomProperty< long long >::*get_function_type )( ::SireMol::CGIdx ) const;
            get_function_type get_function_value( &::SireMol::AtomProperty< long long >::get );
            
            AtomIntProperty_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< long long >::get
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef long long int const & ( ::SireMol::AtomProperty< long long >::*get_function_type )( ::SireMol::CGAtomIdx const & ) const;
            get_function_type get_function_value( &::SireMol::AtomProperty< long long >::get );
            
            AtomIntProperty_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< long long >::isCompatibleWith
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< long long >::*isCompatibleWith_function_type )( ::SireMol::MoleculeInfoData const & ) const;
            isCompatibleWith_function_type isCompatibleWith_function_value( &::SireMol::AtomProperty< long long >::isCompatibleWith );
            
            AtomIntProperty_exposer.def( 
                "isCompatibleWith"
                , isCompatibleWith_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        { //::SireMol::AtomProperty< long long >::isEmpty
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< long long >::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::AtomProperty< long long >::isEmpty );
            
            AtomIntProperty_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::AtomProperty< long long >::matchToSelection
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::SireMol::AtomProperty< long long > ( ::SireMol::AtomProperty< long long >::*matchToSelection_function_type )( ::SireMol::AtomSelection const & ) const;
            matchToSelection_function_type matchToSelection_function_value( &::SireMol::AtomProperty< long long >::matchToSelection );
            
            AtomIntProperty_exposer.def( 
                "matchToSelection"
                , matchToSelection_function_value
                , ( bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< long long >::nAtoms
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< long long >::*nAtoms_function_type )(  ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::AtomProperty< long long >::nAtoms );
            
            AtomIntProperty_exposer.def( 
                "nAtoms"
                , nAtoms_function_value );
        
        }
        { //::SireMol::AtomProperty< long long >::nAtoms
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< long long >::*nAtoms_function_type )( ::SireMol::CGIdx ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::AtomProperty< long long >::nAtoms );
            
            AtomIntProperty_exposer.def( 
                "nAtoms"
                , nAtoms_function_value
                , ( bp::arg("cgidx") ) );
        
        }
        { //::SireMol::AtomProperty< long long >::nCutGroups
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< long long >::*nCutGroups_function_type )(  ) const;
            nCutGroups_function_type nCutGroups_function_value( &::SireMol::AtomProperty< long long >::nCutGroups );
            
            AtomIntProperty_exposer.def( 
                "nCutGroups"
                , nCutGroups_function_value );
        
        }
        AtomIntProperty_exposer.def( bp::self != bp::self );
        { //::SireMol::AtomProperty< long long >::operator=
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::SireMol::AtomProperty< long long > & ( ::SireMol::AtomProperty< long long >::*assign_function_type )( ::SireMol::AtomProperty< long long > const & ) ;
            assign_function_type assign_function_value( &::SireMol::AtomProperty< long long >::operator= );
            
            AtomIntProperty_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        AtomIntProperty_exposer.def( bp::self == bp::self );
        { //::SireMol::AtomProperty< long long >::operator[]
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< long long int > const & ( ::SireMol::AtomProperty< long long >::*__getitem___function_type )( ::SireMol::CGIdx ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::AtomProperty< long long >::operator[] );
            
            AtomIntProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< long long >::operator[]
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef long long int const & ( ::SireMol::AtomProperty< long long >::*__getitem___function_type )( ::SireMol::CGAtomIdx const & ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::AtomProperty< long long >::operator[] );
            
            AtomIntProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< long long >::set
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::SireMol::AtomProperty< long long > & ( ::SireMol::AtomProperty< long long >::*set_function_type )( ::SireMol::CGAtomIdx const &,long long int const & ) ;
            set_function_type set_function_value( &::SireMol::AtomProperty< long long >::set );
            
            AtomIntProperty_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgatomidx"), bp::arg("value") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::AtomProperty< long long >::set
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::SireMol::AtomProperty< long long > & ( ::SireMol::AtomProperty< long long >::*set_function_type )( ::SireMol::CGIdx,::QVector< long long > const & ) ;
            set_function_type set_function_value( &::SireMol::AtomProperty< long long >::set );
            
            AtomIntProperty_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgidx"), bp::arg("values") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::AtomProperty< long long >::size
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< long long >::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMol::AtomProperty< long long >::size );
            
            AtomIntProperty_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMol::AtomProperty< long long >::toString
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::QString ( ::SireMol::AtomProperty< long long >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::AtomProperty< long long >::toString );
            
            AtomIntProperty_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::AtomProperty< long long >::toVariant
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::SireMol::AtomProperty< QVariant > ( ::SireMol::AtomProperty< long long >::*toVariant_function_type )(  ) const;
            toVariant_function_type toVariant_function_value( &::SireMol::AtomProperty< long long >::toVariant );
            
            AtomIntProperty_exposer.def( 
                "toVariant"
                , toVariant_function_value );
        
        }
        { //::SireMol::AtomProperty< long long >::toVector
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::QVector< long long > ( ::SireMol::AtomProperty< long long >::*toVector_function_type )(  ) const;
            toVector_function_type toVector_function_value( &::SireMol::AtomProperty< long long >::toVector );
            
            AtomIntProperty_exposer.def( 
                "toVector"
                , toVector_function_value );
        
        }
        { //::SireMol::AtomProperty< long long >::toVector
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef ::QVector< long long > ( ::SireMol::AtomProperty< long long >::*toVector_function_type )( ::SireMol::AtomSelection const & ) const;
            toVector_function_type toVector_function_value( &::SireMol::AtomProperty< long long >::toVector );
            
            AtomIntProperty_exposer.def( 
                "toVector"
                , toVector_function_value
                , ( bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< long long >::typeName
        
            typedef SireMol::AtomProperty< long long > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::AtomProperty< long long >::typeName );
            
            AtomIntProperty_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        AtomIntProperty_exposer.staticmethod( "fromVariant" );
        AtomIntProperty_exposer.staticmethod( "typeName" );
        AtomIntProperty_exposer.def( "__copy__", &__copy__);
        AtomIntProperty_exposer.def( "__deepcopy__", &__copy__);
        AtomIntProperty_exposer.def( "clone", &__copy__);
        AtomIntProperty_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::AtomProperty<long long> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AtomIntProperty_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::AtomProperty<long long> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AtomIntProperty_exposer.def( "__str__", &__str__< ::SireMol::AtomProperty<long long> > );
        AtomIntProperty_exposer.def( "__repr__", &__str__< ::SireMol::AtomProperty<long long> > );
    }

}
