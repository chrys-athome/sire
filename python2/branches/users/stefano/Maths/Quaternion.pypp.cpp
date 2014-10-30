// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Quaternion.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "matrix.h"

#include "quaternion.h"

#include "vector.h"

#include <QDebug>

#include <QRegExp>

#include <QString>

#include <cmath>

#include "quaternion.h"

SireMaths::Quaternion __copy__(const SireMaths::Quaternion &other){ return SireMaths::Quaternion(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_Quaternion_class(){

    { //::SireMaths::Quaternion
        typedef bp::class_< SireMaths::Quaternion > Quaternion_exposer_t;
        Quaternion_exposer_t Quaternion_exposer = Quaternion_exposer_t( "Quaternion", bp::init< >() );
        bp::scope Quaternion_scope( Quaternion_exposer );
        Quaternion_exposer.def( bp::init< SireMaths::Quaternion const & >(( bp::arg("p1") )) );
        Quaternion_exposer.def( bp::init< SireUnits::Dimension::Angle, SireMaths::Vector const & >(( bp::arg("angle"), bp::arg("axis") )) );
        Quaternion_exposer.def( bp::init< SireMaths::Matrix const & >(( bp::arg("m") )) );
        Quaternion_exposer.def( bp::init< double, double, double, double >(( bp::arg("x"), bp::arg("y"), bp::arg("z"), bp::arg("w") )) );
        { //::SireMaths::Quaternion::conjugate
        
            typedef ::SireMaths::Quaternion ( ::SireMaths::Quaternion::*conjugate_function_type )(  ) const;
            conjugate_function_type conjugate_function_value( &::SireMaths::Quaternion::conjugate );
            
            Quaternion_exposer.def( 
                "conjugate"
                , conjugate_function_value );
        
        }
        { //::SireMaths::Quaternion::dot
        
            typedef double ( ::SireMaths::Quaternion::*dot_function_type )( ::SireMaths::Quaternion const & ) const;
            dot_function_type dot_function_value( &::SireMaths::Quaternion::dot );
            
            Quaternion_exposer.def( 
                "dot"
                , dot_function_value
                , ( bp::arg("q") ) );
        
        }
        { //::SireMaths::Quaternion::fromMatrix
        
            typedef void ( ::SireMaths::Quaternion::*fromMatrix_function_type )( ::SireMaths::Matrix const & ) ;
            fromMatrix_function_type fromMatrix_function_value( &::SireMaths::Quaternion::fromMatrix );
            
            Quaternion_exposer.def( 
                "fromMatrix"
                , fromMatrix_function_value
                , ( bp::arg("m") ) );
        
        }
        { //::SireMaths::Quaternion::fromString
        
            typedef ::SireMaths::Quaternion ( *fromString_function_type )( ::QString const & );
            fromString_function_type fromString_function_value( &::SireMaths::Quaternion::fromString );
            
            Quaternion_exposer.def( 
                "fromString"
                , fromString_function_value
                , ( bp::arg("str") ) );
        
        }
        { //::SireMaths::Quaternion::identity
        
            typedef ::SireMaths::Quaternion ( *identity_function_type )(  );
            identity_function_type identity_function_value( &::SireMaths::Quaternion::identity );
            
            Quaternion_exposer.def( 
                "identity"
                , identity_function_value );
        
        }
        { //::SireMaths::Quaternion::inverse
        
            typedef ::SireMaths::Quaternion ( ::SireMaths::Quaternion::*inverse_function_type )(  ) const;
            inverse_function_type inverse_function_value( &::SireMaths::Quaternion::inverse );
            
            Quaternion_exposer.def( 
                "inverse"
                , inverse_function_value );
        
        }
        { //::SireMaths::Quaternion::isIdentity
        
            typedef bool ( ::SireMaths::Quaternion::*isIdentity_function_type )(  ) const;
            isIdentity_function_type isIdentity_function_value( &::SireMaths::Quaternion::isIdentity );
            
            Quaternion_exposer.def( 
                "isIdentity"
                , isIdentity_function_value );
        
        }
        Quaternion_exposer.def( bp::self != bp::self );
        { //::SireMaths::Quaternion::operator=
        
            typedef ::SireMaths::Quaternion & ( ::SireMaths::Quaternion::*assign_function_type )( ::SireMaths::Quaternion const & ) ;
            assign_function_type assign_function_value( &::SireMaths::Quaternion::operator= );
            
            Quaternion_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("p") )
                , bp::return_self< >() );
        
        }
        Quaternion_exposer.def( bp::self == bp::self );
        { //::SireMaths::Quaternion::pow
        
            typedef ::SireMaths::Quaternion ( ::SireMaths::Quaternion::*pow_function_type )( double ) const;
            pow_function_type pow_function_value( &::SireMaths::Quaternion::pow );
            
            Quaternion_exposer.def( 
                "pow"
                , pow_function_value
                , ( bp::arg("n") ) );
        
        }
        { //::SireMaths::Quaternion::renormalise
        
            typedef void ( ::SireMaths::Quaternion::*renormalise_function_type )(  ) ;
            renormalise_function_type renormalise_function_value( &::SireMaths::Quaternion::renormalise );
            
            Quaternion_exposer.def( 
                "renormalise"
                , renormalise_function_value );
        
        }
        { //::SireMaths::Quaternion::rotate
        
            typedef ::SireMaths::Vector ( ::SireMaths::Quaternion::*rotate_function_type )( ::SireMaths::Vector const & ) const;
            rotate_function_type rotate_function_value( &::SireMaths::Quaternion::rotate );
            
            Quaternion_exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("p") ) );
        
        }
        { //::SireMaths::Quaternion::rotate
        
            typedef ::QVector< SireMaths::Vector > ( ::SireMaths::Quaternion::*rotate_function_type )( ::QVector< SireMaths::Vector > const & ) const;
            rotate_function_type rotate_function_value( &::SireMaths::Quaternion::rotate );
            
            Quaternion_exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("points") ) );
        
        }
        { //::SireMaths::Quaternion::slerp
        
            typedef ::SireMaths::Quaternion ( ::SireMaths::Quaternion::*slerp_function_type )( ::SireMaths::Quaternion const &,double ) const;
            slerp_function_type slerp_function_value( &::SireMaths::Quaternion::slerp );
            
            Quaternion_exposer.def( 
                "slerp"
                , slerp_function_value
                , ( bp::arg("q"), bp::arg("lambda") ) );
        
        }
        { //::SireMaths::Quaternion::toMatrix
        
            typedef ::SireMaths::Matrix ( ::SireMaths::Quaternion::*toMatrix_function_type )(  ) const;
            toMatrix_function_type toMatrix_function_value( &::SireMaths::Quaternion::toMatrix );
            
            Quaternion_exposer.def( 
                "toMatrix"
                , toMatrix_function_value );
        
        }
        { //::SireMaths::Quaternion::toString
        
            typedef ::QString ( ::SireMaths::Quaternion::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMaths::Quaternion::toString );
            
            Quaternion_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMaths::Quaternion::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMaths::Quaternion::typeName );
            
            Quaternion_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMaths::Quaternion::w
        
            typedef double ( ::SireMaths::Quaternion::*w_function_type )(  ) const;
            w_function_type w_function_value( &::SireMaths::Quaternion::w );
            
            Quaternion_exposer.def( 
                "w"
                , w_function_value );
        
        }
        { //::SireMaths::Quaternion::what
        
            typedef char const * ( ::SireMaths::Quaternion::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMaths::Quaternion::what );
            
            Quaternion_exposer.def( 
                "what"
                , what_function_value );
        
        }
        { //::SireMaths::Quaternion::x
        
            typedef double ( ::SireMaths::Quaternion::*x_function_type )(  ) const;
            x_function_type x_function_value( &::SireMaths::Quaternion::x );
            
            Quaternion_exposer.def( 
                "x"
                , x_function_value );
        
        }
        { //::SireMaths::Quaternion::y
        
            typedef double ( ::SireMaths::Quaternion::*y_function_type )(  ) const;
            y_function_type y_function_value( &::SireMaths::Quaternion::y );
            
            Quaternion_exposer.def( 
                "y"
                , y_function_value );
        
        }
        { //::SireMaths::Quaternion::z
        
            typedef double ( ::SireMaths::Quaternion::*z_function_type )(  ) const;
            z_function_type z_function_value( &::SireMaths::Quaternion::z );
            
            Quaternion_exposer.def( 
                "z"
                , z_function_value );
        
        }
        Quaternion_exposer.staticmethod( "fromString" );
        Quaternion_exposer.staticmethod( "identity" );
        Quaternion_exposer.staticmethod( "typeName" );
        Quaternion_exposer.def( bp::self * bp::self );
        Quaternion_exposer.def( bp::self * bp::other< SireMaths::Vector >() );
        Quaternion_exposer.def( bp::self + bp::self );
        Quaternion_exposer.def( bp::self - bp::self );
        Quaternion_exposer.def( "__copy__", &__copy__);
        Quaternion_exposer.def( "__deepcopy__", &__copy__);
        Quaternion_exposer.def( "clone", &__copy__);
        Quaternion_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMaths::Quaternion >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Quaternion_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMaths::Quaternion >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        Quaternion_exposer.def( "__str__", &__str__< ::SireMaths::Quaternion > );
        Quaternion_exposer.def( "__repr__", &__str__< ::SireMaths::Quaternion > );
    }

}
