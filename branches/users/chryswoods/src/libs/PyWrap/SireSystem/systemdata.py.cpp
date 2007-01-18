
#include <boost/python.hpp>

#include "SireMol/wrapID.hpp"

#include "SireSystem/systemdata.h"
#include "SireSystem/moleculegroup.h"
#include "SireSystem/ffexpression.h"

using namespace SireMol;
using namespace SireBase;

using namespace boost::python;

namespace SireSystem
{

void SIRESYSTEM_EXPORT export_SystemData()
{
    wrap_ID<SystemID>("SystemID");
    
    class_<SystemData, boost::noncopyable>( "SystemData", no_init )
    
        .def( self == self )
        .def( self != self )
        
        .def( "name", &SystemData::name, 
                      return_value_policy<copy_const_reference>() )
        
        .def( "ID", &SystemData::ID )
        
        .def( "version", &SystemData::version,
                      return_value_policy<copy_const_reference>() )
      
        .def( "group", &SystemData::group,
                      return_value_policy<copy_const_reference>() )
        
        .def( "groups", &SystemData::groups,
                      return_value_policy<copy_const_reference>() )
                      
        .def( "equations", &SystemData::equations )
        
        .def( "parameters", &SystemData::parameters,
                      return_value_policy<copy_const_reference>() )
    ;
}

}
