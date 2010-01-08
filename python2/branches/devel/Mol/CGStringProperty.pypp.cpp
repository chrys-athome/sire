// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "CGStringProperty.pypp.hpp"

namespace bp = boost::python;

#include "cgproperty.hpp"

#include "cgproperty.hpp"

SireMol::CGProperty<QString> __copy__(const SireMol::CGProperty<QString> &other){ return SireMol::CGProperty<QString>(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_CGStringProperty_class(){

    { //::SireMol::CGProperty< QString >
        typedef bp::class_< SireMol::CGProperty< QString >, bp::bases< SireMol::CGProp, SireMol::MolViewProperty, SireBase::Property > > CGStringProperty_exposer_t;
        CGStringProperty_exposer_t CGStringProperty_exposer = CGStringProperty_exposer_t( "CGStringProperty" );
        bp::scope CGStringProperty_scope( CGStringProperty_exposer );
        CGStringProperty_exposer.def( bp::init< >() );
        CGStringProperty_exposer.def( bp::init< SireMol::MoleculeInfoData const & >(( bp::arg("molinfo") )) );
        CGStringProperty_exposer.def( bp::init< QVector< QString > const & >(( bp::arg("values") )) );
        CGStringProperty_exposer.def( bp::init< SireMol::CGProperty< QString > const & >(( bp::arg("other") )) );
        { //::SireMol::CGProperty< QString >::array
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef ::QVector< QString > const & ( ::SireMol::CGProperty< QString >::*array_function_type )(  ) const;
            array_function_type array_function_value( &::SireMol::CGProperty< QString >::array );
            
            CGStringProperty_exposer.def( 
                "array"
                , array_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::CGProperty< QString >::assertCanConvert
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef void ( ::SireMol::CGProperty< QString >::*assertCanConvert_function_type )( ::QVariant const & ) const;
            assertCanConvert_function_type assertCanConvert_function_value( &::SireMol::CGProperty< QString >::assertCanConvert );
            
            CGStringProperty_exposer.def( 
                "assertCanConvert"
                , assertCanConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::CGProperty< QString >::assignFrom
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef void ( ::SireMol::CGProperty< QString >::*assignFrom_function_type )( ::SireMol::CGProperty< QVariant > const & ) ;
            assignFrom_function_type assignFrom_function_value( &::SireMol::CGProperty< QString >::assignFrom );
            
            CGStringProperty_exposer.def( 
                "assignFrom"
                , assignFrom_function_value
                , ( bp::arg("variant") ) );
        
        }
        { //::SireMol::CGProperty< QString >::at
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef ::QString const & ( ::SireMol::CGProperty< QString >::*at_function_type )( ::SireMol::CGIdx const & ) const;
            at_function_type at_function_value( &::SireMol::CGProperty< QString >::at );
            
            CGStringProperty_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::CGProperty< QString >::canConvert
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef bool ( ::SireMol::CGProperty< QString >::*canConvert_function_type )( ::QVariant const & ) const;
            canConvert_function_type canConvert_function_value( &::SireMol::CGProperty< QString >::canConvert );
            
            CGStringProperty_exposer.def( 
                "canConvert"
                , canConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::CGProperty< QString >::count
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef int ( ::SireMol::CGProperty< QString >::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMol::CGProperty< QString >::count );
            
            CGStringProperty_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMol::CGProperty< QString >::fromVariant
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef ::SireMol::CGProperty< QString > ( *fromVariant_function_type )( ::SireMol::CGProperty< QVariant > const & );
            fromVariant_function_type fromVariant_function_value( &::SireMol::CGProperty< QString >::fromVariant );
            
            CGStringProperty_exposer.def( 
                "fromVariant"
                , fromVariant_function_value
                , ( bp::arg("variant") ) );
        
        }
        { //::SireMol::CGProperty< QString >::get
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef ::QString const & ( ::SireMol::CGProperty< QString >::*get_function_type )( ::SireMol::CGIdx const & ) const;
            get_function_type get_function_value( &::SireMol::CGProperty< QString >::get );
            
            CGStringProperty_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::CGProperty< QString >::isCompatibleWith
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef bool ( ::SireMol::CGProperty< QString >::*isCompatibleWith_function_type )( ::SireMol::MoleculeInfoData const & ) const;
            isCompatibleWith_function_type isCompatibleWith_function_value( &::SireMol::CGProperty< QString >::isCompatibleWith );
            
            CGStringProperty_exposer.def( 
                "isCompatibleWith"
                , isCompatibleWith_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        { //::SireMol::CGProperty< QString >::isEmpty
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef bool ( ::SireMol::CGProperty< QString >::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::CGProperty< QString >::isEmpty );
            
            CGStringProperty_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::CGProperty< QString >::nCutGroups
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef int ( ::SireMol::CGProperty< QString >::*nCutGroups_function_type )(  ) const;
            nCutGroups_function_type nCutGroups_function_value( &::SireMol::CGProperty< QString >::nCutGroups );
            
            CGStringProperty_exposer.def( 
                "nCutGroups"
                , nCutGroups_function_value );
        
        }
        CGStringProperty_exposer.def( bp::self != bp::self );
        { //::SireMol::CGProperty< QString >::operator=
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef ::SireMol::CGProperty< QString > & ( ::SireMol::CGProperty< QString >::*assign_function_type )( ::SireMol::CGProperty< QString > const & ) ;
            assign_function_type assign_function_value( &::SireMol::CGProperty< QString >::operator= );
            
            CGStringProperty_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        CGStringProperty_exposer.def( bp::self == bp::self );
        { //::SireMol::CGProperty< QString >::operator[]
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef ::QString const & ( ::SireMol::CGProperty< QString >::*__getitem___function_type )( ::SireMol::CGIdx const & ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::CGProperty< QString >::operator[] );
            
            CGStringProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::CGProperty< QString >::set
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef ::SireMol::CGProperty< QString > & ( ::SireMol::CGProperty< QString >::*set_function_type )( ::SireMol::CGIdx,::QString const & ) ;
            set_function_type set_function_value( &::SireMol::CGProperty< QString >::set );
            
            CGStringProperty_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgidx"), bp::arg("value") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::CGProperty< QString >::size
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef int ( ::SireMol::CGProperty< QString >::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMol::CGProperty< QString >::size );
            
            CGStringProperty_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMol::CGProperty< QString >::toString
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef ::QString ( ::SireMol::CGProperty< QString >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::CGProperty< QString >::toString );
            
            CGStringProperty_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::CGProperty< QString >::toVariant
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef ::SireMol::CGProperty< QVariant > ( ::SireMol::CGProperty< QString >::*toVariant_function_type )(  ) const;
            toVariant_function_type toVariant_function_value( &::SireMol::CGProperty< QString >::toVariant );
            
            CGStringProperty_exposer.def( 
                "toVariant"
                , toVariant_function_value );
        
        }
        { //::SireMol::CGProperty< QString >::typeName
        
            typedef SireMol::CGProperty< QString > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::CGProperty< QString >::typeName );
            
            CGStringProperty_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        CGStringProperty_exposer.staticmethod( "fromVariant" );
        CGStringProperty_exposer.staticmethod( "typeName" );
        CGStringProperty_exposer.def( "__copy__", &__copy__);
        CGStringProperty_exposer.def( "__deepcopy__", &__copy__);
        CGStringProperty_exposer.def( "clone", &__copy__);
        CGStringProperty_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::CGProperty<QString> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CGStringProperty_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::CGProperty<QString> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CGStringProperty_exposer.def( "__str__", &__str__< ::SireMol::CGProperty<QString> > );
        CGStringProperty_exposer.def( "__repr__", &__str__< ::SireMol::CGProperty<QString> > );
    }

}
