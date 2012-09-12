// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "AtomVariantProperty.pypp.hpp"

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

SireMol::AtomProperty<QVariant> __copy__(const SireMol::AtomProperty<QVariant> &other){ return SireMol::AtomProperty<QVariant>(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_AtomVariantProperty_class(){

    { //::SireMol::AtomProperty< QVariant >
        typedef bp::class_< SireMol::AtomProperty< QVariant >, bp::bases< SireMol::AtomProp, SireMol::MolViewProperty, SireBase::Property > > AtomVariantProperty_exposer_t;
        AtomVariantProperty_exposer_t AtomVariantProperty_exposer = AtomVariantProperty_exposer_t( "AtomVariantProperty", bp::init< >() );
        bp::scope AtomVariantProperty_scope( AtomVariantProperty_exposer );
        AtomVariantProperty_exposer.def( bp::init< SireMol::MoleculeInfoData const & >(( bp::arg("molinfo") )) );
        AtomVariantProperty_exposer.def( bp::init< SireMol::MoleculeInfoData const &, QVariant const & >(( bp::arg("molinfo"), bp::arg("default_value") )) );
        AtomVariantProperty_exposer.def( bp::init< QVariant const & >(( bp::arg("value") )) );
        AtomVariantProperty_exposer.def( bp::init< SireBase::PackedArray2D< QVariant > const & >(( bp::arg("values") )) );
        AtomVariantProperty_exposer.def( bp::init< SireMol::AtomProperty< QVariant > const & >(( bp::arg("other") )) );
        { //::SireMol::AtomProperty< QVariant >::array
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::SireBase::PackedArray2D< QVariant > const & ( ::SireMol::AtomProperty< QVariant >::*array_function_type )(  ) const;
            array_function_type array_function_value( &::SireMol::AtomProperty< QVariant >::array );
            
            AtomVariantProperty_exposer.def( 
                "array"
                , array_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QVariant >::assertCanConvert
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< QVariant >::*assertCanConvert_function_type )( ::QVariant const & ) const;
            assertCanConvert_function_type assertCanConvert_function_value( &::SireMol::AtomProperty< QVariant >::assertCanConvert );
            
            AtomVariantProperty_exposer.def( 
                "assertCanConvert"
                , assertCanConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::AtomProperty< QVariant >::assignFrom
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< QVariant >::*assignFrom_function_type )( ::SireMol::AtomProperty< QVariant > const & ) ;
            assignFrom_function_type assignFrom_function_value( &::SireMol::AtomProperty< QVariant >::assignFrom );
            
            AtomVariantProperty_exposer.def( 
                "assignFrom"
                , assignFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::AtomProperty< QVariant >::at
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< QVariant > const & ( ::SireMol::AtomProperty< QVariant >::*at_function_type )( ::SireMol::CGIdx ) const;
            at_function_type at_function_value( &::SireMol::AtomProperty< QVariant >::at );
            
            AtomVariantProperty_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QVariant >::at
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::QVariant const & ( ::SireMol::AtomProperty< QVariant >::*at_function_type )( ::SireMol::CGAtomIdx const & ) const;
            at_function_type at_function_value( &::SireMol::AtomProperty< QVariant >::at );
            
            AtomVariantProperty_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QVariant >::canConvert
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< QVariant >::*canConvert_function_type )( ::QVariant const & ) const;
            canConvert_function_type canConvert_function_value( &::SireMol::AtomProperty< QVariant >::canConvert );
            
            AtomVariantProperty_exposer.def( 
                "canConvert"
                , canConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::AtomProperty< QVariant >::copyFrom
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< QVariant >::*copyFrom_function_type )( ::QVector< QVariant > const & ) ;
            copyFrom_function_type copyFrom_function_value( &::SireMol::AtomProperty< QVariant >::copyFrom );
            
            AtomVariantProperty_exposer.def( 
                "copyFrom"
                , copyFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::AtomProperty< QVariant >::copyFrom
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< QVariant >::*copyFrom_function_type )( ::QVector< QVariant > const &,::SireMol::AtomSelection const & ) ;
            copyFrom_function_type copyFrom_function_value( &::SireMol::AtomProperty< QVariant >::copyFrom );
            
            AtomVariantProperty_exposer.def( 
                "copyFrom"
                , copyFrom_function_value
                , ( bp::arg("values"), bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< QVariant >::count
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< QVariant >::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMol::AtomProperty< QVariant >::count );
            
            AtomVariantProperty_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMol::AtomProperty< QVariant >::fromVariant
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::SireMol::AtomProperty< QVariant > ( *fromVariant_function_type )( ::SireMol::AtomProperty< QVariant > const & );
            fromVariant_function_type fromVariant_function_value( &::SireMol::AtomProperty< QVariant >::fromVariant );
            
            AtomVariantProperty_exposer.def( 
                "fromVariant"
                , fromVariant_function_value
                , ( bp::arg("variant") ) );
        
        }
        { //::SireMol::AtomProperty< QVariant >::get
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< QVariant > const & ( ::SireMol::AtomProperty< QVariant >::*get_function_type )( ::SireMol::CGIdx ) const;
            get_function_type get_function_value( &::SireMol::AtomProperty< QVariant >::get );
            
            AtomVariantProperty_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QVariant >::get
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::QVariant const & ( ::SireMol::AtomProperty< QVariant >::*get_function_type )( ::SireMol::CGAtomIdx const & ) const;
            get_function_type get_function_value( &::SireMol::AtomProperty< QVariant >::get );
            
            AtomVariantProperty_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QVariant >::isCompatibleWith
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< QVariant >::*isCompatibleWith_function_type )( ::SireMol::MoleculeInfoData const & ) const;
            isCompatibleWith_function_type isCompatibleWith_function_value( &::SireMol::AtomProperty< QVariant >::isCompatibleWith );
            
            AtomVariantProperty_exposer.def( 
                "isCompatibleWith"
                , isCompatibleWith_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        { //::SireMol::AtomProperty< QVariant >::isEmpty
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< QVariant >::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::AtomProperty< QVariant >::isEmpty );
            
            AtomVariantProperty_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::AtomProperty< QVariant >::matchToSelection
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::SireMol::AtomProperty< QVariant > ( ::SireMol::AtomProperty< QVariant >::*matchToSelection_function_type )( ::SireMol::AtomSelection const & ) const;
            matchToSelection_function_type matchToSelection_function_value( &::SireMol::AtomProperty< QVariant >::matchToSelection );
            
            AtomVariantProperty_exposer.def( 
                "matchToSelection"
                , matchToSelection_function_value
                , ( bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< QVariant >::nAtoms
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< QVariant >::*nAtoms_function_type )(  ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::AtomProperty< QVariant >::nAtoms );
            
            AtomVariantProperty_exposer.def( 
                "nAtoms"
                , nAtoms_function_value );
        
        }
        { //::SireMol::AtomProperty< QVariant >::nAtoms
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< QVariant >::*nAtoms_function_type )( ::SireMol::CGIdx ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::AtomProperty< QVariant >::nAtoms );
            
            AtomVariantProperty_exposer.def( 
                "nAtoms"
                , nAtoms_function_value
                , ( bp::arg("cgidx") ) );
        
        }
        { //::SireMol::AtomProperty< QVariant >::nCutGroups
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< QVariant >::*nCutGroups_function_type )(  ) const;
            nCutGroups_function_type nCutGroups_function_value( &::SireMol::AtomProperty< QVariant >::nCutGroups );
            
            AtomVariantProperty_exposer.def( 
                "nCutGroups"
                , nCutGroups_function_value );
        
        }
        AtomVariantProperty_exposer.def( bp::self != bp::self );
        { //::SireMol::AtomProperty< QVariant >::operator=
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::SireMol::AtomProperty< QVariant > & ( ::SireMol::AtomProperty< QVariant >::*assign_function_type )( ::SireMol::AtomProperty< QVariant > const & ) ;
            assign_function_type assign_function_value( &::SireMol::AtomProperty< QVariant >::operator= );
            
            AtomVariantProperty_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        AtomVariantProperty_exposer.def( bp::self == bp::self );
        { //::SireMol::AtomProperty< QVariant >::operator[]
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< QVariant > const & ( ::SireMol::AtomProperty< QVariant >::*__getitem___function_type )( ::SireMol::CGIdx ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::AtomProperty< QVariant >::operator[] );
            
            AtomVariantProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QVariant >::operator[]
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::QVariant const & ( ::SireMol::AtomProperty< QVariant >::*__getitem___function_type )( ::SireMol::CGAtomIdx const & ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::AtomProperty< QVariant >::operator[] );
            
            AtomVariantProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QVariant >::set
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::SireMol::AtomProperty< QVariant > & ( ::SireMol::AtomProperty< QVariant >::*set_function_type )( ::SireMol::CGAtomIdx const &,::QVariant const & ) ;
            set_function_type set_function_value( &::SireMol::AtomProperty< QVariant >::set );
            
            AtomVariantProperty_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgatomidx"), bp::arg("value") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::AtomProperty< QVariant >::set
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::SireMol::AtomProperty< QVariant > & ( ::SireMol::AtomProperty< QVariant >::*set_function_type )( ::SireMol::CGIdx,::QVector< QVariant > const & ) ;
            set_function_type set_function_value( &::SireMol::AtomProperty< QVariant >::set );
            
            AtomVariantProperty_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgidx"), bp::arg("values") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::AtomProperty< QVariant >::size
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< QVariant >::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMol::AtomProperty< QVariant >::size );
            
            AtomVariantProperty_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMol::AtomProperty< QVariant >::toString
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::QString ( ::SireMol::AtomProperty< QVariant >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::AtomProperty< QVariant >::toString );
            
            AtomVariantProperty_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::AtomProperty< QVariant >::toVariant
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::SireMol::AtomProperty< QVariant > ( ::SireMol::AtomProperty< QVariant >::*toVariant_function_type )(  ) const;
            toVariant_function_type toVariant_function_value( &::SireMol::AtomProperty< QVariant >::toVariant );
            
            AtomVariantProperty_exposer.def( 
                "toVariant"
                , toVariant_function_value );
        
        }
        { //::SireMol::AtomProperty< QVariant >::toVector
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::QVector< QVariant > ( ::SireMol::AtomProperty< QVariant >::*toVector_function_type )(  ) const;
            toVector_function_type toVector_function_value( &::SireMol::AtomProperty< QVariant >::toVector );
            
            AtomVariantProperty_exposer.def( 
                "toVector"
                , toVector_function_value );
        
        }
        { //::SireMol::AtomProperty< QVariant >::toVector
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef ::QVector< QVariant > ( ::SireMol::AtomProperty< QVariant >::*toVector_function_type )( ::SireMol::AtomSelection const & ) const;
            toVector_function_type toVector_function_value( &::SireMol::AtomProperty< QVariant >::toVector );
            
            AtomVariantProperty_exposer.def( 
                "toVector"
                , toVector_function_value
                , ( bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< QVariant >::typeName
        
            typedef SireMol::AtomProperty< QVariant > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::AtomProperty< QVariant >::typeName );
            
            AtomVariantProperty_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        AtomVariantProperty_exposer.staticmethod( "fromVariant" );
        AtomVariantProperty_exposer.staticmethod( "typeName" );
        AtomVariantProperty_exposer.def( "__copy__", &__copy__);
        AtomVariantProperty_exposer.def( "__deepcopy__", &__copy__);
        AtomVariantProperty_exposer.def( "clone", &__copy__);
        AtomVariantProperty_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::AtomProperty<QVariant> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AtomVariantProperty_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::AtomProperty<QVariant> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AtomVariantProperty_exposer.def( "__str__", &__str__< ::SireMol::AtomProperty<QVariant> > );
        AtomVariantProperty_exposer.def( "__repr__", &__str__< ::SireMol::AtomProperty<QVariant> > );
    }

}
