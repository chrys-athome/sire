// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Force3D.pypp.hpp"

namespace bp = boost::python;

#include "atomforces.h"

#include "atomforces.h"

SireBase::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > __copy__(const SireBase::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > &other){ return SireBase::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> >(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Force3D_class(){

    { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >
        typedef bp::class_< SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > > Force3D_exposer_t;
        Force3D_exposer_t Force3D_exposer = Force3D_exposer_t( "Force3D", bp::init< >() );
        bp::scope Force3D_scope( Force3D_exposer );
        Force3D_exposer.def( bp::init< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > const & >(( bp::arg("val") )) );
        Force3D_exposer.def( bp::init< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > const &, SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > const &, SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > const & >(( bp::arg("x"), bp::arg("y"), bp::arg("z") )) );
        Force3D_exposer.def( bp::init< SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > const & >(( bp::arg("other") )) );
        { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::at
        
            typedef SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > const & ( ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::*at_function_type )( int ) const;
            at_function_type at_function_value( &::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::at );
            
            Force3D_exposer.def( 
                "at"
                , at_function_value
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::count
        
            typedef SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef int ( ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::*count_function_type )(  ) const;
            count_function_type count_function_value( &::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::count );
            
            Force3D_exposer.def( 
                "count"
                , count_function_value );
        
        }
        Force3D_exposer.def( bp::self != bp::self );
        { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::operator=
        
            typedef SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > & ( ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::*assign_function_type )( ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > const & ) ;
            assign_function_type assign_function_value( &::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::operator= );
            
            Force3D_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        Force3D_exposer.def( bp::self == bp::self );
        { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::operator[]
        
            typedef SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > const & ( ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::*__getitem___function_type )( int ) const;
            __getitem___function_type __getitem___function_value( &::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::operator[] );
            
            Force3D_exposer.def( 
                "__getitem__"
                , __getitem___function_value
                , ( bp::arg("i") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::set
        
            typedef SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef void ( ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::*set_function_type )( int,::SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > const & ) ;
            set_function_type set_function_value( &::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::set );
            
            Force3D_exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("i"), bp::arg("value") ) );
        
        }
        { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::toString
        
            typedef SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::QString ( ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::toString );
            
            Force3D_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::typeName
        
            typedef SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::typeName );
            
            Force3D_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::x
        
            typedef SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > const & ( ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::*x_function_type )(  ) const;
            x_function_type x_function_value( &::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::x );
            
            Force3D_exposer.def( 
                "x"
                , x_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::y
        
            typedef SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > const & ( ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::*y_function_type )(  ) const;
            y_function_type y_function_value( &::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::y );
            
            Force3D_exposer.def( 
                "y"
                , y_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::z
        
            typedef SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > > exported_class_t;
            typedef ::SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > const & ( ::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::*z_function_type )(  ) const;
            z_function_type z_function_value( &::SireBase::Vector3D< SireUnits::Dimension::PhysUnit< 1, 1, -2, 0, 0, 0, 0 > >::z );
            
            Force3D_exposer.def( 
                "z"
                , z_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        Force3D_exposer.staticmethod( "typeName" );
        Force3D_exposer.def( "__copy__", &__copy__);
        Force3D_exposer.def( "__deepcopy__", &__copy__);
        Force3D_exposer.def( "clone", &__copy__);
        Force3D_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireBase::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Force3D_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireBase::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Force3D_exposer.def( "__str__", &__str__< ::SireBase::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > > );
        Force3D_exposer.def( "__repr__", &__str__< ::SireBase::Vector3D<SireUnits::Dimension::PhysUnit<1, 1, -2, 0, 0, 0, 0> > > );
    }

}
