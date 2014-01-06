// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "SegVariantProperty.pypp.hpp"

namespace bp = boost::python;

#include "segproperty.hpp"

#include "segproperty.hpp"

#include "SireMaths/vector.h"

SireMol::SegProperty<QVariant> __copy__(const SireMol::SegProperty<QVariant> &other){ return SireMol::SegProperty<QVariant>(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_SegVariantProperty_class(){

    { //::SireMol::SegProperty< QVariant >
        typedef bp::class_< SireMol::SegProperty< QVariant >, bp::bases< SireMol::SegProp, SireMol::MolViewProperty, SireBase::Property > > SegVariantProperty_exposer_t;
        SegVariantProperty_exposer_t SegVariantProperty_exposer = SegVariantProperty_exposer_t( "SegVariantProperty", bp::init< >() );
        bp::scope SegVariantProperty_scope( SegVariantProperty_exposer );
        SegVariantProperty_exposer.def( bp::init< SireMol::MoleculeInfoData const & >(( bp::arg("molinfo") )) );
        SegVariantProperty_exposer.def( bp::init< QVector< QVariant > const & >(( bp::arg("values") )) );
        SegVariantProperty_exposer.def( bp::init< SireMol::SegProperty< QVariant > const & >(( bp::arg("other") )) );
        { //::SireMol::SegProperty< QVariant >::array
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef ::QVector< QVariant > const & ( ::SireMol::SegProperty< QVariant >::*array_function_type )(  ) const;
            array_function_type array_function_value( &::SireMol::SegProperty< QVariant >::array );
            
            SegVariantProperty_exposer.def( 
                "array"
                , array_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::SegProperty< QVariant >::assertCanConvert
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef void ( ::SireMol::SegProperty< QVariant >::*assertCanConvert_function_type )( ::QVariant const & ) const;
            assertCanConvert_function_type assertCanConvert_function_value( &::SireMol::SegProperty< QVariant >::assertCanConvert );
            
            SegVariantProperty_exposer.def( 
                "assertCanConvert"
                , assertCanConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::SegProperty< QVariant >::assignFrom
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef void ( ::SireMol::SegProperty< QVariant >::*assignFrom_function_type )( ::SireMol::SegProperty< QVariant > const & ) ;
            assignFrom_function_type assignFrom_function_value( &::SireMol::SegProperty< QVariant >::assignFrom );
            
            SegVariantProperty_exposer.def( 
                "assignFrom"
                , assignFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::SegProperty< QVariant >::at
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef ::QVariant const & ( ::SireMol::SegProperty< QVariant >::*at_function_type )( ::SireMol::SegIdx const & ) const;
            at_function_type at_function_value( &::SireMol::SegProperty< QVariant >::at );
            
            SegVariantProperty_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("segidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::SegProperty< QVariant >::canConvert
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef bool ( ::SireMol::SegProperty< QVariant >::*canConvert_function_type )( ::QVariant const & ) const;
            canConvert_function_type canConvert_function_value( &::SireMol::SegProperty< QVariant >::canConvert );
            
            SegVariantProperty_exposer.def( 
                "canConvert"
                , canConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::SegProperty< QVariant >::count
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef int ( ::SireMol::SegProperty< QVariant >::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMol::SegProperty< QVariant >::count );
            
            SegVariantProperty_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMol::SegProperty< QVariant >::fromVariant
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef ::SireMol::SegProperty< QVariant > ( *fromVariant_function_type )( ::SireMol::SegProperty< QVariant > const & );
            fromVariant_function_type fromVariant_function_value( &::SireMol::SegProperty< QVariant >::fromVariant );
            
            SegVariantProperty_exposer.def( 
                "fromVariant"
                , fromVariant_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::SegProperty< QVariant >::get
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef ::QVariant const & ( ::SireMol::SegProperty< QVariant >::*get_function_type )( ::SireMol::SegIdx const & ) const;
            get_function_type get_function_value( &::SireMol::SegProperty< QVariant >::get );
            
            SegVariantProperty_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("segidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::SegProperty< QVariant >::isCompatibleWith
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef bool ( ::SireMol::SegProperty< QVariant >::*isCompatibleWith_function_type )( ::SireMol::MoleculeInfoData const & ) const;
            isCompatibleWith_function_type isCompatibleWith_function_value( &::SireMol::SegProperty< QVariant >::isCompatibleWith );
            
            SegVariantProperty_exposer.def( 
                "isCompatibleWith"
                , isCompatibleWith_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        { //::SireMol::SegProperty< QVariant >::isEmpty
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef bool ( ::SireMol::SegProperty< QVariant >::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::SegProperty< QVariant >::isEmpty );
            
            SegVariantProperty_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::SegProperty< QVariant >::nSegments
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef int ( ::SireMol::SegProperty< QVariant >::*nSegments_function_type )(  ) const;
            nSegments_function_type nSegments_function_value( &::SireMol::SegProperty< QVariant >::nSegments );
            
            SegVariantProperty_exposer.def( 
                "nSegments"
                , nSegments_function_value );
        
        }
        SegVariantProperty_exposer.def( bp::self != bp::self );
        { //::SireMol::SegProperty< QVariant >::operator=
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef ::SireMol::SegProperty< QVariant > & ( ::SireMol::SegProperty< QVariant >::*assign_function_type )( ::SireMol::SegProperty< QVariant > const & ) ;
            assign_function_type assign_function_value( &::SireMol::SegProperty< QVariant >::operator= );
            
            SegVariantProperty_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        SegVariantProperty_exposer.def( bp::self == bp::self );
        { //::SireMol::SegProperty< QVariant >::operator[]
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef ::QVariant const & ( ::SireMol::SegProperty< QVariant >::*__getitem___function_type )( ::SireMol::SegIdx const & ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::SegProperty< QVariant >::operator[] );
            
            SegVariantProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("segidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::SegProperty< QVariant >::set
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef ::SireMol::SegProperty< QVariant > & ( ::SireMol::SegProperty< QVariant >::*set_function_type )( ::SireMol::SegIdx,::QVariant const & ) ;
            set_function_type set_function_value( &::SireMol::SegProperty< QVariant >::set );
            
            SegVariantProperty_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("segidx"), bp::arg("value") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::SegProperty< QVariant >::size
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef int ( ::SireMol::SegProperty< QVariant >::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMol::SegProperty< QVariant >::size );
            
            SegVariantProperty_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMol::SegProperty< QVariant >::toString
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef ::QString ( ::SireMol::SegProperty< QVariant >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::SegProperty< QVariant >::toString );
            
            SegVariantProperty_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::SegProperty< QVariant >::toVariant
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef ::SireMol::SegProperty< QVariant > ( ::SireMol::SegProperty< QVariant >::*toVariant_function_type )(  ) const;
            toVariant_function_type toVariant_function_value( &::SireMol::SegProperty< QVariant >::toVariant );
            
            SegVariantProperty_exposer.def( 
                "toVariant"
                , toVariant_function_value );
        
        }
        { //::SireMol::SegProperty< QVariant >::typeName
        
            typedef SireMol::SegProperty< QVariant > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::SegProperty< QVariant >::typeName );
            
            SegVariantProperty_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        SegVariantProperty_exposer.staticmethod( "fromVariant" );
        SegVariantProperty_exposer.staticmethod( "typeName" );
        SegVariantProperty_exposer.def( "__copy__", &__copy__);
        SegVariantProperty_exposer.def( "__deepcopy__", &__copy__);
        SegVariantProperty_exposer.def( "clone", &__copy__);
        SegVariantProperty_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::SegProperty<QVariant> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        SegVariantProperty_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::SegProperty<QVariant> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        SegVariantProperty_exposer.def( "__str__", &__str__< ::SireMol::SegProperty<QVariant> > );
        SegVariantProperty_exposer.def( "__repr__", &__str__< ::SireMol::SegProperty<QVariant> > );
        SegVariantProperty_exposer.def( "__len__", &__len_size< ::SireMol::SegProperty<QVariant> > );
    }

}
