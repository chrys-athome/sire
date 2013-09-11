// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "PackedArray2D_int_.pypp.hpp"

namespace bp = boost::python;

#include "packedarrays.h"

#include "packedarrays.h"

SireBase::PackedArray2D<int> __copy__(const SireBase::PackedArray2D<int> &other){ return SireBase::PackedArray2D<int>(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_PackedArray2D_int__class(){

    { //::SireBase::PackedArray2D< int >
        typedef bp::class_< SireBase::PackedArray2D< int > > PackedArray2D_int__exposer_t;
        PackedArray2D_int__exposer_t PackedArray2D_int__exposer = PackedArray2D_int__exposer_t( "PackedArray2D_int_", bp::init< >() );
        bp::scope PackedArray2D_int__scope( PackedArray2D_int__exposer );
        PackedArray2D_int__exposer.def( bp::init< SireBase::detail::PackedArray2D_Array< int > const & >(( bp::arg("array") )) );
        PackedArray2D_int__exposer.def( bp::init< QVector< SireBase::detail::PackedArray2D_Array< int > > const & >(( bp::arg("arrays") )) );
        PackedArray2D_int__exposer.def( bp::init< QVector< int > const & >(( bp::arg("values") )) );
        PackedArray2D_int__exposer.def( bp::init< QVector< QVector< int > > const & >(( bp::arg("values") )) );
        PackedArray2D_int__exposer.def( bp::init< SireBase::PackedArray2D< int > const &, SireBase::PackedArray2D< int > const & >(( bp::arg("array0"), bp::arg("array1") )) );
        PackedArray2D_int__exposer.def( bp::init< SireBase::PackedArray2D< int > const & >(( bp::arg("other") )) );
        { //::SireBase::PackedArray2D< int >::append
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef void ( ::SireBase::PackedArray2D< int >::*append_function_type )( ::SireBase::detail::PackedArray2D_Array< int > const & ) ;
            append_function_type append_function_value( &::SireBase::PackedArray2D< int >::append );
            
            PackedArray2D_int__exposer.def( 
                "append"
                , append_function_value
                , ( bp::arg("array") ) );
        
        }
        { //::SireBase::PackedArray2D< int >::append
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef void ( ::SireBase::PackedArray2D< int >::*append_function_type )( ::SireBase::PackedArray2D< int > const & ) ;
            append_function_type append_function_value( &::SireBase::PackedArray2D< int >::append );
            
            PackedArray2D_int__exposer.def( 
                "append"
                , append_function_value
                , ( bp::arg("arrays") ) );
        
        }
        { //::SireBase::PackedArray2D< int >::append
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef void ( ::SireBase::PackedArray2D< int >::*append_function_type )( ::QVector< int > const & ) ;
            append_function_type append_function_value( &::SireBase::PackedArray2D< int >::append );
            
            PackedArray2D_int__exposer.def( 
                "append"
                , append_function_value
                , ( bp::arg("array") ) );
        
        }
        { //::SireBase::PackedArray2D< int >::append
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef void ( ::SireBase::PackedArray2D< int >::*append_function_type )( ::QVector< QVector< int > > const & ) ;
            append_function_type append_function_value( &::SireBase::PackedArray2D< int >::append );
            
            PackedArray2D_int__exposer.def( 
                "append"
                , append_function_value
                , ( bp::arg("arrays") ) );
        
        }
        { //::SireBase::PackedArray2D< int >::assertValidIndex
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef void ( ::SireBase::PackedArray2D< int >::*assertValidIndex_function_type )( ::quint32 ) const;
            assertValidIndex_function_type assertValidIndex_function_value( &::SireBase::PackedArray2D< int >::assertValidIndex );
            
            PackedArray2D_int__exposer.def( 
                "assertValidIndex"
                , assertValidIndex_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireBase::PackedArray2D< int >::at
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< int > const & ( ::SireBase::PackedArray2D< int >::*at_function_type )( ::quint32 ) const;
            at_function_type at_function_value( &::SireBase::PackedArray2D< int >::at );
            
            PackedArray2D_int__exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireBase::PackedArray2D< int >::at
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef int const & ( ::SireBase::PackedArray2D< int >::*at_function_type )( ::quint32,::quint32 ) const;
            at_function_type at_function_value( &::SireBase::PackedArray2D< int >::at );
            
            PackedArray2D_int__exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("i"), bp::arg("j") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireBase::PackedArray2D< int >::count
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef int ( ::SireBase::PackedArray2D< int >::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireBase::PackedArray2D< int >::count );
            
            PackedArray2D_int__exposer.def( 
                "count"
                , count_function_value );
        
        }
        { //::SireBase::PackedArray2D< int >::fromVariant
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef ::SireBase::PackedArray2D< int > ( *fromVariant_function_type )( ::SireBase::PackedArray2D< QVariant > const & );
            fromVariant_function_type fromVariant_function_value( &::SireBase::PackedArray2D< int >::fromVariant );
            
            PackedArray2D_int__exposer.def( 
                "fromVariant"
                , fromVariant_function_value
                , ( bp::arg("variant") ) );
        
        }
        { //::SireBase::PackedArray2D< int >::isEmpty
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef bool ( ::SireBase::PackedArray2D< int >::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireBase::PackedArray2D< int >::isEmpty );
            
            PackedArray2D_int__exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireBase::PackedArray2D< int >::nArrays
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef int ( ::SireBase::PackedArray2D< int >::*nArrays_function_type )(  ) const;
            nArrays_function_type nArrays_function_value( &::SireBase::PackedArray2D< int >::nArrays );
            
            PackedArray2D_int__exposer.def( 
                "nArrays"
                , nArrays_function_value );
        
        }
        { //::SireBase::PackedArray2D< int >::nValues
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef int ( ::SireBase::PackedArray2D< int >::*nValues_function_type )(  ) const;
            nValues_function_type nValues_function_value( &::SireBase::PackedArray2D< int >::nValues );
            
            PackedArray2D_int__exposer.def( 
                "nValues"
                , nValues_function_value );
        
        }
        { //::SireBase::PackedArray2D< int >::nValues
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef int ( ::SireBase::PackedArray2D< int >::*nValues_function_type )( ::quint32 ) const;
            nValues_function_type nValues_function_value( &::SireBase::PackedArray2D< int >::nValues );
            
            PackedArray2D_int__exposer.def( 
                "nValues"
                , nValues_function_value
                , ( bp::arg("i") ) );
        
        }
        PackedArray2D_int__exposer.def( bp::self != bp::self );
        { //::SireBase::PackedArray2D< int >::operator()
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef int const & ( ::SireBase::PackedArray2D< int >::*__call___function_type )( ::quint32,::quint32 ) const;
            __call___function_type __call___function_value( &::SireBase::PackedArray2D< int >::operator() );
            
            PackedArray2D_int__exposer.def( 
                "__call__"
                , __call___function_value
                , ( bp::arg("i"), bp::arg("j") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireBase::PackedArray2D< int >::operator=
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef ::SireBase::PackedArray2D< int > & ( ::SireBase::PackedArray2D< int >::*assign_function_type )( ::SireBase::PackedArray2D< int > const & ) ;
            assign_function_type assign_function_value( &::SireBase::PackedArray2D< int >::operator= );
            
            PackedArray2D_int__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        PackedArray2D_int__exposer.def( bp::self == bp::self );
        { //::SireBase::PackedArray2D< int >::operator[]
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef ::SireBase::detail::PackedArray2D_Array< int > const & ( ::SireBase::PackedArray2D< int >::*__getitem___function_type )( ::quint32 ) const;
            __getitem___function_type __getitem___function_value( &::SireBase::PackedArray2D< int >::operator[] );
            
            PackedArray2D_int__exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireBase::PackedArray2D< int >::remove
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef void ( ::SireBase::PackedArray2D< int >::*remove_function_type )( ::quint32 ) ;
            remove_function_type remove_function_value( &::SireBase::PackedArray2D< int >::remove );
            
            PackedArray2D_int__exposer.def( 
                "remove"
                , remove_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireBase::PackedArray2D< int >::removeAll
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef void ( ::SireBase::PackedArray2D< int >::*removeAll_function_type )( ::QVarLengthArray< int, 256 > const & ) ;
            removeAll_function_type removeAll_function_value( &::SireBase::PackedArray2D< int >::removeAll );
            
            PackedArray2D_int__exposer.def( 
                "removeAll"
                , removeAll_function_value
                , ( bp::arg("idxs") ) );
        
        }
        { //::SireBase::PackedArray2D< int >::size
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef int ( ::SireBase::PackedArray2D< int >::*size_function_type )(  ) const;
            size_function_type size_function_value( &::SireBase::PackedArray2D< int >::size );
            
            PackedArray2D_int__exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::SireBase::PackedArray2D< int >::toQVector
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef ::QVector< int > ( ::SireBase::PackedArray2D< int >::*toQVector_function_type )(  ) const;
            toQVector_function_type toQVector_function_value( &::SireBase::PackedArray2D< int >::toQVector );
            
            PackedArray2D_int__exposer.def( 
                "toQVector"
                , toQVector_function_value );
        
        }
        { //::SireBase::PackedArray2D< int >::toQVectorVector
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef ::QVector< QVector< int > > ( ::SireBase::PackedArray2D< int >::*toQVectorVector_function_type )(  ) const;
            toQVectorVector_function_type toQVectorVector_function_value( &::SireBase::PackedArray2D< int >::toQVectorVector );
            
            PackedArray2D_int__exposer.def( 
                "toQVectorVector"
                , toQVectorVector_function_value );
        
        }
        { //::SireBase::PackedArray2D< int >::toString
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef ::QString ( ::SireBase::PackedArray2D< int >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireBase::PackedArray2D< int >::toString );
            
            PackedArray2D_int__exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireBase::PackedArray2D< int >::toVariant
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef ::SireBase::PackedArray2D< QVariant > ( ::SireBase::PackedArray2D< int >::*toVariant_function_type )(  ) const;
            toVariant_function_type toVariant_function_value( &::SireBase::PackedArray2D< int >::toVariant );
            
            PackedArray2D_int__exposer.def( 
                "toVariant"
                , toVariant_function_value );
        
        }
        { //::SireBase::PackedArray2D< int >::update
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef void ( ::SireBase::PackedArray2D< int >::*update_function_type )( ::quint32,::SireBase::detail::PackedArray2D_Array< int > const & ) ;
            update_function_type update_function_value( &::SireBase::PackedArray2D< int >::update );
            
            PackedArray2D_int__exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("i"), bp::arg("array") ) );
        
        }
        { //::SireBase::PackedArray2D< int >::update
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef void ( ::SireBase::PackedArray2D< int >::*update_function_type )( ::quint32,::QVector< int > const & ) ;
            update_function_type update_function_value( &::SireBase::PackedArray2D< int >::update );
            
            PackedArray2D_int__exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("i"), bp::arg("array") ) );
        
        }
        { //::SireBase::PackedArray2D< int >::updateAll
        
            typedef SireBase::PackedArray2D< int > exported_class_t;
            typedef void ( ::SireBase::PackedArray2D< int >::*updateAll_function_type )( ::QVarLengthArray< int, 256 > const &,::SireBase::PackedArray2D< int > const & ) ;
            updateAll_function_type updateAll_function_value( &::SireBase::PackedArray2D< int >::updateAll );
            
            PackedArray2D_int__exposer.def( 
                "updateAll"
                , updateAll_function_value
                , ( bp::arg("idxs"), bp::arg("arrays") ) );
        
        }
        PackedArray2D_int__exposer.staticmethod( "fromVariant" );
        PackedArray2D_int__exposer.def( "__copy__", &__copy__);
        PackedArray2D_int__exposer.def( "__deepcopy__", &__copy__);
        PackedArray2D_int__exposer.def( "clone", &__copy__);
        PackedArray2D_int__exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireBase::PackedArray2D<int> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        PackedArray2D_int__exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireBase::PackedArray2D<int> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        PackedArray2D_int__exposer.def( "__str__", &__str__< ::SireBase::PackedArray2D<int> > );
        PackedArray2D_int__exposer.def( "__repr__", &__str__< ::SireBase::PackedArray2D<int> > );
    }

}
