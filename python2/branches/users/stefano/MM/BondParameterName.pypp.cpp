// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "BondParameterName.pypp.hpp"

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

SireMM::BondParameterName __copy__(const SireMM::BondParameterName &other){ return SireMM::BondParameterName(other); }

const char* pvt_get_name(const SireMM::BondParameterName&){ return "SireMM::BondParameterName";}

void register_BondParameterName_class(){

    { //::SireMM::BondParameterName
        typedef bp::class_< SireMM::BondParameterName > BondParameterName_exposer_t;
        BondParameterName_exposer_t BondParameterName_exposer = BondParameterName_exposer_t( "BondParameterName", bp::init< >() );
        bp::scope BondParameterName_scope( BondParameterName_exposer );
        { //::SireMM::BondParameterName::bond
        
            typedef ::SireBase::PropertyName const & ( ::SireMM::BondParameterName::*bond_function_type )(  ) const;
            bond_function_type bond_function_value( &::SireMM::BondParameterName::bond );
            
            BondParameterName_exposer.def( 
                "bond"
                , bond_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        BondParameterName_exposer.def( "__copy__", &__copy__);
        BondParameterName_exposer.def( "__deepcopy__", &__copy__);
        BondParameterName_exposer.def( "clone", &__copy__);
        BondParameterName_exposer.def( "__str__", &pvt_get_name);
        BondParameterName_exposer.def( "__repr__", &pvt_get_name);
    }

}
