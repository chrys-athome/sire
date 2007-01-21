
#include <boost/python.hpp>

#include "SireMol/wrapID.hpp"

#include "SireSystem/systemdata.h"

using namespace SireMol;
using namespace SireBase;

using namespace boost::python;

namespace SireSystem
{

void SIRESYSTEM_EXPORT export_SystemData()
{
    wrap_ID<SystemID>("SystemID");
    
    class_<SystemData>( "SystemData", init<>() )
    ;
}

}
