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
