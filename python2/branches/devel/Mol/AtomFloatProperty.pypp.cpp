// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "AtomFloatProperty.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireMaths/vector.h"

#include "atombeads.h"

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

SireMol::AtomProperty<double> __copy__(const SireMol::AtomProperty<double> &other){ return SireMol::AtomProperty<double>(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_AtomFloatProperty_class(){

    { //::SireMol::AtomProperty< double >
        typedef bp::class_< SireMol::AtomProperty< double >, bp::bases< SireMol::AtomProp, SireMol::MolViewProperty, SireBase::Property > > AtomFloatProperty_exposer_t;
        AtomFloatProperty_exposer_t AtomFloatProperty_exposer = AtomFloatProperty_exposer_t( "AtomFloatProperty" );
        bp::scope AtomFloatProperty_scope( AtomFloatProperty_exposer );
        AtomFloatProperty_exposer.def( bp::init< >() );
        AtomFloatProperty_exposer.def( bp::init< SireMol::MoleculeInfoData const & >(( bp::arg("molinfo") )) );
        AtomFloatProperty_exposer.def( bp::init< SireMol::MoleculeInfoData const &, double const & >(( bp::arg("molinfo"), bp::arg("default_value") )) );
        AtomFloatProperty_exposer.def( bp::init< double const & >(( bp::arg("value") )) );
        AtomFloatProperty_exposer.def( bp::init< SireBase::PackedArray2D< double > const & >(( bp::arg("values") )) );
        AtomFloatProperty_exposer.def( bp::init< SireMol::AtomProperty< double > const & >(( bp::arg("other") )) );
        { //::SireMol::AtomProperty< double >::array
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::SireBase::PackedArray2D< double > const & ( ::SireMol::AtomProperty< double >::*array_function_type )(  ) const;
            array_function_type array_function_value( &::SireMol::AtomProperty< double >::array );
            
            AtomFloatProperty_exposer.def( 
                "array"
                , array_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< double >::assertCanConvert
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< double >::*assertCanConvert_function_type )( ::QVariant const & ) const;
            assertCanConvert_function_type assertCanConvert_function_value( &::SireMol::AtomProperty< double >::assertCanConvert );
            
            AtomFloatProperty_exposer.def( 
                "assertCanConvert"
                , assertCanConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::AtomProperty< double >::assignFrom
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< double >::*assignFrom_function_type )( ::SireMol::AtomProperty< QVariant > const & ) ;
            assignFrom_function_type assignFrom_function_value( &::SireMol::AtomProperty< double >::assignFrom );
            
            AtomFloatProperty_exposer.def( 
                "assignFrom"
                , assignFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::AtomProperty< double >::at
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< double > const & ( ::SireMol::AtomProperty< double >::*at_function_type )( ::SireMol::CGIdx ) const;
            at_function_type at_function_value( &::SireMol::AtomProperty< double >::at );
            
            AtomFloatProperty_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< double >::at
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef double const & ( ::SireMol::AtomProperty< double >::*at_function_type )( ::SireMol::CGAtomIdx const & ) const;
            at_function_type at_function_value( &::SireMol::AtomProperty< double >::at );
            
            AtomFloatProperty_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< double >::canConvert
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< double >::*canConvert_function_type )( ::QVariant const & ) const;
            canConvert_function_type canConvert_function_value( &::SireMol::AtomProperty< double >::canConvert );
            
            AtomFloatProperty_exposer.def( 
                "canConvert"
                , canConvert_function_value
                , ( bp::arg("value") ) );
        
        }
        { //::SireMol::AtomProperty< double >::copyFrom
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< double >::*copyFrom_function_type )( ::QVector< double > const & ) ;
            copyFrom_function_type copyFrom_function_value( &::SireMol::AtomProperty< double >::copyFrom );
            
            AtomFloatProperty_exposer.def( 
                "copyFrom"
                , copyFrom_function_value
                , ( bp::arg("values") ) );
        
        }
        { //::SireMol::AtomProperty< double >::copyFrom
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef void ( ::SireMol::AtomProperty< double >::*copyFrom_function_type )( ::QVector< double > const &,::SireMol::AtomSelection const & ) ;
            copyFrom_function_type copyFrom_function_value( &::SireMol::AtomProperty< double >::copyFrom );
            
            AtomFloatProperty_exposer.def( 
                "copyFrom"
                , copyFrom_function_value
                , ( bp::arg("values"), bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< double >::count
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< double >::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireMol::AtomProperty< double >::count );
            
            AtomFloatProperty_exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireMol::AtomProperty< double >::fromVariant
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::SireMol::AtomProperty< double > ( *fromVariant_function_type )( ::SireMol::AtomProperty< QVariant > const & );
            fromVariant_function_type fromVariant_function_value( &::SireMol::AtomProperty< double >::fromVariant );
            
            AtomFloatProperty_exposer.def( 
                "fromVariant"
                , fromVariant_function_value
                , ( bp::arg("variant") ) );
        
        }
        { //::SireMol::AtomProperty< double >::get
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< double > const & ( ::SireMol::AtomProperty< double >::*get_function_type )( ::SireMol::CGIdx ) const;
            get_function_type get_function_value( &::SireMol::AtomProperty< double >::get );
            
            AtomFloatProperty_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< double >::get
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef double const & ( ::SireMol::AtomProperty< double >::*get_function_type )( ::SireMol::CGAtomIdx const & ) const;
            get_function_type get_function_value( &::SireMol::AtomProperty< double >::get );
            
            AtomFloatProperty_exposer.def( 
                "get"
                , get_function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< double >::isCompatibleWith
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< double >::*isCompatibleWith_function_type )( ::SireMol::MoleculeInfoData const & ) const;
            isCompatibleWith_function_type isCompatibleWith_function_value( &::SireMol::AtomProperty< double >::isCompatibleWith );
            
            AtomFloatProperty_exposer.def( 
                "isCompatibleWith"
                , isCompatibleWith_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        { //::SireMol::AtomProperty< double >::isEmpty
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef bool ( ::SireMol::AtomProperty< double >::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::AtomProperty< double >::isEmpty );
            
            AtomFloatProperty_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::AtomProperty< double >::matchToSelection
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::SireMol::AtomProperty< double > ( ::SireMol::AtomProperty< double >::*matchToSelection_function_type )( ::SireMol::AtomSelection const & ) const;
            matchToSelection_function_type matchToSelection_function_value( &::SireMol::AtomProperty< double >::matchToSelection );
            
            AtomFloatProperty_exposer.def( 
                "matchToSelection"
                , matchToSelection_function_value
                , ( bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< double >::nAtoms
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< double >::*nAtoms_function_type )(  ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::AtomProperty< double >::nAtoms );
            
            AtomFloatProperty_exposer.def( 
                "nAtoms"
                , nAtoms_function_value );
        
        }
        { //::SireMol::AtomProperty< double >::nAtoms
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< double >::*nAtoms_function_type )( ::SireMol::CGIdx ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMol::AtomProperty< double >::nAtoms );
            
            AtomFloatProperty_exposer.def( 
                "nAtoms"
                , nAtoms_function_value
                , ( bp::arg("cgidx") ) );
        
        }
        { //::SireMol::AtomProperty< double >::nCutGroups
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< double >::*nCutGroups_function_type )(  ) const;
            nCutGroups_function_type nCutGroups_function_value( &::SireMol::AtomProperty< double >::nCutGroups );
            
            AtomFloatProperty_exposer.def( 
                "nCutGroups"
                , nCutGroups_function_value );
        
        }
        AtomFloatProperty_exposer.def( bp::self != bp::self );
        { //::SireMol::AtomProperty< double >::operator=
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::SireMol::AtomProperty< double > & ( ::SireMol::AtomProperty< double >::*assign_function_type )( ::SireMol::AtomProperty< double > const & ) ;
            assign_function_type assign_function_value( &::SireMol::AtomProperty< double >::operator= );
            
            AtomFloatProperty_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        AtomFloatProperty_exposer.def( bp::self == bp::self );
        { //::SireMol::AtomProperty< double >::operator[]
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< double > const & ( ::SireMol::AtomProperty< double >::*__getitem___function_type )( ::SireMol::CGIdx ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::AtomProperty< double >::operator[] );
            
            AtomFloatProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< double >::operator[]
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef double const & ( ::SireMol::AtomProperty< double >::*__getitem___function_type )( ::SireMol::CGAtomIdx const & ) const;
            __getitem___function_type __getitem___function_value( &::SireMol::AtomProperty< double >::operator[] );
            
            AtomFloatProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("cgatomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::AtomProperty< double >::set
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::SireMol::AtomProperty< double > & ( ::SireMol::AtomProperty< double >::*set_function_type )( ::SireMol::CGAtomIdx const &,double const & ) ;
            set_function_type set_function_value( &::SireMol::AtomProperty< double >::set );
            
            AtomFloatProperty_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgatomidx"), bp::arg("value") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::AtomProperty< double >::set
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::SireMol::AtomProperty< double > & ( ::SireMol::AtomProperty< double >::*set_function_type )( ::SireMol::CGIdx,::QVector< double > const & ) ;
            set_function_type set_function_value( &::SireMol::AtomProperty< double >::set );
            
            AtomFloatProperty_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("cgidx"), bp::arg("values") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::AtomProperty< double >::size
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef int ( ::SireMol::AtomProperty< double >::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireMol::AtomProperty< double >::size );
            
            AtomFloatProperty_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireMol::AtomProperty< double >::toString
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::QString ( ::SireMol::AtomProperty< double >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::AtomProperty< double >::toString );
            
            AtomFloatProperty_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::AtomProperty< double >::toVariant
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::SireMol::AtomProperty< QVariant > ( ::SireMol::AtomProperty< double >::*toVariant_function_type )(  ) const;
            toVariant_function_type toVariant_function_value( &::SireMol::AtomProperty< double >::toVariant );
            
            AtomFloatProperty_exposer.def( 
                "toVariant"
                , toVariant_function_value );
        
        }
        { //::SireMol::AtomProperty< double >::toVector
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::QVector< double > ( ::SireMol::AtomProperty< double >::*toVector_function_type )(  ) const;
            toVector_function_type toVector_function_value( &::SireMol::AtomProperty< double >::toVector );
            
            AtomFloatProperty_exposer.def( 
                "toVector"
                , toVector_function_value );
        
        }
        { //::SireMol::AtomProperty< double >::toVector
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef ::QVector< double > ( ::SireMol::AtomProperty< double >::*toVector_function_type )( ::SireMol::AtomSelection const & ) const;
            toVector_function_type toVector_function_value( &::SireMol::AtomProperty< double >::toVector );
            
            AtomFloatProperty_exposer.def( 
                "toVector"
                , toVector_function_value
                , ( bp::arg("selection") ) );
        
        }
        { //::SireMol::AtomProperty< double >::typeName
        
            typedef SireMol::AtomProperty< double > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::AtomProperty< double >::typeName );
            
            AtomFloatProperty_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        AtomFloatProperty_exposer.staticmethod( "fromVariant" );
        AtomFloatProperty_exposer.staticmethod( "typeName" );
        AtomFloatProperty_exposer.def( "__copy__", &__copy__);
        AtomFloatProperty_exposer.def( "__deepcopy__", &__copy__);
        AtomFloatProperty_exposer.def( "clone", &__copy__);
        AtomFloatProperty_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::AtomProperty<double> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AtomFloatProperty_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::AtomProperty<double> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AtomFloatProperty_exposer.def( "__str__", &__str__< ::SireMol::AtomProperty<double> > );
        AtomFloatProperty_exposer.def( "__repr__", &__str__< ::SireMol::AtomProperty<double> > );
    }

}
