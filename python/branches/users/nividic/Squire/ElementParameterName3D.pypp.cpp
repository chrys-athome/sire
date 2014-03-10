// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ElementParameterName3D.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/errors.h"

#include "SireError/errors.h"

#include "SireFF/errors.h"

#include "SireFF/ff.h"

#include "SireMol/atomelements.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "qmpotential.h"

#include "qmprogram.h"

#include "qmpotential.h"

Squire::ElementParameterName3D __copy__(const Squire::ElementParameterName3D &other){ return Squire::ElementParameterName3D(other); }

const char* pvt_get_name(const Squire::ElementParameterName3D&){ return "Squire::ElementParameterName3D";}

void register_ElementParameterName3D_class(){

    { //::Squire::ElementParameterName3D
        typedef bp::class_< Squire::ElementParameterName3D, bp::bases< Squire::ElementParameterName > > ElementParameterName3D_exposer_t;
        ElementParameterName3D_exposer_t ElementParameterName3D_exposer = ElementParameterName3D_exposer_t( "ElementParameterName3D", bp::init< >() );
        bp::scope ElementParameterName3D_scope( ElementParameterName3D_exposer );
        ElementParameterName3D_exposer.def( "__copy__", &__copy__);
        ElementParameterName3D_exposer.def( "__deepcopy__", &__copy__);
        ElementParameterName3D_exposer.def( "clone", &__copy__);
        ElementParameterName3D_exposer.def( "__str__", &pvt_get_name);
        ElementParameterName3D_exposer.def( "__repr__", &pvt_get_name);
    }

}
