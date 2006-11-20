
/**
  * This file contains the boost::python wrapping of ResBondInfo
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireMM/molbondinfo.h"
#include "SireMM/resbondinfo.h"

#include "wrapInternalInfo.hpp"

using namespace boost::python;
using namespace SireMol;

namespace SireMM
{

void  
SIREMM_EXPORT
export_ResBondInfo()
{
    class_<ResBondInfo> wrap_ResBondInfo("ResBondInfo", init<>());
    
    //wrap the common parts of this object
    wrap_ResInternalInfo<ResBondInfo>( wrap_ResBondInfo );
    
    //now wrap the specific parts
    wrap_ResBondInfo
        .def( init<const MolBondInfo&, ResNum>() )
        .def( "nBonds", &ResBondInfo::nBonds )
        .def( "nInterBonds", &ResBondInfo::nInterBonds )
        .def( "nIntraBonds", &ResBondInfo::nIntraBonds )
        .def( "bonds", &ResBondInfo::bonds )
        .def( "interBonds", &ResBondInfo::interBonds )
        .def( "intraBonds", &ResBondInfo::intraBonds )
    ;

}

}
