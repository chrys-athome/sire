// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "BendBendParameterName.pypp.hpp"

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

SireMM::BendBendParameterName __copy__(const SireMM::BendBendParameterName &other){ return SireMM::BendBendParameterName(other); }

const char* pvt_get_name(const SireMM::BendBendParameterName&){ return "SireMM::BendBendParameterName";}

void register_BendBendParameterName_class(){

    { //::SireMM::BendBendParameterName
        typedef bp::class_< SireMM::BendBendParameterName > BendBendParameterName_exposer_t;
        BendBendParameterName_exposer_t BendBendParameterName_exposer = BendBendParameterName_exposer_t( "BendBendParameterName" );
        bp::scope BendBendParameterName_scope( BendBendParameterName_exposer );
        BendBendParameterName_exposer.def( bp::init< >() );
        { //::SireMM::BendBendParameterName::bendBend
        
            typedef ::SireBase::PropertyName const & ( ::SireMM::BendBendParameterName::*bendBend_function_type )(  ) const;
            bendBend_function_type bendBend_function_value( &::SireMM::BendBendParameterName::bendBend );
            
            BendBendParameterName_exposer.def( 
                "bendBend"
                , bendBend_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        BendBendParameterName_exposer.def( "__copy__", &__copy__);
        BendBendParameterName_exposer.def( "__deepcopy__", &__copy__);
        BendBendParameterName_exposer.def( "clone", &__copy__);
        BendBendParameterName_exposer.def( "__str__", &pvt_get_name);
        BendBendParameterName_exposer.def( "__repr__", &pvt_get_name);
    }

}