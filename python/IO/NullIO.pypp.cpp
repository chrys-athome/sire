// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "NullIO.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireMol/cuttingfunction.h"

#include "SireMol/molecule.h"

#include "SireMol/molidx.h"

#include "SireMol/mover.hpp"

#include "SireStream/datastream.h"

#include "iobase.h"

#include <QDebug>

#include <QFile>

#include "iobase.h"

SireIO::NullIO __copy__(const SireIO::NullIO &other){ return SireIO::NullIO(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_NullIO_class(){

    { //::SireIO::NullIO
        typedef bp::class_< SireIO::NullIO, bp::bases< SireIO::IOBase, SireBase::Property > > NullIO_exposer_t;
        NullIO_exposer_t NullIO_exposer = NullIO_exposer_t( "NullIO", bp::init< >() );
        bp::scope NullIO_scope( NullIO_exposer );
        NullIO_exposer.def( bp::init< SireIO::NullIO const & >(( bp::arg("other") )) );
        NullIO_exposer.def( bp::self != bp::self );
        { //::SireIO::NullIO::operator=
        
            typedef ::SireIO::NullIO & ( ::SireIO::NullIO::*assign_function_type )( ::SireIO::NullIO const & ) ;
            assign_function_type assign_function_value( &::SireIO::NullIO::operator= );
            
            NullIO_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        NullIO_exposer.def( bp::self == bp::self );
        { //::SireIO::NullIO::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireIO::NullIO::typeName );
            
            NullIO_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        NullIO_exposer.staticmethod( "typeName" );
        NullIO_exposer.def( "__copy__", &__copy__);
        NullIO_exposer.def( "__deepcopy__", &__copy__);
        NullIO_exposer.def( "clone", &__copy__);
        NullIO_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireIO::NullIO >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullIO_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireIO::NullIO >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullIO_exposer.def( "__str__", &__str__< ::SireIO::NullIO > );
        NullIO_exposer.def( "__repr__", &__str__< ::SireIO::NullIO > );
    }

}
