
/**
  * This file contains the boost::python wrapping of ResAngleInfo
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireMM/molangleinfo.h"
#include "SireMM/resangleinfo.h"

#include "wrapInternalInfo.hpp"

using namespace boost::python;
using namespace SireMol;

namespace SireMM
{

void  
SIREMM_EXPORT
export_ResAngleInfo()
{
    class_<ResAngleInfo> wrap_ResAngleInfo("ResAngleInfo", init<>());
    
    //wrap the common parts of this object
    wrap_ResInternalInfo<ResAngleInfo>( wrap_ResAngleInfo );
    
    //now wrap the specific parts
    wrap_ResAngleInfo
        .def( init<const MolAngleInfo&, ResNum>() )
        .def( "nAngles", &ResAngleInfo::nAngles )
        .def( "nInterAngles", &ResAngleInfo::nInterAngles )
        .def( "nIntraAngles", &ResAngleInfo::nIntraAngles )
        .def( "angles", &ResAngleInfo::angles )
        .def( "interAngles", &ResAngleInfo::interAngles )
        .def( "intraAngles", &ResAngleInfo::intraAngles )
    ;

}

}
