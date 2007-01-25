/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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
