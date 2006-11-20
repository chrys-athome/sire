
/**
  * This file contains the boost::python wrapping of ResDihedralInfo
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include "SireMM/moldihedralinfo.h"
#include "SireMM/resdihedralinfo.h"

#include "wrapInternalInfo.hpp"

using namespace boost::python;
using namespace SireMol;

namespace SireMM
{

void  
SIREMM_EXPORT
export_ResDihedralInfo()
{
    class_<ResDihedralInfo> wrap_ResDihedralInfo("ResDihedralInfo", init<>());
    
    //wrap the common parts of this object
    wrap_ResInternalInfo<ResDihedralInfo>( wrap_ResDihedralInfo );
    
    //now wrap the specific parts
    wrap_ResDihedralInfo
        .def( init<const MolDihedralInfo&, ResNum>() )
        .def( "nDihedrals", &ResDihedralInfo::nDihedrals )
        .def( "nInterDihedrals", &ResDihedralInfo::nInterDihedrals )
        .def( "nIntraDihedrals", &ResDihedralInfo::nIntraDihedrals )
        .def( "dihedrals", &ResDihedralInfo::dihedrals )
        .def( "interDihedrals", &ResDihedralInfo::interDihedrals )
        .def( "intraDihedrals", &ResDihedralInfo::intraDihedrals )
    ;

}

}
