// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "AtomStringProperty.pypp.hpp"

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

SireMol::AtomProperty<QString> __copy__(const SireMol::AtomProperty<QString> &other){ return SireMol::AtomProperty<QString>(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_AtomStringProperty_class(){

    { //::SireMol::AtomProperty< QString >
        typedef bp::class_< SireMol::AtomProperty< QString >, bp::bases< SireMol::AtomProp, SireMol::MolViewProperty, SireBase::Property > > AtomStringProperty_exposer_t;
        AtomStringProperty_exposer_t AtomStringProperty_exposer = AtomStringProperty_exposer_t( "AtomStringProperty", bp::init< >() );
        bp::scope AtomStringProperty_scope( AtomStringProperty_exposer );
        AtomStringProperty_exposer.def( bp::init< SireMol::MoleculeInfoData const & >(( bp::arg("molinfo") )) );
        AtomStringProperty_exposer.def( bp::init< SireMol::MoleculeInfoData const &, QString const & >(( bp::arg("molinfo"), bp::arg("default_value") )) );
        AtomStringProperty_exposer.def( bp::init< QString const & >(( bp::arg("value") )) );
        AtomStringProperty_exposer.def( bp::init< SireBase::PackedArray2D< QString > const & >(( bp::arg("values") )) );
        AtomStringProperty_exposer.def( bp::init< SireMol::AtomProperty< QString > const & >(( bp::arg("other") )) );
        { //::SireMol::AtomProperty< QString >::array
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::SireBase::PackedArray2D< QString > const & ( ::SireMol::AtomProperty< QString >::*array_function_type )(  ) const;
            array_function_type array_function_value( &::SireMol::AtomProperty< QString >::array );
            
            AtomStringProperty_exposer.def( 
                "array"
                , array_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QString >::assertCanConvert
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< QString >::*assertCanConvert_function_type )( ::QVariant const & ) const;
            assertCanConvert_function_type assertCanConvert_function_value( &::SireMol::AtomProperty< QString >::assertCanConvert );
            
            AtomStringProperty_exposer.def( 
                "assertCanConvert"
                , assertCanConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::AtomProperty< QString >::assignFrom
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< QString >::*assignFrom_function_type )( ::SireMol::AtomProperty< QVariant > const & ) ;
            assignFrom_function_type assignFrom_function_value( &::SireMol::AtomProperty< QString >::assignFrom );
            
            AtomStringProperty_exposer.def( 
                "assignFrom"
                , assignFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::AtomProperty< QString >::at
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< QString > const & ( ::SireMol::AtomProperty< QString >::*at_function_type )( ::SireMol::CGIdx ) const;
            at_function_type at_function_value( &::SireMol::AtomProperty< QString >::at );
            
            AtomStringProperty_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QString >::at
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::QString const & ( ::SireMol::AtomProperty< QString >::*at_function_type )( ::SireMol::CGAtomIdx const & ) const;
            at_function_type at_function_value( &::SireMol::AtomProperty< QString >::at );
            
            AtomStringProperty_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QString >::canConvert
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< QString >::*canConvert_function_type )( ::QVariant const & ) const;
            canConvert_function_type canConvert_function_value( &::SireMol::AtomProperty< QString >::canConvert );
            
            AtomStringProperty_exposer.def( 
                "canConvert"
                , canConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::AtomProperty< QString >::copyFrom
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< QString >::*copyFrom_function_type )( ::QVector< QString > const & ) ;
            copyFrom_function_type copyFrom_function_value( &::SireMol::AtomProperty< QString >::copyFrom );
            
            AtomStringProperty_exposer.def( 
                "copyFrom"
                , copyFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::AtomProperty< QString >::copyFrom
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< QString >::*copyFrom_function_type )( ::QVector< QString > const &,::SireMol::AtomSelection const & ) ;
            copyFrom_function_type copyFrom_function_value( &::SireMol::AtomProperty< QString >::copyFrom );
            
            AtomStringProperty_exposer.def( 
                "copyFrom"
                , copyFrom_function_value
                , ( bp::arg("values"), bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< QString >::count
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< QString >::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMol::AtomProperty< QString >::count );
            
            AtomStringProperty_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMol::AtomProperty< QString >::fromVariant
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::SireMol::AtomProperty< QString > ( *fromVariant_function_type )( ::SireMol::AtomProperty< QVariant > const & );
            fromVariant_function_type fromVariant_function_value( &::SireMol::AtomProperty< QString >::fromVariant );
            
            AtomStringProperty_exposer.def( 
                "fromVariant"
                , fromVariant_function_value
                , ( bp::arg("variant") ) );
        
        }
        { //::SireMol::AtomProperty< QString >::get
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< QString > const & ( ::SireMol::AtomProperty< QString >::*get_function_type )( ::SireMol::CGIdx ) const;
            get_function_type get_function_value( &::SireMol::AtomProperty< QString >::get );
            
            AtomStringProperty_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QString >::get
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::QString const & ( ::SireMol::AtomProperty< QString >::*get_function_type )( ::SireMol::CGAtomIdx const & ) const;
            get_function_type get_function_value( &::SireMol::AtomProperty< QString >::get );
            
            AtomStringProperty_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QString >::isCompatibleWith
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< QString >::*isCompatibleWith_function_type )( ::SireMol::MoleculeInfoData const & ) const;
            isCompatibleWith_function_type isCompatibleWith_function_value( &::SireMol::AtomProperty< QString >::isCompatibleWith );
            
            AtomStringProperty_exposer.def( 
                "isCompatibleWith"
                , isCompatibleWith_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        { //::SireMol::AtomProperty< QString >::isEmpty
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< QString >::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::AtomProperty< QString >::isEmpty );
            
            AtomStringProperty_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::AtomProperty< QString >::matchToSelection
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::SireMol::AtomProperty< QString > ( ::SireMol::AtomProperty< QString >::*matchToSelection_function_type )( ::SireMol::AtomSelection const & ) const;
            matchToSelection_function_type matchToSelection_function_value( &::SireMol::AtomProperty< QString >::matchToSelection );
            
            AtomStringProperty_exposer.def( 
                "matchToSelection"
                , matchToSelection_function_value
                , ( bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< QString >::nAtoms
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< QString >::*nAtoms_function_type )(  ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::AtomProperty< QString >::nAtoms );
            
            AtomStringProperty_exposer.def( 
                "nAtoms"
                , nAtoms_function_value );
        
        }
        { //::SireMol::AtomProperty< QString >::nAtoms
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< QString >::*nAtoms_function_type )( ::SireMol::CGIdx ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::AtomProperty< QString >::nAtoms );
            
            AtomStringProperty_exposer.def( 
                "nAtoms"
                , nAtoms_function_value
                , ( bp::arg("cgidx") ) );
        
        }
        { //::SireMol::AtomProperty< QString >::nCutGroups
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< QString >::*nCutGroups_function_type )(  ) const;
            nCutGroups_function_type nCutGroups_function_value( &::SireMol::AtomProperty< QString >::nCutGroups );
            
            AtomStringProperty_exposer.def( 
                "nCutGroups"
                , nCutGroups_function_value );
        
        }
        AtomStringProperty_exposer.def( bp::self != bp::self );
        { //::SireMol::AtomProperty< QString >::operator=
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::SireMol::AtomProperty< QString > & ( ::SireMol::AtomProperty< QString >::*assign_function_type )( ::SireMol::AtomProperty< QString > const & ) ;
            assign_function_type assign_function_value( &::SireMol::AtomProperty< QString >::operator= );
            
            AtomStringProperty_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        AtomStringProperty_exposer.def( bp::self == bp::self );
        { //::SireMol::AtomProperty< QString >::operator[]
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< QString > const & ( ::SireMol::AtomProperty< QString >::*__getitem___function_type )( ::SireMol::CGIdx ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::AtomProperty< QString >::operator[] );
            
            AtomStringProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QString >::operator[]
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::QString const & ( ::SireMol::AtomProperty< QString >::*__getitem___function_type )( ::SireMol::CGAtomIdx const & ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::AtomProperty< QString >::operator[] );
            
            AtomStringProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< QString >::set
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::SireMol::AtomProperty< QString > & ( ::SireMol::AtomProperty< QString >::*set_function_type )( ::SireMol::CGAtomIdx const &,::QString const & ) ;
            set_function_type set_function_value( &::SireMol::AtomProperty< QString >::set );
            
            AtomStringProperty_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgatomidx"), bp::arg("value") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::AtomProperty< QString >::set
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::SireMol::AtomProperty< QString > & ( ::SireMol::AtomProperty< QString >::*set_function_type )( ::SireMol::CGIdx,::QVector< QString > const & ) ;
            set_function_type set_function_value( &::SireMol::AtomProperty< QString >::set );
            
            AtomStringProperty_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgidx"), bp::arg("values") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::AtomProperty< QString >::size
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< QString >::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMol::AtomProperty< QString >::size );
            
            AtomStringProperty_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMol::AtomProperty< QString >::toString
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::QString ( ::SireMol::AtomProperty< QString >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::AtomProperty< QString >::toString );
            
            AtomStringProperty_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::AtomProperty< QString >::toVariant
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::SireMol::AtomProperty< QVariant > ( ::SireMol::AtomProperty< QString >::*toVariant_function_type )(  ) const;
            toVariant_function_type toVariant_function_value( &::SireMol::AtomProperty< QString >::toVariant );
            
            AtomStringProperty_exposer.def( 
                "toVariant"
                , toVariant_function_value );
        
        }
        { //::SireMol::AtomProperty< QString >::toVector
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::QVector< QString > ( ::SireMol::AtomProperty< QString >::*toVector_function_type )(  ) const;
            toVector_function_type toVector_function_value( &::SireMol::AtomProperty< QString >::toVector );
            
            AtomStringProperty_exposer.def( 
                "toVector"
                , toVector_function_value );
        
        }
        { //::SireMol::AtomProperty< QString >::toVector
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef ::QVector< QString > ( ::SireMol::AtomProperty< QString >::*toVector_function_type )( ::SireMol::AtomSelection const & ) const;
            toVector_function_type toVector_function_value( &::SireMol::AtomProperty< QString >::toVector );
            
            AtomStringProperty_exposer.def( 
                "toVector"
                , toVector_function_value
                , ( bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< QString >::typeName
        
            typedef SireMol::AtomProperty< QString > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::AtomProperty< QString >::typeName );
            
            AtomStringProperty_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        AtomStringProperty_exposer.staticmethod( "fromVariant" );
        AtomStringProperty_exposer.staticmethod( "typeName" );
        AtomStringProperty_exposer.def( "__copy__", &__copy__);
        AtomStringProperty_exposer.def( "__deepcopy__", &__copy__);
        AtomStringProperty_exposer.def( "clone", &__copy__);
        AtomStringProperty_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::AtomProperty<QString> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AtomStringProperty_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::AtomProperty<QString> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AtomStringProperty_exposer.def( "__str__", &__str__< ::SireMol::AtomProperty<QString> > );
        AtomStringProperty_exposer.def( "__repr__", &__str__< ::SireMol::AtomProperty<QString> > );
    }

}
