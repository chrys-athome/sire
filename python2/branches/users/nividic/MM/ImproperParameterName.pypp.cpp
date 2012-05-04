// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ImproperParameterName.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/errors.h"

#include "SireBase/property.h"

#include "SireError/errors.h"

#include "SireFF/detail/atomiccoords3d.h"

#include "SireFF/errors.h"

#include "SireMaths/line.h"

#include "SireMaths/torsion.h"

#include "SireMaths/triangle.h"

#include "SireMol/mover.hpp"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireUnits/dimensions.h"

#include "SireUnits/units.h"

#include "internalff.h"

#include "tostring.h"

#include <QDebug>

#include <cstdio>

#include "internalff.h"

SireMM::ImproperParameterName __copy__(const SireMM::ImproperParameterName &other){ return SireMM::ImproperParameterName(other); }

const char* pvt_get_name(const SireMM::ImproperParameterName&){ return "SireMM::ImproperParameterName";}

void register_ImproperParameterName_class(){

    { //::SireMM::ImproperParameterName
        typedef bp::class_< SireMM::ImproperParameterName > ImproperParameterName_exposer_t;
        ImproperParameterName_exposer_t ImproperParameterName_exposer = ImproperParameterName_exposer_t( "ImproperParameterName" );
        bp::scope ImproperParameterName_scope( ImproperParameterName_exposer );
        ImproperParameterName_exposer.def( bp::init< >() );
        { //::SireMM::ImproperParameterName::improper
        
            typedef ::SireBase::PropertyName const & ( ::SireMM::ImproperParameterName::*improper_function_type )(  ) const;
            improper_function_type improper_function_value( &::SireMM::ImproperParameterName::improper );
            
            ImproperParameterName_exposer.def( 
                "improper"
                , improper_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        ImproperParameterName_exposer.def( "__copy__", &__copy__);
        ImproperParameterName_exposer.def( "__deepcopy__", &__copy__);
        ImproperParameterName_exposer.def( "clone", &__copy__);
        ImproperParameterName_exposer.def( "__str__", &pvt_get_name);
        ImproperParameterName_exposer.def( "__repr__", &pvt_get_name);
    }

}
